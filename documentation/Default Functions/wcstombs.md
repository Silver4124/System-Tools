# wcstombs
### - ESP
Firma:
```cpp
UWORD wcstombs(char* dst, const wchar* src, UWORD dst_size, UWORD src_size);
```
Parámetros:
* **Dst**: Cadena de texto o buffer, donde se almacenará el resultado textual.
* **Src**: Cadena de texto largo o buffer, donde se consultará el texto que se desea transformar.
* **Dst_size**: Número entero sin signo del tamaño de la palabra del procesador, que representa el tamaño máximo posible de la cadena de texto o buffer del argumento **Dst**.
* **Src_size**: Número entero sin signo del tamaño de la palabra del procesador, que representa el tamaño máximo posible de la cadena de texto largo o buffer del argumento **Src**.

Retorno:\
Número entero sin signo del tamaño de la palabra del procesador, que representa el tamaño máximo en `char`s que ocupa el resultado en la memoria, excluyendo el carácter de terminación nula (`'\0'`).\
`~0`, el mayor entero de ese tamaño, cuando **Src** o **Dst** son `nullptr` o cuando **Dst_size** es `0`; es el único aviso de fallo que entrega la función.

\
Comportamiento:\
Convierte texto en formato UTF desde **Src** a texto en formato ASCII dentro de una cadena de texto o buffer preexistente **Dst**.\
Los tamaños manejan capacidades que excluyen el carácter de terminación nula (`'\0'` o `L'\0'`).\
Si se pasa la capacidad total de un buffer, reste `1` a **Dst_size** y **Src_size** para evitar lectura y escritura fuera de dichos rangos.\
Adaptación de `wcstombs` de `<stdlib.h>` (véase [Default Functions](README.md)).

### - ENG
Signature:
```cpp
UWORD wcstombs(char* dst, const wchar* src, UWORD dst_size, UWORD src_size);
```
Parameters:
* **Dst**: Text string or buffer where the textual result will be stored.
* **Src**: Wide-text string or buffer containing the source text to be transformed.
* **Dst_size**: Processor word-sized unsigned integer representing the maximum possible capacity of the text string or buffer passed in **Dst**.
* **Src_size**: Processor word-sized unsigned integer representing the maximum possible size of the wide-text string or buffer passed in **Src**.

Return:\
Processor word-sized unsigned integer representing the maximum size in char units that the result occupies in memory, excluding the null-terminator character (`'\0'`).\
`~0`, that integer's highest value, when **Src** or **Dst** is `nullptr` or when **Dst_size** is `0`; it is the only failure notice the function gives.

\
Behavior:\
It converts UTF-formatted text from **Src** into ASCII-formatted text inside a preexisting text string or buffer **Dst**.\
The sizes handle capacities that exclude the null-terminator character (`'\0'` or `L'\0'`).\
If passing the total capacity of a buffer, subtract `1` from **Dst_size** and **Src_size** to avoid reading and writing out of range.\
Adaptation of `wcstombs` from `<stdlib.h>` (see [Default Functions](README.md)).

## Overloads
### - ESP
Firma:
```cpp
UWORD wcstombs(char** dst, const wchar* src, UWORD size, bool stacked = false);
```
Parámetros:
* **Dst**: Puntero a un puntero de tipo `char*`. La función asignará memoria y almacenará aquí la dirección del nuevo buffer de caracteres, realizando de forma autónoma los trabajos de liberación y creación de memoria pertinentes.
* **Src**: Cadena de texto largo o buffer, donde se consultará el texto que se desea transformar.
* **Size**: Número entero sin signo del tamaño de la palabra del procesador, que representa el tamaño máximo posible de la cadena de texto largo o buffer del argumento **Src**.
* **Stacked**: Determina el método de gestión de memoria para la destrucción del viejo buffer y la asignación del nuevo buffer.
  * `false` (Por Defecto): Utiliza los métodos de asignación de C.
  * `true`: Utiliza los métodos de asignación de memoria de C++.

  Pese al nombre, este flag no tiene relación con la pila de ejecución; el régimen de liberación resultante (`memory::free` o `delete`) queda decidido en tiempo de ejecución. Consulte ["The `stacked` term"](../Types.md).

Retorno:\
Número entero sin signo del tamaño de la palabra del procesador, que representa el tamaño máximo en `char`s que ocupa el resultado en la memoria, excluyendo el carácter de terminación nula (`'\0'`).\
`~0`, el mayor entero de ese tamaño, únicamente cuando **Src** es `nullptr`.\
Entregar **Size** en `0` no es un fallo: libera el buffer previo, deja `*Dst` en `nullptr` y responde `0`.

\
Comportamiento:\
Esta variante trabaja con buffers de tamaño y existencia volátiles en la memoria.\
Al igual que la versión padre, los tamaños excluyen el carácter de terminación nula (`'\0'` o `L'\0'`); reste `1` a **Size** si pasa la capacidad total del buffer para evitar lectura fuera de rango.\
Si es la primera vez que se interactúa con un buffer sin inicializar, conviene que su puntero sea `nullptr` antes de invocar.\
Cuando **Dst** es `nullptr` (o un puntero a un puntero nulo), la función no reserva ni escribe: solo calcula y retorna la longitud que tendría el resultado, la forma de sondear el tamaño antes de asignar.\
Ideal para consultar cuántos `char` va a ocupar un texto UTF al convertirse a ASCII:
```cpp
UWORD len_in_char(const wchar* text){
    if(!text || !*text) return 0;
    UWORD len = 0; while(text[len]) ++len;
    return wcstombs(nullptr, text, len, false);
}
```

### - ENG
Signature:
```cpp
UWORD wcstombs(char** dst, const wchar* src, UWORD size, bool stacked = false);
```
Parameters:
* **Dst**: Pointer to a `char*` pointer. The function will allocate memory and store the address of the newly allocated character buffer here, autonomously handling any necessary memory deallocation and creation.
* **Src**: Wide-text string or buffer containing the source text to be transformed.
* **Size**: Processor word-sized unsigned integer representing the maximum possible size of the wide-text string or buffer passed in **Src**.
* **Stacked**: Specifies the memory management method for destroying the old buffer and allocating the new one.
  * `false` (Default): Uses C memory allocation methods.
  * `true`: Uses C++ memory allocation methods.

  Despite its name, this flag has nothing to do with the execution stack; the resulting release regime (`memory::free` or `delete`) is decided at runtime. See ["The `stacked` term"](../Types.md).

Return:\
Processor word-sized unsigned integer representing the maximum size in `char` units that the result occupies in memory, excluding the null-terminator character (`'\0'`).\
`~0`, that integer's highest value, only when **Src** is `nullptr`.\
Handing **Size** as `0` is not a failure: it frees the previous buffer, leaves `*Dst` at `nullptr` and answers `0`.

\
Behavior:\
This variant works with buffers whose size and existence in memory are volatile.\
Like the parent version, the sizes exclude the null-terminator character (`'\0'` or `L'\0'`); subtract `1` from **Size** if passing the buffer's total capacity, to avoid out-of-bounds reading.\
On the first interaction with an uninitialized buffer, its pointer should be `nullptr` before invoking.\
When **Dst** is `nullptr` (or a pointer to a null pointer), the function allocates and writes nothing: it only computes and returns the length the result would have, which is the way to probe the size before allocating.\
Ideal for querying how many `char` a text originally in UTF format will occupy once converted to ASCII:
```cpp
UWORD len_in_char(const wchar* text){
    if(!text || !*text) return 0;
    UWORD len = 0; while(text[len]) ++len;
    return wcstombs(nullptr, text, len, false);
}
```