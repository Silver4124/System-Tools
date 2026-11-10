# apendws
### - ESP
Firma:
```cpp
UWORD apendws(void* file, const wchar* wstr);
```
Parámetros:
* **File**: Handle Fichero de destino. Debe poseer el permiso `file_mode::write`.
* **Wstr**: Cadena de caracteres anchos terminada en nulo a escribir.

Retorno:\
Entero sin signo del tamaño de la palabra del procesador: cantidad de bytes escritos, incluyendo el terminador nulo.\
`0` en caso de error.

\
Comportamiento:\
Escribe una cadena de caracteres anchos en el fichero, codificando cada carácter a UTF-8 y añadiendo un byte nulo de cierre al final.\
Cada `wchar` se trata como un punto de código independiente: los pares sustitutos no se combinan.\
Un carácter fuera del plano multilingüe básico representado como dos `wchar` se volcará como dos secuencias UTF-8 sueltas.\
La escritura se realiza en la posición actual del puntero de E/S y, pese al nombre, no fuerza el final del fichero.\
Si **Wstr** es `nullptr`, o si la handle carece del permiso de escritura, retorna `0`.\
Una cadena vacía no produce un retorno `0`, sino la escritura del byte terminador.\
Para un añadido real está `file::seek(file, 0, seek_mode::end)`.\
Para leer la cadena de vuelta está `file::getws`.\
Para ASCII o UTF-8 ya codificado está `file::apends`, que escribe los bytes sin transcodificar.

### - ENG
Signature:
```cpp
UWORD apendws(void* file, const wchar* wstr);
```
Parameters:
* **File**: Destination Handle File. It must hold the `file_mode::write` permission.
* **Wstr**: Null-terminated wide-character string to write.

Return:\
Processor word-sized unsigned integer: the amount of bytes written, including the null terminator.\
`0` upon failure.

\
Behavior:\
It writes a wide-character string into the file, encoding each character to UTF-8 and appending a closing null byte at the end.\
Every `wchar` is treated as an independent codepoint: surrogate pairs are not combined.\
A character outside the Basic Multilingual Plane represented as two `wchar` values is dumped as two separate UTF-8 sequences.\
The writing happens at the current I/O pointer position and, despite its name, it does not force the end of the file.\
If **Wstr** is `nullptr`, or the handle lacks the write permission, it returns `0`.\
An empty string does not produce a `0` return, but the writing of the terminator byte.\
For a real append there is `file::seek(file, 0, seek_mode::end)`.\
To read the string back there is `file::getws`.\
For ASCII or already-encoded UTF-8 there is `file::apends`, which writes the bytes without transcoding.
