# gets
### - ESP
Firma:
```cpp
UWORD gets(void* file, char** str);
```
Parámetros:
* **File**: Handle Fichero de origen. Debe poseer el permiso `file_mode::read`.
* **Str**: Puntero a un `char*` donde la función almacena la dirección del búfer recién creado con la cadena.

Retorno:\
Entero sin signo del tamaño de la palabra del procesador: bytes consumidos por la lectura, terminador nulo incluido, que es la longitud de la cadena más uno.\
`0` cuando no había nada que leer: final del fichero, o handle sin el permiso `file_mode::read`.

\
Comportamiento:\
Lee una cadena desde la posición actual del puntero de E/S y la deja lista en un búfer creado por la propia función.\
No se detiene en el salto de línea ni en los espacios: el delimitador es el byte nulo (`'\0'`).\
Ese es justo el terminador que `file::apends` añade al escribir, así que las dos forman la pareja natural para guardar y recuperar texto.\
El contenido apuntado por **Str** se destruye y sustituye por el búfer nuevo, reservado con el asignador de C++: se conserva y se libera con `delete[]`.\
La primera vez conviene entregar el puntero en `nullptr`.\
No transcodifica nada: los bytes se copian tal cual, así que una cadena en UTF-8 sigue siendo UTF-8.\
El puntero de E/S queda situado justo después del byte nulo que cerró la cadena, así que una segunda llamada seguida sobre cadenas separadas por nulos comienza en la cadena siguiente sin pasos intermedios.\
Cuando retorna `0` no hay cadena que mostrar: el búfer queda sin contenido definido.\
Comprueba el retorno antes de leer **Str**, o entrégalo ya inicializado a cadena vacía.\
Para decodificar en caracteres anchos está `file::getws`.

### - ENG
Signature:
```cpp
UWORD gets(void* file, char** str);
```
Parameters:
* **File**: Source Handle File. It must hold the `file_mode::read` permission.
* **Str**: Pointer to a `char*` where the function stores the address of the freshly created buffer holding the string.

Return:\
Processor word-sized unsigned integer: bytes consumed by the read, null terminator included, which is the length of the string plus one.\
`0` when there was nothing to read: end of file, or a handle lacking the `file_mode::read` permission.

\
Behavior:\
It reads a string from the current I/O pointer position and leaves it ready inside a buffer created by the function itself.\
It does not stop at line breaks or spaces: the delimiter is the null byte (`'\0'`).\
That is precisely the terminator `file::apends` appends when writing, so the two make the natural pair for storing and retrieving text.\
The content pointed by **Str** is destroyed and replaced by the new buffer, reserved with the C++ allocator: keep it and release it with `delete[]`.\
On the first call it is worth handing the pointer over as `nullptr`.\
Nothing gets transcoded: bytes are copied as they are, so a UTF-8 string remains UTF-8.\
The I/O pointer ends up right after the null byte that closed the string, so a second back-to-back call over null-separated strings begins on the next string without any intermediate step.\
When it returns `0` there is no string to show: the buffer is left with no defined content.\
Check the return value before reading **Str**, or hand the pointer over already initialized to an empty string.\
For wide-character decoding there is `file::getws`.
