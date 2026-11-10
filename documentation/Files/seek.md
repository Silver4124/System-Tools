# seek
### - ESP
Firma:
```cpp
UWORD seek(void* file, WORD length, BYTE mode);
```
Parámetros:
* **File**: Handle Fichero a manipular.
* **Length**: Desplazamiento con signo, medido en bytes. Su tipo es `WORD`, por lo que admite valores negativos para retroceder.
* **Mode**: Punto de referencia desde el cual se calcula el desplazamiento.

Retorno:\
Entero sin signo del tamaño de la palabra del procesador: la nueva posición absoluta del puntero de E/S.\
`0` en caso de error.\
Mover el puntero hasta el inicio del fichero también produce `0`, así que ese valor no garantiza que la operación haya fallado.

\
Comportamiento:\
**Mode** recibe las etiquetas de [[[Enum seek_mode]]](Enum%20seek_mode.md): `seek_mode::begin`, `seek_mode::current` o `seek_mode::end`.\
Desplazar más allá del final del fichero es una operación permitida: el puntero queda fuera del contenido.\
Las lecturas siguientes retornarán `0` hasta que se escriba en esa posición o se retroceda.\
Retroceder por debajo del inicio es un desplazamiento inválido y produce el retorno `0`.\
En ficheros de más de 4 GB la posición se calcula combinando la mitad baja y la mitad alta de 32 bits, usando **Length** como palabra con signo de tamaño de procesador.\
Sobre un Handle Folder el resultado no es significativo: las carpetas no poseen un puntero de lectura/escritura, así que esta función se reserva para handles de fichero.\
Para consultar la posición sin moverla está `file::tell`.

### - ENG
Signature:
```cpp
UWORD seek(void* file, WORD length, BYTE mode);
```
Parameters:
* **File**: Handle File to manipulate.
* **Length**: Signed offset, measured in bytes. Its type is `WORD`, so it accepts negative values to move backwards.
* **Mode**: Reference point from which the offset is calculated.

Return:\
Processor word-sized unsigned integer: the new absolute position of the I/O pointer.\
`0` upon failure.\
Moving the pointer to the beginning of the file also produces `0`, so that value does not prove the operation failed.

\
Behavior:\
**Mode** takes the tags of [[[Enum seek_mode]]](Enum%20seek_mode.md): `seek_mode::begin`, `seek_mode::current` or `seek_mode::end`.\
Moving beyond the end of the file is an allowed operation: the pointer ends up outside the content.\
The following reads will return `0` until something is written at that position or the pointer is rewound.\
Going below the beginning is an invalid offset and produces the `0` return.\
For files larger than 4 GB the position is computed by combining the low and high 32-bit halves, using **Length** as a processor-sized signed word.\
On a Handle Folder the result is not meaningful: folders do not own a read/write pointer, so this function is reserved for file handles.\
To query the position without moving it there is `file::tell`.
