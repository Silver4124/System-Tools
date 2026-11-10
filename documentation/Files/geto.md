# geto
### - ESP
Firma:
```cpp
UWORD geto(void* file, void** Hobj);
```
Parámetros:
* **File**: Handle Fichero de origen. Debe poseer el permiso `file_mode::read`.
* **Hobj**: Puntero a un `void*` donde la función almacena la handle del objeto reconstruido.

Retorno:\
Entero sin signo del tamaño de la palabra del procesador: cantidad de bytes consumidos por la lectura.\
`0` cuando no se reconoce ningún objeto en la posición actual.

\
Comportamiento:\
Reconstruye un objeto a partir de los bytes grabados por su pareja, leyendo desde la posición actual del puntero de E/S.\
No devuelve un estado inerte: el objeto renace en el programa actual.\
Una Handle Fichero vuelve a abrir su ruta con los permisos grabados.\
Una Handle Window crea una ventana real en pantalla.\
Una Handle JSON devuelve una estructura lista para `json::value`.\
Si **Hobj** ya contenía una handle, esa handle se destruye antes de asignar la nueva, según el tipo que se lee en su propia cabecera: los ficheros se cierran, las ventanas y los sonidos se destruyen, los hilos se eliminan.\
Por eso conviene entregar siempre un puntero `nullptr`, o uno ya vaciado por la propia función.\
La función que graba esos bytes es `file::apendo`.

### - ENG
Signature:
```cpp
UWORD geto(void* file, void** Hobj);
```
Parameters:
* **File**: Source Handle File. It must hold the `file_mode::read` permission.
* **Hobj**: Pointer to a `void*` where the function stores the handle of the rebuilt object.

Return:\
Processor word-sized unsigned integer: the amount of bytes consumed by the reading.\
`0` when no object is recognized at the current position.

\
Behavior:\
It rebuilds an object from the bytes recorded by its counterpart, reading from the current I/O pointer position.\
What comes back is not an inert state: the object is reborn in the current program.\
A Handle File opens its path again with the recorded permissions.\
A Handle Window creates a real window on screen.\
A Handle JSON delivers a structure ready for `json::value`.\
If **Hobj** already held a handle at the moment of the call, that handle is destroyed before assigning the new one, according to the type read in its own header: files are closed, windows and sounds are destroyed, threads are killed.\
That is why it is best to always hand over a `nullptr` pointer, or one already emptied by the function itself.\
The function that recorded those bytes is `file::apendo`.