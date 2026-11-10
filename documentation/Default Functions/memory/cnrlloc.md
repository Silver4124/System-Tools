# cnrlloc
### - ESP
Firma:
```cpp
void* cnrlloc(const void* p1, const void* p2, UWORD size);
```
Parámetros:
* **P1**: Primer puntero a un buffer en memoria.
* **P2**: Segundo puntero a un buffer en memoria.
* **Size**: Número entero sin signo del tamaño de la palabra del procesador, que representa el tamaño de ambos buffers a comparar.

Retorno:\
XOR (`^`) entre las direcciones de **P1** y **P2** (`P1 ^ P2`) si ambos buffers son idénticos en contenido.\
El valor del argumento **P1** si **P1** y **P2** comparten dirección en memoria.\
Si los datos difieren, retorna el puntero del buffer que prevalezca, según la resta de bytes entre cada buffer: **P1** si la diferencia es negativa, **P2** si es positiva.\
`nullptr` únicamente en caso de error de lectura o falla durante la ejecución.

\
Comportamiento:\
Además de verificar la igualdad, evalúa la precedencia de los datos al encontrar diferencias.\
Cuando los contenidos difieren, el puntero devuelto es el del buffer que precede al otro en la comparación byte a byte, es decir, el menor.\
Alternativa ternaria/comparativa a `memcmp` de `<string.h>`; la variante booleana es [`cmplloc`](cmplloc.md).\
Su nombre viene de "Counter" ("Contador"): el que te cuenta quién fue el culpable.

### - ENG
Signature:
```cpp
void* cnrlloc(const void* p1, const void* p2, UWORD size);
```
Parameters:
* **P1**: First pointer to an buffer in memory.
* **P2**: Second pointer to an buffer in memory.
* **Size**: Unsigned integer with the processor word size, which represents the size of both buffers to be compared.

Return:\
XOR (`^`) between the addresses of **P1** and **P2** (`P1 ^ P2`) if both buffers are identical in content.\
The value of the **P1** argument if **P1** and **P2** share the same memory address.\
If the data differs, the pointer of the prevailing buffer is returned, according to the subtraction of the bytes between each buffer: **P1** if the difference is negative, **P2** if it is positive.\
`nullptr` only in case of a read error or a failure during execution.

\
Behavior:\
Besides checking equality, it evaluates data precedence when differences are found.\
When the contents differ, the pointer returned is the one of the buffer that precedes the other in the byte-by-byte comparison, that is, the lesser one.\
Ternary/comparative alternative to `memcmp` from `<string.h>`; the boolean variant is [`cmplloc`](cmplloc.md).\
Its name comes from "Counter": the one that tells you who was to blame.