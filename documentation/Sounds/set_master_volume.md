# set_master_volume
### - ESP
Firma:
```cpp
bool set_master_volume(void* snd, float* mst_vol);
```
Parámetros:
* **Snd**: Handle Sonido o Handle Sonido Hilo que desea enlazarse a un maestro.
* **Mst_vol**: dirección de un `float` propiedad del programa, con la ganancia compartida. `nullptr` desenlaza el maestro.

Retorno:\
`true` si el enlace quedó registrado, incluido el caso `nullptr`, que simplemente deja al handle sin maestro. `false` si el handle es `nullptr` o inválido.

\
Comportamiento:\
Esta función no entrega un número sino la dirección de uno: el handle guarda el puntero y lo lee cada vez que `sound::stream` escala una muestra, multiplicando la ganancia por el valor que allí esté en ese momento.\
La ganancia efectiva de una entrega es entonces `volumen del handle × valor del maestro`, y el maestro no interviene en nada más: ni se suma, ni se guarda, ni se libera.\
Al ser una dirección compartida, un mismo `float` puede gobernar todos los handles que se quiera, lo que da un deslizador general sin tocar uno por uno.\
Cambiar el valor desde el programa se refleja en la siguiente entrega de cada handle enlazado, sin avisar a ninguno.\
El módulo no toma la propiedad del valor: el programa debe mantenerlo vivo mientras algún handle lo apunte.\
Liberarlo antes deja al handle leyendo memoria ajena en cada entrega, con el resultado que le plazca.\
El enlace no se pierde al detener o reabrir la salida, y sobre un Handle Sonido Hilo sobrevive a la reconstrucción que provoca un cambio de tono.\
Se consulta con `sound::master_volume`, que devuelve la dirección, nunca el valor: para la cifra hay que desreferenciar el puntero entregado.

### - ENG
Signature:
```cpp
bool set_master_volume(void* snd, float* mst_vol);
```
Parameters:
* **Snd**: Handle Sound or Handle Threaded Sound to be linked to a master.
* **Mst_vol**: Address of a `float` owned by the program, holding the shared gain. `nullptr` unlinks the master.

Return:\
`true` if the link was recorded, the `nullptr` case included, which simply leaves the handle without a master. `false` if the handle is `nullptr` or invalid.

\
Behavior:\
This function hands over no number but the address of one: the handle keeps the pointer and reads it every time `sound::stream` scales a sample, multiplying the gain by whatever value sits there at that moment.\
The effective gain of a delivery is then `handle volume × master value`, and the master intervenes in nothing else: it is neither added, nor stored, nor freed.\
Being a shared address, one same `float` can rule as many handles as desired, which gives a master slider without touching each one.\
Changing the value from the program shows up in the next delivery of every linked handle, with no notice to any of them.\
The module takes no ownership of the value: the program must keep it alive while some handle points at it.\
Freeing it earlier leaves the handle reading foreign memory on every delivery, with whatever result follows.\
The link is not lost by stopping or reopening the output, and over a Handle Threaded Sound it survives the rebuild caused by a pitch change.\
It is consulted with `sound::master_volume`, which returns the address, never the value: the figure requires dereferencing the pointer handed back.
