/* // ESP
	Contrajemplo: la funcion recadera (mailman) escrita con los tipos de C y no con los del modulo.
	Compilar: g++ window_mailman_signature.cpp -o window_mailman_signature
	Fallo: invalid conversion from 'UWORD (*)(void*, unsigned int, unsigned int, int)' to
	'UWORD (*)(void*, UWORD, UWORD, WORD)' [-fpermissive], inicializando el argumento 5 de window::create
	Causa: UWORD y WORD cambian de tamano con la macro WORD_SIZE: en 64 bits valen 8 bytes y en
	32 bits 4, mientras unsigned int e int se quedan en 4 siempre. La firma escrita a mano coincide
	en 32 bits por casualidad y deja de coincidir en 64: el mismo codigo se rompe al cambiar de target.
	Salida: declarar la recadera con los tipos del modulo, UWORD y WORD, que es lo que create espera.
	Nota: con -fpermissive baja a aviso. Este fichero tampoco se ejecuta: pide ventanas graficas.
	// ENG
	Counterexample: the mailman written with the C types instead of the module types.
	Build: g++ window_mailman_signature.cpp -o window_mailman_signature
	Failure: invalid conversion from 'UWORD (*)(void*, unsigned int, unsigned int, int)' to
	'UWORD (*)(void*, UWORD, UWORD, WORD)' [-fpermissive], initializing argument 5 of window::create
	Cause: UWORD and WORD change size with the WORD_SIZE macro: on 64 bits they are 8 bytes and on
	32 bits 4, while unsigned int and int stay at 4 always. The hand written signature matches on
	32 bits by luck and stops matching on 64: the same code breaks when the target changes.
	The way out: declare the mailman with the module types, UWORD and WORD, which is what create expects.
	Note: with -fpermissive it drops to a warning. This file does not get run either: it asks for graphic windows.
*/
#define ST_IMPORT_WINDOW
#include "System_Tools.h"
using namespace System_Tools;

// ESP: La recadera mal firmada: los cuatro argumentos con tipos fijos de C.
// ENG: The badly signed mailman: the four arguments with fixed C types.
static UWORD mailman(void* wnd, unsigned int msg, unsigned int wp, int lp){
	(void)wnd; (void)msg; (void)wp; (void)lp;
	return 0;
}

// ESP: La firma buena, en el bloque de abajo: los tipos del modulo en cada argumento, y el
// mismo nombre, porque las dos no pueden convivir. Apague la mala antes de probarla.
// ENG: The good signature, in the block below: the module types on every argument, and the
// same name, since the two cannot live together. Turn the bad one off before testing it.
#if 0
static UWORD mailman(void* wnd, UWORD msg, UWORD wp, WORD lp){
	(void)wnd; (void)msg; (void)wp; (void)lp;
	return 0;
}
#endif

int main(){
	// ESP: aqui esta el fallo: create pide UWORD(*)(void*,UWORD,UWORD,WORD).
	// ENG: here is the failure: create asks for UWORD(*)(void*,UWORD,UWORD,WORD).
	void* wnd = window::create(nullptr, "contrajemplo", 200u, 120u, mailman);

	window::destroy(wnd);
	return 0;
}
