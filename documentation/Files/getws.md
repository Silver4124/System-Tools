# getws
### - ESP
Firma:
```cpp
UWORD getws(void* file, wchar** wstr);
```
Parámetros:
* **File**: Handle Fichero de origen. Debe poseer el permiso `file_mode::read`.
* **Wstr**: Puntero a un `wchar*` donde la función almacena la dirección del búfer recién creado con la cadena.

Retorno:\
Entero sin signo del tamaño de la palabra del procesador: bytes consumidos por la lectura, terminador nulo incluido.\
La cadena puede contener menos caracteres anchos que esa cifra, porque cada secuencia UTF-8 de varios bytes se reduce a un solo carácter.\
Para contar los caracteres anchos reales, recorre **Wstr** hasta su terminador.\
`0` cuando no había nada que leer: final del fichero, o handle sin el permiso `file_mode::read`.

\
Comportamiento:\
Lee una cadena terminada en nulo desde la posición actual del puntero de E/S, decodificando cada carácter del formato UTF-8 del fichero a su equivalente en `wchar`, y deja el resultado en un búfer creado por la propia función.\
El contenido apuntado por **Wstr** se destruye y sustituye por el búfer nuevo, reservado con el asignador de C++: se conserva y se libera con `delete[]`.\
La primera vez conviene entregar el puntero en `nullptr`.\
Al igual que `file::gets`, el puntero de E/S queda situado justo después del byte nulo de cierre, por lo que la lectura de una segunda cadena contigua comienza correctamente en el byte siguiente.\
La decodificación respeta los caracteres de un solo byte y las secuencias de hasta cuatro.\
`wchar` solo admite 16 bits en Windows: los puntos de código por encima de `0xFFFF` se almacenan recortados, mientras que en Linux caben íntegros.\
Su pareja, que escribe una cadena de caracteres anchos con su terminador, es `file::apendws`.\
Para decodificar un solo carácter a la vez está `file::getc`.

### - ENG
Signature:
```cpp
UWORD getws(void* file, wchar** wstr);
```
Parameters:
* **File**: Source Handle File. It must hold the `file_mode::read` permission.
* **Wstr**: Pointer to a `wchar*` where the function stores the address of the freshly created buffer holding the string.

Return:\
Processor word-sized unsigned integer: bytes consumed by the read, null terminator included.\
The string may hold fewer wide characters than that figure, because each multi-byte UTF-8 sequence collapses into a single character.\
Walk **Wstr** up to its terminator to count the actual wide characters.\
`0` when there was nothing to read: end of file, or a handle lacking the `file_mode::read` permission.

\
Behavior:\
It reads a null-terminated string from the current I/O pointer position, decoding each character from the file's UTF-8 format into its `wchar` equivalent, and leaves the result in a buffer created by the function itself.\
The content pointed by **Wstr** is destroyed and replaced by the new buffer, reserved with the C++ allocator: keep it and release it with `delete[]`.\
On the first call it is worth handing the pointer over as `nullptr`.\
Just like `file::gets`, the I/O pointer ends up right after the closing null byte, so reading a second contiguous string correctly starts at the following byte.\
The decoding honors single-byte characters and sequences up to four bytes long.\
`wchar` only holds 16 bits on Windows: codepoints above `0xFFFF` are stored trimmed, while on Linux they fit whole.\
Its counterpart, which writes a wide-character string with its terminator, is `file::apendws`.\
To decode a single character at a time there is `file::getc`.
