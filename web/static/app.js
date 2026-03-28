const { createApp, ref, computed, watch, nextTick } = Vue;

const COMMANDS = [
  { name: "crea_especie", label: "crea_especie", group: "Species" },
  { name: "obtener_gen", label: "obtener_gen", group: "Species" },
  { name: "distancia", label: "distancia", group: "Species" },
  { name: "elimina_especie", label: "elimina_especie", group: "Species" },
  { name: "existe_especie", label: "existe_especie", group: "Species" },
  { name: "lee_cjt_especies", label: "lee_cjt_especies", group: "Species" },
  { name: "imprime_cjt_especies", label: "imprime_cjt_especies", group: "Species" },
  { name: "tabla_distancias", label: "tabla_distancias", group: "Species" },
  { name: "inicializa_clusters", label: "inicializa_clusters", group: "Clusters" },
  { name: "ejecuta_paso_wpgma", label: "ejecuta_paso_wpgma", group: "Clusters" },
  { name: "imprime_cluster", label: "imprime_cluster", group: "Clusters" },
  { name: "imprime_arbol_filogenetico", label: "imprime_arbol_filogenetico", group: "Tree" },
];

const GROUPS = ["Species", "Clusters", "Tree"];

/* ---------- Tree bracket-notation parser ---------- */

function parseTree(str) {
  str = str.trim();
  let pos = 0;

  function parseNode() {
    if (pos >= str.length || str[pos] !== "[") return null;
    pos++; // skip '['

    if (str[pos] === "(") {
      // Internal node: [(id, dist) left right]
      pos++; // skip '('
      let idStr = "";
      while (pos < str.length && str[pos] !== ",") idStr += str[pos++];
      pos++; // skip ','
      while (pos < str.length && str[pos] === " ") pos++;
      let distStr = "";
      while (pos < str.length && str[pos] !== ")") distStr += str[pos++];
      pos++; // skip ')'
      while (pos < str.length && str[pos] === " ") pos++;

      const children = [];
      while (pos < str.length && str[pos] === "[") {
        const child = parseNode();
        if (child) children.push(child);
      }
      if (pos < str.length && str[pos] === "]") pos++; // skip ']'

      return {
        name: idStr.trim(),
        distance: parseFloat(distStr.trim()),
        children: children.length > 0 ? children : undefined,
      };
    } else {
      // Leaf node: [id]
      let id = "";
      while (pos < str.length && str[pos] !== "]") id += str[pos++];
      pos++; // skip ']'
      return { name: id.trim(), distance: 0 };
    }
  }

  return parseNode();
}

/* ---------- Distance table parser ---------- */

function parseDistanceTable(text) {
  const lines = text.split("\n").filter((l) => l.trim());
  const ids = [];
  const rowMap = {};

  for (const line of lines) {
    const match = line.match(/^(\S+):\s*(.*)/);
    if (!match) continue;
    const id = match[1];
    ids.push(id);
    const distances = {};
    const pairs = match[2].matchAll(/(\S+)\s+\(([^)]+)\)/g);
    for (const p of pairs) {
      distances[p[1]] = parseFloat(p[2]);
    }
    rowMap[id] = distances;
  }

  // Build symmetric rows
  const rows = ids.map((id) => {
    const dists = {};
    for (const otherId of ids) {
      if (otherId === id) continue;
      dists[otherId] =
        rowMap[id]?.[otherId] ?? rowMap[otherId]?.[id] ?? null;
    }
    return { id, distances: dists };
  });

  return ids.length > 0 ? { ids, rows } : null;
}

/* ---------- Species list parser ---------- */

function parseSpeciesList(text) {
  const lines = text.split("\n").filter((l) => l.trim());
  const species = [];
  for (const line of lines) {
    const parts = line.trim().split(/\s+/);
    if (parts.length >= 2) {
      species.push({ id: parts[0], gen: parts.slice(1).join("") });
    }
  }
  return species.length > 0 ? species : null;
}

/* ---------- D3 tree rendering ---------- */

function renderTree(container, treeData) {
  if (!container || !treeData) return;
  container.innerHTML = "";

  const margin = { top: 20, right: 140, bottom: 20, left: 40 };

  const root = d3.hierarchy(treeData);
  const leaves = root.leaves().length;
  const height = Math.max(leaves * 32, 120);
  const width = Math.max(500, container.clientWidth - margin.left - margin.right);

  const svg = d3
    .select(container)
    .append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom);

  const g = svg
    .append("g")
    .attr("transform", `translate(${margin.left},${margin.top})`);

  const tree = d3.tree().size([height, width]);
  tree(root);

  // Links (elbow connectors)
  g.selectAll(".link")
    .data(root.links())
    .join("path")
    .attr("class", "link")
    .attr("d", (d) =>
      `M${d.source.y},${d.source.x}` +
      `H${d.target.y}` +
      `V${d.target.x}`
    );

  // Nodes
  const node = g
    .selectAll(".node")
    .data(root.descendants())
    .join("g")
    .attr("transform", (d) => `translate(${d.y},${d.x})`);

  node
    .append("circle")
    .attr("class", "node-dot")
    .attr("r", 3.5);

  // Labels
  node
    .append("text")
    .attr("class", "node-label")
    .attr("dy", "0.32em")
    .attr("x", (d) => (d.children ? -8 : 8))
    .attr("text-anchor", (d) => (d.children ? "end" : "start"))
    .text((d) => d.data.name);

  // Distance labels on internal nodes
  node
    .filter((d) => d.data.distance > 0)
    .append("text")
    .attr("class", "node-dist")
    .attr("dy", "-0.8em")
    .attr("x", -8)
    .attr("text-anchor", "end")
    .text((d) => d.data.distance.toFixed(4));
}

