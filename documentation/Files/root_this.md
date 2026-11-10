# root_this
### - ESP
Firma:
```cpp
void root_this(char** fname, UWORD* fnsize, bool stacked = false);
```
Parámetros:
* **Fname**: Puntero a un `char*` donde la función almacena la dirección del búfer recién creado con la ruta raíz. Puede ser `nullptr` si solo se desea la longitud.
* **Fnsize**: Dirección donde la función almacena la longitud de la ruta entregada. Puede ser `nullptr` si solo se desea la cadena.
* **Stacked**: Gestión de memoria usada para destruir el búfer anterior y crear el nuevo. Con `false` (por defecto) se emplean los asignadores de C y el resultado se libera con `memory::free`; con `true` se emplean los de C++ y se libera con `delete[]`. Pese al nombre, no tiene relación con la pila de ejecución; consulta ["The `stacked` term"](../Types.md).

Comportamiento:\
Localiza el directorio desde el cual se ejecuta el programa, en forma absoluta.\
No depende del directorio de trabajo actual, que puede ser otro cuando el programa se lanza desde fuera de su carpeta.\
La ruta incluye el separador final, así que basta con concatenar el nombre del recurso para obtener una ruta válida.\
Con **Fname** y **Fnsize** ambos en `nullptr`, la función no hace nada.\
El búfer lo crea la función; si **Fname** ya apuntaba a un búfer propio, ese búfer se destruye y se sustituye cuando **Stacked** es `true`.\
Cómo liberarlo depende de ese mismo valor, y la primera vez conviene entregar el puntero en `nullptr`.\
Se consulta una sola vez al inicio del programa y se reutiliza para abrir con `file::open` los recursos que viajan junto al ejecutable.\
La longitud entregada cuando **Fname** es `nullptr` corresponde a la ruta completa del ejecutable, sin recortar hasta el separador.\
Esa cifra difiere de la del directorio obtenido al pedir la cadena, así que para evitar la discrepancia consulta ambos argumentos a la vez.

### - ENG
Signature:
```cpp
void root_this(char** fname, UWORD* fnsize, bool stacked = false);
```
Parameters:
* **Fname**: Pointer to a `char*` where the function stores the address of the freshly created buffer holding the root path. It may be `nullptr` when only the length is wanted.
* **Fnsize**: Address where the function stores the length of the delivered path. It may be `nullptr` when only the string is wanted.
* **Stacked**: Memory management used to destroy the previous buffer and create the new one. With `false` (default) the C allocators are used and the result is released with `memory::free`; with `true` the C++ allocators are used and it is released with `delete[]`. Despite its name, it has nothing to do with the execution stack; see ["The `stacked` term"](../Types.md).

Behavior:\
It locates the directory from which the program runs, in absolute form.\
It does not depend on the current working directory, which may be elsewhere when the program is launched from outside its folder.\
The path carries the trailing separator, so concatenating the resource name is enough to get a valid path.\
With **Fname** and **Fnsize** both `nullptr`, the function does nothing.\
The function creates the buffer; if **Fname** already pointed to your own buffer, that buffer is destroyed and replaced when **Stacked** is `true`.\
How you release it depends on that same value, and on the first call it is worth handing the pointer over as `nullptr`.\
It is queried once at program start and then reused to open the resources shipped next to the executable with `file::open`.\
The length delivered when **Fname** is `nullptr` corresponds to the full executable path, not trimmed down to the separator.\
That figure differs from the directory length obtained when asking for the string, so to avoid the mismatch query both arguments together.

## Overloads
### - ESP
Firma:
```cpp
void root_this(char* fname, UWORD fnsize);
```
Parámetros:
* **Fname**: Búfer del llamador donde se copia la ruta. No puede ser `nullptr`.
* **Fnsize**: Cantidad de caracteres de texto que la función puede escribir en **Fname**, sin contar el terminador.

Comportamiento:\
Vuelca la ruta en un búfer que aportas tú, así que no asigna memoria ni deja nada que liberar.\
A diferencia de la versión principal, la entrega es la ruta completa del ejecutable, sin recortar hasta el separador: obtienes la ubicación del binario, no su carpeta.\
Para quedarte con el directorio, recorta la cadena en su último separador.\
Cuando el ejecutable no puede localizarse, **Fname** queda en cadena vacía.\
La escritura se recorta a **Fnsize** en ambas plataformas: si la ruta mide más, se copian solo los primeros **Fnsize** caracteres y la cadena queda cortada a media ruta.\
El terminador se escribe justo después del último carácter copiado, fuera de la cuenta de **Fnsize**: **Fname** necesita **Fnsize** más un bytes disponibles.\
Con **Fnsize** en `0`, **Fname** queda en cadena vacía.\
Para conocer la medida escrita, calcula la longitud de la cadena resultante.
### - ENG
Signature:
```cpp
void root_this(char* fname, UWORD fnsize);
```
Parameters:
* **Fname**: Caller-supplied buffer where the path is copied. It cannot be `nullptr`.
* **Fnsize**: Amount of text characters the function may write into **Fname**, not counting the terminator.

Behavior:\
This overload pours the path into a buffer you supply, so it allocates no memory and leaves nothing to release.\
Unlike the main version, the delivery here is the full executable path, not trimmed down to the separator: you get the binary location, not its folder.\
To keep just the directory, trim the string at its last separator.\
When the executable cannot be located, **Fname** is left as an empty string.\
The write is trimmed to **Fnsize** on both platforms: when the path measures more, only the first **Fnsize** characters are copied and the string is cut in the middle of the route.\
The terminator goes right after the last character copied, outside the **Fnsize** count: **Fname** needs **Fnsize** plus one bytes available.\
With **Fnsize** at `0`, **Fname** is left as an empty string.\
To know the length written, compute the length of the resulting string.