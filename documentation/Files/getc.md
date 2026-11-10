# getc
### - ESP
Firma:
```cpp
UWORD getc(void* file, int* chr);
```
Parámetros:
* **File**: Handle Fichero de origen. Debe poseer el permiso `file_mode::read`.
* **Chr**: Dirección donde la función almacena el punto de código Unicode leído.

Retorno:\
Entero sin signo del tamaño de la palabra del procesador: cantidad de bytes que ocupó el carácter leído en el fichero, entre `1` y `4`.\
`0` en caso de error, al alcanzar el final del fichero o al encontrar una secuencia inválida o incompleta.

\
Comportamiento:\
Decodifica un único carácter desde la posición actual del puntero de E/S, interpretando UTF-8, y avanza el puntero exactamente los bytes que consumió.\
Un retorno distinto de `0` indica cuántos bytes se leyeron, no el valor del carácter.\
Un byte `0x00` se reporta como `1` byte consumido y deja en **Chr** el valor `0`, condición que `file::getws` aprovecha como delimitador de fin de cadena.\
Cuando una secuencia queda incompleta porque el fichero termina, o cuando el byte inicial no abre una secuencia válida, la función retorna `0`.\
En esos casos el puntero puede haber avanzado los bytes ya leídos sin devolver un carácter.\
**Chr** conserva su valor anterior tras un fallo, así que no conviene reutilizarlo.\
Su pareja, que escribe un punto de código codificándolo, es `file::apendc`.\
Para leer texto de una sola pasada y con delimitador nulo está `file::getws`.

### - ENG
Signature:
```cpp
UWORD getc(void* file, int* chr);
```
Parameters:
* **File**: Source Handle File. It must hold the `file_mode::read` permission.
* **Chr**: Address where the function stores the Unicode codepoint read.

Return:\
Processor word-sized unsigned integer: the amount of bytes the read character occupied in the file, between `1` and `4`.\
`0` upon failure, when the end of the file is reached, or when an invalid or incomplete sequence is found.

\
Behavior:\
It decodes a single character from the current I/O pointer position, reading UTF-8, and advances the pointer by exactly the bytes it consumed.\
A non-zero return tells how many bytes were read, not the character value.\
A `0x00` byte is reported as `1` byte consumed and leaves `0` in **Chr**, the condition `file::getws` takes as its end-of-string delimiter.\
When a sequence is left incomplete because the file ends, or when the leading byte opens no valid sequence, the function returns `0`.\
In those cases the pointer may have advanced by the bytes already read without handing back a character.\
**Chr** keeps its previous value after a failure, so it should not be reused.\
Its counterpart, which writes a codepoint by encoding it, is `file::apendc`.\
To read text in one pass and with a null delimiter there is `file::getws`.
