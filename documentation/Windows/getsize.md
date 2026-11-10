# getsize
### - ESP
Firma:
```cpp
void getsize(void* wnd, int* _x, int* _y, int* _width, int* _height);
```
Parámetros:
* **Wnd**: Handle Ventana a consultar.
* **_x**: Puntero a `int` donde la función almacena la coordenada **x** de la ventana en pantalla. Puede ser `nullptr` si no se desea ese dato.
* **_y**: Puntero a `int` donde la función almacena la coordenada **y** de la ventana en pantalla. Puede ser `nullptr`.
* **_width**: Puntero a `int` donde la función almacena el ancho actual de la superficie dibujable. Puede ser `nullptr`.
* **_height**: Puntero a `int` donde la función almacena el alto actual de la superficie dibujable. Puede ser `nullptr`.

Comportamiento:\
Esta función entrega el emplazamiento y las dimensiones con que la ventana existe en el momento de la consulta.\
Si **Wnd** es inválido, si la ventana ya cerró o si se trata de un handle externo, ningún destino es modificado.\
Con los cuatro destinos en `nullptr`, la función no realiza consulta alguna.\
El ancho y el alto describen la superficie dibujable (la misma que se pidió a `window::create` y que `window::resize` modifica), por lo que coinciden con las dimensiones del búfer entregado por `window::pixels`.\
La posición es la que el sistema registra para la ventana: cambia cuando el usuario la arrastra y cuando se invoca `window::resize` con **SetPosition** en `true`.\
Los cambios de tamaño se notifican al mailman con el evento [[[Enum window_box]]](Enum%20window_box.md) `window_box::size`.\
Mover la ventana sin cambiar su tamaño no tiene etiqueta propia: puede llegar como ese mismo evento `size` o no llegar.\
Es la consulta necesaria antes de dibujar por zonas con `window::draw`, pues sus coordenadas se interpretan respecto a la esquina superior izquierda de la superficie dibujable.

### - ENG
Signature:
```cpp
void getsize(void* wnd, int* _x, int* _y, int* _width, int* _height);
```
Parameters:
* **Wnd**: Handle Window to query.
* **_x**: Pointer to `int` where the function stores the **x** coordinate of the window on screen. May be `nullptr` if that datum is not wanted.
* **_y**: Pointer to `int` where the function stores the **y** coordinate of the window on screen. May be `nullptr`.
* **_width**: Pointer to `int` where the function stores the current width of the drawable surface. May be `nullptr`.
* **_height**: Pointer to `int` where the function stores the current height of the drawable surface. May be `nullptr`.

Behavior:\
This function delivers the placement and dimensions the window holds at the moment of the query.\
If **Wnd** is invalid, the window already closed, or the handle is external, no destination is modified.\
With all four destinations `nullptr`, the function performs no query at all.\
The width and height describe the drawable surface (the same one requested from `window::create` and modified by `window::resize`), so they match the dimensions of the buffer delivered by `window::pixels`.\
The position is the one the system records for the window: it changes when the user drags it and when `window::resize` is invoked with **SetPosition** as `true`.\
Size changes are notified to the mailman with the [[[Enum window_box]]](Enum%20window_box.md) `window_box::size` event.\
Moving the window without changing its size has no tag of its own: it may arrive as that same `size` event, or not arrive at all.\
It is the query needed before drawing region by region with `window::draw`, since its coordinates are interpreted from the upper-left corner of the drawable surface.
