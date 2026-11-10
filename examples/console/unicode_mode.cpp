/* // ESP
	Ejemplo del modo de caracteres de la consola.
	Muestra: console::setunicode, console::getunicode, console::restoreunicode
	y su efecto sobre la salida de la biblioteca C.
	Compilar: g++ unicode_mode.cpp -o unicode_mode
	Requiere: #define ST_IMPORT_CONSOLE antes de incluir la cabecera.
	Nota: en Windows el cambio de pagina de codigo puede ser rechazado por el host.
	// ENG
	Example of the console character mode.
	Shows: console::setunicode, console::getunicode, console::restoreunicode
	and their effect over the C library output.
	Build: g++ unicode_mode.cpp -o unicode_mode
	Needs: #define ST_IMPORT_CONSOLE before including the header.
	Note: on Windows the code page switch may be refused by the host.
*/
#define ST_IMPORT_CONSOLE
#include "System_Tools.h"
using namespace System_Tools;

// ESP: C3 A1 C3 A9 C3 B3 son los bytes UTF-8 de "aeo con acento", escritos como hex para
// que el fuente siga siendo ASCII: lo que se ve depende de como la consola lea esos bytes.
// ENG: C3 A1 C3 A9 C3 B3 are the UTF-8 bytes of "a e o with accents", written as hex so the
// source stays ASCII: what shows up depends on how the console reads those bytes.
#define UTF8_ACCENTS "\xC3\xA1\xC3\xA9\xC3\xB3"

int main(){
	// ESP: El handle trae memorizado el modo con el que se abrio el programa.
	// ENG: The handle carries memorized the mode the program was opened with.
	if(!console::get_handle()){ puts("sin consola utilizable / no usable console"); return 1; }
	printf("modo inicial (UTF-8 = 1): %d\n", console::getunicode(nullptr));

	// ESP: console::write con wchar* no depende de este modo: siempre usa la ruta ancha.
	// El modo decide como se traducen los bytes que sueltan printf o puts.
	// ENG: console::write with wchar* does not depend on this mode: it always takes the wide route.
	// The mode decides how the bytes released by printf or puts get translated.
	console::write(nullptr, L"ancho\r\n", 7);
	puts("^ escrito con wchar*, sin importar el modo de pagina");

	// ESP: setunicode devuelve el handle si la consola acepto el cambio, nullptr si no.
	// ENG: setunicode returns the handle if the console took the change, nullptr if it did not.
	void* ok = console::setunicode(nullptr, true);
	if(ok) puts("pagina UTF-8 puesta / UTF-8 page set");
	else puts("la consola rechazo la pagina / the console refused the page");
	printf("getunicode: %d, acentos por la biblioteca C: " UTF8_ACCENTS "\n", console::getunicode(nullptr));

	// ESP: Con el modo apagado los mismos bytes pasan por la pagina heredada y se ven partidos.
	// ENG: With the mode off the same bytes go through the legacy page and show in pieces.
	if(console::setunicode(nullptr, false)){
		printf("getunicode: %d, los mismos bytes: " UTF8_ACCENTS "\n", console::getunicode(nullptr));
	}else puts("no se pudo apagar la pagina / the page could not be turned off");

	// ESP: El handle acepta nullptr en todas las funciones del modulo y se sirve el unico.
	// ENG: Every module function takes nullptr for the handle and fetches the unique one.
	printf("getunicode(nullptr) = getunicode(handle): %d %d\n",
		console::getunicode(nullptr), console::getunicode(console::get_handle()));

	// ESP: restoreunicode vuelve al modo capturado al construirse el handle, no al de este momento.
	// ENG: restoreunicode goes back to the mode captured when the handle was built, not to this one.
	console::restoreunicode(nullptr);
	printf("tras restoreunicode: %d\n", console::getunicode(nullptr));

	// ESP: Al terminar el programa el handle restituye su modo por cuenta propia.
	// ENG: When the program ends the handle restores its mode on its own.
	return 0;
}
