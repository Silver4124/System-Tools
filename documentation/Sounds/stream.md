# stream
### - ESP
Firma:
```cpp
UWORD stream(void* snd, void* external_data, UWORD size);
```
Parámetros:
* **Snd**: Handle Sonido que recibirá los datos. Un Handle Sonido Hilo no acepta muestras propias.
* **External_data**: dirección de las muestras ya codificadas en el formato configurado en el handle.
* **Size**: cantidad de bytes de **External_data** que desean entregarse.

Retorno:\
Número entero sin signo del tamaño de la palabra del procesador con los bytes efectivamente aceptados: **Size** recortado a la capacidad de un búfer.\
`0` si el handle es `nullptr` o inválido, si es un Handle Sonido Hilo, si **External_data** es `nullptr`, si la salida no está sonando o si los dos búferes siguen ocupados.

\
Comportamiento:\
Copia el audio que el programa acaba de producir al primer búfer libre del handle y entrega ese búfer a la salida.\
No espera a que el sonido se escuche: responde en cuanto el envío queda en camino.\
Antes de copiar, escala cada muestra con la ganancia del handle (el producto de su volumen y de su volumen maestro, si lo tiene) y la satura al rango del formato, así que la ganancia queda impresa en los datos ya enviados: lo que viajó antes de un cambio de volumen conserva el anterior.\
El envío nunca supera la capacidad fijada en `sound::create`; para entregar más se manda el resto en la llamada siguiente, cuando `sound::sync` vuelva a responder `true`.\
La copia avanza de muestra en muestra, no de trama en trama, por lo que **Size** debe ser múltiplo del tamaño de trama (`canales × bits / 8`); una entrega que parte una muestra escribe la muestra completa y desalinea lo que se envíe después.\
Los anchos de muestra que el módulo recorre son `8`, `16` y `32` bits: las de `8` son sin signo (silencio en `128`, rango de `0` a `255`), como las que producen los archivos WAV y la mayoría de herramientas, y las de `16` y `32` son con signo.\
Con `24` u otro ancho la función acepta el búfer y responde la cifra pedida, pero ninguna muestra se copia: suena lo que hubiera en la memoria del búfer.\
El tono del handle no altera lo que **External_data** contiene: aquí solo viajan muestras, y el tono se aplica sobre la velocidad de la salida.\
Mientras la salida no está sonando no hay adónde enviar: hay que invocar `sound::play` primero.\
Si el programa no consume el turno de búfer, `sound::sync` responde `false` y esta función se queda en `0`; ese es el momento de esperar, no de insistir.\
La desconexión de la salida la atiende esta misma función, y las demás funciones del módulo remiten a esta cuenta: si el programa pidió `sound::play` y el altavoz se va en plena actuación, la siguiente entrega lo detecta, suelta el dispositivo y responde `0` sin consumir el turno, de modo que los datos siguen esperando donde estaban.\
Cuando una salida vuelve a conectarse, la entrega siguiente la abre ahí mismo y el audio continúa sin perder posición y sin volver a llamar a `sound::play`.\
Si quien se fue fue el propio programa con `sound::stop`, nada se reconecta: los `0` son permanentes hasta el próximo `play`.\
Sobre un Handle Sonido Hilo la respuesta es siempre `0`: ese handle ya se alimenta solo desde su archivo.

### - ENG
Signature:
```cpp
UWORD stream(void* snd, void* external_data, UWORD size);
```
Parameters:
* **Snd**: Handle Sound that will receive the data. A Handle Threaded Sound takes no custom samples.
* **External_data**: Address of the samples already encoded in the format configured on the handle.
* **Size**: Amount of bytes of **External_data** to be handed over.

Return:\
Processor word-sized unsigned integer with the bytes actually accepted: **Size** trimmed to one buffer's capacity.\
`0` if the handle is `nullptr` or invalid, if it is a Handle Threaded Sound, if **External_data** is `nullptr`, if the output is not sounding, or if both buffers are still occupied.

\
Behavior:\
It copies the audio the program just produced into the handle's first free buffer and hands that buffer to the output.\
It does not wait for the sound to be heard: it answers as soon as the delivery is on its way.\
Before copying, it scales every sample with the handle's gain (the product of its volume and of its master volume, if it has one) and saturates it to the format's range, so gain stays printed on the data already sent: whatever traveled before a volume change keeps the old one.\
The delivery never exceeds the capacity fixed in `sound::create`; to hand over more, send the rest in the next call, once `sound::sync` answers `true` again.\
The copy moves sample by sample, not frame by frame, so **Size** must be a multiple of the frame size (`channels × bits / 8`); a delivery that splits a sample writes the whole sample and misaligns whatever is sent afterwards.\
The sample widths the module traverses are `8`, `16` and `32` bits: the `8`-bit ones are unsigned (silence at `128`, range `0` to `255`), like the ones WAV files and most tooling produce, and the `16` and `32` bit ones are signed.\
With `24` or any other width the function accepts the buffer and answers the requested figure, but no sample is copied: what sounds is whatever the buffer memory already held.\
The handle's pitch does not alter what **External_data** holds: only samples travel here, and pitch is applied on the output's speed.\
While the output is not sounding there is nowhere to send: `sound::play` must be invoked first.\
If the program does not consume the buffer turn, `sound::sync` answers `false` and this function stays at `0`; that is the moment to wait, not to insist.\
This same function attends to the output's disconnection, and the rest of the module's functions refer to this account: if the program asked for `sound::play` and the speaker is taken away mid-performance, the next delivery notices, releases the device and answers `0` without consuming the turn, so the data keeps waiting where it was.\
Once an output is plugged back in, the following delivery opens it right there and the audio continues without losing position and without calling `sound::play` again.\
If it was the program itself that left with `sound::stop`, nothing reconnects: the `0`s are permanent until the next `play`.\
Over a Handle Threaded Sound the answer is always `0`: that handle already feeds itself from its own file.
