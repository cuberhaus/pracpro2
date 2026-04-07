**Nombre del Proyecto:** Práctica PRO2 Primavera 2020  
Versión: 1.0  
Lenguaje i Versión: C++ 17  
El programa principal se encuentra en el módulo program.cc.
Para Almacenar y tratar los datos descritos en el enunciado de la práctica,
usaremos el módulo especie para representar las especies son sus
identificadores y genes, el módulo cjt_especies para representar un conjunto
de especies donde almacenaremos las distancias entre especies i el conjunto.
Para los clusters usaremos el módulo cluster i el módulo cjt_clusters para
almacenar un conjunto de estos.

**Funciones:**
  1. _crea_especie:_ Crea una especie con el identificador y gen (dos strings) dados. Escribe un mensaje de error si ya existe una especie con el mismo identificador. La especie creada, si no hay error, se agrega al conjunto de especies.
  2. _obtener_gen:_ Dado un identificador de especie, imprime el gen asociado a la especie. Escribe un mensaje de error si no existe una especie con el identificador dado.
  3. _distancia:_ Dados dos identificadores de especies, imprime la distancia entre las dos especies. Se escribe un mensaje de error si alguna de las dos especies cuyos identificadores se dan no existen.
  4. _elimina_especie:_ Dado el identificador de una especie e la elimina del conjunto de especies. Escribe un mensaje de error si la especie con el identificador dado no existe.
  5. _existe_especie:_ Dado el identificador de una especie e imprime una indicación de si dicha especie existe.
  6. _lee_cjt_especies:_ Lee del canal estándar de entrada un entero n ≥ 0 y a continuación una secuencia de n especies. Las n especies dadas tienen identificadores distintos entre sí. Los contenidos previos del conjunto de especies se descartan  y las n especies leídas se agregan al conjunto de especies.
  7. _imprime_cjt_especies:_ Imprime en el canal estándar de salida el conjunto de especies.
  8. _tabla_distancias:_ Imprime la tabla de distancias entre cada par de especies del conjunto de especies.
  9. _inicializa_clusters:_ Inicializa el conjunto de clústers con el conjunto de especies en el estado en el que esté en ese momento, e imprime la tabla de distancias entre clústers. Al imprimir la tabla de distancias se usarán los identificadores de los clústers para indexar filas y columnas.
  10. _ejecuta_paso_wpgma:_ ejecuta un paso del algoritmo WPGMA (fusiona los dos clústers a menor distancia en uno nuevo) e imprime la tabla de distancias entre clústers resultante. Al imprimir la tabla de distancias se usarán los identificadores de los clústers para indexar filas y columnas. En caso de que el número de clústers del conjunto sea menor o igual que uno solamente se debe imprimir un mensaje de error.
  11. _imprime_cluster:_ dado un identificador α, imprime el clúster con el identificador dado, o un error si no existe un clúster con dicho identificador en el conjunto de clústers.
  12. _imprime_arbol_filogenetico:_ imprime el árbol filogenético para el conjunto deespecies actual; dicho árbol es el clúster que agrupa todas las especies, resultante de aplicar el algoritmo WPGMA. El contenido del conjunto de clústers previo se descarta y se reinicializa con el conjunto de especies en el estado en el que esté en ese momento, para a continuación aplicar el algoritmo. El conjunto de clústers final es el que queda después de aplicar el algoritmo.
  Se imprimirá la estructura arborescente del clúster con los identificadores de los clústers (raíces de los subárboles) y la distancia entre cada clúster y sus hojas descendientes.
  13. _fin:_ finaliza la ejecución del programa.

---

## Compilación y Ejecución

El proyecto incluye un `Makefile` para gestionar la compilación del código C++ (estándar C++11).

Para compilar el proyecto y generar el ejecutable `program.exe`:
```bash
make
```

Para limpiar los archivos objeto y ejecutables generados:
```bash
make clean
```

Para ejecutar el programa de forma manual:
```bash
./program.exe
```

---

## Testeo (Jocs de Prova)

La carpeta `jocs_de_prova` contiene juegos de prueba para validar el correcto funcionamiento del algoritmo WPGMA y la gestión de especies.

### Cómo ejecutar los tests manualmente

1. **Compilar el programa**: Asegúrate de haber ejecutado `make`.
2. **Ejecutar con redirección**: Usa los archivos de entrada (`.txt` o `.input`) y redirige la salida a un archivo temporal.
   ```bash
   ./program.exe < jocs_de_prova/entrada.txt > mi_salida.txt
   ```
3. **Comparar resultados**: Usa el comando `diff` para comparar tu salida con la salida correcta esperada.
   ```bash
   diff mi_salida.txt jocs_de_prova/correct.txt
   ```
   Si el comando no devuelve nada, significa que la salida es idéntica y el test ha pasado correctamente.

### Generación de documentación
Puedes generar la documentación técnica en HTML y LaTeX usando Doxygen:
```bash
make html
```
La documentación se generará en las carpetas `html/` y `latex/`.

---

## Web App

An interactive web frontend for the WPGMA clustering algorithm: input species with gene sequences and watch the algorithm merge clusters step by step into a dendrogram.

**Stack:** Vue 3 (CDN) + D3.js dendrogram + FastAPI backend wrapping the compiled C++ engine

### Quick Start

```bash
# Docker (recommended)
docker build -t pracpro2 . && docker run -p 8000:8000 pracpro2
# http://localhost:8000

# Dev mode (requires compiled program.exe)
make
cd web && pip install -r requirements.txt
uvicorn app:app --host 127.0.0.1 --port 8000 --reload
```

### Features

- Add/remove species with gene sequences through an interactive form
- Step-by-step WPGMA execution with animated D3.js dendrogram visualization
- Distance matrix display updated after each merge step
- Full phylogenetic tree generation from current species set

### Web Structure

```
web/
├── static/
│   ├── index.html     # Vue 3 + D3.js single-page app
│   ├── app.js         # Vue application logic
│   └── style.css      # Dark theme CSS
├── app.py             # FastAPI wrapping C++ program.exe via stdin/stdout
└── requirements.txt
```