/* ---------- Vue App ---------- */

createApp({
  setup() {
    const alive = ref(false);
    const kValue = ref(null);
    const kInput = ref(3);
    const selectedCommand = ref("crea_especie");
    const loading = ref(false);
    const lastOutput = ref(null);
    const showRaw = ref(false);
    const history = ref([]);
    const treeContainer = ref(null);

    const formData = ref({
      id: "",
      gen: "",
      id1: "",
      id2: "",
      speciesText: "",
    });

    const commandGroups = computed(() =>
      GROUPS.map((label) => ({
        label,
        commands: COMMANDS.filter((c) => c.group === label),
      }))
    );

    const selectedCommandLabel = computed(
      () => COMMANDS.find((c) => c.name === selectedCommand.value)?.label ?? ""
    );

    const isError = computed(
      () => lastOutput.value && lastOutput.value.startsWith("ERROR")
    );

    const distanceTable = computed(() => {
      if (!lastOutput.value) return null;
      const cmd = selectedCommand.value;
      if (
        [
          "tabla_distancias",
          "inicializa_clusters",
          "ejecuta_paso_wpgma",
        ].includes(cmd)
      ) {
        return parseDistanceTable(lastOutput.value);
      }
      return null;
    });

    const speciesTable = computed(() => {
      if (!lastOutput.value) return null;
      if (selectedCommand.value === "imprime_cjt_especies") {
        return parseSpeciesList(lastOutput.value);
      }
      return null;
    });

    const treeData = computed(() => {
      if (!lastOutput.value) return null;
      const cmd = selectedCommand.value;
      if (
        ["imprime_arbol_filogenetico", "imprime_cluster"].includes(cmd) &&
        !isError.value
      ) {
        const trimmed = lastOutput.value.trim();
        if (trimmed.startsWith("[")) {
          return parseTree(trimmed);
        }
      }
      return null;
    });

    const reversedHistory = computed(() => [...history.value].reverse());

    watch(treeData, async (data) => {
      if (data) {
        await nextTick();
        renderTree(treeContainer.value, data);
      }
    });

    function selectCommand(name) {
      selectedCommand.value = name;
      formData.value = { id: "", gen: "", id1: "", id2: "", speciesText: "" };
      lastOutput.value = null;
    }

    async function api(path, body) {
      const resp = await fetch(path, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(body),
      });
      if (!resp.ok) {
        const err = await resp.json().catch(() => ({ detail: "Request failed" }));
        throw new Error(err.detail || "Request failed");
      }
      return resp.json();
    }

    async function initProcess() {
      loading.value = true;
      try {
        await api("/api/init", { k: kInput.value });
        alive.value = true;
        kValue.value = kInput.value;
        history.value = [];
        lastOutput.value = null;
      } catch (e) {
        alert("Failed to initialize: " + e.message);
      } finally {
        loading.value = false;
      }
    }

    async function resetProcess() {
      alive.value = false;
      kValue.value = null;
      lastOutput.value = null;
      history.value = [];
    }

    async function executeCommand() {
      const cmd = selectedCommand.value;
      loading.value = true;
      lastOutput.value = null;

      try {
        let result;

        if (cmd === "lee_cjt_especies") {
          const lines = formData.value.speciesText
            .trim()
            .split("\n")
            .filter((l) => l.trim());
          const species = lines.map((l) => {
            const parts = l.trim().split(/\s+/);
            return { id: parts[0], gen: parts.slice(1).join("") };
          });
          result = await api("/api/read_species", { species });
        } else {
          const args = buildArgs(cmd);
          result = await api("/api/command", { command: cmd, args });
        }

        lastOutput.value = result.output || "(no output)";
        history.value.push({
          command: cmd,
          output: lastOutput.value,
          isError: lastOutput.value.startsWith("ERROR"),
        });
      } catch (e) {
        lastOutput.value = "ERROR: " + e.message;
        if (e.message.includes("not running")) {
          alive.value = false;
        }
      } finally {
        loading.value = false;
      }
    }

    function buildArgs(cmd) {
      const f = formData.value;
      switch (cmd) {
        case "crea_especie":
          return [f.id, f.gen];
        case "obtener_gen":
        case "elimina_especie":
        case "existe_especie":
        case "imprime_cluster":
          return [f.id];
        case "distancia":
          return [f.id1, f.id2];
        default:
          return [];
      }
    }

    // Check status on load
    fetch("/api/status")
      .then((r) => r.json())
      .then((data) => {
        alive.value = data.alive;
        kValue.value = data.k;
      })
      .catch(() => {});

    return {
      alive,
      kValue,
      kInput,
      selectedCommand,
      loading,
      lastOutput,
      showRaw,
      history,
      formData,
      commandGroups,
      selectedCommandLabel,
      isError,
      distanceTable,
      speciesTable,
      treeData,
      reversedHistory,
      treeContainer,
      selectCommand,
      initProcess,
      resetProcess,
      executeCommand,
    };
  },
}).mount("#app");
