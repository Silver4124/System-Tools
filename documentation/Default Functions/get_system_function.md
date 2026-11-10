# get_system_function
### - ESP
Firma:
```cpp
void* get_system_function(const char* RESTRICT func_name, const char* RESTRICT lib_target = nullptr);
```
Parámetros:
* **Func_name**: Nombre de la función que se desea obtener.
* **Lib_target**: Nombre de la librería dinamica de donde se desea obtener la función. En caso de ser `nullptr`, su comportamiento varia dependiendo de la plataforma:
  * **En Windows**: Se asume que quieres obtener la función de `kernel32.dll`.
  * **En Linux**: Se asume que quieres obtener la función de `libc.so` o de `libc.so.X` (La `.X` siendo el número de versión correspondiente).

Retorno:\
En caso de exito, retorna un puntero a la función que deseabas obtener, listo para ser ejecutado como un puntero a función de C.\
En caso de fallo, retorna `nullptr`: porque no se pudo cargar dicha función, o porque dicha función no existe dentro de esa libreria dinamica.

\
Comportamiento:\
Obtiene un puntero a una función dentro de una libreria dinamica cargada en la RAM, para ejecutarla como una función de C.\
Si la libreria dinamica no está cargada en la RAM, esta función la carga automaticamente y reintenta la busqueda desde alli.\
La auto-carga solo cubre librerias dinamicas del sistema.

### - ENG
Signature:
```cpp
void* get_system_function(const char* RESTRICT func_name, const char* RESTRICT lib_target = nullptr);
```
Parameters:
* **Func_name**: Name of the function you want to obtain.
* **Lib_target**: Name of the dynamic library from which you want to obtain the function. If set to `nullptr`, behavior varies by platform:
  * **On Windows**: It assumes you want to retrieve the function from `kernel32.dll`. 
  * **On Linux**: It is assumed that you want to obtain the function from `libc.so` or `libc.so.X` (the `.X` being the corresponding version number).

Return:\
On success, it returns a pointer to the desired function, ready to be executed as a C function pointer.\
On failure, it returns `nullptr`: because the function could not be loaded, or because it does not exist within the specified dynamic library.

\
Behavior:\
Obtains a pointer to a function inside a dynamic library loaded in RAM, to run it as a C function.\
If the dynamic library is not loaded in RAM, this function loads it automatically and retries the search from there.\
Auto-loading only covers system dynamic libraries.