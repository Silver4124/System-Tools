/* // ESP
	Contrajemplo: la etiqueta de una entrada json escrita como literal de texto.
	Compilar: g++ json_add_elem_literal.cpp -o json_add_elem_literal
	Fallo: invalid conversion from 'const void*' to 'void*' [-fpermissive]
	inicializando el argumento 2 de 'void* System_Tools::json::add_elem(void*, void*, BYTE)'
	y el mismo traje para el numero suelto: invalid conversion from 'int' to 'void*'
	Causa: add_elem recibe la posicion o el nombre como void*, un puntero sin const, y una
	literal de texto es const char*; la conversion hacia atras no la permite el C++ normal.
	Salida: el cast (void*)"nombre" en cada etiqueta y (void*)1 en cada indice, como hacen los ejemplos.
	Nota: con -fpermissive el error baja a aviso y el fichero si compila; con los flags del
	Makefile (-std=c++11 -O2) se queda en error. Este fichero esta escrito para fallar.
	// ENG
	Counterexample: the label of a json entry written as a plain text literal.
	Build: g++ json_add_elem_literal.cpp -o json_add_elem_literal
	Failure: invalid conversion from 'const void*' to 'void*' [-fpermissive]
	initializing argument 2 of 'void* System_Tools::json::add_elem(void*, void*, BYTE)'
	and the same suit for the loose number: invalid conversion from 'int' to 'void*'
	Cause: add_elem takes the position or the name as void*, a pointer with no const, and a
	text literal is const char*; the backwards conversion is not allowed by regular C++.
	The way out: the cast (void*)"nombre" on every label and (void*)1 on every index, like the examples do.
	Note: with -fpermissive the error drops to a warning and the file does compile; with the
	Makefile flags (-std=c++11 -O2) it stays an error. This file is written to fail.
*/
#define ST_IMPORT_JSON
#include "System_Tools.h"
using namespace System_Tools;

int main(){
	void* root = json::init(json_type::obj);

	// ESP: aqui esta el fallo: "nombre" llega como const char* y add_elem pide void*.
	// ENG: here is the failure: "nombre" arrives as const char* and add_elem asks for void*.
	json::add_elem(root, "nombre", json_type::str);

	// ESP: El mismo hueco en el indice de una matriz: el numero suelto tampoco es void*.
	// ENG: The same hole on an array index: the loose number is no void* either.
	json::add_elem(root, (void*)"lista", json_type::ary);
	void* lista = json::value(root, (void*)"lista");
	json::add_elem(lista, 1, json_type::num);
	json::detach(lista);

	// ESP: Ojo con el cero suelto: ese si compila, porque 0 es la constante de puntero nulo y el
	// void* nulo vale como indice 0; es un accidente del lenguaje, no la via prevista por la firma.
	// ENG: Beware the loose zero: that one does compile, because 0 is the null pointer constant and
	// the null void* serves as index 0; it is an accident of the language, not the road the signature plans.

	// ESP: La forma correcta, en el bloque de abajo: el cast en los dos sitios. Apague las lineas
	// del fallo antes de probarla, si no las dos versiones quedan conviviendo en el mismo main.
	// ENG: The correct shape, in the block below: the cast in both places. Turn the failing lines
	// off before testing it, or the two versions end up living in the same main.
#if 0
	json::add_elem(root, (void*)"nombre", json_type::str);
	json::add_elem(lista, (void*)1, json_type::num);
#endif

	json::detach(root);
	return 0;
}
