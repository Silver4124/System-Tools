# is_folder
### - ESP
Firma:
```cpp
bool is_folder(void* file);
```
Parámetros:
* **File**: Handle Fichero o Handle Folder a clasificar.

Retorno:\
`true` si la handle corresponde a una carpeta, `false` en caso contrario, incluyendo cuando la handle es `nullptr` o ya no es válida.

\
Comportamiento:\
Distingue el tipo de una handle antes de operarla, porque muchas funciones del módulo aceptan ambas clases y cambian su comportamiento.\
`file::size` cuenta elementos en vez de medir bytes.\
`file::clear` vacía una carpeta borrando sus elementos.\
`file::inside` solo tiene sentido sobre carpetas.\
La clasificación depende de cómo fue creada la handle, no del elemento que haya en disco.\
Una handle obtenida con `file::folder` siempre responde `true`, incluso si la ruta apunta a un fichero.\
Una obtenida con `file::open` siempre responde `false`.\
Es una consulta inmediata que no recorre el sistema de archivos ni altera la handle, así que sirve como comprobación previa dentro de un bucle de recorrido.

### - ENG
Signature:
```cpp
bool is_folder(void* file);
```
Parameters:
* **File**: Handle File or Handle Folder to classify.

Return:\
`true` if the handle corresponds to a folder, `false` otherwise, including when the handle is `nullptr` or is no longer valid.

\
Behavior:\
It tells apart the kind of a handle before operating it, because many module functions accept both classes and change their behavior.\
`file::size` counts entries instead of measuring bytes.\
`file::clear` empties a folder by deleting its entries.\
`file::inside` only makes sense on folders.\
The classification depends on how the handle was created, not on what actually sits on disk.\
A handle obtained with `file::folder` always answers `true`, even if the path points to a file.\
One obtained with `file::open` always answers `false`.\
It is an immediate query that neither walks the filesystem nor alters the handle, so it serves as a prior check inside a traversal loop.
