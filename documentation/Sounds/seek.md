# seek
### - ESP
Firma:
```cpp
bool seek(void* th_snd, float seconds);
```
Parámetros:
* **Th_snd**: Handle Sonido Hilo cuya posición desea moverse.
* **Seconds**: segundos de audio a los que desea saltarse, contados desde el principio del sonido.

Retorno:\
`true` si el destino quedó dentro del fichero y la posición de lectura fue movida.\
`false` si el handle no es un Handle Sonido Hilo, si le falta la salida o el fichero, o si el destino cae en o más allá del último byte.\
`false` deja la posición del fichero exactamente como estaba.

\
Comportamiento:\
Esta función traduce los segundos a bytes con el formato vigente de la salida y coloca el puntero de lectura del fichero en ese punto, justo después de los 44 bytes de la cabecera estándar.\
El destino se redondea hacia abajo a la frontera de trama más cercana (tamaño de trama = `canales × bits / 8`), de modo que un salto nunca parte una trama ni cambia de sitio los canales.\
El salto lo consume el hilo de alimentación en su siguiente porción, así que no corta lo que ya estaba entregado: eso se escucha entero antes del salto.\
No toca nada más: la salida sigue abierta, el volumen, el tono y el maestro se conservan y un desvanecido en curso continúa avanzando hacia su destino.\
Es la forma de llevar un sonido a un momento concreto, y encaja con `sound::get_position` y `sound::get_duration` porque las tres miden en los mismos segundos de contenido.\
Conviene comprobar el resultado antes de fiarse de la posición: un destino más allá del final no produce salto, aunque el número de segundos parezca razonable cuando el fichero trae basura al final.\
Con un handle normal, o con `nullptr`, la llamada no hace nada.\
Los segundos negativos no tienen significado para esta función: la cuenta se hace sobre una posición sin signo, así que el resultado no es fiable y no debe usarse.

### - ENG
Signature:
```cpp
bool seek(void* th_snd, float seconds);
```
Parameters:
* **Th_snd**: Handle Threaded Sound whose position is to be moved.
* **Seconds**: Audio seconds to jump to, counted from the start of the sound.

Return:\
`true` if the destination landed inside the file and the read position was moved.\
`false` if the handle is not a Handle Threaded Sound, if its output or file is missing, or if the destination falls on or past the last byte.\
`false` leaves the file's position exactly as it was.

\
Behavior:\
This function translates seconds into bytes using the output's current format and places the file's read pointer at that spot, right after the 44 bytes of the standard header.\
The destination is rounded down to the nearest frame boundary (frame size = `channels × bits / 8`), so a jump never splits a frame nor swaps the channels' places.\
The jump is consumed by the feeding thread on its next piece, so it does not cut what was already delivered: that plays out in full before the jump.\
It touches nothing else: the output stays open, volume, pitch and master are kept, and an ongoing fade keeps advancing toward its target.\
It is the way to carry a sound to a concrete moment, and it fits with `sound::get_position` and `sound::get_duration` because all three measure in the same seconds of content.\
It is worth checking the result before trusting the position: a destination past the end produces no jump, even when the seconds figure looks reasonable because the file carries junk at the end.\
With a plain handle, or with `nullptr`, the call does nothing.\
Negative seconds mean nothing to this function: the arithmetic runs over an unsigned position, so the result is not dependable and should not be used.
