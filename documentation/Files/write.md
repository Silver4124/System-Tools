# write
### - ESP
Firma:
```cpp
UWORD write(void* file, const void* buffer, UWORD bytes);
```
Parámetros:
* **File**: Handle Fichero de destino. Debe poseer el permiso `file_mode::write`.
* **Buffer**: Dirección de memoria de los datos a escribir.
* **Bytes**: Número entero sin signo del tamaño de la palabra del procesador, que representa la cantidad de bytes a escribir desde **Buffer**.

Retorno:\
Número entero sin signo del tamaño de la palabra del procesador, con la cantidad de bytes realmente escritos.\
`0` en caso de error.

\
Comportamiento:\
Vuelca datos brutos en un fichero ya abierto, sin añadir codificación, saltos de línea ni ningún otro carácter por su cuenta.\
La escritura se realiza en la posición actual del puntero de E/S del handle y lo avanza tantos bytes como los retornados.\
Si el puntero apunta al inicio, el contenido existente será sobrescrito; si apunta al final, el comportamiento es el de un añadido.\
El handle valida sus permisos antes de operar: si no fue abierto con `file_mode::write`, la función retorna `0` sin afectar el fichero, aunque la apertura haya incluido `file_mode::create`.\
Si **Bytes** es `0` o **Buffer** es `nullptr`, la función retorna `0` sin tocar el fichero.\
Un retorno menor que **Bytes** significa que la escritura se interrumpió.\
`0` no permite distinguir entre un handle inválido y un fallo del disco.\
Colocar el puntero está en `file::seek` y consultarlo en `file::tell`.\
Sobre esta función se construyen `file::apends`, `file::apendws`, `file::apendc` y `file::apendo`.

### - ENG
Signature:
```cpp
UWORD write(void* file, const void* buffer, UWORD bytes);
```
Parameters:
* **File**: Destination Handle File. It must hold the `file_mode::write` permission.
* **Buffer**: Memory address of the data to write.
* **Bytes**: Processor word-sized unsigned integer representing the amount of bytes to write from **Buffer**.

Return:\
Processor word-sized unsigned integer holding the amount of bytes actually written.\
`0` upon failure.

\
Behavior:\
It dumps raw data into an already opened file, adding no encoding, line breaks, or any other character on its own.\
The writing happens at the current I/O pointer position of the handle and advances it by as many bytes as were returned.\
If the pointer is at the beginning, the existing content gets overwritten; if it is at the end, the behavior is that of an append.\
The handle validates its permissions before operating: if it was not opened with `file_mode::write`, the function returns `0` without affecting the file, even when the opening included `file_mode::create`.\
If **Bytes** is `0` or **Buffer** is `nullptr`, the function returns `0` without touching the file.\
A return value lower than **Bytes** means the writing was interrupted.\
`0` alone cannot tell an invalid handle apart from a disk failure.\
Placing the pointer is `file::seek`'s job, querying it is `file::tell`'s.\
This function is what `file::apends`, `file::apendws`, `file::apendc` and `file::apendo` are built on.
