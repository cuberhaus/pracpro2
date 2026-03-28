"""FastAPI backend that wraps the C++ program.exe via stdin/stdout."""

from __future__ import annotations

import os
import subprocess
import threading
from collections import deque
from pathlib import Path
from typing import Optional

from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import FileResponse
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel

PROJECT_ROOT = Path(__file__).resolve().parent.parent
EXE_PATH = PROJECT_ROOT / "program.exe"
STATIC_DIR = Path(__file__).resolve().parent / "static"

OPCIONS_MARKER = "OPCIONS:"
INIT_MARKER = "Inserta k"

MENU_LINES = {
    "crea_especie", "obtener_gen", "distancia", "elimina_especie",
    "existe_especie", "lee_cjt_especies", "imprime_cjt_especies",
    "tabla_distancias", "inicialitza_clusters", "ejecuta_paso_wpgma",
    "imprime_cluster", "imprime_arbol_filogenetico", "fin",
}


class ProcessManager:
    """Manages the C++ subprocess lifecycle and I/O."""

    def __init__(self) -> None:
        self.proc: Optional[subprocess.Popen] = None
        self.output_lines: deque[str] = deque()
        self._reader_thread: Optional[threading.Thread] = None
        self._lock = threading.Lock()
        self._stop_event = threading.Event()
        self.k_value: Optional[int] = None

    @property
    def alive(self) -> bool:
        return self.proc is not None and self.proc.poll() is None

    def start(self, k: int) -> str:
        self.stop()
        self.k_value = k
        self.output_lines.clear()
        self._stop_event.clear()

        env = os.environ.copy()
        msys_paths = [r"C:\msys64\ucrt64\bin", r"C:\msys64\usr\bin"]
        env["PATH"] = ";".join(msys_paths) + ";" + env.get("PATH", "")

        self.proc = subprocess.Popen(
            [str(EXE_PATH)],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1,
            cwd=str(PROJECT_ROOT),
            env=env,
        )

        self._reader_thread = threading.Thread(target=self._read_output, daemon=True)
        self._reader_thread.start()

        self._send_raw(f"{k}\n")
        return self._collect_until_opcions()

    def stop(self) -> None:
        if self.proc and self.alive:
            try:
                self.proc.stdin.write("fin\n")
                self.proc.stdin.flush()
            except (BrokenPipeError, OSError):
                pass
            self._stop_event.set()
            try:
                self.proc.wait(timeout=3)
            except subprocess.TimeoutExpired:
                self.proc.kill()
        self.proc = None
        self._reader_thread = None

    def send_command(self, command: str, args: list[str]) -> str:
        if not self.alive:
            raise RuntimeError("Process is not running. Initialize first with POST /api/init.")

        line = command
        if args:
            line += " " + " ".join(args)
        line += "\n"

        if command == "lee_cjt_especies" and args:
            n = int(args[0])
            species_lines = args[1:]
            line = f"lee_cjt_especies\n{n}\n"
            for s in species_lines:
                line += s + "\n"

        self._send_raw(line)
        raw = self._collect_until_opcions()
        return self._strip_echo(raw)

    def send_raw_and_collect(self, text: str) -> str:
        """Send raw text to stdin and return parsed output."""
        self._send_raw(text)
        return self._strip_echo(self._collect_until_opcions())

    def _send_raw(self, text: str) -> None:
        if not self.alive:
            return
        try:
            self.proc.stdin.write(text)
            self.proc.stdin.flush()
        except (BrokenPipeError, OSError):
            pass

    def _read_output(self) -> None:
        """Background thread: reads stdout line by line into the deque."""
        try:
            while not self._stop_event.is_set():
                if self.proc is None or self.proc.stdout is None:
                    break
                line = self.proc.stdout.readline()
                if not line:
                    break
                with self._lock:
                    self.output_lines.append(line.rstrip("\n").rstrip("\r"))
        except (ValueError, OSError):
            pass

    def _collect_until_opcions(self, timeout: float = 5.0) -> str:
        """Drain output lines until we see the full OPCIONS menu block."""
        collected: list[str] = []
        import time
        deadline = time.time() + timeout
        seen_opcions = False
        menu_count = 0
        expected_menu_count = len(MENU_LINES) + 1  # OPCIONS: + 13 command names

        while time.time() < deadline:
            with self._lock:
                while self.output_lines:
                    line = self.output_lines.popleft()
                    if line == OPCIONS_MARKER:
                        seen_opcions = True
                        menu_count = 1
                        continue
                    if seen_opcions:
                        if line.strip() in MENU_LINES:
                            menu_count += 1
                            if menu_count >= expected_menu_count:
                                return "\n".join(collected)
                            continue
                        else:
                            seen_opcions = False
                            menu_count = 0
                    if line == INIT_MARKER:
                        continue
                    collected.append(line)
            time.sleep(0.02)

        return "\n".join(collected)

    @staticmethod
    def _strip_echo(raw: str) -> str:
        """Remove the '# command args' echo line from output."""
        lines = raw.split("\n")
        result: list[str] = []
        for line in lines:
            if line.startswith("# "):
                continue
            result.append(line)
        while result and result[-1].strip() == "":
            result.pop()
        while result and result[0].strip() == "":
            result.pop(0)
        return "\n".join(result)


app = FastAPI(title="pracpro2 Web Interface")
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

pm = ProcessManager()


class InitRequest(BaseModel):
    k: int


class CommandRequest(BaseModel):
    command: str
    args: list[str] = []


class ReadSpeciesRequest(BaseModel):
    species: list[dict]  # [{"id": "a", "gen": "ACGT..."}, ...]


@app.post("/api/init")
async def init_process(req: InitRequest):
    if req.k < 1:
        raise HTTPException(400, "k must be >= 1")
    try:
        pm.start(req.k)
    except Exception as e:
        raise HTTPException(500, f"Failed to start process: {e}") from e
    return {"status": "ok", "k": req.k}


@app.post("/api/command")
async def run_command(req: CommandRequest):
    if not pm.alive:
        raise HTTPException(400, "Process not running. Call POST /api/init first.")

    cmd = req.command.strip()
    if cmd == "fin":
        pm.stop()
        return {"output": "Process terminated.", "lines": []}

    try:
        output = pm.send_command(cmd, req.args)
    except RuntimeError as e:
        raise HTTPException(400, str(e)) from e

    lines = [l for l in output.split("\n") if l.strip()] if output.strip() else []
    return {"output": output, "lines": lines}


@app.post("/api/read_species")
async def read_species(req: ReadSpeciesRequest):
    """Convenience endpoint for lee_cjt_especies."""
    if not pm.alive:
        raise HTTPException(400, "Process not running. Call POST /api/init first.")

    n = len(req.species)
    raw_input = f"lee_cjt_especies\n{n}\n"
    for sp in req.species:
        raw_input += f"{sp['id']} {sp['gen']}\n"

    output = pm.send_raw_and_collect(raw_input)
    lines = [l for l in output.split("\n") if l.strip()] if output.strip() else []
    return {"output": output, "lines": lines}


@app.get("/api/status")
async def status():
    return {
        "alive": pm.alive,
        "k": pm.k_value,
    }


@app.get("/")
async def index():
    return FileResponse(STATIC_DIR / "index.html")


app.mount("/static", StaticFiles(directory=str(STATIC_DIR)), name="static")


@app.on_event("shutdown")
def shutdown():
    pm.stop()
