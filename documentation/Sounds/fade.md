# fade
### - ESP
Firma:
```cpp
bool fade(void* th_snd, float target_volume, UWORD ms);
```
Parámetros:
* **Th_snd**: Handle Sonido Hilo cuya ganancia desea llevarse suavemente a otro valor.
* **Target_volume**: ganancia de destino, en la misma escala que `sound::set_volume`: `0.0` silencio, `1.0` señal sin modificar.
* **ms**: duración deseada de la transición, en milisegundos.

Retorno:\
`true` si el handle es un Handle Sonido Hilo y la transición quedó registrada.\
`false` sobre un Handle Sonido normal, sobre `nullptr` o sobre un handle inválido.\
El retorno dice si se apuntó el destino, no si el audio ya cambió: la transición avanza por su cuenta.

\
Comportamiento:\
Esta función fija un destino de ganancia y deja que sea el hilo de alimentación quien camine hacia él, corrigiendo el volumen del sonido interno porción a porción.\
El resultado es una subida o una bajada progresiva, sin saltos audibles.\
El camino se parte en pasos de alrededor de uno cada cincuenta milisegundos: la distancia hasta el destino se divide entre `ms / 50`, con un mínimo de un paso, así que los `ms` menores de cincuenta se resuelven en un único salto en la siguiente porción.\
La duración real no la marca el reloj sino el ritmo del hilo.\
Mientras el audio se está entregando, cada paso cae aproximadamente a los cincuenta milisegundos y se estira o se acorta con el tono.\
Si la salida está cerrada por un `sound::stop`, el hilo itera mucho más deprisa y la transición se recorre en una fracción del tiempo pedido.\
El volumen se corrige sobre las muestras que aún no fueron copiadas, así que lo que ya estaba dentro del handle conserva la ganancia anterior: el efecto se escucha con el retraso de la cola.\
Al alcanzar el destino lo fija con exactitud y deja de avanzar, sin sobrepaso ni vaivén.\
Desde ese punto el handle queda con ese volumen, que `sound::volume` devuelve.\
Una segunda llamada a `fade` reemplaza la transición en curso y recalcula el paso desde el volumen actual.\
`sound::set_volume` en cambio corta el recorrido y salta directo al valor nuevo.\
Bajar a `0.0` no detiene la reproducción: el audio sigue consumiéndose con ganancia nula hasta el final del archivo o hasta que el programa intervenga, y para apagar de verdad hace falta `sound::stop` o `sound::destroy`.\
Sobre un handle con bucle la transición continúa a través de las pasadas, porque el destino y el paso viven en el handle, no en el archivo.

### - ENG
Signature:
```cpp
bool fade(void* th_snd, float target_volume, UWORD ms);
```
Parameters:
* **Th_snd**: Handle Threaded Sound whose gain is to be carried smoothly to another value.
* **Target_volume**: Gain destination, on the same scale as `sound::set_volume`: `0.0` silence, `1.0` untouched signal.
* **ms**: Desired duration of the transition, in milliseconds.

Return:\
`true` if the handle is a Handle Threaded Sound and the transition was recorded.\
`false` over a plain Handle Sound, over `nullptr`, or over an invalid handle.\
The return says the destination was noted, not that the audio already changed: the transition walks on its own.

\
Behavior:\
This function fixes a gain destination and lets the feeding thread walk toward it, correcting the internal sound's volume piece by piece.\
The result is a rising or falling ramp with no audible jumps.\
The road is split into steps of roughly one every fifty milliseconds: the distance to the destination is divided by `ms / 50`, with a minimum of one step, so an `ms` below fifty resolves into a single jump on the next piece.\
Real duration is set by the thread's rhythm, not by the clock.\
While audio is being delivered, each step lands at about fifty milliseconds and stretches or shrinks with pitch.\
If the output is closed by a `sound::stop`, the thread iterates far faster and the transition runs through a fraction of the requested time.\
Volume is corrected over samples not yet copied, so what was already inside the handle keeps the previous gain: the effect is heard after the queue's delay.\
On reaching the destination it fixes it exactly and stops advancing, with no overshoot or sway.\
From there the handle holds that volume, which `sound::volume` returns.\
A second `fade` call replaces the transition in progress and recomputes the step from the current volume.\
`sound::set_volume`, by contrast, cuts the walk short and jumps straight to the new value.\
Lowering to `0.0` does not stop playback: the audio keeps being consumed at zero gain until the file ends or until the program intervenes, and to actually shut it down one needs `sound::stop` or `sound::destroy`.\
Over a looping handle the transition continues across passes, because the destination and the step live in the handle, not in the file.
