# remove
### - ESP
Firma:
```cpp
const char* remove(const char* filepath);
```
Parámetros:
* **Filepath**: Ruta del fichero o carpeta a eliminar.

Retorno:\
La misma dirección de **Filepath**, en caso de éxito.\
`nullptr` si la ruta es `nullptr`, si no existe, o si el elemento no pudo eliminarse.

\
Comportamiento:\
Borra de disco el elemento que se encuentra en la ruta dada, ya sea fichero o carpeta.\
Su ámbito es únicamente la ubicación: no cierra ni libera handles, así que una handle abierta sobre el elemento borrado sigue existiendo en memoria y debe cerrarse con `file::close`.\
Sobre una carpeta, el borrado es recursivo: primero se eliminan todos sus elementos, incluidas las subcarpetas con su contenido, y solo después la carpeta misma.\
Un elemento abierto por otro proceso, o una carpeta con elementos protegidos, puede impedir el borrado.\
Por seguridad, cierra primero tus propias handles y luego invoca esta función.\
Para cambiar la ubicación del elemento en vez de destruirlo está `file::move`.\
Para conservar el elemento y vaciar su contenido está `file::clear`.

### - ENG
Signature:
```cpp
const char* remove(const char* filepath);
```
Parameters:
* **Filepath**: Path of the file or folder to delete.

Return:\
The very same **Filepath** address, upon success.\
`nullptr` if the path is `nullptr`, if it does not exist, or if the element could not be deleted.

\
Behavior:\
It deletes from disk the element found at the given path, whether a file or a folder.\
Its scope is the location only: it neither closes nor releases handles, so a handle opened over the deleted element still exists in memory and must be closed with `file::close`.\
On a folder the deletion is recursive: every entry is deleted first, including subfolders with their content, and only then the folder itself.\
An element opened by another process, or a folder holding protected entries, may prevent the deletion.\
For safety, close your own handles first and then call this function.\
To change the element's location instead of destroying it there is `file::move`.\
To keep the element and empty its content there is `file::clear`.

## Overloads
### - ESP
Firma:
```cpp
void* remove(void* file);
```
Parámetros:
* **File**: Handle Fichero o Handle Folder del elemento a eliminar.

Retorno:\
La misma dirección de la handle recibida, en caso de éxito.\
`nullptr` si la handle es inválida o si el elemento no pudo eliminarse.\
La dirección devuelta es solo el aviso de éxito: al volver de la llamada ya no señala nada válido.

\
Comportamiento:\
Elimina de disco la ubicación a la que apunta la handle, sin que reconstruyas la ruta a partir de `file::path`.\
A diferencia de la versión por ruta, aquí la handle sí se consume: la llamada cierra el handle de sistema y devuelve la memoria de la handle al montón antes de volver.\
Cualquier consulta posterior sobre esa dirección —`file::size`, `file::opened_like`, `file::path` o un segundo `remove`— es leer memoria liberada.\
No debe volverse a usar ni cerrarse con `file::close`: la handle ya llegó al final de su vida y `file::close` sobre esa dirección no encuentra nada que cerrar.\
El motivo es la propia tarea: borrado el elemento, la handle ya no describe nada del disco, y dejarla viva solo invita a pedir los datos de un fichero que no existe.

### - ENG
Signature:
```cpp
void* remove(void* file);
```
Parameters:
* **File**: Handle File or Handle Folder of the element to delete.

Return:\
The very same handle address received, upon success.\
`nullptr` if the handle is invalid or if the element could not be deleted.\
The address handed back is only the notice of success: once the call returns it points at nothing valid.

\
Behavior:\
It deletes from disk the location the handle points to, without forcing you to rebuild the path through `file::path`.\
Unlike the path-based version, the handle is consumed here: the call closes the system handle and returns the handle memory to the heap before it comes back.\
Any later query over that address —`file::size`, `file::opened_like`, `file::path` or a second `remove`— is reading freed memory.\
It must neither be used again nor closed with `file::close`: the handle already reached the end of its life and `file::close` over that address finds nothing left to close.\
The reason is the task itself: once the element is deleted the handle describes nothing on disk, and keeping it alive only invites reading the data of a file that does not exist.
