# defMailMan
### - ESP
Firma:
```cpp
UWORD defMailMan(void* wnd, UWORD msg, UWORD wp, WORD lp);
```
Parámetros:
* **Wnd**: Handle Ventana que originó el evento.
* **Msg**: Etiqueta del evento recibido, tomada de [[[Enum window_box]]](Enum%20window_box.md).
* **Wp**: Complemento corto del evento.
* **Lp**: Complemento largo del evento.

Retorno:\
`window_box::def` para cualquier etiqueta, sin excepción.\
La función no atiende evento alguno: los deja todos para que la librería los resuelva con su comportamiento por defecto.

\
Comportamiento:\
Función opcional del módulo, de declaración pública.\
Existe como plantilla de ejemplo: al leerla se ve cómo se redacta un **Mailman** personalizado para `window::create`.\
Si el programa define la macro `ST_STOP_MAILMAN_DECL` antes de incluir la librería, esta función no se declara.

### - ENG
Signature:
```cpp
UWORD defMailMan(void* wnd, UWORD msg, UWORD wp, WORD lp);
```
Parameters:
* **Wnd**: Handle Window that originated the event.
* **Msg**: Tag of the received event, taken from [[[Enum window_box]]](Enum%20window_box.md).
* **Wp**: Short complement of the event.
* **Lp**: Long complement of the event.

Return:\
`window_box::def` for every tag, without exception.\
The function attends no event on its own: it leaves all of them for the library to resolve with its default behaviour.

\
Behavior:\
An optional function of the module, with a public declaration.\
It exists as an example template: reading it shows how a custom **Mailman** for `window::create` is written.\
If the program defines the `ST_STOP_MAILMAN_DECL` macro before including the library, this function is not declared.
