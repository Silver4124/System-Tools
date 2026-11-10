# clear
### - ESP
Firma:
```cpp
void* clear(void* wnd, unsigned int _color = 0x000000);
```
Parámetros:
* **Wnd**: Handle Ventana cuya superficie dibujable se desea rellenar.
* **_color**: Color con el que se rellenará cada píxel de la superficie, en el formato `0x00BBGGRR`. Por defecto `0x000000`, negro puro.

Retorno:\
El mismo handle entregado, si la ventana estaba en pie. `nullptr` si el handle es inválido, si la ventana ya había cerrado o si se trata de un handle externo.\
Si el búfer de píxeles de la ventana no está disponible, la función no rellena nada y regresa igualmente el handle intacto.

\
Comportamiento:\
Esta función tiñe de un único color toda la superficie dibujable de la ventana, sin importar su tamaño actual.\
Acto seguido pide el repintado para que el cambio se haga visible.\
Es la forma habitual de limpiar el fotograma anterior al comenzar a dibujar el siguiente, y también la manera de conseguir un fondo de color uniforme.\
El relleno solo afecta al búfer de la ventana: no borra la ventana, no oculta su contenido de pantalla ni modifica su título, su tamaño o su estado de foco.\
El color sigue el formato `0x00BBGGRR` descrito en `window::draw`.\
Para rellenar solo una zona con un mapa de píxeles, usa `window::draw`; para escribir píxel a píxel, usa `window::pixels`.
Tras un cambio de tamaño, las zonas recién creadas aparecen ya en negro; `window::clear` es la forma de teñir la superficie entera con otro color después de redimensionar.\
Sobre una ventana oculta, el relleno se aplica igualmente al búfer y quedará visible cuando `window::show` la devuelva a pantalla.

### - ENG
Signature:
```cpp
void* clear(void* wnd, unsigned int _color = 0x000000);
```
Parameters:
* **Wnd**: Handle Window whose drawable surface is to be filled.
* **_color**: Color with which every pixel of the surface will be filled, in the `0x00BBGGRR` format. Defaults to `0x000000`, pure black.

Return:\
The very same handle if the window was standing. `nullptr` if the handle is invalid, the window had already closed, or the handle is external.\
If the window's pixel buffer is unavailable, the function fills nothing and still returns the handle untouched.

\
Behavior:\
This function dyes a single color across the window's entire drawable surface, regardless of its current size.\
It then requests repainting so the change becomes visible.\
It is the usual way to wipe the previous frame before starting to draw the next one, and also the way to get a uniform colored background.\
The filling only affects the window's buffer: it does not erase the window, hide its content from the screen, or modify its title, size, or focus state.\
The color follows the `0x00BBGGRR` format described in `window::draw`.\
To fill only one zone with a pixel map, use `window::draw`; to write pixel by pixel, use `window::pixels`.
After a size change, the newly created zones already appear black; `window::clear` is the way to dye the whole surface another color after resizing.\
Over a hidden window, the filling is applied to the buffer just the same and will show once `window::show` brings it back to the screen.
