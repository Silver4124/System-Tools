# open
### - ESP
Firma:
```cpp
void* open(const char* filepath, BYTE openMode = file_mode::read);
```
Parámetros:
* **Filepath**: Ruta del fichero a abrir. Acepta rutas relativas y absolutas.
* **OpenMode**: Permisos de acceso y creación con los que se abrirá el fichero. Por defecto `file_mode::read`.

Retorno:\
Handle Fichero recién creado, en caso de éxito.\
`nullptr` si **Filepath** es `nullptr`, si **OpenMode** es `file_mode::none` o no pide ningún permiso de acceso, o si la ruta no pudo abrirse.

\
Comportamiento:\
Abre o crea un fichero en disco y entrega un **Handle Fichero** listo para las demás funciones del módulo.\
Las rutas relativas se resuelven respecto al directorio de trabajo actual.\
**OpenMode** recibe las etiquetas de [[[Enum file_mode]]](Enum%20file_mode.md), combinables con el operador OR (`|`).\
`file_mode::create` es la única etiqueta que puede dar a luz un fichero nuevo: sin ella, intentar abrir una ruta inexistente retorna `nullptr`.\
Entregada en solitario tampoco abre nada, pues necesita acompañar a `file_mode::read` o a `file_mode::write`.\
Usar `file_mode::create` sobre un fichero que ya existía descarta todo su contenido.\
Los permisos declarados aquí son los que limitan las operaciones posteriores: un handle abierto solo con `file_mode::read` no podrá escribir, y uno abierto solo con `file_mode::write` no podrá leer.\
Un directorio no es aperturable con esta función.\
El handle devuelto debe liberarse cuando deje de ser necesario.\
Para la ruta absoluta resultante está `file::path`.\
Para revisar los permisos con los que quedó abierto un handle está `file::opened_like`.\
Para manipular carpetas está `file::folder`.\
Para liberar el handle está `file::close`.

### - ENG
Signature:
```cpp
void* open(const char* filepath, BYTE openMode = file_mode::read);
```
Parameters:
* **Filepath**: Path of the file to open. Accepts both relative and absolute paths.
* **OpenMode**: Access and creation permissions used to open the file. Defaults to `file_mode::read`.

Return:\
The newly created Handle File upon success.\
`nullptr` if **Filepath** is `nullptr`, if **OpenMode** is `file_mode::none` or asks for no access permission at all, or if the path could not be opened.

\
Behavior:\
It opens or creates a file on disk and delivers a **Handle File** ready for the rest of the module's functions.\
Relative paths are resolved against the current working directory.\
**OpenMode** takes the tags of [[[Enum file_mode]]](Enum%20file_mode.md), combinable with the OR operator (`|`).\
`file_mode::create` is the only tag able to bring a new file into existence: without it, trying to open a non-existent path returns `nullptr`.\
Handed over alone it opens nothing either, since it must accompany `file_mode::read` or `file_mode::write`.\
Using `file_mode::create` on a file that already existed discards all of its contents.\
The permissions declared here are what limit the later operations: a handle opened with `file_mode::read` alone cannot write, and one opened with `file_mode::write` alone cannot read.\
A directory cannot be opened with this function.\
The returned handle must be released once it is no longer needed.\
For the resulting absolute path there is `file::path`.\
To review the permissions a handle ended up with there is `file::opened_like`.\
To handle folders there is `file::folder`.\
To release the handle there is `file::close`.