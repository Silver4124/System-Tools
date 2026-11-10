# ⚙️ Default Functions
### - ESP
Este apartado respeta a todo el conjunto de funciones de System Tools que no dependan de una macro de inclusión de modulo para ser concebidas y otorgadas al desarrollador.
* **Cero dependencias externas**: Todas las funciones pertenecientes a este conjunto resuelven internamente sus definiciones por medio de solicitudes de funciones escenciales al sistema operativo (Por medio de `get_system_function` y `get_system_variable`) y/o adaptaciones escritas desde cero en C++.
* **Portabilidad Multiplataforma**: Internamente, estas funciones procuran comportamientos y desgloses de desarrollo iguales entre Windows y Unix.
* **Bases Iniciales**: Estas funciones sirven de ladrillos fundamentales para la creación de los modulos.
### - ENG
This section respects the entire set of System Tools functions that do not depend on a module inclusion macro to be conceived and granted to the developer.
* **Zero external dependencies**: All functions in this set resolve their definitions internally through calls to essential operating system functions (via `get_system_function` and `get_system_variable`) and/or adaptations written from scratch in C++.
* **Cross-platform portability**: Internally, these functions ensure consistent behavior and implementation across Windows and Unix environments.
* **Initial Bases**: These functions serve as fundamental building blocks for the creation of the modules.

## Structuration
### - ESP
* **System Core**: Toda aquella función que sirve de átomos para la creación de resto de funciones de este apartado. Este subconjunto respecta a las funciones:
  * [`get_system_function`](get_system_function.md).
  * [`get_system_variable`](get_system_variable.md).
* **Adaptaciones de C y E/S basicas**: Toda aquella función que sirve como una adaptación de las funciones de librerias estandar de C, proporcionando funcionalidades basicas al programador para empezar a programar programas simples. Este subconjunto compone funciones que sufrieron cambios, algunos semanticos y otros operativos, aunque la mayoria de firmas se mantienen igual respecto a sus contrapartes de C, ademas de proporcionar nuevas implementaciones y abstracciones en pro de la comodidad.
  * [`system`](system.md).
  * [`clear`](clear.md): Abstracción de la expresión `system("clear")` o `system("cls")`.
  * [`puts`](puts.md).
  * [`putchar`](putchar.md).
  * [`printf`](printf.md).
  * [`sprintf`](sprintf.md).
  * [`scanf`](scanf.md).
  * [`sscanf`](sscanf.md).
  * [`mbstowcs`](mbstowcs.md): Ahora retorna la cantidad de elementos `wchar` del resultado, excluyendo el terminador, y ademas posee una variante para buffers de longitud variable.
  * [`wcstombs`](wcstombs.md): Ahora retorna la cantidad de elementos `char` del resultado, excluyendo el terminador, y ademas posee una variante para buffers de longitud variable.
  * [`language`](language.md): Retorna el idioma del usuario, codificado en un conjunto de 4 caracteres ASCII empaquetados en un unico `UWORD`.
  * [`sleep`](sleep.md): Ahora funciona en milisegundos.
  * [`message`](message.md): Permite crear una ventana gráfica, de interrupción al programa.
  * [`Enum message_box`](Enum%20message_box.md): Las etiquetas con las que se arma la apariencia de esa ventana.
* [**Administración de la memoria**](memory/README.md): Toda aquella función que pertenezca al `namespace memory`.
* [**Administración de ejecución de hilos en paralelo**](thread/README.md): Toda aquella función que pertenezca al `namespace thread`.
### - ENG
* **System Core**: All those functions that serve as atoms for the creation of the rest of the functions in this section. This subset pertains to the following functions:
  * [`get_system_function`](get_system_function.md).
  * [`get_system_variable`](get_system_variable.md).
* **C and Basic I/O Adaptations**: Functions that adapt standard C library functions, providing the programmer with basic capabilities to start writing simple programs. This subset comprises functions that underwent changes, some semantic and others operational, although most signatures remain the same with respect to their C counterparts, in addition to providing new implementations and abstractions for convenience.
  * [`system`](system.md).
  * [`clear`](clear.md): Abstraction of the expression `system("clear")` or `system("cls")`.
  * [`puts`](puts.md).
  * [`putchar`](putchar.md).
  * [`printf`](printf.md).
  * [`sprintf`](sprintf.md).
  * [`scanf`](scanf.md).
  * [`sscanf`](sscanf.md).
  * [`mbstowcs`](mbstowcs.md): Now returns the number of `wchar` elements in the result, excluding the terminator, and also has a variant for variable-length buffers. 
  * [`wcstombs`](wcstombs.md): Now returns the number of `char` elements in the result, excluding the terminator, and also has a variant for variable-length buffers. 
  * [`language`](language.md): Returns the user's language, encoded in a set of 4 ASCII characters packaged into a single `UWORD`. 
  * [`sleep`](sleep.md): Now operates in milliseconds. 
  * [`message`](message.md): Allows for the creation of a graphical window that interrupts program execution.
  * [`Enum message_box`](Enum%20message_box.md): The tags that build up that window's appearance.
* [**Memory Management**](memory/README.md): Any function that belongs to the `namespace memory`.
* [**Thread Management**](thread/README.md): Any function that belongs to the `namespace thread`.
