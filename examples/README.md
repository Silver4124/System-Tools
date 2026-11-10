# 🧪 Examples
### - ESP
Un ejemplo por tema, comentado en español y en inglés dentro del mismo fichero. Ninguno obliga a compilar la librería aparte: cada carpeta lleva un `System_Tools.h` de treinta bytes que reenvía al de la raíz, así que cualquier ejemplo se compila desde su propia carpeta y sin banderas de include.

```sh
cd examples/file
g++ -std=c++11 -O2 -Wall write_read.cpp -o write_read
```

Desde la raíz, `make` construye los treinta y seis ejemplos de las ocho carpetas en `bin/<carpeta>/<nombre>`, con el sufijo `.exe` que añade el compilador en Windows. La novena carpeta, [`counterexamples/`](counterexamples/README.md), queda fuera a propósito: sus seis ficheros están escritos para fallar y su README dice cuál es el fallo esperado de cada uno.

## - Qué hace falta para ejecutar cada ejemplo
Veintidós corren en cualquier terminal sin pedir nada más: cuatro de `defaults/`, cuatro de `console/`, ocho de `file/`, dos de `json/`, dos de `memory/` y dos de `thread/`. Los catorce restantes piden algo que una terminal no puede darles, y por eso se comprueban compilando.

| Ficheros | Cuántos | Qué piden | Por qué no vale con ejecutarlos |
| ---- | ---- | ---- | ---- |
| `defaults/message_dialog.cpp`, `defaults/message_variants.cpp` | 2 | escritorio y un clic | `message()` abre el cuadro modal y no devuelve nada hasta que alguien responde |
| `defaults/read_input.cpp`, `console/read_line.cpp` | 2 | teclado | se quedan parados esperando un Enter; redirigir una línea también vale |
| `window/` (los cinco) | 5 | sesión gráfica | cada uno crea una o varias ventanas y espera sus eventos |
| `sound/` (los cinco) | 5 | dispositivo de reproducción | sueltan audio real por la salida del sistema |

## - Los recursos que se generan
Ningún ejemplo descarga nada ni arrastra ficheros binarios: todos se fabrican lo que necesitan, con prefijo `st_`, y se lo borran al terminar. Ejecútalos desde una carpeta donde se pueda escribir; si la corrida se interrumpe a mitad, esos `st_*` quedan tirados por ahí y el [`.gitignore`](../.gitignore) ya se encarga de que no lleguen al historial.

## - Orden de lectura sugerido
Empieza por [`defaults/hello_output.cpp`](defaults/hello_output.cpp) y [`memory/blocks.cpp`](memory/blocks.cpp), sigue con [`file/write_read.cpp`](file/write_read.cpp), [`json/build_object.cpp`](json/build_object.cpp) y [`thread/run_join.cpp`](thread/run_join.cpp), y cierra con el módulo que te interese: los cuatro de consola que no leen teclado, los cinco de ventanas y los cinco de audio.

### - ENG
One example per topic, commented in Spanish and English inside the same file. None of them forces you to compile the library separately: every folder carries a thirty-byte `System_Tools.h` that forwards to the root one, so any example compiles from inside its own folder with no include flags.

```sh
cd examples/file
g++ -std=c++11 -O2 -Wall write_read.cpp -o write_read
```

From the repository root, `make` builds the thirty-six examples of the eight folders into `bin/<folder>/<name>`, with the `.exe` suffix the compiler adds on Windows. The ninth folder, [`counterexamples/`](counterexamples/README.md), is left out on purpose: its six files are written to fail, and its README spells out which failure each one proves.

## - What running each example takes
Twenty-two of them run in any terminal asking for nothing else: four in `defaults/`, four in `console/`, eight in `file/`, two in `json/`, two in `memory` and two in `thread/`. The remaining fourteen want something a terminal cannot give them, which is why they get checked by compiling instead.

| Files | How many | What they need | Why running them is not enough |
| ---- | ---- | ---- | ---- |
| `defaults/message_dialog.cpp`, `defaults/message_variants.cpp` | 2 | a desktop and one click | `message()` opens the modal box and returns nothing until somebody answers |
| `defaults/read_input.cpp`, `console/read_line.cpp` | 2 | a keyboard | they sit waiting for an Enter; piping a line works too |
| `window/` (all five) | 5 | a graphical session | each one creates one or more windows and waits for their events |
| `sound/` (all five) | 5 | an output device | they push real audio through the system output |

## - The resources they generate
No example downloads anything or drags binary files along: every one manufactures what it needs, with the `st_` prefix, and deletes it before finishing. Run them from a folder where writing is allowed; if a run gets interrupted halfway, those `st_*` files stay behind and the [`.gitignore`](../.gitignore) already keeps them out of the history.

## - Suggested reading order
Start with [`defaults/hello_output.cpp`](defaults/hello_output.cpp) and [`memory/blocks.cpp`](memory/blocks.cpp), continue through [`file/write_read.cpp`](file/write_read.cpp), [`json/build_object.cpp`](json/build_object.cpp) and [`thread/run_join.cpp`](thread/run_join.cpp), then close with whichever module interests you: the four console examples that read no keyboard, the five windows ones and the five audio ones.
