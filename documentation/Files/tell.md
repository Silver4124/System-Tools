# tell
### - ESP
Firma:
```cpp
UWORD tell(void* file);
```
Parámetros:
* **File**: Handle Fichero a consultar.

Retorno:\
Número entero sin signo del tamaño de la palabra del procesador, con la posición absoluta del puntero de E/S en ese momento.\
La posición se mide en bytes desde el inicio del fichero.\
`0` en caso de error, y también cuando el puntero realmente se encuentra en el inicio.

\
Comportamiento:\
Consulta en qué punto del fichero se encontrará la próxima lectura o escritura, sin desplazar el puntero.\
Su uso habitual es tomar un punto de referencia antes de una lectura o escritura, para después poder volver a él con `file::seek`.\
Por eso comparte la ambigüedad de esa función: un fallo no es distinguible de una consulta hecha mientras el puntero apuntaba al byte inicial.\
La posición devuelta es independiente del tamaño del fichero: el puntero puede superar el final del contenido sin que esto se refleje en el retorno.\
Conocer la longitud real del fichero: `file::size`.

### - ENG
Signature:
```cpp
UWORD tell(void* file);
```
Parameters:
* **File**: Handle File to query.

Return:\
Processor word-sized unsigned integer holding the absolute position of the I/O pointer at that moment.\
The position is measured in bytes from the beginning of the file.\
`0` upon failure, and also when the pointer genuinely sits at the beginning.

\
Behavior:\
It reports at which point of the file the next reading or writing will happen, without moving the pointer.\
Its common use is taking a reference point before a reading or writing operation so the pointer can be brought back later with `file::seek`.\
For that reason it shares that function's ambiguity: a failure cannot be told apart from a query made while the pointer was at the very first byte.\
The returned position is independent of the file size: the pointer may go beyond the end of the content without that being reflected in the return value.\
Knowing the real length of the file: `file::size`.
