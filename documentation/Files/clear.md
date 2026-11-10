# clear
### - ESP
Firma:
```cpp
UWORD clear(const char* filepath);
```
Parámetros:
* **Filepath**: Ruta del fichero o carpeta a vaciar.

Retorno:\
Entero sin signo del tamaño de la palabra del procesador: el tamaño que tenía el fichero antes de vaciarse, medido en bytes.\
Sobre una carpeta, la cantidad de elementos eliminados.\
`0` en caso de error.

\
Comportamiento:\
Deja el contenido del elemento indicado en cero, conservando el elemento mismo.\
Tras vaciar un fichero, este sigue existiendo en su ruta.\
Tras vaciar una carpeta, sigue existiendo aunque quede sin elementos.\
No exige que tú dispongas de una handle propia sobre la ruta.\
Si la ruta no existe o no puede abrirse, retorna `0` sin modificar nada.\
Tras vaciar el contenido, la posición en que queda el puntero de E/S de una handle abierta aparte no es la misma en un sistema que en otro.\
Para colocarlo donde lo necesites está `file::seek`.\
Para eliminar el elemento por completo está `file::remove`.

### - ENG
Signature:
```cpp
UWORD clear(const char* filepath);
```
Parameters:
* **Filepath**: Path of the file or folder to empty.

Return:\
Processor word-sized unsigned integer: the size the file had before being emptied, measured in bytes.\
On a folder, the amount of entries deleted.\
`0` upon failure.

\
Behavior:\
It leaves the content of the indicated element at zero, keeping the element itself.\
After emptying a file, the file still exists at its path.\
After emptying a folder, the folder still exists even though it has no entries.\
It does not require you to hold a handle of your own over the path.\
If the path does not exist or cannot be opened, it returns `0` without modifying anything.\
Once the content is emptied, the position where a separately open handle's I/O pointer ends up is not the same on one system as on the other.\
To place it where you need it there is `file::seek`.\
To erase the element completely there is `file::remove`.

## Overloads
### - ESP
Firma:
```cpp
UWORD clear(void* file);
```
Parámetros:
* **File**: Handle Fichero o Handle Folder a vaciar.

Retorno:\
Entero sin signo del tamaño de la palabra del procesador: el tamaño que tenía el fichero antes de vaciarse, medido en bytes.\
Sobre una carpeta, la cantidad de elementos eliminados.\
`0` en caso de error.

\
Comportamiento:\
Vacía el elemento desde una handle ya abierta, sin necesidad de volver a abrirla.\
No comprueba los permisos de la handle: si esta no fue abierta con `file_mode::write`, el contenido no se ve afectado, aunque la función devuelve igualmente la longitud anterior como si el vaciado hubiera tenido lugar.\
Sobre una Handle Folder, todos sus elementos directos son eliminados de disco, incluyendo el contenido de las subcarpetas, y el retorno es la cantidad de elementos borrados.\
La carpeta en sí permanece intacta.\
La handle sigue siendo válida después del vaciado; solo su contenido desaparece.\
Para verificar los permisos antes de confiar en el resultado está `file::opened_like`.

### - ENG
Signature:
```cpp
UWORD clear(void* file);
```
Parameters:
* **File**: Handle File or Handle Folder to empty.

Return:\
Processor word-sized unsigned integer: the size the file had before being emptied, measured in bytes.\
On a folder, the amount of entries deleted.\
`0` upon failure.

\
Behavior:\
It empties the element from an already open handle, without needing to open it again.\
It does not check the permissions of the handle: if it was not opened with `file_mode::write`, the content remains untouched, yet the function still returns the previous length as if the emptying had taken place.\
On a Handle Folder, all of its direct entries are deleted from disk, including the contents of subfolders, and the return value is the amount of erased entries.\
The folder itself stays intact.\
The handle remains valid after emptying; only its content disappears.\
To check the permissions before trusting the result there is `file::opened_like`.
