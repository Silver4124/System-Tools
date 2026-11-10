# read
### - ESP
Firma:
```cpp
UWORD read(void* file, void* buffer, UWORD bytes);
```
Parámetros:
* **File**: Handle Fichero de origen. Debe poseer el permiso `file_mode::read`.
* **Buffer**: Dirección de memoria donde se volcarán los bytes leídos. Debe tener capacidad suficiente para albergar **Bytes** bytes.
* **Bytes**: Número entero sin signo del tamaño de la palabra del procesador, que representa la cantidad máxima de bytes a leer.

Retorno:\
Entero sin signo del tamaño de la palabra del procesador: cantidad de bytes realmente leídos.\
`0` en caso de error o al alcanzar el final del fichero.

\
Comportamiento:\
Toma los datos brutos de un fichero ya abierto, tal como están guardados, sin aplicar decodificación ni interpretación.\
Lee desde la posición actual del puntero de E/S del handle y lo avanza tantos bytes como los retornados.\
Si el handle no fue abierto con `file_mode::read`, retorna `0` sin afectar el fichero.\
Nunca añade el carácter de terminación nula (`'\0'`) al **Buffer**, ni se detiene antes de lo pedido para respetar delimitadores.\
Ante una petición mayor que lo que resta de fichero, retorna únicamente los bytes disponibles.\
El valor `0` es ambiguo por naturaleza: significa tanto "no se leyó nada" como "se alcanzó el final del fichero", y también "el handle era inválido o carecía de permisos".\
Para distinguir un fallo real de una lectura al final del fichero, compara el retorno con `file::size(file) - file::tell(file)` antes de la llamada.\
Para leer una cadena terminada en nulo está `file::gets`.\
Para gobernar esa posición están `file::tell` y `file::seek`.

### - ENG
Signature:
```cpp
UWORD read(void* file, void* buffer, UWORD bytes);
```
Parameters:
* **File**: Source Handle File. It must hold the `file_mode::read` permission.
* **Buffer**: Memory address where the read bytes will be dumped. It must have enough capacity to hold **Bytes** bytes.
* **Bytes**: Processor word-sized unsigned integer representing the maximum amount of bytes to read.

Return:\
Processor word-sized unsigned integer: amount of bytes actually read.\
`0` upon failure or when the end of the file is reached.

\
Behavior:\
It takes the raw data of an already opened file, exactly as stored, with no decoding or interpretation.\
It reads from the handle's current I/O pointer position and advances it by as many bytes as were returned.\
If the handle was not opened with `file_mode::read`, it returns `0` without affecting the file.\
It never appends the null-terminator character (`'\0'`) to **Buffer**, nor does it stop short of the request to honor delimiters.\
When asking for more than what is left in the file, it only returns the available bytes.\
The `0` value is ambiguous by nature: it means both "nothing was read" and "the end of the file was reached", as well as "the handle was invalid or lacked permissions".\
To tell a real failure apart from an end-of-file reading, compare the return value with `file::size(file) - file::tell(file)` before the call.\
For a null-terminated string there is `file::gets`.\
For steering that position there are `file::tell` and `file::seek`.
