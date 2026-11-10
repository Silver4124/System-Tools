# playing
### - ESP
Firma:
```cpp
bool playing(void* snd);
```
Parámetros:
* **Snd**: Handle Sonido o Handle Sonido Hilo cuyo estado desea consultarse.

Retorno:\
`true` mientras la salida de audio esté abierta al servicio del handle.\
`false` si está cerrada, si el handle es `nullptr` o si es inválido.\
Sobre un Handle Sonido Hilo responde por su sonido interno.

\
Comportamiento:\
Esta función informa del estado de la salida, no de si hay sonido: un handle abierto al que el programa no le entrega datos está reproduciendo, aunque solo se escuche silencio.\
El estado se enciende cuando `sound::play` logra abrir la salida y se apaga cuando `sound::stop` la cierra o `sound::destroy` la suelta.\
No depende de los búferes, así que no sirve para saber si lo enviado ya terminó de sonar: para eso está `sound::sync`.\
También se apaga sola, sin que nadie la llame, cuando la salida se desconecta a media reproducción, y vuelve a encenderse cuando `sound::stream` monta la nueva, que es donde está contada la reconexión.\
El `false` por desconexión y el `false` por `sound::stop` se leen igual pero no significan lo mismo: el primero se arregla enchufando el altavoz.\
Responde al instante y no modifica nada, lo que la vuelve apta para un bucle de trabajo junto a `sound::sync`.\
Su lectura es el modo de detectar que un Handle Sonido dejó de servir tras un fallo de apertura: si `sound::play` respondió `false`, la salida sigue cerrada y ningún `sound::stream` aceptará datos.\
Cuando un Handle Sonido Hilo agota su archivo sin bucle, su hilo cierra la salida pero conserva el sonido interno vivo hasta el `sound::destroy`: la consulta responde `false` con seguridad y el handle sigue siendo utilizable para las demás funciones del módulo mientras dure sin destruirse.

### - ENG
Signature:
```cpp
bool playing(void* snd);
```
Parameters:
* **Snd**: Handle Sound or Handle Threaded Sound whose state is to be consulted.

Return:\
`true` while the audio output is open and at the handle's service.\
`false` if it is closed, if the handle is `nullptr` or if it is invalid.\
Over a Handle Threaded Sound it answers for the internal sound.

\
Behavior:\
This function reports the state of the output, not whether there is sound: a handle left open to which the program hands no data is playing, even though only silence is heard.\
The state turns on when `sound::play` manages to open the output and turns off when `sound::stop` closes it or `sound::destroy` releases it.\
It does not depend on the buffers, so it does not tell whether what was sent has finished sounding: `sound::sync` covers that.\
It also turns off on its own, with nobody calling, when the output is disconnected mid-playback, and lights up again when `sound::stream` mounts the new one, which is where the reconnection is accounted for.\
`false` from disconnection and `false` from `sound::stop` read alike but mean otherwise: the first is fixed by plugging the speaker back in.\
It answers at once and changes nothing, which makes it fit for a work loop next to `sound::sync`.\
Reading it is also how one notices a Handle Sound stopped being serviceable after a failed opening: if `sound::play` answered `false`, the output stays closed and no `sound::stream` will accept data.\
When a Handle Threaded Sound runs out of file without looping, its thread closes the output but keeps the internal sound alive until `sound::destroy`: this query answers `false` safely, and the handle remains usable for the module's other functions for as long as it is left undestroyed.
