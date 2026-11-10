# message
### - ESP
Firma:
```cpp
int message(const char* RESTRICT txt, const char* RESTRICT ttl, int type = message_box::ok);
```
Parámetros:
* **Txt**: Cadena de texto o buffer de texto ASCII, que representa el contenido textual general de la ventana.
* **Ttl**: Cadena de texto o buffer de texto ASCII, que representa el titulo que tendrá la ventana.
* **Type** (Por defecto en `message_box::ok`): Número entero, que representa el formato de botones e icono que tendrá la ventana.

Retorno:\
Número entero con la respuesta del usuario, según la tabla de códigos de [[[Enum message_box]]](Enum%20message_box.md): `1` Aceptar, `2` Cancelar, `3` Abortar, `4` Reintentar, `5` Ignorar, `6` Sí, `7` No.\
`-1` si la función de sistema que muestra la ventana no pudo resolverse.\
`-2` si `Txt` o `Ttl` son nulos.\
Un valor `0` o negativo nunca debe compararse como si fuera una respuesta del usuario.

\
Comportamiento:\
Causa una interrupción bloqueante en el programa o hilo que la ejecuta, como `scanf`: espera a que el usuario pulse uno de los botones de la ventana o la cierre.\
En Linux necesita el comando de terminal `zenity`; sin esa herramienta instalada, la función no tiene efecto alguno.\
Las etiquetas se descomponen por máscara (`0xF00` botón por defecto, `0x0F0` icono, `0x00F` botones).\
Una posición de botón por defecto incompatible con la cantidad de botones se degrada silenciosamente a la posición válida más alta, sin producir error.\
Posee sobrecargas discretas para cadenas de texto largo o buffers UTF (`wchar*`) y para casos extremos de conversión de tipos en tiempo de compilación.\
El comportamiento visual de la ventana está en [[[Enum message_box]]](Enum%20message_box.md).

### - ENG
Signature:
```cpp
int message(const char* RESTRICT txt, const char* RESTRICT ttl, int type = message_box::ok);
```
Parameters:
* **Txt**: Text string or ASCII text buffer representing the general textual content of the window.
* **Ttl**: Text string or ASCII text buffer representing the window's title.
* **Type** (Defaults to `message_box::ok`): An integer representing the button and icon format of the window.

Return:\
An integer with the user's response, per the code table in [[[Enum message_box]]](Enum%20message_box.md): `1` OK, `2` Cancel, `3` Abort, `4` Retry, `5` Ignore, `6` Yes, `7` No.\
`-1` if the system function that shows the dialog could not be resolved.\
`-2` if `Txt` or `Ttl` is null.\
A value of `0` or below must never be compared as if it were a user response.

\
Behavior:\
It causes a blocking interruption in the program or thread that executes it, like `scanf`: it waits for the user to press one of the window's buttons or close it.\
On Linux it requires the `zenity` terminal command; without that tool installed, the function has no effect at all.\
The tags decompose by mask (`0xF00` default button, `0x0F0` icon, `0x00F` buttons).\
A default-button position incompatible with the button count is silently degraded to the highest valid position, without raising an error.\
It has discrete overloads for wide-text strings or UTF buffers (`wchar*`) and for extreme cases of type conversion at compile time.\
The window's visual behavior is in [[[Enum message_box]]](Enum%20message_box.md).