# to_time
### - ESP
Firma:
```cpp
void to_time(UWORD* ts, BYTE sec, BYTE min, BYTE hour, BYTE day, BYTE month, unsigned int year);
```
Parámetros:
* **Ts**: Dirección donde la función almacena la marca de tiempo resultante, en segundos desde el 1 de enero de 1970 a las 00:00:00 (UTC). No puede ser `nullptr`.
* **Sec**: Segundos, de `0` a `59`.
* **Min**: Minutos, de `0` a `59`.
* **Hour**: Horas, de `0` a `23`.
* **Day**: Día del mes, desde `1`.
* **Month**: Mes, del `1` al `12`.
* **Year**: Año completo.

Comportamiento:\
Empaqueta una fecha y una hora concretas en una única marca de tiempo comparable, el formato que `file::timestamp` consulta en los elementos de disco.\
Con **Ts** en `nullptr`, no ocurre nada.\
El cálculo se hace en horario universal coordinado, sin aplicar zona horaria ni cambios de horario de verano.\
Si la fecha que manejas es local, réstale primero el desfase correspondiente.\
Los años bisiestos se tienen en cuenta, tanto al sumar los días de cada año como al añadir el día extra de febrero, así que fechas límite como el 29 de febrero se resuelven correctamente.\
Las componentes no se validan: un **Month** fuera del rango `1`-`12`, un **Day** mayor que el mes correspondiente o unos segundos de `60` producen una marca desplazada hacia delante, no un error.\
Comprime los datos antes de llamar a la función si provienen del usuario.\
Una **Year** anterior a `1970` no es representable: la cuenta de años no avanza y el resultado queda reducido a lo que sumen el mes y el día, sin valor de año.\
El coste de la operación crece con la distancia del año a 1970.\
En una compilación de 32 bits el valor se estrecha y deja de ser correcto más allá del año 2106.\
Para el camino inverso está `file::to_date`.

### - ENG
Signature:
```cpp
void to_time(UWORD* ts, BYTE sec, BYTE min, BYTE hour, BYTE day, BYTE month, unsigned int year);
```
Parameters:
* **Ts**: Address where the function stores the resulting time mark, in seconds since January 1st, 1970 at 00:00:00 (UTC). It cannot be `nullptr`.
* **Sec**: Seconds, from `0` to `59`.
* **Min**: Minutes, from `0` to `59`.
* **Hour**: Hours, from `0` to `23`.
* **Day**: Day of the month, starting at `1`.
* **Month**: Month, from `1` to `12`.
* **Year**: Full year.

Behavior:\
It packs a concrete date and time into a single comparable time mark, the format `file::timestamp` queries on disk elements.\
With **Ts** as `nullptr`, nothing happens.\
The calculation runs in Coordinated Universal Time, applying neither a time zone nor daylight-saving changes.\
If the date you handle is local, subtract the corresponding offset first.\
Leap years are taken into account, both when summing the days of each year and when adding the extra February day, so boundary dates like February 29th resolve correctly.\
Components are not validated: a **Month** outside `1`-`12`, a **Day** larger than its month allows, or sixty seconds produce a mark shifted forward, not an error.\
Sanitize the data before calling the function when it comes from the user.\
A **Year** before `1970` is not representable: the year count never advances and the result is reduced to whatever the month and day add up to, with no year value.\
The cost of the operation grows with the distance of the year from 1970.\
In a 32-bit build the value is narrowed and stops being correct beyond the year 2106.\
For the way back there is `file::to_date`.
