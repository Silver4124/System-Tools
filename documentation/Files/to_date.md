# to_date
### - ESP
Firma:
```cpp
void to_date(UWORD ts, BYTE* sec, BYTE* min, BYTE* hour, BYTE* day, BYTE* month, unsigned int* year);
```
Parámetros:
* **Ts**: Marca de tiempo a desglosar, en segundos transcurridos desde el 1 de enero de 1970 a las 00:00:00 (UTC).
* **Sec, Min, Hour, Day, Month, Year**: Direcciones donde la función almacena cada componente del resultado. Cada una es opcional: las que se pasen en `nullptr` se ignoran.

Comportamiento:\
Convierte una marca de tiempo en sus componentes de fecha y hora, y escribe cada uno directamente donde tú indiques.\
El desglose se hace en horario universal coordinado, sin aplicar ninguna zona horaria, así que la hora obtenida puede diferir de la que muestra el reloj del usuario.\
Si necesitas la hora local, suma el desfase correspondiente antes o después del desglose.\
Las convenciones son: segundos de `0` a `59`, minutos de `0` a `59`, horas de `0` a `23`, días desde `1`, meses del `1` al `12` y el año completo en **Year**.\
**Sec**, **Min**, **Hour**, **Day** y **Month** son `BYTE`, no enteros: conviene promoverlos a un tipo entero antes de imprimirlos o compararlos.\
Un **ts** correspondiente a una fecha anterior a 1970 se interpreta como un número sin signo enorme y produce un resultado sin sentido.\
Es la inversa de `file::to_time`.\
El valor numérico que aquí se desglosa lo entrega `file::timestamp`.

### - ENG
Signature:
```cpp
void to_date(UWORD ts, BYTE* sec, BYTE* min, BYTE* hour, BYTE* day, BYTE* month, unsigned int* year);
```
Parameters:
* **Ts**: Time mark to break down, in seconds elapsed since January 1st, 1970 at 00:00:00 (UTC).
* **Sec, Min, Hour, Day, Month, Year**: Addresses where the function stores each component of the result. Each one is optional: those passed as `nullptr` are ignored.

Behavior:\
It turns a time mark into its date and time components, writing each one exactly where you point.\
The breakdown happens in Coordinated Universal Time, with no time zone applied, so the hour obtained may differ from the one the user's clock shows.\
If local time is needed, add the corresponding offset before or after the breakdown.\
The conventions are: seconds from `0` to `59`, minutes from `0` to `59`, hours from `0` to `23`, days starting at `1`, months from `1` to `12`, and the full year in **Year**.\
**Sec**, **Min**, **Hour**, **Day** and **Month** are `BYTE`, not integers: promote them to an integer type before printing or comparing them.\
A **ts** matching a date before 1970 is interpreted as an enormous unsigned number and produces a meaningless result.\
It is the inverse of `file::to_time`.\
The numeric value broken down here is the one `file::timestamp` delivers.
