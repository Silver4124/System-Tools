# copy
### - ESP
Firma:
```cpp
const char* copy(const char* source, const char* dest, bool overwrite = true);
```
Parámetros:
* **Source**: Ruta del fichero o carpeta a duplicar. Debe existir.
* **Dest**: Ruta destino. Con un origen de tipo fichero es el nombre final del duplicado; con un origen de tipo carpeta es la carpeta raíz donde se vuelca el contenido, creada si faltaba. Señalar una carpeta existente como destino de un fichero no está soportado: la operación falla.
* **Overwrite**: Determina si un destino ya existente puede ser reemplazado. Por defecto `true`.

Retorno:\
La misma dirección de **Dest**, en caso de éxito.\
`nullptr` si alguna ruta es `nullptr` o está vacía, si **Source** no existe, o si la copia no pudo completarse.\
Ese fallo puede haber ocurrido al iniciarse o a mitad del contenido.

\
Comportamiento:\
Duplica un elemento de disco conservando el original, trabajando únicamente con rutas: no hace falta abrir ni cerrar ninguna handle.\
Cuando **Source** apunta a una carpeta, **Dest** se crea si no existe y todo el contenido se duplica de forma recursiva, manteniendo la estructura de subcarpetas.\
Con `overwrite = true` (por defecto) los elementos ya presentes en el destino se reemplazan.\
Con `overwrite = false` el primer elemento que ya exista interrumpe la operación y retorna `nullptr`.\
Un `nullptr` puede llegar después de haber volcado parte del contenido: la copia se detiene en el primer elemento que falla y deja en **Dest** lo que ya se había copiado, sin eliminarlo.\
Para trasladar el elemento en lugar de duplicarlo está `file::move`.\
Para revisar el destino cuando la integridad importe están `file::exist` y `file::size`.

### - ENG
Signature:
```cpp
const char* copy(const char* source, const char* dest, bool overwrite = true);
```
Parameters:
* **Source**: Path of the file or folder to duplicate. It must exist.
* **Dest**: Destination path. With a file source it is the final name of the duplicate; with a folder source it is the root folder where the content is dumped, created when missing. Pointing at an already existing folder as the destination of a file is not supported: the operation fails.
* **Overwrite**: Determines whether an already existing destination may be replaced. Defaults to `true`.

Return:\
The very same **Dest** address, upon success.\
`nullptr` if either path is `nullptr` or empty, if **Source** does not exist, or if the copy could not be completed.\
That failure may have happened at the start or in the middle of the content.

\
Behavior:\
It duplicates a disk element while keeping the original, working only with paths: no handle needs to be opened or closed.\
When **Source** points to a folder, **Dest** is created if it does not exist and the whole content is duplicated recursively, preserving the subfolder structure.\
With `overwrite = true` (default) elements already present at the destination get replaced.\
With `overwrite = false` the first element that already exists interrupts the operation and returns `nullptr`.\
A `nullptr` may arrive after part of the content was already dumped: the copy stops at the first failing element and leaves in **Dest** whatever got copied, without deleting it.\
To transfer the element instead of duplicating it there is `file::move`.\
To check the destination whenever integrity matters there are `file::exist` and `file::size`.
