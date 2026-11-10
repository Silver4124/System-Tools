# name
### - ESP
Firma:
```cpp
void* name(void* file, char** fname, UWORD* fnsize, bool with_extension = false, bool stacked = false);
```
Parámetros:
* **File**: Handle Fichero o Handle Folder a consultar.
* **Fname**: Puntero a un `char*` donde la función almacena la dirección del búfer recién creado con el nombre. No puede ser `nullptr`.
* **Fnsize**: Dirección donde la función almacena la longitud del nombre, excluyendo el terminador nulo. No puede ser `nullptr`.
* **With_Extension**: Determina si el nombre incluye o no su extensión. Por defecto `false`.
* **Stacked**: Gestión de memoria usada para destruir el búfer anterior y crear el nuevo. Con `false` (por defecto) se emplean los asignadores de C y el resultado se libera con `memory::free`; con `true` se emplean los de C++ y se libera con `delete[]`. Pese al nombre, no tiene relación con la pila de ejecución; consulta ["The `stacked` term"](../Types.md).

Retorno:\
La misma dirección de la handle recibida, en caso de éxito.\
`nullptr` si la handle es inválida o si **Fname** o **Fnsize** son `nullptr`.

\
Comportamiento:\
Entrega el último componente de la ruta de la handle, el nombre del fichero o de la carpeta, sin el resto del camino que lo precede.\
Con `with_extension = false` (por defecto) la cadena se corta en el último punto del nombre y queda solo el cuerpo.\
Con `with_extension = true` se entrega el nombre completo, tal como figura en disco.\
El corte se hace en el último punto encontrado, así que un nombre con varios puntos pierde solo su último tramo: `archivo.tar.gz` se devuelve como `archivo.tar`.\
Un punto inicial no cuenta como separador de extensión, sino como parte del nombre: el fichero `.gitignore` se devuelve como cadena vacía cuando `with_extension` es `false`, mientras que su extensión permanece intacta.\
Los nombres sin punto se entregan completos en ambos casos.\
El búfer lo crea la función y sustituye al anterior si **Fname** ya apuntaba a uno propio; cómo liberarlo depende de **Stacked**.\
A diferencia de `file::path`, que entrega justo lo contrario, aquí ambos argumentos de salida son obligatorios.

### - ENG
Signature:
```cpp
void* name(void* file, char** fname, UWORD* fnsize, bool with_extension = false, bool stacked = false);
```
Parameters:
* **File**: Handle File or Handle Folder to query.
* **Fname**: Pointer to a `char*` where the function stores the address of the freshly created buffer holding the name. It cannot be `nullptr`.
* **Fnsize**: Address where the function stores the length of the name, excluding the null terminator. It cannot be `nullptr`.
* **With_Extension**: Determines whether the name includes its extension. Defaults to `false`.
* **Stacked**: Memory management used to destroy the previous buffer and create the new one. With `false` (default) the C allocators are used and the result is released with `memory::free`; with `true` the C++ ones are used and it is released with `delete[]`. Despite its name, it has nothing to do with the execution stack; see ["The `stacked` term"](../Types.md).

Return:\
The very same handle address received, upon success.\
`nullptr` if the handle is invalid or if **Fname** or **Fnsize** is `nullptr`.

\
Behavior:\
It delivers the last component of the handle's path, the file or folder name, without the rest of the road preceding it.\
With `with_extension = false` (default) the string is cut at the last dot of the name, keeping only the body.\
With `with_extension = true` the full name is delivered, exactly as it stands on disk.\
The cut happens at the last dot found, so a name carrying several dots loses only its last stretch: `archive.tar.gz` comes back as `archive.tar`.\
A leading dot does not count as an extension separator but as part of the name: the file `.gitignore` is returned as an empty string when `with_extension` is `false`, while its extension stays intact.\
Names without any dot are returned complete in both cases.\
The function creates the buffer and replaces the previous one if **Fname** already pointed to your own; how you release it depends on **Stacked**.\
Unlike `file::path`, which delivers exactly the opposite, both output arguments are mandatory here.

## Overloads
### - ESP
Firma:
```cpp
void* name(void* file, char* fname, UWORD fnsize, bool with_extension = false);
```
Parámetros:
* **File**: Handle Fichero o Handle Folder a consultar.
* **Fname**: Búfer del llamador donde se copia el nombre. No puede ser `nullptr`.
* **Fnsize**: Cantidad de caracteres de texto que la función puede escribir en **Fname**, sin contar el terminador.
* **With_Extension**: Determina si el nombre incluye o no su extensión. Por defecto `false`.

Retorno:\
La misma dirección de la handle recibida, en caso de éxito.\
`nullptr` si la handle es inválida o si **Fname** es `nullptr`.

\
Comportamiento:\
Escribe el nombre sobre un búfer que aportas tú, así que no asigna memoria y no deja nada que liberar.\
El criterio de corte es el mismo que el de la versión principal.\
Si **Fnsize** no alcanza la longitud del nombre, se copian solo los primeros **Fnsize** caracteres y el nombre llega recortado.\
Con `0` la entrega queda en una cadena vacía.\
El terminador se escribe justo después del último carácter copiado, fuera de la cuenta de **Fnsize**: **Fname** necesita **Fnsize** más un bytes disponibles.
### - ENG
Signature:
```cpp
void* name(void* file, char* fname, UWORD fnsize, bool with_extension = false);
```
Parameters:
* **File**: Handle File or Handle Folder to query.
* **Fname**: Caller-supplied buffer where the name is copied. It cannot be `nullptr`.
* **Fnsize**: Amount of text characters the function may write into **Fname**, not counting the terminator.
* **With_Extension**: Determines whether the name includes its extension. Defaults to `false`.

Return:\
The very same handle address received, upon success.\
`nullptr` if the handle is invalid or if **Fname** is `nullptr`.

\
Behavior:\
This overload writes the name into a buffer you supply, so it allocates no memory and leaves nothing to release.\
The cutting rule is the same as the main version's.\
If **Fnsize** does not reach the length of the name, only the first **Fnsize** characters are copied and the name comes out trimmed.\
With `0` the delivery is an empty string.\
The terminator is written right after the last copied character, outside the **Fnsize** count: **Fname** needs **Fnsize** plus one bytes available.