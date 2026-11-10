# set_type
### - ESP
Firma:
```cpp
void* set_type(void* jsn, BYTE type);
```
Parámetros:
* **Jsn**: Handle JSON que se va a modificar.
* **Type**: tipo nuevo que se le va a poner.

Retorno:\
El valor entregado en **Jsn** una vez cambiado el tipo.\
`nullptr` solo si el handle es inválido.

\
Comportamiento:\
El elemento pasa a ser del tipo **Type**, y su espacio se acomoda al tamaño de ese tipo.\
Si **Type** coincide con el tipo que ya tenía, el handle se devuelve sin tocar nada.\
Entre los escalares el valor se traduce al tipo nuevo (un número se vuelve su texto, un texto numérico se vuelve número, `true` vale `1` y `false` vale `0`).\
Un texto con decimales que pasa a `json_type::num` se trunca al entero: `"0.5"` queda en `0` y `"-2.9e2"` en `-290`; el mismo texto pasado a `json_type::flt` conserva la fracción.\
Un cruce sin traducción posible vacía el elemento y le deja el valor en blanco del tipo nuevo.\
Pasar a `json_type::nll` quita el almacenamiento por completo.\
Los tipos que se pueden pedir están en [[[Enum json_type]]](Enum%20json_type.md).

### - ENG
Signature:
```cpp
void* set_type(void* jsn, BYTE type);
```
Parameters:
* **Jsn**: JSON Handle to be modified.
* **Type**: new type to be put on it.

Return:\
The value handed in **Jsn** once the type is changed.\
`nullptr` only if the handle is invalid.

\
Behavior:\
The element turns into type **Type**, and its space gets fitted to that type's size.\
If **Type** matches the type it already had, the handle comes back with nothing touched.\
Among the scalars the value gets translated into the new type (a number becomes its text, a numeric text becomes a number, `true` counts as `1` and `false` as `0`).\
A text with decimals turning into `json_type::num` gets truncated to the integer: `"0.5"` lands on `0` and `"-2.9e2"` on `-290`; the very same text turned into `json_type::flt` keeps the fraction.\
A crossing with no possible translation empties the element and leaves it the new type's blank value.\
Turning into `json_type::nll` takes the storage away altogether.\
The types that can be asked for are in [[[Enum json_type]]](Enum%20json_type.md).