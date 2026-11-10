# kill
### - ESP
Firma:
```cpp
void* kill(void* thr);
```
Parámetros:
* **Thr**: Handle al hilo.

Retorno:\
Handle al hilo en caso de éxito.\
`nullptr` en caso de fallo, o en caso de que el valor pasado al argumento **Thr** sea `nullptr`.\
El handle devuelto es un "comportamiento inteligente" como el de `memory::free`: se entrega la dirección **ya liberada**, por lo que un retorno distinto de `nullptr` afirma que el hilo fue destruido, no que sigue usable.\
Compare el resultado como booleano y descartelo como puntero inmediatamente después.

\
Comportamiento:\
Finaliza de forma abrupta la ejecución de un hilo que aún se está ejecutando, sin importar las etapas de ejecución que posea internamente.\
Como sea que finalice, con resultado positivo y handle válido el hilo se elimina de la memoria automaticamente para evitar fugas: todos los punteros que servían como handle quedarán apuntando a memoria inválida una vez pasada la liberación.\
Al ser una terminación abrupta, úsela con responsabilidad: pueden perderse recursos y procesamientos según lo que hiciera la función asignada al hilo.

### - ENG
Signature:
```cpp
void* kill(void* thr);
```
Parameters:
* **Thr**: Handle thread.

Return:\
The thread handle on success.\
`nullptr` on failure, or if the **Thr** argument was `nullptr`.\
The returned handle is "smart behavior" like `memory::free`: the **already released** address is handed back, so a non-`nullptr` result asserts that the thread was destroyed, not that it is still usable.\
Treat the result as a boolean and discard it as a pointer immediately after the call.

\
Behavior:\
It abruptly terminates the execution of a thread that is still running, regardless of its internal execution stages.\
However it ends, with a positive result and a valid thread handle the thread is automatically removed from memory to avoid leaks: every pointer that served as the handle will point to invalid memory once the release has happened.\
Because the termination is abrupt, use it responsibly: resources and processing may be lost depending on what the thread's function was doing.