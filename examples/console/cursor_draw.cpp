/* // ESP
	Ejemplo del posicionamiento de cursor en la terminal.
	Muestra: console::gotoxy, console::getxy y console::write escritos en un marco.
	Compilar: g++ cursor_draw.cpp -o cursor_draw
	Requiere: #define ST_IMPORT_CONSOLE antes de incluir la cabecera.
	Nota: las coordenadas van en celdas de caracter contadas desde 0, no son pixeles.
	// ENG
	Example of cursor positioning in the terminal.
	Shows: console::gotoxy, console::getxy and console::write written into a frame.
	Build: g++ cursor_draw.cpp -o cursor_draw
	Needs: #define ST_IMPORT_CONSOLE before including the header.
	Note: coordinates are character cells counted from 0, they are not pixels.
*/
#define ST_IMPORT_CONSOLE
#include "System_Tools.h"
using namespace System_Tools;

// ESP: console::write no recorre la cadena: le hace falta la cantidad de unidades.
// La medida se cuenta aqui a mano, igual que la hacen las variantes sin Len.
// ENG: console::write does not walk the string: it needs the amount of units.
// The measure is counted here by hand, just like the length-free variants do.
UWORD wlen(const wchar* text){
	UWORD n = 0;
	while(text[n]) ++n;
	return n;
}

void put(UWORD x, UWORD y, const wchar* text){
	// ESP: gotoxy devuelve el handle, o nullptr si la consola no acepto el movimiento.
	// ENG: gotoxy returns the handle, or nullptr if the console refused the move.
	if(!console::gotoxy(nullptr, x, y)) return;
	console::write(nullptr, text, wlen(text));
}

int main(){
	// ESP: La consola puede no existir (salida redirigida a un archivo, sin tty).
	// ENG: The console may not exist (output redirected to a file, no tty).
	if(!console::get_handle()){ puts("sin consola utilizable / no usable console"); return 1; }

	UWORD x0 = 2, y0 = 2, wide = 34, tall = 9;
	put(x0, y0, L"+--------------------------------+");
	put(x0, y0 + tall - 1, L"+--------------------------------+");
	for(UWORD row = 1; row < tall - 1; ++row){
		put(x0, y0 + row, L"|");
		put(x0 + wide - 1, y0 + row, L"|");
	}
	put(x0 + 2, y0 + 2, L"cursor en celdas, no en pixeles");
	put(x0 + 2, y0 + 4, L"gotoxy(2,2) fue el origen del marco");

	// ESP: getxy pregunta donde esta el cursor: write lo deja justo despues de lo ultimo.
	// ENG: getxy asks where the cursor stands: write leaves it right after the last thing.
	UWORD cx = 0, cy = 0;
	put(x0 + 2, y0 + 6, L"fin de linea->");
	if(console::getxy(nullptr, &cx, &cy))
		printf("cursor tras escribir: x=" UWORD_PRINT ", y=" UWORD_PRINT "\n", cx, cy);
	else puts("la consola no respondo por el cursor / console did not answer for the cursor");

	// ESP: getxy puede pedir solo una coordenada y dejar la otra en nullptr.
	// ENG: getxy can ask for one coordinate only and leave the other in nullptr.
	UWORD only_y = 0;
	console::getxy(nullptr, nullptr, &only_y);
	printf("solo la fila: " UWORD_PRINT "\n", only_y);

	// ESP: nullptr en el handle hace que la funcion se sirva el handle unico.
	// ENG: nullptr on the handle makes the function fetch the unique handle.
	console::gotoxy(nullptr, 0, y0 + tall + 1);
	// ESP: printf y console::write no comparten bufer: el orden visible puede no ser el de las llamadas.
	// ENG: printf and console::write share no buffer: the visible order may not be the call order.
	puts("esta linea sale por la biblioteca C, no por console::write");

	// ESP: Un destino fuera de la pantalla queda a criterio de la consola: el veredicto esta en el retorno.
	// ENG: A destination off the screen is up to the console: the verdict sits in the return.
	void* moved = console::gotoxy(nullptr, 4000, 4000);
	printf("gotoxy fuera de pantalla -> %p\n", moved);

	return 0;
}
