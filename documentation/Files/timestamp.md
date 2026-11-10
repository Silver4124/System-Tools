# timestamp
### - ESP
Firma:
```cpp
UWORD timestamp(void* file, BYTE type = time_mode::creation);
```
Parámetros:
* **File**: Handle Fichero o Handle Folder a consultar.
* **Type**: Momento temporal que se desea obtener. Por defecto `time_mode::creation`.

Retorno:\
Entero sin signo del tamaño de la palabra del procesador: segundos transcurridos desde el 1 de enero de 1970 a las 00:00:00 (UTC).\
`0` en caso de error.

\
Comportamiento:\
Recupera la marca de creación, de último acceso o de última escritura de un elemento, según la etiqueta de [[[Enum time_mode]]](Enum%20time_mode.md) pasada a **Type**.\
Funciona igual sobre Handle Fichero que sobre Handle Folder.\
Cualquier valor de **Type** fuera de esas tres etiquetas se toma como `time_mode::writed`.\
Linux no guarda una fecha de nacimiento del elemento: `time_mode::creation` entrega el instante del último cambio de su estado (permisos, enlaces o contenido), que suele coincidir con la creación pero no está garantizado.\
En Windows la creación es la fecha real que registra el sistema de archivos.\
La medida está en segundos y en horario universal coordinado, sin aplicar la zona horaria local, así que es comparable entre máquinas y adecuada para guardar en disco.\
El rango depende del tamaño de palabra del procesador: en una compilación de 32 bits las marcas posteriores al año 2106 no son representables.\
Una fecha a cero no siempre significa un fallo: algunos sistemas de archivos no registran la creación, y entonces `time_mode::creation` devuelve `0` legítimamente.\
Ante la duda, prueba `time_mode::writed`, que suele estar siempre disponible.\
No se requiere ningún permiso especial sobre la handle, y la consulta no desplaza su puntero de E/S.\
Para desglosar el valor está `file::to_date`.\
Para construir uno a partir de sus componentes está `file::to_time`.

### - ENG
Signature:
```cpp
UWORD timestamp(void* file, BYTE type = time_mode::creation);
```
Parameters:
* **File**: Handle File or Handle Folder to query.
* **Type**: Time moment to retrieve. Defaults to `time_mode::creation`.

Return:\
Processor word-sized unsigned integer: the amount of seconds elapsed since January 1st, 1970 at 00:00:00 (UTC).\
`0` upon failure.

\
Behavior:\
It retrieves the creation, last-access or last-write mark of an element, following the tag of [[[Enum time_mode]]](Enum%20time_mode.md) passed to **Type**.\
It works the same on a Handle File as on a Handle Folder.\
Any **Type** value outside those three tags is taken as `time_mode::writed`.\
Linux stores no birth date for the element: `time_mode::creation` delivers the instant of its last status change (permissions, links or content), which usually coincides with creation but is far from guaranteed.\
On Windows creation is the real date the filesystem records.\
The measure is in seconds and in Coordinated Universal Time, with no local time zone applied, which makes it comparable between machines and suitable for storing on disk.\
The range depends on the processor word size: in a 32-bit build marks after the year 2106 are not representable.\
A date sitting at zero does not always mean a failure: some filesystems do not record the creation date, and then `time_mode::creation` legitimately returns `0`.\
When in doubt, try `time_mode::writed`, which is almost always available.\
No special permission is required on the handle, and the query does not move its I/O pointer.\
To break the value down there is `file::to_date`.\
To build one from its components there is `file::to_time`.

## Overloads
### - ESP
Firma:
```cpp
UWORD timestamp(const char* filepath, BYTE type = time_mode::creation);
```
Parámetros:
* **Filepath**: Ruta del fichero o carpeta a consultar.
* **Type**: Momento temporal que se desea obtener. Por defecto `time_mode::creation`.

Retorno:\
Entero sin signo del tamaño de la palabra del procesador: segundos transcurridos desde el 1 de enero de 1970 a las 00:00:00 (UTC).\
`0` si la ruta no existe o no pudo consultarse.

\
Comportamiento:\
Resuelve la marca directamente desde una ruta, sin que tú mantengas ninguna handle abierta.\
Prueba primero como fichero y, si eso no funciona, como carpeta, así que ambas clases de elemento son válidas.\
La apertura es temporal: si otro proceso mantiene el fichero bloqueado, la consulta puede fallar aunque la ruta exista.

### - ENG
Signature:
```cpp
UWORD timestamp(const char* filepath, BYTE type = time_mode::creation);
```
Parameters:
* **Filepath**: Path of the file or folder to query.
* **Type**: Time moment to retrieve. Defaults to `time_mode::creation`.

Return:\
Processor word-sized unsigned integer: the amount of seconds elapsed since January 1st, 1970 at 00:00:00 (UTC).\
`0` if the path does not exist or could not be queried.

\
Behavior:\
It resolves the mark straight from a path, without you keeping any handle open.\
It tries the path as a file first and, if that fails, as a folder, so both kinds of element are valid.\
The opening is temporary: if another process holds the file locked, the query may fail even though the path exists.
