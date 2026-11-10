# system
### - ESP
Firma:
```cpp
int system(const char* cmd);
```
Parámetros:
* **Cmd**: Comando de terminal a ejecutar.

Retorno:\
Número entero, resultante del comando ejecutado.\
`-1` si **Cmd** es `nullptr` o si la función del sistema no está disponible.

\
Comportamiento:\
Actúa de forma bloqueante para el programa o hilo donde se ejecuta, hasta que el comando solicitado se resuelve.\
El tiempo de bloqueo depende de la naturaleza del comando: los que llevan intervalos de espera o requieren interacción del usuario son los que más consumen.\
Adaptación de `system` de `<stdlib.h>` (véase [Default Functions](README.md)).

### - ENG
Signature:
```cpp
int system(const char* cmd);
```
Parameters:
* **Cmd**: Terminal command to execute.

Return:\
Integer resulting from the executed command.\
`-1` if **Cmd** is `nullptr` or if the system function is not available.

\
Behavior:\
It blocks the program or thread where it runs until the requested command is resolved.\
The blocking time depends on the nature of the command: those with wait intervals or requiring user interaction are the slowest.\
Adaptation of `system` from `<stdlib.h>` (see [Default Functions](README.md)).