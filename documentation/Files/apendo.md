# apendo
### - ESP
Firma:
```cpp
UWORD apendo(void* file, void* Hobj);
```
Parámetros:
* **File**: Handle Fichero de destino. Debe poseer el permiso `file_mode::write`.
* **Hobj**: Handle del objeto de System Tools que se desea volcar en el fichero.

Retorno:\
Entero sin signo del tamaño de la palabra del procesador: cantidad de bytes escritos.\
`0` si el objeto no es serializable, si la handle de destino es inválida o si carece del permiso de escritura.

\
Comportamiento:\
Vuelca en el fichero la representación de una handle de System Tools, para que pueda reconstruirse más tarde.\
El objeto queda precedido por una cabecera que identifica su tipo, el tamaño de palabra con el que fue creado y el orden de bytes del sistema de origen.\
Esa cabecera permite reajustar la lectura posterior y recuperar el objeto aunque el fichero se abra en una máquina distinta.\
Qué objetos pueden volcarse depende de los módulos importados, y el programa destino debe importar también el módulo correspondiente para poder leerlos de vuelta.\
Una Handle Fichero guarda su ruta y sus permisos, nunca su contenido, así que reconstruirla significa volver a abrir ese mismo fichero.\
La escritura ocurre en la posición actual del puntero de E/S, así que un añadido real exige colocar antes el puntero al final.\
Su pareja exacta, que devuelve la handle viva, es `file::geto`.\
Para colocar el puntero está `file::seek`.

### - ENG
Signature:
```cpp
UWORD apendo(void* file, void* Hobj);
```
Parameters:
* **File**: Destination Handle File. It must hold the `file_mode::write` permission.
* **Hobj**: Handle of the System Tools object to dump into the file.

Return:\
Processor word-sized unsigned integer: the amount of bytes written.\
`0` if the object is not serializable, if the destination handle is invalid, or if it lacks the write permission.

\
Behavior:\
It dumps into the file the representation of a System Tools handle, so it can be rebuilt later.\
The object is preceded by a header that identifies its type, the word size it was created with, and the byte order of the source system.\
That header lets a later reading readjust itself and recover the object even when the file is opened on a different machine.\
Which objects may be dumped depends on the imported modules, and the destination program must import the corresponding module as well to read them back.\
A Handle File stores its path and permissions, never its content, so rebuilding it means opening that same file again.\
The writing happens at the current I/O pointer position, so a real append requires placing the pointer at the end first.\
Its exact counterpart, which hands back the living handle, is `file::geto`.\
To place the pointer there is `file::seek`.
