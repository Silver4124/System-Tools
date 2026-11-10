# normalize
### - ESP
Firma:
```cpp
char* normalize(char* path, UWORD* path_size = nullptr);
```
Parámetros:
* **Path**: Ruta a normalizar, en un búfer modificable y terminado en nulo. La operación se realiza sobre el propio búfer, sin crear uno nuevo.
* **Path_size**: Dirección donde la función almacena la nueva longitud de la ruta, excluyendo el terminador nulo. Opcional.

Retorno:\
La misma dirección de **Path**, con la ruta ya normalizada y terminada en nulo.\
`nullptr` si **Path** es `nullptr`.

\
Comportamiento:\
Deja una ruta en el formato que la plataforma anfitriona espera: traduce los separadores extranjeros al separador propio y colapsa los repetidos.\
`datos//imágenes`, `datos\\imágenes` y `datos\/imágenes` acaban todos en la misma forma.\
El trabajo se hace en el sitio y solo acorta la cadena, nunca la alarga, así que el búfer original siempre tiene capacidad suficiente y no es necesario copiar el resultado.\
No resuelve `.` ni `..`, no elimina el separador final, no cambia mayúsculas ni minúsculas y no convierte una ruta relativa en absoluta.\
Es la operación previa habitual cuando una ruta proviene de una fuente externa, como un fichero de configuración o una entrada del usuario.\
**Path** no puede ser una literal ni un búfer de solo lectura: debe ser un arreglo modificable o memoria obtenida con los asignadores de C++.\
Para volver absoluta una ruta está `file::path`, tras abrirla con `file::open` o `file::folder`.

### - ENG
Signature:
```cpp
char* normalize(char* path, UWORD* path_size = nullptr);
```
Parameters:
* **Path**: Path to normalize, inside a mutable null-terminated buffer. The operation happens over the buffer itself, without creating a new one.
* **Path_size**: Address where the function stores the new length of the path, excluding the null terminator. Optional.

Return:\
The very same **Path** address, holding the already normalized and null-terminated path.\
`nullptr` if **Path** is `nullptr`.

\
Behavior:\
It leaves a path in the format the host platform expects: foreign separators are translated into the native one and repeated ones are collapsed.\
`data//images`, `data\\images` and `data\/images` all end up in the same shape.\
The work happens in place and only shortens the string, never lengthens it, so the original buffer always has enough capacity and there is no need to copy the result.\
It does not resolve `.` or `..`, does not remove a trailing separator, does not change letter case, and does not turn a relative path into an absolute one.\
It is the usual prior step whenever a path comes from an outside source, like a configuration file or user input.\
The **Path** argument cannot be a literal nor a read-only buffer: it must be a mutable array or memory obtained through the C++ allocators.\
To make a path absolute there is `file::path`, once it has been opened with `file::open` or `file::folder`.
