# inside
### - ESP
Firma:
```cpp
void* inside(void* folder, UWORD idx);
```
Parámetros:
* **Folder**: Handle Folder a recorrer.
* **Idx**: Número entero sin signo del tamaño de la palabra del procesador, que representa el indice del elemento dentro de la carpeta. Comienza en `0`.

Retorno:\
Handle Fichero o Handle Folder del elemento solicitado, en caso de éxito.\
`nullptr` si **Folder** no es una Handle Folder válida o si **Idx** queda fuera del rango de la carpeta.

\
Comportamiento:\
Abre el elemento que ocupa la posición **Idx** dentro de la carpeta y entrega una handle completamente funcional.\
Si el elemento es un fichero se obtiene una Handle Fichero, y si es una carpeta se obtiene una Handle Folder que puede recorrerse con esta misma función.\
Así se recorren árboles de directorios completos mediante recursión.\
Los índices válidos van de `0` a `file::size(folder) - 1`.\
El orden de los elementos no está garantizado: ni alfabético ni por fecha.\
Una posición puede no corresponder al mismo elemento entre dos ejecuciones.\
La numeración es la misma que cuenta `file::size`: solo se dejan fuera `.` y `..`, así que los elementos ocultos cuentan en las dos funciones y los índices no se desalinean.\
La handle entregada hereda los permisos con los que fue abierta la carpeta de origen.\
Cerrar la carpeta no cierra los elementos ya abiertos: cada uno se cierra por separado.\
Para el bucle de recorrido está `file::size`.\
Para cerrar cada hijo obtenido está `file::close`.

### - ENG
Signature:
```cpp
void* inside(void* folder, UWORD idx);
```
Parameters:
* **Folder**: Handle Folder to walk.
* **Idx**: Processor word-sized unsigned integer representing the index of the entry inside the folder. It starts at `0`.

Return:\
The Handle File or Handle Folder of the requested entry, upon success.\
`nullptr` if **Folder** is not a valid Handle Folder or if **Idx** falls outside the folder range.

\
Behavior:\
It opens the entry sitting at position **Idx** inside the folder and delivers a fully functional handle.\
When the entry is a file you get a Handle File, and when it is a folder you get a Handle Folder that can be walked with this same function.\
That is how whole directory trees are traversed through recursion.\
Valid indexes go from `0` to `file::size(folder) - 1`.\
The order of the entries is not guaranteed: neither alphabetical nor date-based.\
A position may therefore not match the same entry between two runs.\
The numbering is the one `file::size` counts with: only `.` and `..` are left out, so hidden entries count in both functions and the indexes never fall out of step.\
The delivered handle inherits the permissions the source folder was opened with.\
Closing the folder does not close the entries already opened: each one is closed on its own.\
For the traversal loop there is `file::size`.\
For closing each child obtained there is `file::close`.
