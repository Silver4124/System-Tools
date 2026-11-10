# 🚫 Counterexamples
### - ESP
Esta carpeta **no es un conjunto de ejemplos para usar**: es el reverso de `examples/`. Cada fichero de aquí está escrito a propósito para fallar, y el fallo es el contenido del ejemplo. Sirven para dos cosas: dejar por escrito dónde están los bordes filosos de la librería, y dar una prueba reproduzible de cada borde.

Hay dos familias:

* **No compilan, o compilan y no enlazan.** El fallo lo da el compilador o el enlazador. Son el grupo más útil: el error te avisa antes de ejecutar nada, y la mayoría se arregla con un cast o con la firma correcta.
* **Compilan, corren, y fallan (o mienten) en tiempo de ejecución.** Este grupo es el delicado: unos tumban el proceso (segmentation fault), otros entregan un resultado equivocado sin avisar. Ninguno toca ficheros del disco, pero sí pueden dejar memoria mala detrás.

### Cómo probar uno
Desde la carpeta `examples/counterexamples`, con el uso de `g++`:

```
g++ -std=c++11 -O2 -Wall json_add_elem_literal.cpp -o prueba
```

* Si es de la primera familia, el comando **debe fallar**; la línea `Fallo:` del encabezado de cada fichero dice exactamente qué mensaje espera ver.
* Si es de la segunda, el binario se construye bien y el fallo sale al ejecutarlo. En MSYS bash un `segmentation fault` se reporta como código `139`; en `cmd` como `-1073741819` (`0xC0000005`).
* Cuando el programa se cae puede no imprimir nada, ni siquiera las líneas previas: la salida de `printf` va bufferizada y el corte abrupto se la lleva.
* Cada fichero trae la versión correcta comentada debajo del fallo. Para verla compilar hay que comentar primero la línea que falla, porque las dos juntas chocan entre sí (repiten el nombre o la firma).

### Aviso de compilación global
El `Makefile` y el `CMakeLists.txt` de la raíz barrén `examples/*/*.cpp`. Esta carpeta está excluida de ese barrido en ambos: si se metiera en el build, `make` fallaría siempre, que es justo lo que estos fichero provocan. Para compilar cualquiera hay que llamarlo a mano, uno por uno.

### No es una lista de tareas
Los tres ficheros que probaban defectos de la librería se fueron al repararse esos defectos: `file::normalize` ahora convierte y colapsa, `memory::cmplloc` enlaza con el tamaño en `UWORD`, y `json::mve_elem` admite una etiqueta de otra medida. La conversión de separadores y las tres comprobaciones del JSON —el negativo del `parse`, el texto decimal pasado a entero y el rename de otra medida— se siguen haciendo dentro de los ejemplos normales [file/paths](../file/paths.cpp), [json/parse_walk](../json/parse_walk.cpp) y [json/build_object](../json/build_object.cpp).\
Que un fichero de aquí falle ya no significa que la librería esté rota: significa que la firma o la sobrecarga es así de filosa, y que lo que hay que ajustar es la llamada.

### Índice
| Fichero | Familia | Lo que pasa | La salida |
| ---- | ---- | ---- | ---- |
| [message_mixed_literals.cpp](message_mixed_literals.cpp) | No compila | Literal ancha y literal de bytes en la misma llamada: ambigüedad | Escribir los dos lados del mismo tipo, o pasar el lado ancho por `(const void*)` |
| [json_add_elem_literal.cpp](json_add_elem_literal.cpp) | No compila | Literal de texto donde `json` pide `void*` | Cast `(void*)"nombre"` en cada etiqueta e índice |
| [sound_stream_const_buffer.cpp](sound_stream_const_buffer.cpp) | No compila | Buffer `const` donde `sound::stream` pide `void*` | Quitar el `const`, o copiar a un buffer propio |
| [window_mailman_signature.cpp](window_mailman_signature.cpp) | No compila | Mailman declarada con `unsigned int` e `int` | Usar los tipos del módulo: `UWORD(*fn)(void*,UWORD,UWORD,WORD)` |
| [normalize_string_literal.cpp](normalize_string_literal.cpp) | Falla al correr | `file::normalize` escribe sobre un literal de solo lectura | Normalizar siempre sobre un buffer escribible |
| [console_read_nullptr_literal.cpp](console_read_nullptr_literal.cpp) | No hace nada | `console::read` con `nullptr` literal elige la sobrecarga que rechaza la lectura | Pasar un puntero de longitud válido y, si se quiere descartar el texto, su variable aparte |

