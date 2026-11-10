# destroy
### - ESP
Firma:
```cpp
void* destroy(void* wnd, bool _wait = true);
```
Parámetros:
* **Wnd**: Handle Ventana que se desea cerrar.
* **_wait**: Si es `true`, la función se queda bloqueada hasta que el cierre se ha consumado. Si es `false`, regresa en cuanto la solicitud de cierre fue enviada.

Retorno:\
El mismo handle entregado, si era válido y la ventana estaba en pie. `nullptr` si el handle es inválido o si la ventana ya había cerrado.

\
Comportamiento:\
Esta función solicita el cierre de la ventana, como si el propio programa oprimiera su botón de cierre.\
En Windows ese cierre lo atiende el módulo sin consultar al mailman.\
Al consumarse el cierre se liberan el búfer de píxeles y los recursos internos, y el propio handle es destruido: deja de ser utilizable para cualquier función del módulo.\
El programador no debe liberarlo por su cuenta.\
Solicitar el cierre de una ventana ya destruida es inocuo y simplemente entrega `nullptr`.\
Si **Wnd** proviene de `window::active`, la función pide el cierre a la ventana ajena y destruye el handle externo, no espera por ella.\
Para esperar el cierre de una ventana propia sin solicitarlo, usa `window::wait`; para comprobar si sigue viva sin bloquear, usa `window::yield`.

### - ENG
Signature:
```cpp
void* destroy(void* wnd, bool _wait = true);
```
Parameters:
* **Wnd**: Handle Window to be closed.
* **_wait**: When `true`, the function stays blocked until the closure has been consummated. When `false`, it returns as soon as the closing request was sent.

Return:\
The very same handle if it was valid and the window was standing. `nullptr` if the handle is invalid or the window had already closed.

\
Behavior:\
This function requests the window to close, as if the program itself pressed its close button.\
On Windows that closure is attended by the module without consulting the mailman.\
Once the closure is consummated, the pixel buffer and the internal resources are freed, and the handle itself is destroyed: it stops being usable for any module function.\
The programmer must not release it by hand.\
Requesting the closure of an already destroyed window is harmless and simply delivers `nullptr`.\
If **Wnd** came from `window::active`, the function asks the foreign window to close, destroys the external handle, and does not wait for it.\
Use `window::wait` to await the closure of one's own window without requesting it, and `window::yield` to check whether it is alive without blocking.
