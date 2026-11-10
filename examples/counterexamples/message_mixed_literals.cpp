/* // ESP
	Contrajemplo: los dos lados de message escritos con literales de distinto tipo.
	Compilar: g++ message_mixed_literals.cpp -o message_mixed_literals
	Fallo: call of overloaded 'message(const wchar_t [6], const char [7], System_Tools::message_box)' is ambiguous
	Causa: la firma ancha pide wchar* en los dos lados y la firma neutra pide const void*;
	una literal de bytes no encaja del todo en la ancha y llega convertible a la neutra,
	así que las dos candidatas quedan empatadas y el compilador no elige por uno.
	Salida: escribir los dos lados del mismo tipo, o pasar el lado ancho por (const void*).
	Nota: este fichero esta escrito para fallar; ni lo ejecute ni lo meta en el build.
	// ENG
	Counterexample: the two sides of message written with literals of different types.
	Build: g++ message_mixed_literals.cpp -o message_mixed_literals
	Failure: call of overloaded 'message(const wchar_t [6], const char [7], System_Tools::message_box)' is ambiguous
	Cause: the wide signature asks wchar* on both sides and the neutral one asks const void*;
	a byte literal does not fit the wide side all the way and reaches the neutral one convertible,
	so the two candidates stay tied and the compiler does not pick for you.
	The way out: write both sides in the same type, or pass the wide side through (const void*).
	Note: this file is written to fail; neither run it nor leave it inside the build.
*/
#include "System_Tools.h"
using namespace System_Tools;

int main(){
	// ESP: aqui esta el fallo: texto ancho y titulo de bytes, los dos como literales.
	// ENG: here is the failure: wide text and byte title, both as literals.
	int r = message(L"texto", "titulo", message_box::ok);

	// ESP: Las dos salidas correctas, en el bloque de abajo: los dos lados anchos, o el lado
	// ancho metido por la firma neutra con el cast. Hay que apagar la linea del fallo primero.
	// ENG: The two correct exits, in the block below: both sides wide, or the wide side fed
	// through the neutral signature with the cast. The failing line has to be turned off first.
#if 0
	r = message(L"texto", L"titulo", message_box::ok);
	r = message((const void*)L"texto", "titulo", message_box::ok);
#endif

	printf("nunca llega: %d\n", r);
	return 0;
}
