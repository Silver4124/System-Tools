# pixels
### - ESP
Firma:
```cpp
void pixels(void* wnd, unsigned int** _px, UWORD* _px_am);
```
Parámetros:
* **Wnd**: Handle Ventana cuyo búfer de píxeles se desea obtener.
* **_px**: Puntero a `unsigned int*` donde la función almacena la dirección del búfer de píxeles de la ventana. Puede ser `nullptr`.
* **_px_am**: Puntero a `UWORD` donde la función almacena la cantidad de elementos que ocupa ese búfer. Puede ser `nullptr`.

Comportamiento:\
Esta función entrega acceso directo al búfer de píxeles que la ventana mantiene en memoria: no copia nada ni reserva memoria nueva, simplemente señala el búfer propio de la ventana.\
Si **Wnd** es inválido, si la ventana ya cerró o si se trata de un handle externo, ningún puntero es modificado.\
Con ambos punteros en `nullptr`, la función no realiza consulta alguna.\
El búfer es propiedad del módulo: bajo ninguna circunstancia debe liberarse con `delete` ni con `delete[]`, y su contenido puede desaparecer cuando la ventana cierre.\
La organización es fila tras fila, con tantos elementos como ancho tenga la superficie dibujable: el píxel en la columna **x** y la fila **y** es el elemento `x + y * width`.\
El ancho y el alto se consultan con `window::getsize`, y la cantidad total de elementos coincide con su producto; también puede leerse a través de **_px_am**.\
Cada elemento codifica su color con el formato `0x00BBGGRR`: el byte de mayor orden se ignora y los tres siguientes contienen azul, verde y rojo. Al crear la ventana, todo el búfer arranca en negro.\
Escribir sobre el búfer no modifica lo que se ve en pantalla hasta que la ventana repinta; pide el repintado inyectando la etiqueta [[[Enum window_box]]](Enum%20window_box.md) `window_box::paint` con `window::sendmessage`.\
El puntero entregado deja de ser fiable cuando la ventana cambia de tamaño, pues su búfer se reorganiza: vuelve a consultarlo después de cualquier `window::resize` o del evento `window_box::size`.\
Si en lugar de escribir en el búfer de la ventana prefieres copiar un mapa de píxeles tuyo hacia una zona concreta, usa `window::draw`.

### - ENG
Signature:
```cpp
void pixels(void* wnd, unsigned int** _px, UWORD* _px_am);
```
Parameters:
* **Wnd**: Handle Window whose pixel buffer is to be obtained.
* **_px**: Pointer to `unsigned int*` where the function stores the address of the window's pixel buffer. May be `nullptr`.
* **_px_am**: Pointer to `UWORD` where the function stores the amount of elements that buffer occupies. May be `nullptr`.

Behavior:\
This function delivers direct access to the pixel buffer the window keeps in memory: it copies nothing and reserves no new memory, it simply points at the window's own buffer.\
If **Wnd** is invalid, the window already closed, or the handle is external, no pointer is modified.\
With both pointers `nullptr`, the function performs no query at all.\
The buffer belongs to the module: under no circumstance may it be released with `delete` or `delete[]`, and its content can vanish when the window closes.\
The organization is row after row, with as many elements as the drawable surface's width: the pixel at column **x** and row **y** is the element `x + y * width`.\
Width and height are consulted with `window::getsize`, and the total amount of elements matches their product; it can also be read through **_px_am**.\
Each element encodes its color in the `0x00BBGGRR` format: the highest-order byte is ignored and the following three carry blue, green, and red. Upon creating the window, the whole buffer starts black.\
Writing into the buffer does not change what is seen on screen until the window repaints; request the repainting by injecting the [[[Enum window_box]]](Enum%20window_box.md) `window_box::paint` tag with `window::sendmessage`.\
The delivered pointer stops being trustworthy when the window changes size, since its buffer gets rearranged: consult it again after any `window::resize` or the `window_box::size` event.\
If instead of writing into the window's buffer you prefer copying one of your own pixel maps into a concrete zone, use `window::draw`.
