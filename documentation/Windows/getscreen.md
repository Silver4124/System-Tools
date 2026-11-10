# getscreen
### - ESP
Firma:
```cpp
void getscreen(int* _width, int* _height);
```
Parámetros:
* **_width**: Puntero a `int` donde la función almacena el ancho del monitor principal en píxeles. Puede ser `nullptr` si no se desea ese dato.
* **_height**: Puntero a `int` donde la función almacena el alto del monitor principal en píxeles. Puede ser `nullptr`.

Comportamiento:\
Esta función entrega las dimensiones completas del monitor principal, sin necesitar ninguna ventana: puede invocarse antes de crear la primera.\
Si el entorno gráfico no responde, ningún destino queda modificado.\
Con ambos punteros en `nullptr`, la función no realiza consulta alguna.\
Sirve como referencia para dimensionar una ventana a pantalla completa con `window::create` o `window::resize`, y para calcular emplazamientos con coordenadas de pantalla.\
Las dimensiones incluyen todo el área del monitor; el espacio realmente disponible para una ventana puede ser menor, pues el sistema reserva zonas para sus propios elementos.\
En equipos con varios monitores se entrega únicamente el tamaño del principal, no la suma del área extendida.

### - ENG
Signature:
```cpp
void getscreen(int* _width, int* _height);
```
Parameters:
* **_width**: Pointer to `int` where the function stores the width of the main monitor in pixels. May be `nullptr` if that datum is not wanted.
* **_height**: Pointer to `int` where the function stores the height of the main monitor in pixels. May be `nullptr`.

Behavior:\
This function delivers the full dimensions of the main monitor without needing any window: it can be invoked before creating the first one.\
If the graphical environment does not respond, no destination is modified.\
With both pointers `nullptr`, the function performs no query at all.\
It serves as the reference for sizing a full-screen window with `window::create` or `window::resize`, and for computing placements with screen coordinates.\
The dimensions cover the whole monitor area; the space actually available to a window may be smaller, since the system reserves areas for its own elements.\
On machines with several monitors only the main monitor's size is delivered, not the sum of the extended area.
