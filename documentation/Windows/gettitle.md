# gettitle
### - ESP
Firma:
```cpp
void* gettitle(void* wnd, char** title, bool* minimized);
```
Parámetros:
* **Wnd**: Handle Ventana a consultar.
* **Title**: Puntero a `char*` donde la función almacena una cadena de terminación nula con el encabezado actual. Puede ser `nullptr` si no se desea ese dato.
* **Minimized**: Puntero a `bool` donde la función deposita `true` si la ventana está minimizada. Puede ser `nullptr`.

Retorno:\
El mismo handle entregado si era válido y la ventana estaba en pie, también cuando ambos punteros de salida son `nullptr`.\
`nullptr` si el handle es inválido, si la ventana ya había cerrado o si el encabezado fue pedido pero no pudo leerse.\
Cuando el encabezado no puede leerse, la salida **Title** no es modificada.

\
Comportamiento:\
Esta función lee el texto que encabeza la ventana y, opcionalmente, su estado de minimizado.\
**Title** recibe siempre una cadena recién creada por el módulo, independiente de la que la ventana muestra en pantalla.\
Cada invocación libera por su cuenta la cadena anterior antes de instalar la nueva.\
No la libere uno mismo entre consultas: liberarla a mano exige dejar el puntero entregado en `nullptr`, o la siguiente llamada intentará liberar una cadena que ya no existe.\
La última cadena obtenida sí debe liberarse con `delete[]` cuando deje de necesitarla.\
El estado minimizado refleja el que el módulo conoce de la ventana.\
En una ventana propia, se registra como minimizada cuando `window::show` la minimiza o cuando el evento [[[Enum window_box]]](Enum%20window_box.md) `window_box::size` llega con alguna dimensión en `0`, y vuelve a considerarse restaurada cuando se muestra de nuevo.\
Sobre un handle externo devuelto por `window::active`, la salida **Minimized** nunca es modificada: el estado de una ventana ajena no es consultable.

### - ENG
Signature:
```cpp
void* gettitle(void* wnd, char** title, bool* minimized);
```
Parameters:
* **Wnd**: Handle Window to query.
* **Title**: Pointer to `char*` where the function stores a null-terminated string with the current heading. May be `nullptr` if that datum is not wanted.
* **Minimized**: Pointer to `bool` where the function deposits `true` when the window is minimized. May be `nullptr`.

Return:\
The very same handle if it was valid and the window was standing, also when both outputs are `nullptr`.\
`nullptr` if the handle is invalid, the window had already closed, or the heading was requested but could not be read.\
When the heading cannot be read, the **Title** output is left unmodified.

\
Behavior:\
This function reads the text heading the window and, optionally, its minimized state.\
**Title** always receives a string freshly created by the module, independent from the one the window shows on screen.\
Each invocation frees the previous string on its own before installing the new one.\
Do not free it yourself between queries: freeing it by hand requires leaving the handed pointer at `nullptr`, or the next call will try to release a string that no longer exists.\
The last string obtained does have to be released with `delete[]` once it is no longer needed.\
The minimized state reflects what the module knows about the window.\
In one's own window it is recorded as minimized when `window::show` minimizes it or when the [[[Enum window_box]]](Enum%20window_box.md) `window_box::size` event arrives with either dimension as `0`, and it is considered restored again once the window is shown anew.\
Over an external handle returned by `window::active`, the **Minimized** output is never modified: the state of a foreign window is not consultable.
