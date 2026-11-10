# alloc
### - ESP
Firma:
```cpp
void* alloc(UWORD size);
```
Parámetros:
* **Size**: Número entero sin signo del tamaño de la palabra del procesador, que representa el tamaño en bytes que va a tener el buffer a crear.

Retorno:\
Puntero a la dirección en memoria del buffer creado.\
`nullptr` en caso de que la cantidad de memoria que se deseaba reservar era `0`, o si hubo algún fallo.

\
Comportamiento:\
Adaptación de `malloc` de la librería estándar de C (véase [Administración de la memoria](README.md)).\
Reserva espacio nuevo en el **Heap** del programa, respetando su paginación, por la cantidad de bytes indicada en **Size**.\
En Linux los bytes llegan puestos a `0`; en Windows no, salen tal como estuvieran guardados.\
Cuando el búfer deba arrancar en cero, conviene limpiarlos con `memory::slloc`.\
Conserva una copia del puntero origen, o del puntero ya reubicado (preferiblemente este último si se reubicó), para su posterior eliminación con `memory::free` y no fugar memoria.\
Al ser memoria de manejo C, no la manipules con `new` ni `delete`.

### - ENG
Signature:
```cpp
void* alloc(UWORD size);
```
Parameters:
* **Size**: Unsigned integer with the processor word size, which represents the size in bytes of the buffer to be created.

Return:\
Pointer to the memory address of the created buffer.\
`nullptr` if the amount of memory requested was `0`, or if a failure occurred.

\
Behavior:\
Adaptation of `malloc` from the C standard library (see [Memory Management](README.md)).\
It reserves new space in the program's **Heap**, respecting its pagination, for the amount of bytes given by **Size**.\
On Linux the bytes arrive set to `0`; on Windows they do not, they come out as they were stored.\
When the buffer must start at zero, it is worth clearing them with `memory::slloc`.\
Keep a copy of the original pointer, or of the relocated one (preferably the latter if relocation was performed), to free it later with `memory::free` and avoid leaking memory.\
Since this is C-managed memory, do not manipulate it with `new` or `delete`.