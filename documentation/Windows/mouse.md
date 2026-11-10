# mouse
### - ESP
Firma:
```cpp
void* mouse(void* wnd, int* _x, int* _y, int* lclick, int* rclick);
```
Parámetros:
* **Wnd**: Handle Ventana respecto a la que se consulta el ratón.
* **_x**: Puntero a `int` donde la función almacena la coordenada **x** del cursor dentro de la superficie dibujable. Puede ser `nullptr`.
* **_y**: Puntero a `int` donde la función almacena la coordenada **y** del cursor dentro de la superficie dibujable. Puede ser `nullptr`.
* **lclick**: Puntero a `int` donde la función evoluciona el estado del botón izquierdo. Puede ser `nullptr`.
* **rclick**: Puntero a `int` donde la función evoluciona el estado del botón derecho. Puede ser `nullptr`.

Retorno:\
El mismo handle entregado, si era válido, la ventana estaba en pie y al menos un puntero fue pedido. `nullptr` si el handle es inválido, si la ventana cerró, si todos los punteros son `nullptr` o si la consulta del ratón no pudo realizarse.\
Cuando la consulta falla, ningún puntero es modificado.

\
Comportamiento:\
Esta función fotografía el estado del ratón en el instante de la llamada: la posición del cursor y si cada botón está hundido en ese momento.\
La posición se entrega en coordenadas de la ventana, medidas desde la esquina superior izquierda de su superficie dibujable.\
No es una posición global: con el cursor sobre otra ventana o fuera de la zona dibujable, la coordenada sigue siendo relativa a la ventana consultada, y resulta negativa o supera su ancho y su alto.\
Los punteros **lclick** y **rclick** funcionan como memoria del estado previo: la función lee el valor que contienen, lo compara con la situación actual y escribe la nueva etiqueta de [[[Enum window_box]]](Enum%20window_box.md) resultante.
* Botón que pasa de suelto a pulsado: `window_box::lbutton_down` / `window_box::rbutton_down`.
* Botón que sigue pulsado entre dos consultas: `window_box::lbutton_hold` / `window_box::rbutton_hold`.
* Botón que se liberó entre dos consultas: `window_box::lbutton_up` / `window_box::rbutton_up`.
* Botón suelto y sin cambio: `window_box::def`.
Por eso debe consultarse en cada vuelta del bucle de trabajo y con regularidad: un pulsado y liberado entre dos llamadas se pierde.\
Si prefieres no hacer seguimiento del estado, puedes ignorar la posición y quedarte solo con los botones, o viceversa, entregando `nullptr` en lo que no te interese.\
La consulta no requiere que la ventana esté enfocada.\
En Linux esta función necesita acceso a los dispositivos de entrada del sistema; si el usuario no pertenece al grupo `input`, la consulta falla y la librería imprime un aviso indicando el comando `sudo usermod -aG input $USER` para conceder el permiso.

### - ENG
Signature:
```cpp
void* mouse(void* wnd, int* _x, int* _y, int* lclick, int* rclick);
```
Parameters:
* **Wnd**: Handle Window relative to which the mouse is consulted.
* **_x**: Pointer to `int` where the function stores the cursor's **x** coordinate inside the drawable surface. May be `nullptr`.
* **_y**: Pointer to `int` where the function stores the cursor's **y** coordinate inside the drawable surface. May be `nullptr`.
* **lclick**: Pointer to `int` where the function evolves the left button's state. May be `nullptr`.
* **rclick**: Pointer to `int` where the function evolves the right button's state. May be `nullptr`.

Return:\
The very same handle if it was valid, the window was standing, and at least one pointer was requested. `nullptr` if the handle is invalid, the window closed, all pointers are `nullptr`, or the mouse query could not be performed.\
When the query fails, no pointer is modified.

\
Behavior:\
This function photographs the mouse state at the instant of the call: the cursor position and whether each button is sunk at that moment.\
The position is delivered in window coordinates, measured from the upper-left corner of the drawable surface.\
It is not a global position: with the cursor over another window or outside the drawable area, the coordinate still belongs to the queried window, and comes out negative or beyond its width and height.\
The **lclick** and **rclick** pointers act as memory of the previous state: the function reads the value they hold, compares it with the current situation, and writes the resulting [[[Enum window_box]]](Enum%20window_box.md) tag.
* Button going from released to pressed: `window_box::lbutton_down` / `window_box::rbutton_down`.
* Button staying pressed between two queries: `window_box::lbutton_hold` / `window_box::rbutton_hold`.
* Button released between two queries: `window_box::lbutton_up` / `window_box::rbutton_up`.
* Released button with no change: `window_box::def`.
That is why it must be consulted on every turn of the work loop and regularly: a press and release happening between two calls is lost.\
If you prefer not to track state, you can ignore the position and keep only the buttons, or vice versa, handing over `nullptr` for whatever does not interest you.\
The query does not require the window to be focused.\
On Linux this function needs access to the system's input devices; if the user does not belong to the `input` group, the query fails and the library prints a notice with the `sudo usermod -aG input $USER` command needed to grant the permission.
