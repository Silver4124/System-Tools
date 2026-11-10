# keyboard
### - ESP
Firma:
```cpp
void* keyboard(void* wnd, BYTE** keys, UWORD* size, bool stacked = false);
```
Parámetros:
* **Wnd**: Handle Ventana de la que se desea leer el teclado.
* **Keys**: Puntero a `BYTE*` donde la función almacena el mapa de teclas. Puede ser `nullptr` si solo se desea conocer el tamaño del mapa.
* **Size**: Puntero a `UWORD` donde la función almacena la cantidad de bytes del mapa entregado. Puede ser `nullptr`.
* **stacked**: `true` para que la función se haga cargo de la memoria: libera el mapa anterior y entrega uno recién creado en cada llamada. `false` para que el mapa crezca con realloc conservando el puntero siempre que le sea posible. Por defecto `false`.

Retorno:\
El mismo handle entregado, si era válido y la ventana estaba en pie con el foco.\
`nullptr` si el handle es inválido, si la ventana cerró, si la ventana no está enfocada, si la consulta de teclado no está disponible, o si **Keys** y **Size** son ambos `nullptr`.\
Cuando **Size** es entregado, siempre recibe `32`, con independencia de que la lectura haya fallado.

\
Comportamiento:\
Esta función fotografía el estado del teclado en el instante de la llamada y lo entrega como un mapa de bits: el bit **n** de **Keys** está puesto si la tecla cuyo código es **n** se encuentra hundida.\
El mapa ocupa siempre 32 bytes, suficiente para cubrir los códigos del `0` al `255`. Para inspeccionar un código basta con mirar el bit correspondiente: byte `code / 8`, bit `code % 8`.\
Los códigos de las teclas con carácter imprimible se entregan traducidos al código de ese carácter (una misma letra puede llegar en mayúscula o minúscula según el estado de las teclas modificadoras).\
Las teclas sin representación imprimible conservan su código de teclado.\
La ventana debe poseer el foco: si no lo tiene, la lectura falla sin tocar el mapa. Usa `window::focus` y `window::focuswait` para asegurarlo.\
Al igual que `window::mouse`, esta consulta no acumula historia: una tecla pulsada y liberada entre dos llamadas no aparecerá en el mapa.\
Para no perder pulsaciones, consulta en cada vuelta del bucle o atiende los eventos [[[Enum window_box]]](Enum%20window_box.md) `key_down` y `key_up` entregados al mailman.\
Con **stacked** en `true`, el mapa anterior queda liberado por la función: no lo conserves ni lo borres por tu cuenta.\
Con **stacked** en `false`, el puntero puede cambiar de dirección tras el crecimiento, así que trabaja siempre contra el valor devuelto en **Keys**.\
Será el programador quien libere el mapa con la gestión de memoria correspondiente cuando ya no lo necesite.

### - ENG
Signature:
```cpp
void* keyboard(void* wnd, BYTE** keys, UWORD* size, bool stacked = false);
```
Parameters:
* **Wnd**: Handle Window from which you want to read the keyboard.
* **Keys**: Pointer to `BYTE*` where the function stores the key map. It can be `nullptr` if only the size of the map is required.
* **Size**: Pointer to a `UWORD` where the function stores the number of bytes of the provided map. Can be `nullptr`.
* **stacked**: `true` to have the function take charge of memory management: it frees the previous map and provides a newly created one with each call. `false` to allow the map to grow using `realloc`, keeping the same pointer whenever possible. Defaults to `false`.

Return:\
The very same handle if it was valid and the window was standing with the focus.\
`nullptr` if the handle is invalid, the window closed, the window is not focused, the keyboard query is unavailable, or both **Keys** and **Size** are `nullptr`.\
When **Size** is provided, it will always receive the value `32`, regardless of whether the read operation failed.

\
Behavior:\
This function captures the state of the keyboard at the moment it is called and returns it as a bitmask: the **n**-th bit of **Keys** is set if the key with code **n** is currently pressed.\
The bitmask always occupies 32 bytes, which is sufficient to cover key codes from `0` to `255`. To check a specific code, simply examine the corresponding bit: byte `code / 8`, bit `code % 8`.\
For keys that produce printable characters, the codes are translated into the character's corresponding code (a same letter may arrive uppercase or lowercase depending on the state of the modifier keys).\
Keys with no printable representation retain their raw keyboard codes.\
The window must have focus: if it does not, the read fails without touching the map. Use `window::focus` and `window::focuswait` to ensure it.\
Like `window::mouse`, this query does not accumulate history: a key pressed and released between two calls will not appear in the map.\
To avoid missing presses, consult on every turn of the loop or attend the [[[Enum window_box]]](Enum%20window_box.md) `key_down` and `key_up` events delivered to the mailman.\
With **stacked** in `true`, the previous map is released by the function: do not keep it or delete it on your own.\
With **stacked** in `false`, the pointer may change address after growing, so always work against the value returned in **Keys**.\
It will be the programmer who frees the map with the corresponding memory management when it is no longer needed.

## Overloads
### - ESP
Firma:
```cpp
void* keyboard(void* wnd, BYTE* keys, UWORD size);
```
Parámetros:
* **Wnd**: Handle Ventana de la que se desea leer el teclado.
* **Keys**: Mapa de `BYTE` donde la función escribe el estado del teclado. Debe tener como mínimo 32 bytes; si **Size** es menor, solo se cubren los bits que caben en él.
* **Size**: Cantidad de bytes del mapa entregado en **Keys**.

Retorno:\
El mismo handle entregado, si era válido y la ventana estaba en pie con el foco. `nullptr` si el handle es inválido, si la ventana cerró, si la ventana no está enfocada, si la consulta de teclado no está disponible, o si **Keys** es `nullptr` o **Size** vale `0`.

\
Comportamiento:\
Esta variante escribe directamente sobre un mapa de bits propiedad del programador, sin intermediarios: la función pone a `0` los primeros 32 bytes (o **Size** si es menor) y luego enciende un bit por cada tecla hundida.\
Comparte con la firma principal el formato del mapa, la traducción de códigos, la exigencia de foco y el carácter instantáneo de la lectura, pero no gestiona ninguna memoria: el mapa es tuyo desde el primer byte.

### - ENG
Signature:
```cpp
void* keyboard(void* wnd, BYTE* keys, UWORD size);
```
Parameters:
* **Wnd**: Handle Window whose keyboard is to be read.
* **Keys**: `BYTE` map where the function writes the keyboard state. It must hold at least 32 bytes; if **Size** is smaller, only the bits that fit in it are covered.
* **Size**: Amount of bytes of the map handed over in **Keys**.

Return:\
The very same handle if it was valid and the window was standing with the focus. `nullptr` if the handle is invalid, the window closed, the window is not focused, the keyboard query is unavailable, **Keys** is `nullptr`, or **Size** is `0`.

\
Behavior:\
This variant writes directly into a bit map owned by the programmer, without intermediaries: the function zeroes the first 32 bytes (or **Size** if smaller) and then lights one bit per sunken key.\
It shares with the main signature the map format, the code translation, the focus requirement, and the instantaneous nature of the reading, but it manages no memory at all: the map is yours from the very first byte.
