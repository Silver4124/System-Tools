# play
### - ESP
Firma:
```cpp
bool play(void* snd);
```
Parámetros:
* **Snd**: Handle Sonido que desea abrirse, o Handle Sonido Hilo cuyo hilo de alimentación quedó detenido.

Retorno:\
`true` si el handle quedó con la intención de reproducir: con la salida abierta y en servicio, o en espera de que aparezca una salida.\
`false` si el handle es `nullptr` o inválido, si ya estaba sonando, o si la biblioteca de sonido del sistema no está disponible.\
Sin ninguna salida de audio en el equipo la respuesta es `true` igual; si la salida llegó a abrirse lo informa `sound::playing`.

\
Comportamiento:\
Abre la salida de audio con el formato del handle y la pone en servicio: a partir de aquí el consumo de datos es real y lo que no reciba muestras se escucha como silencio.\
Abrir la salida no produce sonido por sí solo: en un Handle Sonido la alimentación corre a cargo del programa con `sound::stream`, y en un Handle Sonido Hilo el hilo propio ya se encarga y esta función solo reabre lo que `sound::stop` cerró.\
Responde en cuanto la salida queda en pie, sin bloquear: no espera a que suene nada ni a que se vacíe un búfer.\
El tono se aplica al abrir: la frecuencia con la que se presenta el audio es la configurada multiplicada por el tono vigente, así que `sound::set_pitch` antes de `play` se nota en la primera nota y después de `play` se nota de inmediato sobre la salida ya abierta.\
Un handle ya sonando responde `false` y no reinicia nada: no se vuelve a abrir, no se limpia su cola y no cambia su formato.\
Para reconfigurar hay que pasar por `sound::stop` (`sound::set` rechaza cambios mientras la salida está abierta) y después `play` de nuevo.\
Sobre un Handle Sonido Hilo la llamada actúa sobre el sonido interno: tras un `stop` que hizo de pausa, `play` reanuda la alimentación desde el punto del archivo en que quedó, sin volver al principio.\
La apertura usa la salida de audio que el equipo tenga por defecto en ese momento y la biblioteca de sonido del sistema, de la que el módulo no depende al compilar; si en tiempo de ejecución no está disponible, la respuesta es `false`.\
Sin altavoces conectados no hay error de apertura: el handle queda con la intención de reproducir y la salida se monta donde aparezca y cuando aparezca.\
Esa intención la atiende `sound::stream`, que es donde está contada la desconexión y la reconexión de la salida; `sound::stop` es lo que la apaga.

### - ENG
Signature:
```cpp
bool play(void* snd);
```
Parameters:
* **Snd**: Handle Sound to be opened, or Handle Threaded Sound whose feeding thread was left stopped.

Return:\
`true` if the handle ended up wanting to play: with the output open and in service, or waiting for an output to appear.\
`false` if the handle is `nullptr` or invalid, if it was already sounding, or if the system's sound library is unavailable.\
With no audio output on the machine the answer is still `true`; whether the output actually got opened is what `sound::playing` tells.

\
Behavior:\
It opens the audio output with the handle's format and puts it in service: from here on consumption of data is real, and whatever receives no samples sounds as silence.\
Opening the output produces no sound on its own: on a Handle Sound the program feeds it with `sound::stream`, and on a Handle Threaded Sound its own thread already does and this function merely reopens what `sound::stop` closed.\
It answers as soon as the output is standing, without blocking: it waits for no sound and for no buffer to empty.\
Pitch is applied on opening: the rate at which audio is presented is the configured one multiplied by the current pitch, so `sound::set_pitch` before `play` shows up on the first note and after `play` shows up right away on the already open output.\
A handle already sounding answers `false` and restarts nothing: it is not reopened, its queue is not cleared and its format does not change.\
To reconfigure, one must go through `sound::stop` (`sound::set` refuses changes while the output is open) and then `play` again.\
Over a Handle Threaded Sound the call acts on the internal sound: after a `stop` that served as a pause, `play` resumes feeding from the point of the file where it left off, without going back to the start.\
The opening uses the audio output the machine has as default at that moment and the system's sound library, on which the module does not depend at compile time; if that library is unavailable at run time, the answer is `false`.\
No speakers plugged in is not an opening error: the handle keeps the will to play and the output gets mounted wherever it shows up and whenever it shows up.\
That will is what `sound::stream` attends to, and that is where the account of the output leaving and coming back lives; `sound::stop` is what turns the will off.
