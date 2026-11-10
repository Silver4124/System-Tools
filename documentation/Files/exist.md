# exist
### - ESP
Firma:
```cpp
bool exist(const char* filepath);
```
Parámetros:
* **Filepath**: Ruta de fichero o de carpeta a consultar.

Retorno:\
`true` en caso de que sí exista, `false` en caso de error o de que no exista.\
Una ruta cuyo estado no se puede leer responde igual que una ruta inexistente: el `false` no dice cuál de los dos fue.

\
Comportamiento:\
Comprueba si la ruta apunta a un elemento presente en el disco, sea fichero o carpeta.\
No abre el elemento y no pide permisos sobre él.

### - ENG
Signature:
```cpp
bool exist(const char* filepath);
```
Parameters:
* **Filepath**: Path of the file or folder to query.

Return:\
`true` if it does exist, `false` upon error or if it does not exist.\
A path whose status cannot be read answers the same as a nonexistent path: the `false` does not say which of the two happened.

\
Behavior:\
It checks whether the path points to an element present on disk, file or folder alike.\
It opens nothing and asks no permissions over it.
