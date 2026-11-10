# father
### - ESP
Firma:
```cpp
void* father(void* wnd);
```
Parámetros:
* **Wnd**: Handle Ventana cuya madre se desea conocer.

Retorno:\
Handle de la ventana madre, si la ventana fue creada con una. `nullptr` si **Wnd** es inválido, si la ventana ya había cerrado o si fue creada sin madre.

\
Comportamiento:\
Esta función devuelve el handle que se entregó como **Father** a `window::create`.\
El handle devuelto es solo una referencia de consulta: pertenece a la ventana madre y no debe ser borrado por quien lo recibe.\
El vínculo tiene efecto real sobre el ciclo de vida: si la madre desaparece, la hija se cierra sin consultar a su mailman.\
Sobre un handle externo devuelto por `window::active`, el resultado es siempre `nullptr`, pues una ventana ajena no tiene madre registrada.\
Si en Windows se creó la ventana entregando un Handle Consola como madre, esta función devuelve ese mismo handle de consola, no un Handle Ventana.

### - ENG
Signature:
```cpp
void* father(void* wnd);
```
Parameters:
* **Wnd**: Handle Window whose mother is to be known.

Return:\
The mother window's handle, if the window was created with one. `nullptr` if **Wnd** is invalid, the window had already closed, or it was created without a mother.

\
Behavior:\
This function returns the handle that was handed over as **Father** to `window::create`.\
The returned handle is a reference for consultation only: it belongs to the mother window and must not be erased by whoever receives it.\
The link has a real effect on the life cycle: if the mother disappears, the daughter closes without consulting its mailman.\
Over an external handle returned by `window::active`, the result is always `nullptr`, since a foreign window has no registered mother.\
On Windows, if the window was created handing over a Handle Console as mother, this function returns that same console handle, not a Handle Window.