### - ENG
This folder **is not a set of examples to use**: it is the back side of `examples/`. Every file here is written on purpose to fail, and the failure is the content of the example. They serve two goals: putting on paper where the sharp edges of the library are, and giving a reproducible proof for each edge.

There are two families:

* **They do not compile, or they compile and do not link.** The failure comes from the compiler or from the linker. This is the most useful group: the error warns you before anything runs, and most of them are fixed with a cast or with the right signature.
* **They compile, they run, and they fail (or lie) at run time.** This group is the delicate one: some drop the process (segmentation fault), others hand over a wrong result without warning. None of them touches files on disk, but they can leave bad memory behind.

### How to try one
From the `examples/counterexamples` folder, with the use of `g++`:

```
g++ -std=c++11 -O2 -Wall json_add_elem_literal.cpp -o prueba
```

* If it belongs to the first family, the command **must fail**; the `Fallo:` line in each file header says the exact message to expect.
* If it belongs to the second, the binary builds fine and the failure shows up when running it. In MSYS bash a `segmentation fault` reports as code `139`; in `cmd` as `-1073741819` (`0xC0000005`).
* When the program drops it may print nothing, not even the previous lines: the `printf` output is buffered and the abrupt cut takes it away.
* Each file carries the correct version commented out under the failure. To watch it compile, comment the failing line first, because the two together collide with each other (they repeat the name or the signature).

### Notice about the global build
The `Makefile` and the `CMakeLists.txt` at the root sweep `examples/*/*.cpp`. This folder is excluded from that sweep in both: had it stayed inside the build, `make` would fail always, which is exactly what these files cause. To compile any of them you have to call it by hand, one at a time.

### This is not a to-do list
The three files that proved library defects left when those defects got repaired: `file::normalize` now translates and collapses, `memory::cmplloc` links with the size in `UWORD`, and `json::mve_elem` takes a label of another measure. The separator conversion and the three JSON checks —the negative coming from `parse`, the decimal text turned into an integer and the rename to another measure— keep being made inside the regular examples [file/paths](../file/paths.cpp), [json/parse_walk](../json/parse_walk.cpp) and [json/build_object](../json/build_object.cpp).\
A file here failing no longer means the library is broken: it means the signature or the overload is that sharp, and it is the call what has to be adjusted.

### Index
| File | Family | What happens | The way out |
| ---- | ---- | ---- | ---- |
| [message_mixed_literals.cpp](message_mixed_literals.cpp) | Does not compile | One wide literal and one byte literal in the same call: ambiguity | Write both sides in the same type, or pass the wide side through `(const void*)` |
| [json_add_elem_literal.cpp](json_add_elem_literal.cpp) | Does not compile | A text literal where `json` asks for `void*` | Cast `(void*)"nombre"` on every label and index |
| [sound_stream_const_buffer.cpp](sound_stream_const_buffer.cpp) | Does not compile | A `const` buffer where `sound::stream` asks for `void*` | Drop the `const`, or copy into a buffer of your own |
| [window_mailman_signature.cpp](window_mailman_signature.cpp) | Does not compile | Mailman declared with `unsigned int` and `int` | Use the module types: `UWORD(*fn)(void*,UWORD,UWORD,WORD)` |
| [normalize_string_literal.cpp](normalize_string_literal.cpp) | Fails when running | `file::normalize` writes over a read-only literal | Always normalize over a writable buffer |
| [console_read_nullptr_literal.cpp](console_read_nullptr_literal.cpp) | Does nothing | `console::read` with a literal `nullptr` picks the overload that refuses the reading | Pass a real length pointer, and if the text is to be thrown away, its own separate variable |
