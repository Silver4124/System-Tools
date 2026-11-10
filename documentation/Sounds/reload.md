# reload
### - ESP
Firma:
```cpp
bool reload(void* th_snd);
```
Parámetros:
* **Th_snd**: Handle Sonido Hilo que desea rebobinarse.

Retorno:\
`true` si el handle es un Handle Sonido Hilo, incluso cuando su reproducción ya terminó o cuando el fichero asociado no está disponible.\
`false` sobre un Handle Sonido normal, sobre `nullptr` o sobre un handle inválido.

\
Comportamiento:\
Devuelve el puntero de lectura del fichero al primer byte de audio, así que la siguiente porción que tome el hilo de alimentación saldrá del principio del sonido.\
No es un corte inmediato: lo que ya estaba entregado a la salida se sigue escuchando, unas dos décimas por delante, y el reinicio se oye cuando esa cola se agota.\
Solo mueve la posición de lectura: no reinicia el volumen, ni el tono, ni el maestro, ni cancela un desvanecido en curso, así que si `sound::fade` estaba trabajando seguirá avanzando desde el punto en que va.\
Sobre un handle en pausa (tras `sound::stop`) la posición queda cambiada y el hilo tomará la porción nueva en cuanto `sound::play` reanude la salida.\
Sirve tanto para repetir un efecto desde cero como para volver al principio sin destruir el handle; para saltar a un punto concreto se usa `sound::seek`.\
Un handle cuyo archivo ya se agotó sin bucle responde `true`, pero no hay nadie que atienda el cambio: su hilo ya se despidió y la salida quedó liberada, así que en ese caso hace falta un `sound::file_stream` nuevo.

### - ENG
Signature:
```cpp
bool reload(void* th_snd);
```
Parameters:
* **Th_snd**: Handle Threaded Sound to be rewound.

Return:\
`true` if the handle is a Handle Threaded Sound, even when its playback already ended or when the associated file is not available.\
`false` over a plain Handle Sound, over `nullptr`, or over an invalid handle.

\
Behavior:\
It sends the file's read pointer back to the first audio byte, so the next piece the feeding thread takes will come from the start of the sound.\
It is not an immediate cut: what was already handed to the output keeps being heard, about two tenths of a second ahead, and the restart is heard once that queue drains.\
It moves the read position only: it does not reset volume, pitch, or master, and it does not cancel an ongoing fade, so if `sound::fade` was working it keeps advancing from wherever it is.\
Over a paused handle (after `sound::stop`) the position is changed and the thread will take the new piece as soon as `sound::play` resumes the output.\
It serves both to replay an effect from zero and to go back to the start without destroying the handle; to jump to a specific point one uses `sound::seek`.\
A handle whose file already ran out without looping answers `true`, but nobody is left to attend the change: its thread said goodbye and the output was released, so that case needs a fresh `sound::file_stream`.
