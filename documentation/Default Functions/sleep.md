# sleep
### - ESP
Firma:
```cpp
void sleep(unsigned int ms);
```
Parámetros:
* **Ms**: Número entero sin signo, que representa la cantidad de milisegundos a esperar.

Comportamiento:\
La espera se expresa en milisegundos en los dos sistemas: `sleep(3)` duerme tres milésimas de segundo.\
El parámetro son milisegundos aunque el nombre coincida con `sleep` de POSIX, cuya unidad son los segundos.\
Si tu proyecto incluye cabeceras de C, esta abstracción no es intercambiable con la función homónima del sistema.\
La interrupción es bloqueante para el programa o hilo que la ejecuta: espera la cantidad pedida y la invocación finaliza una vez cumplida, sin otro resultado.\
La espera nunca resulta más corta de lo pedido, aunque un sistema ocupado sí puede alargarla.

### - ENG
Signature:
```cpp
void sleep(unsigned int ms);
```
Parameters:
* **Ms**: Unsigned integer, representing the number of milliseconds to wait.

Behavior:\
The wait is expressed in milliseconds on both systems: `sleep(3)` sleeps three thousandths of a second.\
The parameter is milliseconds even though the name matches POSIX `sleep`, whose unit is seconds.\
If your project includes C headers, this abstraction is not interchangeable with the homonymous system function.\
The interrupt blocks the program or thread that runs it: it waits the amount asked for and the call finishes once that is met, with no other result.\
The wait is never shorter than requested, though a busy system can stretch it.