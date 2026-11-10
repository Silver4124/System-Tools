# szlen
### - ESP
Firma:
```cpp
UWORD szlen(void* ptr);
```
Parámetros:
* **Ptr**: Puntero a la dirección de memoria del buffer a consultar.

Retorno:\
Número entero sin signo del tamaño de la palabra del procesador, que representa el tamaño en bytes del buffer en memoria pasado al argumento **Ptr**. `0` en caso de error, de que este ubicado en el **Stack** de tu programa, no pertenezca a la páginación de memoria de tu programa, o si el valor pasado al argumento **Ptr** es `nullptr`.

\
Comportamiento:\
El manejador de memoria del sistema lleva la cuenta de cuánto ocupó cada reserva, y esta función le pide esa cuenta para el puntero entregado.\
De ahí que el número responda por la capacidad utilizable del bloque y no por lo que se pidió: es habitual que resulte algo mayor que el **Size** entregado a `memory::alloc`, y ese sobrante es del bloque, no un espacio reservado a parte.\
La consulta solo vale para memoria del **Heap** creada por el manejo de memoria de C.\
No permite determinar el tamaño de un dato del **Stack**, dada su naturaleza acumulativa: pedido el tamaño de algo del **Stack**, la función responde `0` sin más.\
Un puntero creado con los manejadores de memoria de C++ no es distinguible de uno creado con los de C, así que consultarlo causa comportamiento indefinido en lugar de responder `0`.

### - ENG
Signature:
```cpp
UWORD szlen(void* ptr);
```
Parameters:
* **Ptr**: Pointer to the memory address of the buffer to be queried.

Return:\
Unsigned integer with the processor word size, which represents the size in bytes of the buffer in memory passed to the **Ptr** argument. Returns `0` in the event of an error, if the buffer is located on your program's stack, if it does not belong to your program's memory paging, or if the value passed to the **Ptr** argument is `nullptr`.

\
Behavior:\
The system's memory manager keeps track of how much it gave each allocation, and this function asks it for the handed pointer.\
That is why the number answers for the block's usable capacity rather than for what was asked: it often comes out a bit larger than the **Size** handed to `memory::alloc`, and that leftover belongs to the block, not to some space reserved apart.\
The query is only valid for **Heap** memory created by C memory management.\
It cannot determine the size of a datum on the **Stack**, given its cumulative nature: asked for the size of something on the **Stack**, the function answers `0` and nothing more.\
A pointer created with the C++ memory managers cannot be told apart from one created with C's, so querying it causes undefined behavior instead of answering `0`.