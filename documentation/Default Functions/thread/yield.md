# yield
### - ESP
Firma:
```cpp
bool yield(void* thr, bool _wait = true);
```
Parámetros:
* **Thr**: Handle al hilo.
* **_wait**:
  * `true` (Por defecto): Realiza un pequeño tiempo de espera.
  * `false`: Solo se limita realizar la comprobación.

Retorno:\
`true` mientras el hilo sigue vivo.\
`false` si el hilo terminó, si **Thr** es `nullptr` o si el handle es inválido.\
El `false` no distingue el hilo que acabó del handle que nunca lo fue: para afirmar el fin hay que haber consultado antes un `true`.

\
Comportamiento:\
Esta función comprueba si el hilo sigue en ejecución, y duerme un mínimo de 10 milisegundos en cada vuelta cuando **_wait** es `true`, incluso en la vuelta que descubre el fin.\
Cuando el hilo resulta haber terminado, la propia función destruye el handle: la dirección deja de ser utilizable a partir de ese `false`, y entregarla después a `thread::kill`, a `thread::join` o a otra `thread::yield` sería usar memoria liberada.\
Con **_wait** en `false` la comprobación es inmediata, y se presta a vigilar el hilo sin interrumpir el programa.

### - ENG
Signature:
```cpp
bool yield(void* thr, bool _wait = true);
```
Parameters:
* **Thr**: Handle thread.
* **_wait**:
  * `true` (Default): Performs a short wait.
  * `false`: Only performs the check.

Return:\
`true` while the thread is still alive.\
`false` if the thread ended, if **Thr** is `nullptr`, or if the handle is invalid.\
The `false` does not tell the finished thread apart from a handle that never was one: to claim the end, an earlier `true` must have been seen.

\
Behavior:\
This function checks whether the thread is still running, and sleeps a minimum of 10 milliseconds on each round when **_wait** is `true`, even on the round that discovers the end.\
When the thread turns out to have finished, the function itself destroys the handle: the address stops being usable from that `false` onward, and handing it afterwards to `thread::kill`, `thread::join` or another `thread::yield` would be using freed memory.\
With **_wait** as `false` the check is immediate, which suits watching a thread without interrupting the program.