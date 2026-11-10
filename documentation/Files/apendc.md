# apendc
### - ESP
Firma:
```cpp
UWORD apendc(void* file, int chr);
```
Parámetros:
* **File**: Handle Fichero de destino. Debe poseer el permiso `file_mode::write`.
* **Chr**: Punto de código Unicode a escribir, como entero con signo.

Retorno:\
Entero sin signo del tamaño de la palabra del procesador: cantidad de bytes escritos, entre `1` y `4`.\
`0` en caso de error.

\
Comportamiento:\
Escribe un único carácter codificándolo en UTF-8, con la longitud de la secuencia según el rango del punto de código: un byte hasta `0x7F`, dos hasta `0x7FF`, tres hasta `0xFFFF` y cuatro hasta `0x10FFFF`.\
La escritura ocurre en la posición actual del puntero de E/S: si el puntero no está al final, el carácter sobrescribe el contenido existente.\
El valor `0` no es escribible por esta vía: el byte nulo se reserva como delimitador, así que con **Chr** en `0` la función retorna `0` sin volcar nada.\
Los puntos de código por encima de `0x10FFFF` producen el mismo retorno `0`.\
Los valores negativos no tienen sentido como puntos de código y terminan escribiendo un único byte con su bajo orden, así que conviene validar **Chr** antes de confiar en el resultado.\
Su pareja, que devuelve el punto de código junto con los bytes que ocupó, es `file::getc`.\
Juntas recorren un fichero carácter a carácter sin depender de la longitud de cada secuencia.

### - ENG
Signature:
```cpp
UWORD apendc(void* file, int chr);
```
Parameters:
* **File**: Destination Handle File. It must hold the `file_mode::write` permission.
* **Chr**: Unicode codepoint to write, as a signed integer.

Return:\
Processor word-sized unsigned integer: the amount of bytes written, between `1` and `4`.\
`0` upon failure.

\
Behavior:\
It writes a single character by encoding it in UTF-8, with the sequence length following the codepoint range: one byte up to `0x7F`, two up to `0x7FF`, three up to `0xFFFF`, four up to `0x10FFFF`.\
The writing happens at the current I/O pointer position: if the pointer is not at the end, the character overwrites the existing content.\
The value `0` is not writable through this route: the null byte is reserved as a delimiter, so with **Chr** at `0` the function returns `0` without dumping anything.\
Codepoints above `0x10FFFF` produce the same `0` return.\
Negative values make no sense as codepoints and end up writing a single byte with their low order, so it is worth validating **Chr** before trusting the result.\
Its counterpart, which returns the codepoint together with the bytes it occupied, is `file::getc`.\
Together they walk a file character by character without depending on the length of each sequence.
