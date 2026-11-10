# 📖 Changelog
### - ESP
Esta hoja arranca tarde, ya que el proyecto no tenia repositorio git, así que no hay historial antes de esta versión y todo lo anterior a 2026-11-07 se lista solo como estado, no como fecha de cambio. Cada entrada separa lo que cambia respecto a la versión anterior, lo que se arregló en el código, lo que se añadió alrededor de la librería y lo que se hereda del diseño.

## - v1.5 — 2026-11-10
* **Macros de argumentación renombradas y pasadas a públicas.** `GET_VA_ARGS` se volvió `VA_START` y `GET_IV_ARG` se volvió `VA_PEEK`, las dos al alcance del programador, y la familia se completa con `VA_NEXT`, `VA_RESET`, `VA_COPY` y `VA_END`.
* **Macros de plataforma añadidas.** La familia `*WORD_PRINT` (`WORD_PRINT`, `UWORD_PRINT`, `OWORD_PRINT`, `XWORD_PRINT`, con `CWORD_PRINT` de helper), `WORD_SIZE`, `ENDIAN_MULT`, `RESTRICT`, `FORCEINLINE`, `ALWAYSINLINE` y `NOINLINE`.
* **`IS_BIG_ENDIAN` independizada.** Dejó de ser el antónimo directo `(!IS_LITTLE_ENDIAN)` y se define por su cuenta, a 0 o a 1 según el test de extremo.
* **Guardias de inclusión circular reforzados.** `__SYSTEM_TOOLS__H_FIRST__` y `__SYSTEM_TOOLS__CPP_FIRST__` ya cortaban las definiciones circulares, ahora lo hacen con más seguridad y además distinguen cuál de los dos ficheros entró primero: el `.cpp` se marca a 1 o a 2 según encuentre o no la cabecera delante, y en el segundo caso le devuelve la marca a ella.
* **El `namespace memory` asigna dentro del heap del programa.** `alloc`, `realloc`, `free` y `szlen` pasan por `GetProcessHeap` con `HeapAlloc`, `HeapReAlloc`, `HeapFree` y `HeapSize` en Windows, y por `malloc`, `realloc`, `free` y `malloc_usable_size` en Linux. Antes no era así, y de ahí salían los crasheos espontáneos que el desarrollador no podía corregir sin meter la mano en la librería.
* **Fuera `ST_VERSION` y `ST_VERDATE`.** El versionado pasa a un sistema interno y de verificación de contenido, más mantenible y más fácil de depurar: cada módulo redefine su macro `ST_IMPORT_*` al número de su firma posicional dentro de la cabecera —Ej.: `json` 1, `file` 2, `sound` 3, `console` 4, `window` 5, etc.—, así que tras el `#include` basta leer el valor para saber si el módulo está dentro y si la versión coincide; `ST_NO_DEFAULT` se resuelve a `(~0)` por el mismo camino.
* **Funciones añadidas.** `get_system_variable`, `sscanf`, `mbstowcs`, `wcstombs`, `memory::szlen` y `memory::on_stack`.
* **Módulo eliminado: `ST_IMPORT_INPUT`.** Metía funciones basura o poco que ver con su nombre. Las relevantes se movieron o se integraron: `input::mouse` a `window::mouse`, `input::keyboard` a `window::keyboard`, `input::language` a `language`, `input::set_console_layout` a `console::setunicode` e `input::gotoxy` a `console::gotoxy`; el resto del módulo se borró.
* **Módulos añadidos: `ST_IMPORT_JSON` y `ST_IMPORT_CONSOLE`.** Los dos módulos nuevos de esta actualización, con su documentación, sus ejemplos y su puesto en el sistema de macros.
* `sound::stop` sobre un handle con hilo ahora pausa de verdad: el alimentador mira `wants_playback`, que `stop` limpia.
* `file::root_this` dejaba de recortar la ruta; `file::remove` en Linux borraba solo ficheros y ahora es recursivo, como en Windows.
* Se añadió `window::release`, que suelta el handle de una ventana ajena sin tocar la ventana.
* Ahora la librería emite sus definiciones como `inline` y deja de exigir una sola unidad de traducción: dos `.cpp` que incluyen la cabecera enlazan sin ningún `multiple definition`. Las cuatro variádicas llevan `NOINLINE` porque el andamiaje `VA_START` de Windows recorre la pila a mano; sin esa marca `console/color_text` se cortaba dentro de `sprintf` con `-O2` o superior.
* El bloque que publica `printf`, `system`, `alloc` y compañía los sigue publicando —un `printf` sin prefijo es el de la librería, sin `using namespace`—, pero ya no escribe `_INC_STDIO` ni `_INC_STDLIB`: esos nombres solo se leen, y `<stdio.h>` y `<stdlib.h>` dejan de desaparecer en silencio llevándose sus tipos. `ST_NO_GLOBAL_FUNCTIONS` apaga la publicación.
* `BYTE`, `wchar`, `WORD` y `UWORD` pasan a `namespace System_Tools` y solo se publican en el espacio global mientras el SDK no esté en la unidad, con lo que el `WORD` de dieciséis bits de `<windows.h>` deja de sustituir al de los recorridos de cabeceras de la librería; `ST_NO_GLOBAL_TYPES` decide lo mismo en el orden inverso.
* `file::normalize` convierte el separador ajeno y colapsa las sucesiones: Se arregló el bug que causaba que rutas como `C:/dir/file.txt` o `C:\dir\file.txt` se convirtieran en `C:dirfile.txt`, sumado al hecho de que ahora la cadena resultante nunca sale más larga que la entrada.
* `memory::cmplloc` se define con el `UWORD size` que prometía el encabezado: en 64 bits la llamada con el tamaño en `UWORD` enlaza.
* El reconstruido del pitch con handle de hilo respeta `owns_master`: si el handle era dueño del volumen maestro recibe un `float` nuevo, y si era prestado sigue apuntando al del llamante.
* `-Wall` sobre la librería: de 286 avisos a 0, medido con `-O0`, `-O1` y `-O2` en C++11, C++17 y C++20.
* Treinta y seis ejemplos en `examples/`, uno por tema, comentados en español y en inglés, más seis [`counterexamples/`](examples/counterexamples/README.md) que se quedan fuera de `make` porque están escritos para fallar a proporsito.
* Índice de [`examples/`](examples/README.md) diciendo qué necesita cada ejemplo para ejecutarse.
* Documentación bilingüe reescrita y reestructurada por módulo, eliminando así el archivo monolitico de documentación `documentation.html`.
* `Makefile` con detección del sistema por `uname`; implementación de `CMakeLists.txt` dentro del repositorio.
* `-ldl -lpthread` dejan de ser obligatorios en Linux: `make` y CMake consultan si `dlopen` y `pthread_create` ya están en `libc.so.6`, que es donde los busca `get_system_function` desde el glibc 2.34, y solo enlazan las dos librerías cuando la consulta falla.
* `.gitignore`, `.gitattributes` implementados en el repositorio, y un flujo de integración en GitHub que compila en Windows y Linux desde C++11 hasta C++20 o superior, y ejecuta solo los veintidós ejemplos sin interfaz.

