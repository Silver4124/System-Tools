# resize
### - ESP
Firma:
```cpp
void* resize(void* wnd, int _x, int _y, int _width, int _height, bool SetPosition = false, bool SetSize = true);
```
Parámetros:
* **Wnd**: Handle Ventana a redimensionar o desplazar.
* **_x**: Coordenada **x** del nuevo emplazamiento de la ventana en pantalla.
* **_y**: Coordenada **y** del nuevo emplazamiento de la ventana en pantalla.
* **_width**: Nuevo ancho, en píxeles, de la superficie dibujable.
* **_height**: Nuevo alto, en píxeles, de la superficie dibujable.
* **SetPosition**: Si es `true`, se aplican también las coordenadas **_x** e **_y**. Por defecto `false`.
* **SetSize**: Si es `true`, se aplican las dimensiones **_width** e **_height**. Por defecto `true`.

Retorno:\
El mismo handle entregado, si era válido y la ventana estaba en pie. `nullptr` si el handle es inválido, si la ventana ya había cerrado, si **SetSize** pide una dimensión menor que un píxel, o si la operación no pudo completarse.

\
Comportamiento:\
Esta función cambia el tamaño, la posición, o ambos, de una ventana existente.\
Por defecto solo se redimensiona: **_x** e **_y** se ignoran salvo que **SetPosition** sea `true`.\
Del mismo modo, **_width** e **_height** se ignoran si **SetSize** es `false`.\
Si ambas banderas valen `false` la llamada equivale a no hacer nada y devuelve el handle intacto.\
Las dimensiones entregadas describen la superficie dibujable; el módulo calcula el marco que el sistema añade alrededor.\
Al cambiar el tamaño, el búfer de píxeles se reconstruye con las nuevas dimensiones y el contenido ya escrito se conserva anclado en la esquina superior izquierda: lo que cabe se mantiene, lo que desborda se pierde, y las zonas recién creadas aparecen en negro.\
El búfer pasa a ser un bloque distinto, de modo que el puntero entregado en su momento por `window::pixels` queda obsoleto: conviene volver a consultar las dimensiones con `window::getsize` y pedir de nuevo el puntero antes de seguir escribiendo.\
El evento `window_box::size` llega al mailman con la reorganización ya hecha, así que desde esa consulta se puede reaprovechar el búfer nuevo.\
Si **SetSize** vale `true` y alguna de las dimensiones es menor que un píxel, la llamada se rechaza: la superficie dibujable nunca se reduce a cero, pues el sistema reserva esa condición para la ventana minimizada.\
El usuario también puede redimensionar o mover la ventana con el ratón, y ese cambio llega al mailman como el evento [[[Enum window_box]]](Enum%20window_box.md) `window_box::size`.

### - ENG
Signature:
```cpp
void* resize(void* wnd, int _x, int _y, int _width, int _height, bool SetPosition = false, bool SetSize = true);
```
Parameters:
* **Wnd**: Handle Window to resize or move.
* **_x**: **x** coordinate of the window's new placement on screen.
* **_y**: **y** coordinate of the window's new placement on screen.
* **_width**: New width, in pixels, of the drawable surface.
* **_height**: New height, in pixels, of the drawable surface.
* **SetPosition**: When `true`, the **_x** and **_y** coordinates are applied as well. Defaults to `false`.
* **SetSize**: When `true`, the **_width** and **_height** dimensions are applied. Defaults to `true`.

Return:\
The very same handle if it was valid and the window was standing. `nullptr` if the handle is invalid, the window had already closed, **SetSize** asks for a dimension below one pixel, or the operation could not be completed.

\
Behavior:\
This function changes the size, the position, or both, of an existing window.\
By default only the size changes: **_x** and **_y** are ignored unless **SetPosition** is `true`.\
Likewise **_width** and **_height** are ignored when **SetSize** is `false`.\
If both flags are `false` the call is equivalent to doing nothing and returns the handle untouched.\
The handed dimensions describe the drawable surface; the module computes the frame the system adds around it.\
When the size changes, the pixel buffer is rebuilt for the new dimensions and the content already written is preserved anchored to the upper-left corner: whatever still fits survives, whatever overflows is lost, and the newly created areas appear black.\
The buffer becomes a different block, so the pointer handed out earlier by `window::pixels` is left obsolete: it is worth consulting the dimensions again with `window::getsize` and asking for the pointer anew before continuing to write.\
The `window_box::size` event reaches the mailman once the rearrangement is done, so the new buffer can be picked up from that callback.\
When **SetSize** is `true` and either dimension is below one pixel, the call is rejected: the drawable surface is never shrunk to zero, since the system reserves that condition for the minimized window.\
The user can also resize or move the window with the mouse, and that change reaches the mailman as the [[[Enum window_box]]](Enum%20window_box.md) `window_box::size` event.
