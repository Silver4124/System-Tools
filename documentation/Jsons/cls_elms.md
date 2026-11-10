# cls_elms
### - ESP
Firma:
```cpp
void* cls_elms(void* jsn);
```
Parámetros:
* **Jsn**: Handle JSON que se va a vaciar.

Retorno:\
El valor entregado en **Jsn** una vez hecho el vaciado.\
`nullptr` solo si el handle es inválido.

\
Comportamiento:\
En un contenedor, `json_type::ary` u `json_type::obj`, borra todos los elementos y las etiquetas que los acompañaban.\
Fuera de los contenedores no hay elementos que borrar, así que el golpe va al valor, que queda en el defecto del tipo: `false` en `json_type::bll`, `0` en los números, `0.0f` en `json_type::flt` y cadena vacía en `json_type::str`.\
Un `json_type::nll` ya viene en blanco, y el handle se devuelve tal cual.

### - ENG
Signature:
```cpp
void* cls_elms(void* jsn);
```
Parameters:
* **Jsn**: JSON Handle to be emptied.

Return:\
The value handed in **Jsn** once the emptying is done.\
`nullptr` only if the handle is invalid.

\
Behavior:\
In a container, `json_type::ary` or `json_type::obj`, it wipes every element and the labels that went with them.\
Outside the containers there are no elements to wipe, so the blow lands on the value, which ends up at the type's default: `false` for `json_type::bll`, `0` for the numbers, `0.0f` for `json_type::flt`, and an empty string for `json_type::str`.\
A `json_type::nll` already comes blank, and the handle is handed back as it was.