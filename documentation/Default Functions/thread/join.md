# join
### - ESP
Firma:
```cpp
void* join(void* thr);
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
Une las ejecuciones: sincroniza un hilo con el programa u otro hilo.\
Genera una interrupción bloqueante en quien la invoque, que finaliza cuando termina la ejecución del hilo consultado.\
Como sea que finalice, con resultado positivo y handle válido el hilo se elimina de la memoria automaticamente para evitar fugas: todos los punteros que servían como handle quedarán apuntando a memoria inválida una vez pasada la liberación.

### - ENG
Signature:
```cpp
void* join(void* thr);
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
It joins the executions: a thread is synchronized with the program or another thread.\
It causes a blocking interruption in the caller, which ends when the queried thread's execution finishes.\
However it ends, with a positive result and a valid thread handle the thread is automatically removed from memory to avoid leaks: every pointer that served as the handle will point to invalid memory once the release has happened.