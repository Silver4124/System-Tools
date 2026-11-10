# apends
### - ESP
Firma:
```cpp
UWORD apends(void* file, const char* str);
```
Parámetros:
* **File**: Handle Fichero de destino. Debe poseer el permiso `file_mode::write`.
* **Str**: Cadena de texto terminada en nulo a escribir.

Retorno:\
Entero sin signo del tamaño de la palabra del procesador: cantidad de bytes escritos, incluyendo el terminador nulo.\
`0` en caso de error.

\
Comportamiento:\
Escribe una cadena de texto y su byte nulo de cierre en la posición actual del puntero de E/S.\
El terminador se escribe siempre, a diferencia del uso corriente de `file::write`, así que los datos quedan autodelimitados: no hace falta conocer ni almacenar la longitud por separado.\
Pese a su nombre, no fuerza la escritura al final del fichero: si el puntero apunta a una posición intermedia, el texto se vuelca ahí y sobrescribe el contenido existente.\
No realiza conversión de codificación alguna: la cadena se escribe tal cual, byte a byte, así que el resultado depende de la codificación con la que esté escrita la literal en el código fuente.\
Si **Str** es `nullptr`, o si la handle carece del permiso de escritura, retorna `0` sin escribir nada.\
Para un añadido real está `file::seek(file, 0, seek_mode::end)`.\
Para la recuperación de la cadena está `file::gets`.\
Para el texto en caracteres anchos está `file::apendws`.

### - ENG
Signature:
```cpp
UWORD apends(void* file, const char* str);
```
Parameters:
* **File**: Destination Handle File. It must hold the `file_mode::write` permission.
* **Str**: Null-terminated text string to write.

Return:\
Processor word-sized unsigned integer: the amount of bytes written, including the null terminator.\
`0` upon failure.

\
Behavior:\
It writes a text string and its closing null byte at the current I/O pointer position.\
The terminator is always written, unlike the plain use of `file::write`, so the data ends up self-delimited: there is no need to know or store the length separately.\
Despite its name, it does not force the writing at the end of the file: if the I/O pointer aims at a middle position, the text is dumped there and overwrites the existing content.\
No encoding conversion is performed: the string is written as it is, byte by byte, so the result depends on the encoding of the literal in the source code.\
If **Str** is `nullptr`, or the handle lacks the write permission, it returns `0` without writing anything.\
For a real append there is `file::seek(file, 0, seek_mode::end)`.\
For retrieving the string there is `file::gets`.\
For wide-character text there is `file::apendws`.
