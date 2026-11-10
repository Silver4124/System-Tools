# path
### - ESP
Firma:
```cpp
void* path(void* file, char** fname, UWORD* fnsize, bool stacked = false);
```
Parámetros:
* **File**: Handle Fichero o Handle Folder a consultar.
* **Fname**: Puntero a un `char*` donde la función almacena la dirección del búfer recién creado con la ruta. Puede ser `nullptr` si solo se desea la longitud.
* **Fnsize**: Dirección donde la función almacena la longitud de la ruta, excluyendo el terminador nulo. Puede ser `nullptr` si solo se desea la cadena.
* **Stacked**: Gestión de memoria usada para destruir el búfer anterior y crear el nuevo. Con `false` (por defecto) se emplean los asignadores de C y el resultado se libera con `memory::free`; con `true` se emplean los de C++ y se libera con `delete[]`. Pese al nombre, no tiene relación con la pila de ejecución; consulta ["The `stacked` term"](../Types.md).

Retorno:\
La misma dirección de la handle recibida, en caso de éxito.\
`nullptr` si la handle es inválida o si **Fname** y **Fnsize** son `nullptr` a la vez.

\
Comportamiento:\
Entrega la ruta completa del elemento apuntado por la handle, en su forma absoluta y ya resuelta.\
Es la forma de saber dónde está realmente una handle abierta con ruta relativa o construida desde otra carpeta.\
La cadena conserva el separador propio de la plataforma en la que se ejecuta el programa.\
No incluye el nombre del elemento como componente separado.\
El búfer lo crea la función, reemplazando al anterior si **Fname** ya apuntaba a uno propio; cómo liberarlo depende de **Stacked**.\
La primera vez conviene entregar el puntero en `nullptr`.\
No recalcula nada: refleja la ruta con la que la handle fue creada, así que tras un `file::move` o un `file::remove` la ruta obtenida sigue siendo la antigua.\
Para el nombre suelto está `file::name`.

### - ENG
Signature:
```cpp
void* path(void* file, char** fname, UWORD* fnsize, bool stacked = false);
```
Parameters:
* **File**: Handle File or Handle Folder to query.
* **Fname**: Pointer to a `char*` where the function stores the address of the freshly created buffer holding the path. It may be `nullptr` when only the length is wanted.
* **Fnsize**: Address where the function stores the length of the path, excluding the null terminator. It may be `nullptr` when only the string is wanted.
* **Stacked**: Memory management used to destroy the previous buffer and create the new one. With `false` (default) the C allocators are used and the result is released with `memory::free`; with `true` the C++ ones are used and it is released with `delete[]`. Despite its name, it has nothing to do with the execution stack; see ["The `stacked` term"](../Types.md).

Return:\
The very same handle address received, upon success.\
`nullptr` if the handle is invalid or if **Fname** and **Fnsize** are both `nullptr`.

\
Behavior:\
It delivers the full path of the element pointed by the handle, in absolute and already resolved form.\
It is how you learn where a handle opened with a relative path, or built from another folder, actually lives.\
The string keeps the separator belonging to the platform the program runs on.\
It never exposes the element name as a separate component.\
The function creates the buffer, replacing the previous one if **Fname** already pointed to your own; how you release it depends on **Stacked**.\
On the first call it is worth handing the pointer over as `nullptr`.\
Nothing gets recomputed: the answer reflects the path the handle was created with, so after a `file::move` or a `file::remove` the path obtained stays the old one.\
For the bare name there is `file::name`.

## Overloads
### - ESP
Firma:
```cpp
void* path(void* file, char* fname, UWORD fnsize);
```
Parámetros:
* **File**: Handle Fichero o Handle Folder a consultar.
* **Fname**: Búfer del llamador donde se copia la ruta. No puede ser `nullptr`.
* **Fnsize**: Cantidad de caracteres de texto que la función puede escribir en **Fname**, sin contar el terminador.

Retorno:\
La misma dirección de la handle recibida, en caso de éxito.\
`nullptr` si la handle es inválida o si **Fname** es `nullptr`.

\
Comportamiento:\
Escribe sobre un búfer que aportas tú, ya sea un arreglo local o memoria propia: no asigna nada y no hay nada que liberar.\
Si **Fnsize** no alcanza la longitud completa de la ruta, se copian solo los primeros **Fnsize** caracteres y la cadena llega recortada, no rechazada.\
Con `0` la entrega queda en una cadena vacía.\
El terminador se escribe justo después del último carácter copiado, fuera de la cuenta de **Fnsize**: **Fname** necesita **Fnsize** más un bytes disponibles.
### - ENG
Signature:
```cpp
void* path(void* file, char* fname, UWORD fnsize);
```
Parameters:
* **File**: Handle File or Handle Folder to query.
* **Fname**: Caller-supplied buffer where the path is copied. It cannot be `nullptr`.
* **Fnsize**: Amount of text characters the function may write into **Fname**, not counting the terminator.

Return:\
The very same handle address received, upon success.\
`nullptr` if the handle is invalid or if **Fname** is `nullptr`.

\
Behavior:\
This overload writes into a buffer you supply, be it a local array or your own memory: nothing gets allocated and nothing needs releasing.\
If **Fnsize** does not reach the full length of the path, only the first **Fnsize** characters are copied and the string comes out trimmed, not rejected.\
With `0` the delivery is an empty string.\
The terminator is written right after the last copied character, outside the **Fnsize** count: **Fname** needs **Fnsize** plus one bytes available.