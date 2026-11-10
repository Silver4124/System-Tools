# move
### - ESP
Firma:
```cpp
const char* move(const char* old_path, const char* new_path, bool overwrite = true);
```
Parámetros:
* **Old_path**: Ruta actual del fichero o carpeta a trasladar. Debe existir.
* **New_path**: Ruta destino.
* **Overwrite**: Determina si un destino ya existente puede ser reemplazado. Por defecto `true`.

Retorno:\
La misma dirección de **New_Path**, en caso de éxito.\
`nullptr` si alguna ruta es `nullptr` o está vacía, si **Old_Path** no existe, si **New_Path** ya existe con `overwrite = false`, o si la operación no pudo completarse.

\
Comportamiento:\
Traslada un fichero o una carpeta a una nueva ubicación: el origen deja de existir y el contenido queda disponible en el destino.\
Trabaja únicamente con rutas, así que no intervienen handles que haya que abrir o cerrar.\
El traslado es efectivo dentro de la misma unidad o volumen.\
Al cruzar entre unidades distintas la operación deja de ser atómica: si algo falla a mitad, tanto la ruta antigua como la nueva pueden seguir existiendo.\
Con `overwrite = false` la función se niega ante un destino existente y no toca ninguna de las dos rutas.\
Tras un traslado correcto, cualquier handle que apuntara a **Old_Path** queda fuera de juego: la ruta que almacenaba ya no corresponde a un elemento real.\
Para cerrar esa handle inútil está `file::close`.\
Para duplicar el elemento en lugar de trasladarlo está `file::copy`.

### - ENG
Signature:
```cpp
const char* move(const char* old_path, const char* new_path, bool overwrite = true);
```
Parameters:
* **Old_Path**: Current path of the file or folder to transfer. It must exist.
* **New_Path**: Destination path.
* **Overwrite**: Determines whether an already existing destination may be replaced. Defaults to `true`.

Return:\
The very same **New_Path** address, upon success.\
`nullptr` if either path is `nullptr` or empty, if **Old_Path** does not exist, if **New_Path** already exists while `overwrite = false`, or if the operation could not be completed.

\
Behavior:\
It transfers a file or folder to a new location: the origin stops existing and its content becomes available at the destination.\
It works only with paths, so no handles need to be opened or closed.\
The transfer is effective within the same drive or volume.\
When crossing between different drives the operation stops being atomic: if something fails halfway, both the old and the new path may still exist.\
With `overwrite = false` the function refuses an existing destination and leaves both paths untouched.\
After a correct transfer, any handle that pointed to **Old_Path** becomes out of play: the path it stored no longer matches a real element.\
To close that now useless handle there is `file::close`.\
To duplicate the element instead of transferring it there is `file::copy`.
