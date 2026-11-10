# get_duration
### - ESP
Firma:
```cpp
float get_duration(void* th_snd);
```
Parámetros:
* **Th_snd**: Handle Sonido Hilo cuya duración desea conocerse.

Retorno:\
Segundos de audio contenido en el fichero, devueltos como `float` de 32 bits.\
`0.0` si el handle no es un Handle Sonido Hilo.\
Un fichero más corto que la cabecera, o sin audio, responde `0.0`.

\
Comportamiento:\
Esta función calcula la duración sobre el tamaño del fichero: toma los bytes totales, le resta los 44 de la cabecera estándar y divide entre los bytes que gasta un segundo de audio en el formato vigente de la salida.\
No consulta la longitud que el propio WAV declara, así que todo lo que ocupe espacio después del audio se cuenta como audio: trozos de metadatos añadidos al final, o un fichero cortado de más, inflan o deforman la cifra.\
La conversión usa el formato de la salida, no el del archivo: si `sound::set` cambió frecuencia, canales o bits sobre este handle, la duración calculada cambia con él.\
El resultado es duración de contenido, no de reloj: con un tono distinto de `1.0` el audio se escuchará en menos o más tiempo real del que aquí se devuelve.\
Con bucle, la cifra describe una pasada, no lo que tardará el programa en cansarse.\
Sirve para colocar una barra de progreso junto a `sound::get_position`; ambas comparten escala, así que son comparables entre sí.

### - ENG
Signature:
```cpp
float get_duration(void* th_snd);
```
Parameters:
* **Th_snd**: Handle Threaded Sound whose duration is to be known.

Return:\
Seconds of audio contained in the file, returned as a 32-bit `float`.\
`0.0` if the handle is not a Handle Threaded Sound.\
A file shorter than the header, or with no audio, answers `0.0`.

\
Behavior:\
This function computes duration from the file's size: it takes the total bytes, deducts the 44 of the standard header, and divides between the bytes one second of audio spends in the output's current format.\
It does not consult the length the WAV itself declares, so anything sitting after the audio counts as audio: metadata chunks appended at the end, or a file padded too far, inflate or bend the figure.\
The conversion uses the output's format, not the file's: if `sound::set` changed frequency, channels or bits over this handle, the computed duration changes with it.\
The result is content duration, not clock duration: with a pitch other than `1.0` the audio will be heard in less or more real time than what is returned here.\
With looping, the figure describes one pass, not how long the program will take to tire.\
It serves to place a progress bar next to `sound::get_position`; both share scale, so they are comparable.
