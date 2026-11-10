# get_spectrum
### - ESP
Firma:
```cpp
bool get_spectrum(void* th_snd, float** buffer, UWORD* samples, float* average);
```
Parámetros:
* **Th_snd**: Handle Sonido Hilo que desea analizarse.
* **Buffer**: puntero a un `float*` donde queda la ventana recién creada. Debe ser `nullptr` o una ventana devuelta por la propia función.
* **Samples**: destino con el número de valores de la ventana.
* **Average**: destino opcional con el indicador de nivel; admite `nullptr`.

Retorno:\
`true` si el handle es un Handle Sonido Hilo y se entregaron **Buffer** y **Samples**, con la ventana ya creada y sus valores escritos.\
`false` si el handle es un Handle Sonido normal, `nullptr` o inválido, o si **Buffer** o **Samples** son `nullptr`.\
Cuando responde `false` no toca **Buffer**: la ventana anterior, si existía, sigue siendo del programador.

\
Comportamiento:\
Esta función toma el audio que hay en el búfer de salida y lo pasa a flotantes normalizados según el ancho de muestra, de modo que el tope de la señal queda en torno a `1.0` y el silencio en `0.0` (en `8` bits el silencio es el valor `128` del PCM sin signo).\
Entrega la dirección de esa ventana junto con su cantidad de valores.\
La ventana se escoge del turno de envío que está sonando en este momento: si solo un búfer sigue en la salida se analiza ese, y si los dos siguen ocupados se analiza el enviado antes.\
Es una lectura de amplitud, no un análisis de frecuencias: sirve para dibujar la forma de la onda o un medidor de nivel, y para sacar las bandas de frecuencia hace falta transformar la ventana por cuenta del programa.\
La cifra de **Samples** sale de dividir los bytes del búfer entre los bytes de cada muestra, así que los canales vienen intercalados: en un fichero estéreo el valor par es un canal y el impar el otro, y la cantidad de tramas reales es la mitad de la cifra devuelta.\
Los valores llegan ya con la ganancia aplicada: lo que se mide es lo que sale por el altavoz, incluidos los efectos de `sound::set_volume` y del maestro.\
La ventana se crea en cada llamada con el asignador de C++ y, si **Buffer** traía una ventana previa, esa se libera primero.\
La nueva es propiedad del programador, que debe soltarla con `delete[]` o reutilizarla en la llamada siguiente.\
Entregar un puntero inventado por el programa o procedente de otra reserva hace que la función libere memoria ajena.\
Sobre un handle en pausa, la ventana conserva el último contenido leído: no se refresca sola.\
Sobre un handle cuyo archivo se agotó y cuyo hilo ya se despidió, la consulta sigue siendo segura: el sonido interno vive hasta el `sound::destroy` y la ventana devuelve el último búfer entregado.\
Sobre un Handle Sonido normal la respuesta es `false`, aunque ese handle también tenga búferes: la función está reservada para los que se alimentan de archivo.

**Average** es la media de las amplitudes absolutas de la ventana: el silencio responde `0.0` y una señal saturada se acerca a `1.0`; en un fichero estéreo ambos canales se mezclan en esa misma media.

### - ENG
Signature:
```cpp
bool get_spectrum(void* th_snd, float** buffer, UWORD* samples, float* average);
```
Parameters:
* **Th_snd**: Handle Threaded Sound to be analysed.
* **Buffer**: Pointer to a `float*` where the freshly created window is left. It must be `nullptr` or a window returned by the function itself.
* **Samples**: Destination holding the number of values in the window.
* **Average**: Optional destination holding the level indicator; `nullptr` allowed.

Return:\
`true` if the handle is a Handle Threaded Sound and **Buffer** and **Samples** were provided, with the window already created and its values written.\
`false` if the handle is a plain Handle Sound, `nullptr` or invalid, or if **Buffer** or **Samples** is `nullptr`.\
When it answers `false` it does not touch **Buffer**: the previous window, if any, remains the programmer's.

\
Behavior:\
This function takes the audio sitting in the output buffer and converts it to normalized floats according to the sample width, so the signal's ceiling lands around `1.0` and silence at `0.0` (at `8` bits silence is the unsigned-PCM value `128`).\
It hands over that window's address along with its value count.\
The window is taken from the sending turn that is sounding right now: if only one buffer is still in the output that one is analysed, and if both are still occupied the earlier delivered one is.\
It is an amplitude readout, not a frequency analysis: it serves to draw the wave's shape or a level meter, and getting frequency bands requires the program to transform the window on its own.\
The **Samples** figure comes from dividing the buffer's bytes by each sample's bytes, so channels arrive interleaved: in a stereo file the even value belongs to one channel and the odd one to the other, and the real frame count is half the figure returned.\
The values arrive with gain already applied: what is measured is what comes out of the speaker, including the effects of `sound::set_volume` and of the master.\
The window is created on each call with the C++ allocator and, if **Buffer** carried a previous window, that one is freed first.\
The new one is the programmer's property, to be released with `delete[]` or reused on the next call.\
Handing over a pointer made by hand or coming from another allocation makes the function free someone else's memory.\
Over a paused handle the window keeps the last content read: it does not refresh on its own.\
Over a handle whose file ran out and whose thread already said goodbye, the query stays safe: the internal sound lives until `sound::destroy` and the window returns the last delivered buffer.\
Over a plain Handle Sound the answer is `false`, even though that handle also has buffers: the function is reserved for those fed from a file.

**Average** is the mean of the window's absolute amplitudes: silence answers `0.0` and a saturated signal approaches `1.0`; in a stereo file both channels mix into that same mean.