Los tres límites de integración están reparados, pero el orden de las cabeceras sigue importando: `<stdio.h>`, `<stdlib.h>` o `<windows.h>` puestos despues de la librería chocan con los nombres publicados y piden `ST_NO_GLOBAL_FUNCTIONS` o `ST_NO_GLOBAL_TYPES`.


### - ENG
This sheet starts late: the project had no git repository, so there is no history before this version and everything earlier than 2026-11-07 is listed as state, not as a dated change. Every entry separates what changed against the previous version, what got repaired in the code, what was added around the library, and what comes from the design.

## - v1.5 — 2026-11-10
* **Argumentation macros renamed and made public.** `GET_VA_ARGS` became `VA_START` and `GET_IV_ARG` became `VA_PEEK`, both within the programmer's reach, and the family is completed by `VA_NEXT`, `VA_RESET`, `VA_COPY` and `VA_END`.
* **Platform macros added.** The `*WORD_PRINT` family (`WORD_PRINT`, `UWORD_PRINT`, `OWORD_PRINT`, `XWORD_PRINT`, with `CWORD_PRINT` as helper), `WORD_SIZE`, `ENDIAN_MULT`, `RESTRICT`, `FORCEINLINE`, `ALWAYSINLINE` and `NOINLINE`.
* **`IS_BIG_ENDIAN` made independent.** It stopped being the direct antonym `(!IS_LITTLE_ENDIAN)` and gets defined on its own now, as 0 or as 1, following the endianness test.
* **Circular-inclusion guards reinforced.** `__SYSTEM_TOOLS__H_FIRST__` and `__SYSTEM_TOOLS__CPP_FIRST__` already cut the circular definitions, they do it more safely now and they also tell which of the two files came first: the `.cpp` marks itself 1 or 2 depending on whether it finds the header ahead, and in the second case it hands the mark back to the header.
* **The `namespace memory` allocates inside the program heap.** `alloc`, `realloc`, `free` and `szlen` go through `GetProcessHeap` with `HeapAlloc`, `HeapReAlloc`, `HeapFree` and `HeapSize` on Windows, and through `malloc`, `realloc`, `free` and `malloc_usable_size` on Linux. That was not the case before, and out of it came the spontaneous crashes the developer could not fix without reaching inside the library.
* **`ST_VERSION` and `ST_VERDATE` gone.** Versioning moves to an internal system that checks content, easier to maintain and to debug: every module redefines its `ST_IMPORT_*` macro with the number of its positional signature inside the header —example: `json` 1, `file` 2, `sound` 3, `console` 4, `window` 5, and so on—, so after the `#include` it is enough to read the value to know whether the module is in and whether the version matches; `ST_NO_DEFAULT` resolves to `(~0)` through the same road.
* **Functions added.** `get_system_variable`, `sscanf`, `mbstowcs`, `wcstombs`, `memory::szlen` and `memory::on_stack`.
* **Module removed: `ST_IMPORT_INPUT`.** It carried junk functions or ones with little to do with its name. The relevant ones moved or got integrated: `input::mouse` into `window::mouse`, `input::keyboard` into `window::keyboard`, `input::language` into `language`, `input::set_console_layout` into `console::setunicode` and `input::gotoxy` into `console::gotoxy`; the rest of the module was deleted.
* **Modules added: `ST_IMPORT_JSON` and `ST_IMPORT_CONSOLE`.** The two new modules of this update, with their documentation, their examples and their place in the macros system.
* `sound::stop` over a handle with a thread really pauses now: the feeder checks `wants_playback`, which `stop` clears.
* `file::root_this` stopped truncating the path; `file::remove` on Linux deleted files only and is now recursive, same as on Windows.
* `window::release` was added, dropping the handle of a foreign window while leaving the window itself alone.
* The library now emits its definitions as `inline` and stops demanding a single translation unit: two `.cpp` files that include the header link without a single `multiple definition`. The four variadic ones carry `NOINLINE` because the Windows `VA_START` scaffolding walks the stack by hand; without that mark `console/color_text` died inside `sprintf` at `-O2` or above.
* The block that publishes `printf`, `system`, `alloc` and friends keeps publishing them — a bare `printf` is the library's one, with no `using namespace` — but it no longer writes `_INC_STDIO` or `_INC_STDLIB`: those names are only read now, and `<stdio.h>` and `<stdlib.h>` stop vanishing silently and taking their types with them. `ST_NO_GLOBAL_FUNCTIONS` switches the publishing off.
* `BYTE`, `wchar`, `WORD` and `UWORD` moved into `namespace System_Tools` and reach the global namespace only while the SDK is not in the unit, so the sixteen-bit `WORD` of `<windows.h>` stops rebinding the library's own header walks; `ST_NO_GLOBAL_TYPES` decides the same in the reverse order.
* `file::normalize` translates the foreign separator and collapses the runs: the bug that turned paths like `C:/dir/file.txt` or `C:\dir\file.txt` into `C:dirfile.txt` is gone, and on top of that the resulting string never comes out longer than the input.
* `memory::cmplloc` is defined with the `UWORD size` the header promised: on 64-bit builds the call with the size in a `UWORD` links.
* The threaded pitch rebuild honours `owns_master`: an owning handle gets a brand new `float`, a borrowing one keeps pointing at the caller address.
* `-Wall` over the library: from 286 warnings down to 0, measured at `-O0`, `-O1` and `-O2` on C++11, C++17 and C++20.
* Thirty-six examples under `examples/`, one per topic, commented in Spanish and English, plus six [`counterexamples/`](examples/counterexamples/README.md) that stay out of `make` because they are written to fail on purpose.
* An [`examples/`](examples/README.md) index stating what each example needs in order to run.
* Bilingual documentation rewritten and restructured per module, which took the monolithic `documentation.html` file out of the way.
* A `Makefile` detecting the system through `uname`; a `CMakeLists.txt` implemented inside the repository.
* `-ldl -lpthread` stop being mandatory on Linux: `make` and CMake ask whether `dlopen` and `pthread_create` already live in `libc.so.6`, which is where `get_system_function` looks for them since glibc 2.34, and link the two libraries only when that question comes back no.
* `.gitignore` and `.gitattributes` implemented in the repository, and a GitHub integration flow that compiles on Windows and Linux from C++11 up to C++20 or newer, and runs only the twenty-two interface-free examples.

The three integration limits are repaired, but the order of the headers still matters: `<stdio.h>`, `<stdlib.h>` or `<windows.h>` placed after the library clash with the published names and ask for `ST_NO_GLOBAL_FUNCTIONS` or `ST_NO_GLOBAL_TYPES`.
