# size
### - ESP
Firma:
```cpp
UWORD size(void* file);
```
Parámetros:
* **File**: Handle Fichero o Handle Folder a consultar.

Retorno:\
Número entero sin signo del tamaño de la palabra del procesador.\
Sobre un Handle Fichero representa la longitud del fichero en bytes.\
Sobre un Handle Folder representa la cantidad de elementos que contiene.\
`0` en caso de error, y también cuando el fichero está vacío o la carpeta no tiene elementos.

\
Comportamiento:\
Informa cuánto contiene realmente el elemento apuntado por el handle, sin desplazar ni modificar su puntero de E/S.\
El significado del retorno cambia según el tipo de handle, por lo que conviene confirmar la naturaleza del elemento con `file::is_folder` antes de interpretar la cifra.\
Al contar una carpeta solo se incluyen sus elementos directos: no se recorren las subcarpetas de forma recursiva.\
Los elementos ocultos, cuyo nombre comienza por `.`, sí se cuentan.\
Quedan dentro de la misma numeración que usa `file::inside`, ya que ambas funciones dejan fuera únicamente `.` y `..`.\
La longitud en bytes es la del fichero tal como está en disco, no la posición del puntero: tras escribir o desplazarse más allá del final, `file::size` seguirá devolviendo el tamaño efectivo.\
Ten presente que `0` es ambiguo, ya que cubre por igual un fichero vacío, una carpeta sin elementos y un handle inválido.

### - ENG
Signature:
```cpp
UWORD size(void* file);
```
Parameters:
* **File**: Handle File or Handle Folder to query.

Return:\
Processor word-sized unsigned integer.\
On a Handle File it represents the length of the file in bytes.\
On a Handle Folder it represents the amount of entries it contains.\
`0` upon failure, and also when the file is empty or the folder has no entries.

\
Behavior:\
It reports how much the element pointed by the handle really contains, without moving or modifying its I/O pointer.\
The meaning of the return value changes with the kind of handle, so it is worth confirming the nature of the element with `file::is_folder` before interpreting the number.\
When counting a folder only its direct entries are included: subfolders are not walked recursively.\
Hidden entries, whose name begins with `.`, are counted.\
They sit inside the very same numbering `file::inside` uses, since both functions leave out only `.` and `..`.\
The byte length is the one of the file as it stands on disk, not the pointer position: after writing or seeking beyond the end, `file::size` still returns the effective size.\
Keep in mind that `0` is ambiguous, covering an empty file, a folder without entries, and an invalid handle alike.
