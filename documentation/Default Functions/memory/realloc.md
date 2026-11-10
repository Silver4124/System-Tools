# realloc
### - ESP
Firma:
```cpp
void* realloc(void* ptr, UWORD size);
```
Parámetros:
* **Ptr**: Puntero al buffer en memoria a reubicar.
* **Size**: Número entero sin signo del tamaño de la palabra del procesador, que representa el nuevo tamaño en bytes para el buffer a reubicar.

Retorno:\
El puntero a la nueva dirección en memoria del buffer ya reubicado, en caso de éxito.\
`nullptr` en caso de fallo, o en caso de que el nuevo tamaño solicitado sea `0`.\
Los dos casos de `nullptr` no se distinguen entre sí: solicitar el tamaño `0` destruye el buffer (se delega en `free`) y también devuelve `nullptr` un fracaso real de reasignación.\
Si necesita saber si la memoria sobrevivió, no use `realloc(ptr, 0)` y llame directamente a `memory::free`, cuyo retorno permite diferenciar éxito de fallo.

\
Comportamiento:\
Cambia el tamaño de un buffer ubicado en el **Heap** del programa, reubicándolo y estirándolo o estrechándolo según **Size**.\
Mejoras de seguridad sobre la versión de C: con **Ptr** en `nullptr` actúa como `memory::alloc`; con **Size** en `0` actúa como `memory::free`.\
Al ser memoria de manejo C, no la manipules con `new` ni `delete`.\
Adaptación de `realloc` de `<stdlib.h>` (véase [Administración de la memoria](README.md)).

### - ENG
Signature:
```cpp
void* realloc(void* ptr, UWORD size);
```
Parameters:
* **Ptr**: Pointer to the buffer in memory to be relocated.
* **Size**: Unsigned integer with the processor word size, which represents the new size in bytes for the buffer to be relocated.

Return:\
The pointer to the new memory address of the relocated buffer upon success.\
`nullptr` upon failure or if the requested new size is `0`.\
The two `nullptr` cases are indistinguishable: requesting size `0` destroys the buffer (delegated to `free`) and a genuine allocation failure also returns `nullptr`.\
If you need to know whether the memory survived, do not use `realloc(ptr, 0)` and call `memory::free` directly, whose return value lets you tell success from failure.

\
Behavior:\
It changes the size of a buffer located in the program's **Heap**, relocating it and stretching or shrinking it according to **Size**.\
Security improvements over the C version: with **Ptr** at `nullptr` it acts like `memory::alloc`; with **Size** at `0` it acts like `memory::free`.\
Since this is C-managed memory, do not manipulate it with `new` or `delete`.\
Adaptation of `realloc` from `<stdlib.h>` (see [Memory Management](README.md)).