# clloc
### - ESP
Firma:
```cpp
void* clloc(void* RESTRICT dest, const void* RESTRICT src, UWORD size);
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
Copia los datos de **Src** a **Dest**; la única condición es que pertenezcan a buffers distintos.\
Copiar datos del mismo buffer hacia la dirección destino es comportamiento indefinido y hará crashear el programa.\
Direcciones idénticas (`Dest == Src`): es un no-op y la función devuelve el propio `Dest`.\
Solapamiento parcial (`Dest != Src`, pero los rangos `[Dest, Dest + Size)` y `[Src, Src + Size)` comparten bytes): comportamiento indefinido, puede bloquear el programa.\
Si deseas copiar datos dentro de un mismo buffer, usa `memory::mlloc`.\
Adaptación de `memcpy` de `<string.h>`; sobre la promesa de no solapamiento, consulta la macro [`RESTRICT`](../../Macros.md#restrict).

### - ENG
Signature:
```cpp
void* clloc(void* RESTRICT dest, const void* RESTRICT src, UWORD size);
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
It copies the data from **Src** to **Dest**; the only condition is that they belong to different buffers.\
Copying data from the same buffer toward the destination address is undefined behavior and will crash your program.\
Identical addresses (`Dest == Src`): it is a no-op and the function returns `Dest` itself.\
Partial overlap (`Dest != Src`, yet the ranges `[Dest, Dest + Size)` and `[Src, Src + Size)` share bytes): undefined behavior, it may block the program.\
If you wish to copy data within the same buffer, use `memory::mlloc`.\
Adaptation of `memcpy` from `<string.h>`; for the no-overlap promise, see the [`RESTRICT`](../../Macros.md#restrict) macro.