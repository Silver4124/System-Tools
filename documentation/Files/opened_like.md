# opened_like
### - ESP
Firma:
```cpp
BYTE opened_like(void* file);
```
Parámetros:
* **File**: Handle Fichero o Handle Folder a evaluar.

Retorno:\
Permisos de acceso y creación del Handle Fichero, en caso de éxito.\
`0xFF` cuando el puntero no es una handle válida de la librería; es el `-1` de error proyectado sobre el tipo `BYTE`.

\
Comportamiento:\
Devuelve exactamente el valor `Mode` que se entregó en la apertura, tal y como quedó guardado en el handle.\
No vuelve a consultar el estado actual del elemento: describe cómo se abrió, no lo que el sistema permite hoy.\
Se puede llamar sobre un Handle Fichero o sobre un Handle Folder, en cualquier momento y sin desplazar el puntero de E/S.\
Sobre una handle inválida devuelve `0xFF`, un valor que no corresponde a ninguna combinación habitual de `file_mode`.\
Para el significado de cada marca consulte [[[Enum file_mode]]](Enum%20file_mode.md).

### - ENG
Signature:
```cpp
BYTE opened_like(void* file);
```
Parameters:
* **File**: Handle File or Handle Folder to evaluate.

Return:\
The access and creation permissions of the Handle File, upon success.\
`0xFF` when the pointer is not a valid handle of the library; that is the error `-1` projected onto the `BYTE` type.

\
Behavior:\
It returns exactly the `Mode` value handed over at opening, as it was stored inside the handle.\
It does not query the element's current state again: it describes how the handle was opened, not what the system allows today.\
It works on a Handle File and on a Handle Folder alike, at any moment and without moving the I/O pointer.\
On an invalid handle it returns `0xFF`, a value that no usual combination of `file_mode` produces.\
For the meaning of each flag see [[[Enum file_mode]]](Enum%20file_mode.md).
