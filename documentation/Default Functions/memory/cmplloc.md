# cmplloc
### - ESP
Firma:
```cpp
void* cmplloc(const void* p1, const void* p2, UWORD size);
```
Parámetros:
* **P1**: Primer puntero a un buffer en memoria.
* **P2**: Segundo puntero a un buffer en memoria.
* **Size**: Número entero sin signo del tamaño de la palabra del procesador, que representa el tamaño de ambos buffers a comparar.

Retorno:\
Valor resultante de la operación binaria XOR (`^`) entre las direcciones de **P1** y **P2** (`P1 ^ P2`) si ambos buffers son iguales.\
El valor pasado al argumento **P1** si **P1** y **P2** apuntan a la misma dirección en memoria.\
`nullptr` si algún valor dentro de los buffers posee alguna diferencia (el caso más probable), o en caso de error.\
`nullptr` cubre tanto "buffers distintos" como "no se pudo comparar": el retorno solo es fiable como booleano de igualdad, trátelo como "distintos o no comparable".\
Si necesitas el valor XOR, usa `cnrlloc`, que sí cuantifica la diferencia.

\
Comportamiento:\
Evalúa la igualdad exacta entre ambos bloques de memoria.\
`memcmp` devuelve una diferencia numérica basada en la resta de bytes (`< 0`, `0`, `> 0`); `cmplloc` solo distingue iguales de distintos.\
Alternativa booleana a `memcmp` de `<string.h>` (véase [Administración de la memoria](README.md)); la variante ternaria es [`cnrlloc`](cnrlloc.md).

### - ENG
Signature:
```cpp
void* cmplloc(const void* p1, const void* p2, UWORD size);
```
Parameters:
* **P1**: First pointer to an buffer in memory.
* **P2**: Second pointer to an buffer in memory.
* **Size**: Unsigned integer with the processor word size, which represents the size of both buffers to be compared.

Return:\
Resulting value of the binary XOR operation (`^`) between the addresses of **P1** and **P2** (`P1 ^ P2`) if both buffers are equal.\
The value passed to the **P1** argument if **P1** and **P2** point to the same memory address.\
`nullptr` if any value within the buffers has a difference (the most likely case), or on error.\
`nullptr` covers both "different buffers" and "could not compare": the return is only reliable as an equality boolean, treat it as "different or not comparable".\
If you need the XOR value, use `cnrlloc`, which does quantify the difference.

\
Behavior:\
It evaluates the exact equality between both memory blocks.\
`memcmp` returns a numerical difference based on byte subtraction (`< 0`, `0`, `> 0`); `cmplloc` only tells equal from different.\
Boolean alternative to `memcmp` from `<string.h>` (see [Memory Management](README.md)); the ternary variant is [`cnrlloc`](cnrlloc.md).