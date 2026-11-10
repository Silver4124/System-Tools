# set_value
### - ESP
Firma:
```cpp
void* set_value(void* jsn, void* value);
```
Parámetros:
* **Jsn**: Handle JSON que se va a modificar.
* **Value**: valor nuevo que se le va a poner.

Retorno:\
El valor entregado en **Jsn** una vez intentado el cambio.\
`nullptr` si el handle es inválido o si el elemento es `json_type::str` y **Value** llega vacío.

\
Comportamiento:\
El valor se escribe según el tipo que el elemento ya tenga, y el tipo en sí no se toca.
* **Para `json_type::bll`**: cualquier **Value** distinto de `nullptr` queda como `true`, y el propio `nullptr` queda como `false`.
* **Para `json_type::num`**: **Value** se toma como un `WORD` o un `UWORD`.
* **Para `json_type::flt`**: **Value** se toma como un `float`, empaquetado en él.
* **Para `json_type::str`**: **Value** se toma como una cadena de texto o búfer de texto ASCII terminado en cero (`'\0'`), y se copia dentro del elemento.
* **Para el resto de los tipos** (`json_type::nll`, `json_type::ary`, `json_type::obj`, `json_type::unk`): no hay valor que escribir, así que el elemento queda igual y la respuesta es buena.

### - ENG
Signature:
```cpp
void* set_value(void* jsn, void* value);
```
Parameters:
* **Jsn**: JSON Handle to be modified.
* **Value**: new value to be put on it.

Return:\
The value handed in **Jsn** once the change has been attempted.\
`nullptr` if the handle is invalid, or if the element is `json_type::str` and **Value** arrives empty.

\
Behavior:\
The value gets written according to the type the element already has, and the type itself is not touched.
* **For `json_type::bll`**: any **Value** other than `nullptr` ends up `true`, and `nullptr` itself ends up `false`.
* **For `json_type::num`**: **Value** is taken as a `WORD` or a `UWORD`.
* **For `json_type::flt`**: **Value** is taken as a `float`, packed into it.
* **For `json_type::str`**: **Value** is taken as a text string or ASCII text buffer terminated with a zero (`'\0'`), and it is copied into the element.
* **For the rest of the types** (`json_type::nll`, `json_type::ary`, `json_type::obj`, `json_type::unk`): there is no value to write, so the element stays as it is and the answer comes back good.