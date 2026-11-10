# extension
### - ESP
Firma:
```cpp
void* extension(void* file, char** fname, UWORD* fnsize, bool with_dot = false, bool stacked = false);
```
Parámetros:
* **File**: Handle Fichero o Handle Folder a consultar.
* **Fname**: Puntero a un `char*` donde la función almacena la dirección del búfer recién creado con la extensión. Puede ser `nullptr` si solo se desea la longitud.
* **Fnsize**: Dirección donde la función almacena la longitud de la extensión. Puede ser `nullptr` si solo se desea la cadena.
* **With_Dot**: Determina si la extensión incluye o no el punto que la precede. Por defecto `false`.
* **Stacked**: Gestión de memoria usada para destruir el búfer anterior y crear el nuevo. Con `false` (por defecto) se emplean los asignadores de C y el resultado se libera con `memory::free`; con `true` se emplean los de C++ y se libera con `delete[]`. Pese al nombre, no tiene relación con la pila de ejecución; consulta ["The `stacked` term"](../Types.md).

Retorno:\
La misma dirección de la handle recibida, en caso de éxito.\
`nullptr` si la handle es inválida o si **Fname** y **Fnsize** son `nullptr` a la vez.

\
Comportamiento:\
Entrega el tramo que va después del último punto del nombre del elemento, sin analizar los bytes del fichero.\
Con `with_dot = false` (por defecto) la extensión comienza en la primera letra.\
Con `with_dot = true` se conserva el punto inicial, lo que facilita comparar contra literales como `".txt"`.\
Un nombre sin punto carece de extensión: la entrega es una cadena vacía y su longitud es `0`.\
Un nombre que empieza por punto y no encierra ningún otro, como `".hidden"`, reparte el resto por separado: todo lo posterior a ese primer punto se toma por extensión, así que se entrega `hidden`.\
El búfer resultante lo crea la función y sustituye al anterior si **Fname** ya apuntaba a uno propio: cómo liberarlo depende de **Stacked**.\
Puede faltar el puntero o la longitud, pero no los dos a la vez.\
Su pareja, que entrega el nombre sin la extensión, es `file::name`.

### - ENG
Signature:
```cpp
void* extension(void* file, char** fname, UWORD* fnsize, bool with_dot = false, bool stacked = false);
```
Parameters:
* **File**: Handle File or Handle Folder to query.
* **Fname**: Pointer to a `char*` where the function stores the address of the freshly created buffer holding the extension. It may be `nullptr` when only the length is wanted.
* **Fnsize**: Address where the function stores the length of the extension. It may be `nullptr` when only the string is wanted.
* **With_Dot**: Determines whether the extension includes the dot preceding it. Defaults to `false`.
* **Stacked**: Memory management used to destroy the previous buffer and create the new one. With `false` (default) the C allocators are used and the result is released with `memory::free`; with `true` the C++ ones are used and it is released with `delete[]`. Despite its name, it has nothing to do with the execution stack; see ["The `stacked` term"](../Types.md).

Return:\
The very same handle address received, upon success.\
`nullptr` if the handle is invalid or if **Fname** and **Fnsize** are both `nullptr`.

\
Behavior:\
It delivers the stretch that comes after the last dot of the element's name, without parsing the file bytes.\
With `with_dot = false` (default) the extension begins at its first letter.\
With `with_dot = true` the leading dot is kept, which eases direct comparisons against literals such as `".txt"`.\
A name without a dot has no extension: the delivery is an empty string whose length is `0`.\
A name that begins with a dot and holds no other one, like `".hidden"`, splits the rest apart: everything after that first dot is taken as the extension, so `hidden` gets delivered.\
The function creates the resulting buffer and replaces the previous one if **Fname** already pointed to your own: how you release it depends on **Stacked**.\
Either the pointer or the length may be missing, but not both at once.\
Its counterpart, which delivers the name without the extension, is `file::name`.

## Overloads
### - ESP
Firma:
```cpp
void* extension(void* file, char* fname, UWORD fnsize, bool with_dot = false);
```
Parámetros:
* **File**: Handle Fichero o Handle Folder a consultar.
* **Fname**: Búfer del llamador donde se copia la extensión. No puede ser `nullptr`.
* **Fnsize**: Cantidad de caracteres de texto que la función puede escribir en **Fname**, sin contar el terminador.
* **With_Dot**: Determina si la extensión incluye o no el punto que la precede. Por defecto `false`.

Retorno:\
La misma dirección de la handle recibida, en caso de éxito.\
`nullptr` si la handle es inválida o si **Fname** es `nullptr`.

\
Comportamiento:\
Escribe la extensión sobre un búfer que aportas tú, así que no asigna memoria y no deja nada que liberar.\
El criterio de extracción es el mismo que el de la versión principal.\
Si **Fnsize** no alcanza la longitud de la extensión, se copian solo los primeros **Fnsize** caracteres y el resultado llega recortado.\
Con `0` la entrega queda en una cadena vacía.\
El terminador se escribe justo después del último carácter copiado, fuera de la cuenta de **Fnsize**: **Fname** necesita **Fnsize** más un bytes disponibles.
### - ENG
Signature:
```cpp
void* extension(void* file, char* fname, UWORD fnsize, bool with_dot = false);
```
Parameters:
* **File**: Handle File or Handle Folder to query.
* **Fname**: Caller-supplied buffer where the extension is copied. It cannot be `nullptr`.
* **Fnsize**: Amount of text characters the function may write into **Fname**, not counting the terminator.
* **With_Dot**: Determines whether the extension includes the dot preceding it. Defaults to `false`.

Return:\
The very same handle address received, upon success.\
`nullptr` if the handle is invalid or if **Fname** is `nullptr`.

\
Behavior:\
This overload writes the extension into a buffer you supply, so it allocates no memory and leaves nothing to release.\
The extraction rule is the same as the main version's.\
If **Fnsize** does not reach the length of the extension, only the first **Fnsize** characters are copied and the result comes out trimmed.\
With `0` the delivery is an empty string.\
The terminator is written right after the last copied character, outside the **Fnsize** count: **Fname** needs **Fnsize** plus one bytes available.