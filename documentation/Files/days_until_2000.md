# days_until_2000
### - ESP
Firma:
```cpp
UWORD days_until_2000();
```
Retorno:\
Entero sin signo del tamaño de la palabra del procesador: segundos transcurridos desde el 1 de enero del año 2000 a las 00:00:00 (UTC).\
`0` si el reloj del sistema marca una fecha anterior a esa.

\
Comportamiento:\
Entrega el instante actual referido a un anclaje cercano en el tiempo, el cambio de milenio, en lugar de la cuenta larga de segundos que arranca en 1970.\
Su interés es de empaquetado: una medida referida al año 2000 necesita menos cifras que la cuenta desde 1970.\
Por eso sirve para registrar marcas compactas dentro de un fichero, para comparar la antigüedad de dos elementos entre sí, o para caducar datos a partir de una fecha fija.\
El valor es siempre una magnitud del presente: no acepta una marca de tiempo como argumento, así que no sirve para convertir una fecha arbitraria a este formato.\
Para ello, réstale a esa marca el anclaje del año 2000, `946684800` segundos desde 1970.\
Un retorno `0` puede deberse a un reloj retrasado más que a un fallo, y no hay forma de distinguir ambos casos desde el valor devuelto.\
Pese al nombre, la unidad entregada son segundos, no días: es la medida que encaja con el resto del módulo.\
Para obtener días, divide el retorno entre `86400`.\
La cuenta larga desde 1970 la entregan `file::timestamp`, `file::to_date` y `file::to_time`.

### - ENG
Signature:
```cpp
UWORD days_until_2000();
```
Return:\
Processor word-sized unsigned integer: the amount of seconds elapsed since January 1st of the year 2000 at 00:00:00 (UTC).\
`0` if the system clock marks an earlier date.

\
Behavior:\
It delivers the current instant referred to an anchor close in time, the millennium turn, instead of the long seconds count starting in 1970.\
Its interest is packing: a measurement referred to the year 2000 needs fewer digits than the count from 1970.\
That is why it suits registering compact marks inside a file, comparing the age of two elements against each other, or expiring data from a fixed date.\
The value is always a present-tense magnitude: it accepts no time mark as an argument, so it cannot convert an arbitrary date into this format.\
To do that, subtract the year-2000 anchor, `946684800` seconds since 1970, from that mark.\
A `0` return may come from a clock running behind rather than from a failure, and there is no way to tell both cases apart from the returned value.\
Despite its name, the unit delivered is seconds, not days: it is the measure that lines up with the rest of the module.\
To get days, divide the return by `86400`.\
The long count from 1970 is what `file::timestamp`, `file::to_date` and `file::to_time` deliver.
