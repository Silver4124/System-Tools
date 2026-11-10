# stop
### - ESP
Firma:
```cpp
void stop(void* snd);
```
Parámetros:
* **Snd**: Handle Sonido cuya salida desea cerrarse, o Handle Sonido Hilo que desea poner en pausa.

Comportamiento:\
Esta función cierra la salida de audio y libera el dispositivo, de forma que el resto de aplicaciones del equipo pueden usarlo.\
Es el corte inmediato, no el final ordenado: lo que aún no había sonado se pierde y deja de oírse de golpe, y los dos turnos de envío quedan libres para la siguiente apertura.\
El handle conserva todo lo demás: formato, volumen, tono, maestro y la memoria de sus búferes, así que `sound::play` puede volver a abrirlo más tarde sin crear nada.\
Detener un handle que no estaba sonando, un handle `nullptr` o uno inválido no produce ningún efecto.\
`stop` es también lo que apaga la voluntad de reproducir: un handle sorprendido por la desconexión y en espera de que vuelva una salida queda, tras detenerlo, sin ninguna reconexión pendiente.\
Mientras la voluntad esté encendida, `stop` igualmente cierra lo abierto, reclama los turnos y libera el dispositivo.\
No rebobina: en un Handle Sonido la siguiente entrega continúa donde el programa la deje, y en un Handle Sonido Hilo la posición del archivo se mantiene, así que `stop` seguido de `play` funciona como pausa y reanudación.\
Sobre un Handle Sonido Hilo la acción llega al sonido interno y su hilo deja de alimentar mientras la salida esté cerrada; el hilo no se destruye ni se pierde el progreso del archivo.\
Para soltar también la memoria del handle existe `sound::destroy`, que detiene antes de liberar.

### - ENG
Signature:
```cpp
void stop(void* snd);
```
Parameters:
* **Snd**: Handle Sound whose output is to be closed, or Handle Threaded Sound to be paused.

Behavior:\
This function closes the audio output and releases the device, so the rest of the machine's applications can use it.\
It is an immediate cut, not an orderly end: whatever had not sounded yet is lost and stops being heard at once, and both sending turns are left free for the next opening.\
The handle keeps everything else: format, volume, pitch, master and the memory of its buffers, so `sound::play` can reopen it later without creating anything.\
Stopping a handle that was not sounding, a `nullptr` handle or an invalid one has no effect.\
`stop` is also what turns the will to play off: a handle surprised by the disconnection and waiting for an output to come back is left, once stopped, with no reconnection pending.\
While the will stays lit, `stop` still closes whatever is open, reclaims the turns and releases the device.\
It does not rewind: on a Handle Sound the next delivery continues wherever the program leaves it, and on a Handle Threaded Sound the file position is kept, so `stop` followed by `play` works as pause and resume.\
Over a Handle Threaded Sound the action reaches the internal sound and its thread stops feeding while the output stays closed; the thread is neither destroyed nor is the file progress lost.\
To release the handle's memory as well there is `sound::destroy`, which stops before freeing.
