# free
### - ESP
Firma:
```cpp
void* free(void* ptr);
```
Parámetros:
* **Ptr**: Puntero del buffer en memoria a eliminar.

Retorno:\
`nullptr` en caso de exito. El valor pasado al argumento **Ptr** en caso de fallo.\
Convención a tener presente: el resultado invierte la lectura intuitiva de C, donde `free` no devuelve nada. Aquí un retorno falso (`nullptr`) significa que la liberación **si** ocurrió, por lo que la comprobación correcta es `if(!memory::free(p))` y no `if(memory::free(p))`. Tras una llamada exitosa, `p` queda colgado y no debe volver a usarse ni liberarse.

\
Comportamiento:\
Elimina memoria reservada en el **Heap** del programa, respetando su paginación.\
Al ser memoria de manejo C, no la manipules con `new` ni `delete`.\
Adaptación de `free` de `<stdlib.h>` (véase [Administración de la memoria](README.md)).

### - ENG
Signature:
```cpp
void* free(void* ptr);
```
Parameters:
* **Ptr**: Pointer to the buffer in memory to be deleted.

Return:\
`nullptr` on success. The value passed to the **Ptr** argument on failure.\
Convention to keep in mind: this return inverts the intuitive C reading, where `free` returns nothing. A falsy result (`nullptr`) here means the release **did** happen, so the correct check is `if(!memory::free(p))` rather than `if(memory::free(p))`. After a successful call, `p` is dangling and must neither be reused nor freed again.

\
Behavior:\
It frees memory reserved in the program's **Heap**, respecting its paging.\
Since this is C-managed memory, do not manipulate it with `new` or `delete`.\
Adaptation of `free` from `<stdlib.h>` (see [Memory Management](README.md)).