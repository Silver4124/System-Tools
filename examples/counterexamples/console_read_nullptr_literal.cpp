/* // ESP
	Contrajemplo: console::read escrito con el literal nullptr en el puesto del texto.
	Compilar: g++ console_read_nullptr_literal.cpp -o console_read_nullptr_literal
	Fallo: ninguno. Compila, corre, no avisa, y la lectura nunca ocurre.
	La funcion elige la sobrecarga 'read(void*, decltype(nullptr), UWORD*)', que devuelve
	nullptr sin tocar la cola: el contador que se paso sigue con el valor que traia.
	Causa: nullptr literal encaja exacto en decltype(nullptr) y esa coincidencia le gana a la
	conversion de puntero nulo hacia wchar**, asi que la variante util queda descartada.
	Salida: pasar la direccion de la variable del texto, &texto, y acordarse de liberar lo que
	la funcion allocate; si solo importa cuanto se consumo, se tira el texto despues de leerlo.
	Nota: la lectura verdadera bloquea hasta que el usuario escribe, por eso esta comentada.
	// ENG
	Counterexample: console::read written with the literal nullptr in the text slot.
	Build: g++ console_read_nullptr_literal.cpp -o console_read_nullptr_literal
	Failure: none. It compiles, it runs, it says nothing, and the reading never happens.
	The function picks the overload 'read(void*, decltype(nullptr), UWORD*)', which answers
	nullptr without touching the queue: the counter that got passed keeps the value it brought.
	Cause: a literal nullptr fits decltype(nullptr) exactly and that match beats the null pointer
	conversion towards wchar**, so the useful variant gets dropped from the race.
	The way out: pass the address of the text variable, &text, and remember to free what the
	function allocates; if only the consumed amount matters, throw the text away after reading it.
	Note: the real reading blocks until the user types, which is why it stays commented out.
*/
#define ST_IMPORT_CONSOLE
#include "System_Tools.h"
using namespace System_Tools;

int main(){
	void* cnsl = console::get_handle();

	// ESP: El contador trae un valor conocido para poder ver que nadie lo escribe.
	// ENG: The counter brings a known value so it can be seen that nobody writes it.
	UWORD count = 999;

	// ESP: aqui esta el fallo: con el literal la eleccion cae en la variante que no lee nada.
	// ENG: here is the failure: with the literal the pick falls on the variant that reads nothing.
	void* back = console::read(cnsl, nullptr, &count);
	printf("literal nullptr : ret %p, contador " UWORD_PRINT "\n", back, count);

	// ESP: No hay segunda llamada que comparar: la via util pide wchar** o char**, o sea la
	// direccion de la variable del texto, y esa si bloquea hasta que algo llegue por la consola.
	// ENG: There is no second call to compare against: the useful road asks wchar** or char**, that is
	// the address of the text variable, and that one blocks until something arrives through the console.

	// ESP: La lectura real, en el bloque de abajo: la direccion de la variable del texto. Bloquea
	// hasta que algo llegue por la consola, por eso se queda apagada en el ejemplo.
	// ENG: The real reading, in the block below: the address of the text variable. It blocks
	// until something arrives through the console, which is why the example keeps it turned off.
#if 0
	wchar* text = nullptr;
	UWORD read = 0;
	console::read(cnsl, &text, &read);
	char* utf8 = nullptr;
	UWORD bytes = wcstombs(&utf8, text, read, false);
	printf("leido " UWORD_PRINT " unidades, " UWORD_PRINT " bytes: %s\n", read, bytes, utf8 ? utf8 : "");
	if(utf8) memory::free(utf8);
	if(text) memory::free(text);
#endif

	return 0;
}
