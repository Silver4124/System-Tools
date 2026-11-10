# parent
### - ESP
Firma:
```cpp
void* parent(void* file);
```
Parámetros:
* **File**: Handle Fichero o Handle Folder cuya carpeta contenedora se desea obtener.

Retorno:\
Handle Folder de la carpeta que contiene el elemento, abierta con los mismos permisos que la handle de origen, en caso de éxito.\
`nullptr` si el elemento está en la raíz de su unidad, si no tiene ruta de contenedor o si la carpeta resultante no pudo abrirse.

\
Comportamiento:\
Devuelve la carpeta inmediatamente superior a la ubicación de una handle, así que se puede subir por un árbol de directorios sin manipular cadenas de ruta.\
El handle devuelto es nuevo e independiente del de origen, por lo que hay que cerrarlo con `file::close` cuando deje de ser necesario.\
La handle consultada permanece intacta y abierta.\
Hereda los permisos de la handle de origen: un padre obtenido desde una handle de solo lectura tampoco permitirá escribir en la carpeta.\
Sobre una Handle Folder, el resultado es su propia carpeta contenedora; no se devuelve la carpeta misma.\
Consultar la ruta del padre: `file::path`.\
Ver qué contiene el padre: `file::size` y `file::inside`.

### - ENG
Signature:
```cpp
void* parent(void* file);
```
Parameters:
* **File**: Handle File or Handle Folder whose containing folder is wanted.

Return:\
Handle Folder of the folder that contains the element, opened with the same permissions as the source handle, upon success.\
`nullptr` if the element sits at the root of its drive, if it has no container path or if the resulting folder could not be opened.

\
Behavior:\
It returns the folder immediately above the location of a handle, so a directory tree can be walked upwards without manipulating path strings.\
The returned handle is new and independent from the source one, so it must be closed with `file::close` once it is no longer needed.\
The queried handle stays intact and open.\
It inherits the permissions of the source handle: a parent obtained from a read-only handle will not allow writing into the folder either.\
On a Handle Folder, the result is its own containing folder; the folder itself is never returned.\
Reading the parent's path: `file::path`.\
Seeing what the parent holds: `file::size` and `file::inside`.
