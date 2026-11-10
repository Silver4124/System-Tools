# get_system_variable
### - ESP
Firma:
```cpp
void* get_system_variable(const char* RESTRICT var_name, const char* RESTRICT lib_target = nullptr);
```
Parámetros:
* **Var_name**: Nombre de la variable que se desea obtener.
* **Lib_target**: Nombre de la librería dinamica de donde se desea obtener la variable. En caso de ser `nullptr`, se asumirá que deseas obtener dicha variable del propio sistema en lugar de alguna librería dinamica.

Retorno:\
En caso de exito absoluto, retorna un puntero al valor dentro de la tabla de simbolos de la variable deseada: se puede leer y cambiar su valor.\
Si la variable está protegida, retorna igualmente un puntero a su valor, pero solo permite la lectura.\
En caso de fallo, retorna `nullptr`: porque no se pudo cargar dicha variable, o porque no existe en esa libreria dinamica ni en el sistema.

\
Comportamiento:\
Obtiene un puntero a una variable dentro de una libreria dinamica cargada en la RAM, o del entorno del propio sistema.\
Si la libreria dinamica no está cargada en la RAM, esta función la carga automaticamente y reintenta la busqueda desde alli.\
La auto-carga solo cubre librerias dinamicas del sistema.\
Sin nombre de libreria, asume que la variable se busca en el propio sistema.\
Si una variable está protegida depende del sistema consultado y del valor pedido; ignorarlo puede derivar en **Segmentation Fault**.

### - ENG
Signature:
```cpp
void* get_system_variable(const char* RESTRICT var_name, const char* RESTRICT lib_target = nullptr);
```
Parameters:
* **Var_name**: Name of the variable to retrieve.
* **Lib_target**: Name of the dynamic library from which to retrieve the variable. If it is `nullptr`, the function assumes that you want to retrieve the variable from the system itself rather than from a dynamic library.

Return:\
On complete success, returns a pointer to the value of the requested variable in the symbol table: it can be read and its value changed.\
If the variable is protected, it still returns a pointer to its value, but read-only.\
On failure, returns `nullptr`: because the variable could not be loaded, or because it does not exist in that dynamic library or in the system.

\
Behavior:\
Obtains a pointer to a variable inside a dynamic library loaded in RAM, or from the system environment itself.\
If the dynamic library is not loaded in RAM, this function loads it automatically and retries the search from there.\
Auto-loading only covers system dynamic libraries.\
With no library name given, it assumes the variable is looked up in the system itself.\
Whether a variable is protected depends on the system queried and the value requested; ignoring this can lead to **Segmentation Fault**.