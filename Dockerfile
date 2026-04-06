# Stage 1: Compile C++ program
FROM gcc:13 AS build

WORKDIR /src
COPY *.cc *.hh Makefile ./
RUN make program.exe

# Stage 2: Python runtime
FROM python:3.12-slim

WORKDIR /app

COPY web/requirements.txt /tmp/requirements.txt
RUN pip install --no-cache-dir -r /tmp/requirements.txt && rm /tmp/requirements.txt

COPY --from=build /src/program.exe ./
COPY web/ ./web/

ENV PORT=8000
EXPOSE 8000

CMD ["python3", "-m", "uvicorn", "web.app:app", "--host", "0.0.0.0", "--port", "8000"]
