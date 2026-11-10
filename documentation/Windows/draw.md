# draw
### - ESP
Firma:
```cpp
void* draw(void* wnd, int offsetX, int offsetY, unsigned int* _pixels, unsigned int _width, unsigned int _height);
```
Parámetros:
* **Wnd**: Handle Ventana sobre cuya superficie se copiará el mapa de píxeles.
* **OffsetX**: Coordenada **x** de la esquina superior izquierda donde aterrizará el mapa dentro de la ventana. Admite valores negativos, desplazando el mapa fuera del borde izquierdo.
* **OffsetY**: Coordenada **y** de esa misma esquina. Admite valores negativos.
* **_pixels**: Mapa de píxeles a copiar, organizado en **_width** \* **_height** elementos de 32 bits, fila tras fila y de izquierda a derecha. Cada elemento codifica un color con el formato `0x00BBGGRR`.
* **_width**: Ancho, en píxeles, del mapa entregado en **_pixels**.
* **_height**: Alto, en píxeles, del mapa entregado en **_pixels**.

Retorno:\
El mismo handle entregado, si la copia tuvo lugar o si fue recortada parcialmente. `nullptr` si el handle es inválido, si la ventana ya había cerrado o si se trata de un handle externo.\
Si **_pixels** es `nullptr`, si **_width** o **_height** valen `0`, o si el mapa cae por completo fuera de la superficie dibujable, la función no copia nada y regresa igualmente el handle intacto.

\
Comportamiento:\
Esta función vuelca un mapa de píxeles propio dentro de la superficie dibujable de la ventana, en la posición pedida, y deja el resto del contenido de la ventana tal como estaba.\
La copia es absoluta: los píxeles del mapa reemplazan a los existentes, sin combinarlos ni transparentarlos. Para dibujar respetando el fondo, entrega un mapa tan pequeño como la zona a tocar.\
Todo lo que desborde la superficie dibujable es recortado, tanto por la derecha y el pie como por la izquierda y el techo cuando los desplazamientos son negativos; si el recorte anula por completo el mapa, no se copia ningún píxel.\
Los colores del mapa siguen el formato `0x00BBGGRR`: el byte de mayor orden se ignora, y los tres siguientes contienen azul, verde y rojo. El color `0x000000` es negro puro y `0x00FFFFFF` es blanco.\
La copia termina en el búfer de la ventana, no en la pantalla: para hacerla visible hay que repintar, y `window::draw` ya lo pide por ti al concluir.\
Si prefieres escribir directamente sobre el búfer de la ventana, sin mapa intermedio ni copia, usa `window::pixels`; para llenar toda la superficie de un único color, usa `window::clear`.\
Tras un cambio de tamaño, la superficie puede haber crecido o encogido: vuelve a consultar `window::getsize` antes de calcular posiciones.

### - ENG
Signature:
```cpp
void* draw(void* wnd, int offsetX, int offsetY, unsigned int* _pixels, unsigned int _width, unsigned int _height);
```
Parameters:
* **Wnd**: Handle Window onto whose drawable surface the pixel map will be copied.
* **OffsetX**: **x** coordinate of the upper-left corner where the map will land inside the window. Negative values are allowed, shifting the map out through the left border.
* **OffsetY**: **y** coordinate of that same corner. Negative values are allowed.
* **_pixels**: Pixel map to copy, organized into **_width** \* **_height** elements of 32 bits, row after row and from left to right. Each element encodes a color in the `0x00BBGGRR` format.
* **_width**: Width, in pixels, of the map handed over in **_pixels**.
* **_height**: Height, in pixels, of the map handed over in **_pixels**.

Return:\
The very same handle if the copy took place or was partially clipped. `nullptr` if the handle is invalid, the window had already closed, or the handle is external.\
If **_pixels** is `nullptr`, if **_width** or **_height** is `0`, or if the map falls entirely outside the drawable surface, the function copies nothing and still returns the handle untouched.

\
Behavior:\
This function dumps one's own pixel map into the window's drawable surface, at the requested position, leaving the rest of the window's content as it was.\
The copy is absolute: the map's pixels replace the existing ones, without blending or transparency. To draw respecting the background, hand over a map as small as the zone to be touched.\
Anything overflowing the drawable surface is clipped, both by the right and bottom edges and by the left and top ones when the offsets are negative; if the clipping cancels the map entirely, no pixel is copied.\
The map's colors follow the `0x00BBGGRR` format: the highest-order byte is ignored and the following three carry blue, green, and red. The color `0x000000` is pure black and `0x00FFFFFF` is white.\
The copy ends in the window's buffer, not on the screen: making it visible requires repainting, and `window::draw` already requests it upon finishing.\
If you prefer writing directly onto the window's buffer, without an intermediate map nor a copy, use `window::pixels`; to fill the whole surface with a single color, use `window::clear`.\
After a size change, the surface may have grown or shrunk: consult `window::getsize` again before computing positions.
