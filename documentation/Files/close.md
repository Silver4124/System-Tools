# close
### - ESP
Firma:
```cpp
void* close(void* file);
```
Parámetros:
* **File**: Handle Fichero o Handle Folder a cerrar.

Retorno:\
El mismo handle recibido, en caso de éxito.\
`nullptr` si el handle es `nullptr` o si ya había sido cerrado previamente.

\
Comportamiento:\
Libera un Handle Fichero o Handle Folder creado por `file::open` o `file::folder`, termina con la validez del handle y deja de retener el recurso que mantenía sobre la ruta abierta.\
Un puntero cerrado ya no puede volver a operarse, y cualquier función del módulo que lo reciba lo rechazará.\
Conviene asignarle `nullptr` en el código de usuario para no reutilizarlo por error.\
Cerrar un handle **no** elimina nada del disco: el fichero o la carpeta siguen existiendo.\
Ambos tipos de handle, de fichero y de carpeta, se cierran con esta misma función.\
Para borrar la ubicación apuntada está `file::remove`, una vez cerrado el handle.

### - ENG
Signature:
```cpp
void* close(void* file);
```
Parameters:
* **File**: Handle File or Handle Folder to close.

Return:\
The very same handle received, upon success.\
`nullptr` if the handle is `nullptr` or if it had already been closed before.

\
Behavior:\
It releases a Handle File or Handle Folder created by `file::open` or `file::folder`, ends the validity of the handle, and stops retaining the resource it held over the opened path.\
A pointer closed with this function can no longer be operated, and any function of the module that receives it will reject it.\
It is worth assigning it `nullptr` in your code to avoid reusing it by mistake.\
Closing a handle does **not** delete anything from disk: the file or folder still exists.\
Both kinds of handle, file and folder, are closed with this same function.\
To erase the pointed location there is `file::remove`, once the handle is closed.
