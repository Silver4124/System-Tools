# mlloc
### - ESP
Firma:
```cpp
void* mlloc(void* dest, const void* src, UWORD size);
```
Parámetros:
* **Dest**: Puntero al buffer en memoria donde se prevé el destino de la copia.
* **Src**: Puntero al buffer en memoria donde se ubican los datos a copiar.
* **Size**: Número entero sin signo del tamaño de la palabra del procesador, que representa el tamaño en bytes de los datos a copiar y la ubicación destino.

Retorno:\
Puntero pasado al argumento **Dest** en caso de éxito, o en caso de que tanto el argumento **Dest** como **Src** apuntan a la misma dirección.\
`nullptr` en caso de fallo.

\
Comportamiento:\
Copia los datos de **Src** a **Dest** sin importar la ubicación.\
A diferencia de `memory::clloc`, permite copiar datos dentro de un mismo buffer sin restricciones.\
Ese permiso la hace más lenta que `memory::clloc`.\
Adaptación de `memmove` de `<string.h>` (véase [Administración de la memoria](README.md)).

### - ENG
Signature:
```cpp
void* mlloc(void* dest, const void* src, UWORD size);
```
Parameters:
* **Dest**: Pointer to the buffer in memory where the copy destination is expected.
* **Src**: Pointer to the buffer in memory where the data to be copied is located.
* **Size**: Unsigned integer with the processor word size, which represents the size in bytes of the data to be copied and the destination location.

Return:\
Pointer passed to the **Dest** argument in case of success, or in case both the **Dest** and **Src** arguments point to the same direction.\
`nullptr` on failure.

\
Behavior:\
It copies the data from **Src** to **Dest** regardless of the location.\
Unlike `memory::clloc`, it allows copying data within the same buffer without restrictions.\
That permission makes it slower than `memory::clloc`.\
Adaptation of `memmove` from `<string.h>` (see [Memory Management](README.md)).