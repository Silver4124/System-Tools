# set_pitch
### - ESP
Firma:
```cpp
void set_pitch(void* snd, float pit);
```
Parámetros:
* **Snd**: Handle Sonido o Handle Sonido Hilo cuyo tono desea modificarse.
* **Pit**: multiplicador de la frecuencia. `1.0` deja el tono original, `2.0` lo sube una octava, `0.5` lo baja otra.

Comportamiento:\
Esta función multiplica la frecuencia con la que se presenta el audio: la salida trabaja a `frecuencia configurada × tono`, así que el tono sube la altura y acelera la reproducción a la vez, o la baja y la ralentiza.\
No es un estirador de tiempo: la duración del contenido cambia con la altura, y un `2.0` consume el archivo o la entrega al doble de velocidad.\
Si la salida está abierta, el cambio llega a ella de inmediato y se oye en la muestra siguiente.\
Si está cerrada, queda guardado y se aplica cuando `sound::play` abra la salida.\
El valor queda guardado aunque se detenga y se vuelva a abrir el handle, y se combina con la frecuencia declarada en `sound::set`: las dos cifras se multiplican entre sí.\
Un handle `nullptr` o inválido no produce ningún efecto.\
Un tono de `0` o negativo deja una frecuencia sin sentido, y `sound::play` no lo avisa: responde `true` porque solo anuncia la intención de sonar.\
Lo que delata el rechazo es `sound::playing` en `false`, con `sound::stream` quedándose en `0`.\
Con la salida ya abierta, el cambio puede no llegar a aplicarse y el audio seguirá como estaba.\
Sobre un Handle Sonido Hilo el valor se fija en el sonido interno y su hilo de alimentación lo detecta solo: reconstruye la salida con el nuevo tamaño de turno, conservando el volumen y el maestro.\
Esa reconstrucción corta el audio un instante.\
El tono no altera las cuentas de `sound::get_position`, `sound::get_duration` ni `sound::seek`: esas cifras se leen en segundos de contenido, y el contenido no cambia, solo se presenta más rápido o más despacio.

### - ENG
Signature:
```cpp
void set_pitch(void* snd, float pit);
```
Parameters:
* **Snd**: Handle Sound or Handle Threaded Sound whose pitch is to be changed.
* **Pit**: Frequency multiplier. `1.0` keeps the original pitch, `2.0` raises it an octave, `0.5` lowers it one.

Behavior:\
This function multiplies the rate at which audio is presented: the output works at `configured frequency × pitch`, so pitch raises the height and speeds up playback at once, or lowers it and slows it down.\
It is not a time stretcher: the content's duration changes along with its height, and a `2.0` consumes the file or the delivery at double speed.\
If the output is open, the change reaches it immediately and is heard on the next sample.\
If it is closed, the value stays saved and applies when `sound::play` opens the output.\
The value stays saved even after the handle is stopped and reopened, and it combines with the frequency declared in `sound::set`: the two figures multiply each other.\
A `nullptr` or invalid handle has no effect.\
A pitch of `0` or negative leaves a meaningless frequency, and `sound::play` does not warn about it: it answers `true` because it only announces the intent to play.\
What gives the rejection away is `sound::playing` at `false`, with `sound::stream` staying at `0`.\
With the output already open, the change may fail to apply and the audio goes on as it was.\
Over a Handle Threaded Sound the value is set on the internal sound and its feeding thread notices it alone: it rebuilds the output with the new turn size, keeping volume and master.\
That rebuild cuts the audio for an instant.\
Pitch does not alter the counts of `sound::get_position`, `sound::get_duration` or `sound::seek`: those figures are read as seconds of content, and the content does not change, it is only presented faster or slower.
