/* // ESP
	Ejemplo del handle Consola y de su control de color.
	Muestra: console::get_handle, console::setcolor, console::getcolor,
	console::restorecolor y console::write.
	Compilar: g++ color_text.cpp -o color_text
	Requiere: #define ST_IMPORT_CONSOLE antes de incluir la cabecera.
	Nota: el color real pide una consola que interprete ANSI (Windows 10 o posterior).
	// ENG
	Example of the Console handle and its color control.
	Shows: console::get_handle, console::setcolor, console::getcolor,
	console::restorecolor and console::write.
	Build: g++ color_text.cpp -o color_text
	Needs: #define ST_IMPORT_CONSOLE before including the header.
	Note: real color asks for an ANSI-capable console (Windows 10 or newer).
*/
#define ST_IMPORT_CONSOLE
#include "System_Tools.h"
using namespace System_Tools;

// ESP: El formato es 0x00BBGGRR, el mismo de window::draw: rojo, verde, azul en ese orden.
// ENG: The format is 0x00BBGGRR, the same one of window::draw: red, green, blue in that order.
#define RED    0x000000FF
#define GREEN  0x0000FF00
#define BLUE   0x00FF0000
#define YELLOW 0x0000FFFF
#define WHITE  0x00FFFFFF
#define BLACK  0x00000000

int main(){
	// ESP: El handle es unico: dos llamadas dan la misma direccion, nunca hay que liberarlo.
	// ENG: The handle is unique: two calls give the same address, it is never freed.
	void* cnsl = console::get_handle();
	if(!cnsl){ puts("sin consola utilizable / no usable console"); return 1; }
	printf("handle: %p == %p\n", cnsl, console::get_handle());

	// ESP: getcolor entrega los colores vigentes, los que el handle capturo al construirse.
	// ENG: getcolor hands back the current colors, the ones the handle captured when built.
	unsigned int base_bg = 0, base_fg = 0;
	console::getcolor(cnsl, &base_bg, &base_fg);
	printf("color de texto  : 0x%08X\n", base_fg);
	printf("color de fondo  : 0x%08X\n", base_bg);

	// ESP: setcolor aplica a lo que se escriba despues, no a lo ya escrito.
	// console::write pide wchar* o char*, y Len cuenta unidades de wchar/char sin incluir el terminador.
	// ENG: setcolor applies to what is written after it, not to what is already written.
	// console::write asks for wchar* or char*, and Len counts wchar/char units without the terminator.
	console::setcolor(cnsl, BLACK, RED);
	console::write(cnsl, L"rojo\r\n", 6);
	console::setcolor(cnsl, BLACK, GREEN);
	console::write(cnsl, L"verde\r\n", 7);
	console::setcolor(cnsl, YELLOW, BLACK);
	console::write(cnsl, L"negro sobre amarillo\r\n", 22);

	// ESP: write no agrega saltos: el \r\n son caracteres escritos por uno.
	// ENG: write adds no line breaks: that \r\n is characters written one by one.
	console::setcolor(cnsl, BLACK, BLUE);
	console::write(cnsl, L"sin salto", 9);
	console::write(cnsl, L" y pegado\r\n", 11);

	// ESP: El handle acepta nullptr en su puesto: la funcion se lo sirve sola.
	// ENG: The handle accepts nullptr in its place: the function fetches it alone.
	console::setcolor(nullptr, BLACK, WHITE);
	console::write(nullptr, L"blanco\r\n", 8);

	// ESP: restorecolor devuelve los colores iniciales capturados por el handle.
	// ENG: restorecolor brings back the initial colors captured by the handle.
	console::restorecolor(cnsl);
	console::write(cnsl, L"colores restaurados\r\n", 21);

	unsigned int now_bg = 0, now_fg = 0;
	console::getcolor(cnsl, &now_bg, &now_fg);
	printf("tras restore: texto 0x%08X, fondo 0x%08X\n", now_fg, now_bg);

	// ESP: getcolor permite pedir solo un color y dejar el otro en nullptr.
	// ENG: getcolor allows asking for one color only and leaving the other in nullptr.
	unsigned int only_fg = 0;
	console::getcolor(cnsl, nullptr, &only_fg);
	printf("solo el texto: 0x%08X\n", only_fg);

	// ESP: Al terminar el programa el propio handle restaura lo que encontraba al abrirse.
	// ENG: When the program ends the handle itself restores what it found on opening.
	return 0;
}
