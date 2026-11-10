# parse
### - ESP
Firma:
```cpp
void* parse(void* src);
```
Parámetros:
* **Src**: cadena de texto o búfer de texto ASCII terminado en cero con el JSON, o Handle del archivo de donde sacarlo.

Retorno:\
Handle del JSON creado.\
`nullptr` si **Src** llega vacío o si el contenido no da para armar un valor JSON reconocible.

\
Comportamiento:\
Arma un dato JSON nuevo a partir de lo que encuentra en **Src**.\
Si **Src** ya es un handle JSON, la respuesta es ese mismo handle: no se crea nada y no se copia nada.\
Los números quedan con la misma codificación que escriben `json::set_value` y `json::set_type`: los enteros negativos viajan en complemento a dos y se sacan con un cast directo a `WORD`.\
El handle queda en el **Heap**, así que hay que pasarle `json::detach` cuando deje de hacer falta.

### - ENG
Signature:
```cpp
void* parse(void* src);
```
Parameters:
* **Src**: text string or ASCII text buffer, zero-terminated, holding the JSON, or the file Handle to take it from.

Return:\
Handle of the JSON created.\
`nullptr` if **Src** arrives empty or if the content does not make up a recognizable JSON value.

\
Behavior:\
It builds a new JSON datum out of whatever **Src** holds.\
If **Src** already is a JSON handle, the answer is that same handle: nothing gets created and nothing gets copied.\
The numbers end up with the very coding `json::set_value` and `json::set_type` write: negative integers travel in two's complement and come out through a plain cast to `WORD`.\
The handle ends up in the **Heap**, so it has to be handed to `json::detach` once it is no longer needed.