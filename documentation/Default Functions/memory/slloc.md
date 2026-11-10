# slloc
### - ESP
Firma:
```cpp
void* slloc(void* dest, BYTE value, UWORD size);
```
Parámetros:
* **Dest**: Puntero al buffer en memoria donde se desea establecer el valor de sus bytes.
* **Value**: Valor que adoptarán los bytes del buffer apuntado por **Dest**.
* **Size**: Número entero sin signo del tamaño de la palabra del procesador, que representa el tamaño en bytes de cuantos de estos mismos se les va a cambiar su valor.

Retorno:\
Puntero pasado al argumento **Dest** en caso de exito. `nullptr` en caso de fallo.

\
Comportamiento:\
Escribe **Value** en los `Size` bytes del buffer apuntado por **Dest**.\
Se usa para poner a cero un búfer reservado por `memory::alloc` en Windows, que llega sin limpiar.\
Adaptación de `memset` de `<string.h>` (véase [Administración de la memoria](README.md)).

### - ENG
Signature:
```cpp
void* slloc(void* dest, BYTE value, UWORD size);
```
Parameters:
* **Dest**: Pointer to the buffer in memory where you want to set the value of its bytes.
* **Value**: Value to be assigned to the bytes of the buffer pointed to by **Dest**.
* **Size**: Unsigned integer with the processor word size, which represents the size in bytes of how many will have their value changed.

Return:\
Value of the **Dest** argument in case of success. `nullptr` on failure.

\
Behavior:\
It writes **Value** into the `Size` bytes of the buffer pointed to by **Dest**.\
It is used to zero a buffer reserved by `memory::alloc` on Windows, which arrives uncleaned.\
Adaptation of `memset` from `<string.h>` (see [Memory Management](README.md)).