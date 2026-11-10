# get_position
### - ESP
Firma:
```cpp
float get_position(void* th_snd);
```
Parámetros:
* **Th_snd**: Handle Sonido Hilo cuya posición desea conocerse.

Retorno:\
Segundos de audio consumidos en la pasada actual, devueltos como `float` de 32 bits.\
`0.0` si el handle no es un Handle Sonido Hilo, si le falta la salida o el fichero, o si el handle es `nullptr`.\
Durante el arranque la cifra puede ser negativa.

\
Comportamiento:\
Esta función estima dónde va la reproducción mirando cuánto fichero se ha leído: toma la posición del Handle Fichero, le descuenta la cabecera de los primeros 44 bytes y divide el resto entre los bytes que gasta un segundo de audio en el formato vigente de la salida.\
Al resultado le resta el audio que aún espera dentro del handle (los dos turnos de envío), con lo que la cifra se acerca a lo que se está escuchando y no a lo que ya se leyó.\
Por eso arranca por debajo de cero: durante los primeros instantes todavía no ha salido nada de lo descontado.\
La medida es de contenido, no de reloj: con un tono de `2.0` la pasada dura la mitad de tiempo real, pero los segundos que devuelve avanzan igual de rápido respecto al audio.\
Las unidades de la conversión salen del formato vigente en la salida, no del que figura en el archivo, así que un `sound::set` sobre el handle cambia la escala de la lectura.\
Tras `sound::seek` o `sound::reload` responde al instante la nueva posición, aunque lo que suene todavía sea la cola anterior.\
Con bucle, el contador vuelve a cero en cada pasada y no queda constancia de cuántas se llevan.\
Sobre un Handle Sonido normal devuelve `0.0`: ahí no hay fichero del que deducir nada, y la única pista útil la tiene el propio programa que produce las muestras.

### - ENG
Signature:
```cpp
float get_position(void* th_snd);
```
Parameters:
* **Th_snd**: Handle Threaded Sound whose position is to be known.

Return:\
Seconds of audio consumed in the current pass, returned as a 32-bit `float`.\
`0.0` if the handle is not a Handle Threaded Sound, if its output or file is missing, or if the handle is `nullptr`.\
During startup the figure may be negative.

\
Behavior:\
This function estimates where playback is going by looking at how much file has been read: it takes the Handle File's position, deducts the header of the first 44 bytes, and divides the rest between the bytes one second of audio spends in the output's current format.\
From the result it subtracts the audio still waiting inside the handle (the two sending buffers), which brings the figure close to what is being heard rather than to what was already read.\
That is why it starts below zero: in the first instants none of what was subtracted has come out yet.\
The measure is of content, not of clock: with a pitch of `2.0` the pass lasts half the real time, but the seconds it returns advance just as fast relative to the audio.\
The conversion's units come from the format in force on the output, not the one written in the file, so a `sound::set` over the handle changes the reading's scale.\
After `sound::seek` or `sound::reload` it answers with the new position at once, even though what is sounding is still the previous queue.\
With looping, the counter goes back to zero on each pass and no record is kept of how many were made.\
Over a plain Handle Sound it returns `0.0`: there is no file there to deduce anything from, and the only useful clue belongs to the program producing the samples.
