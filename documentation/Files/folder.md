# folder
### - ESP
Firma:
```cpp
void* folder(const char* filepath, BYTE openMode = file_mode::read);
```
Parámetros:
* **Filepath**: Ruta de la carpeta a abrir o crear. Acepta rutas relativas y absolutas.
* **OpenMode**: Permisos de acceso y creación con los que se abrirá la carpeta. Por defecto `file_mode::read`.

Retorno:\
Handle Folder recién creado, en caso de éxito.\
`nullptr` si **Filepath** es `nullptr`, si **OpenMode** es `file_mode::none`, o si la ruta no pudo resolverse.

\
Comportamiento:\
Es el equivalente de `file::open` para directorios: entrega una handle con la que recorrer el contenido de una carpeta.\
Con `file_mode::create` la carpeta se crea si aún no existe.\
Sin esa etiqueta, ante una ruta inexistente el resultado cambia según el sistema: en Windows se devuelve igualmente una Handle Folder sin elementos accesibles, y en Linux la función retorna `nullptr`.\
No comprueba que el elemento sea realmente una carpeta: si **Filepath** apunta a un fichero existente se obtiene una Handle Folder que no contiene ningún elemento, no un error.\
Las etiquetas de [[[Enum file_mode]]](Enum%20file_mode.md) no limitan la lectura del listado, pero sí se propagan: los handles hijos creados por `file::inside` heredan exactamente estos permisos.\
Abrir una carpeta con `file_mode::write` es la forma de recorrerla obteniendo handles escriturables.\
La ruta relativa se resuelve respecto al directorio de trabajo actual.\
Para recorrer el contenido están `file::inside` y `file::size`.\
Para liberarla está `file::close`, como cualquier otra handle.\
Para confirmar antes la naturaleza del elemento está `file::exist`.\
Para la forma absoluta de la ruta está `file::path`.\
Para confirmar que una handle es de carpeta está `file::is_folder`.

### - ENG
Signature:
```cpp
void* folder(const char* filepath, BYTE openMode = file_mode::read);
```
Parameters:
* **Filepath**: Path of the folder to open or create. Accepts both relative and absolute paths.
* **OpenMode**: Access and creation permissions used to open the folder. Defaults to `file_mode::read`.

Return:\
The newly created Handle Folder upon success.\
`nullptr` if **Filepath** is `nullptr`, if **OpenMode** is `file_mode::none`, or if the path could not be resolved.

\
Behavior:\
It is the counterpart of `file::open` for directories: it delivers a handle that lets you walk the content of a folder.\
With `file_mode::create` the folder is created if it does not exist yet.\
Without that tag, the outcome on a non-existent path changes with the system: Windows still returns a Handle Folder holding no accessible entries, while Linux returns `nullptr`.\
It does not check that the element really is a folder: if **Filepath** points to an existing file, you get a Handle Folder containing no entries at all, not an error.\
The tags of [[[Enum file_mode]]](Enum%20file_mode.md) do not restrict listing the folder, but they do propagate: the child handles created by `file::inside` inherit these permissions exactly.\
Opening a folder with `file_mode::write` is how you walk it getting writable handles.\
A relative path is resolved against the current working directory.\
To walk the content there are `file::inside` and `file::size`.\
To release it there is `file::close`, like any other handle.\
To confirm the nature of the element beforehand there is `file::exist`.\
For the absolute form of the path there is `file::path`.\
To confirm that a handle is a folder there is `file::is_folder`.
