/* // ESP
	Ejemplo del trazado directo sobre el buffer de pixeles: una linea a mano, sin GDI.
	Muestra: window::create, window::pixels, window::getsize, window::yield, window::sendmessage
	y window::wait, con el algoritmo de Bresenham escrito por el programa.
	Compilar: g++ line_draw.cpp -o line_draw
	Requiere: #define ST_IMPORT_WINDOW antes de incluir la cabecera y una pantalla donde abrir la ventana.
	// ENG
	Example of the direct tracing over the pixel buffer: one line by hand, with no GDI.
	Shows: window::create, window::pixels, window::getsize, window::yield, window::sendmessage
	and window::wait, with Bresenham's algorithm written by the program.
	Build: g++ line_draw.cpp -o line_draw
	Needs: #define ST_IMPORT_WINDOW before including the header and a screen where to open the window.
*/
#define ST_IMPORT_WINDOW
#include "System_Tools.h"
using namespace System_Tools;

// ESP: La funcion pinta dentro del buffer del modulo: el vuelco a pantalla lo tiene que pedir el programa.
// ENG: The function paints inside the module's buffer: the dump to screen has to get asked by the program.
BYTE draw_line(void* wnd, int x1, int y1, int x2, int y2, unsigned int color){
	// ESP: yield sin banderin equivale a preguntar si el handle sigue en pie antes de tocar nada.
	// ENG: yield without the flag comes down to asking if the handle still stands before touching anything.
	if(!window::yield(wnd)) return 0;
	unsigned int *pixels = nullptr; int width, height;
	window::pixels(wnd, &pixels, nullptr);
	window::getsize(wnd, nullptr, nullptr, &width, &height);
	if(!pixels || width <= 0 || height <= 0) return 0;

	// ESP: Los dos extremos fuera de la misma banda es un fuera de medida: se responde el exito teorico.
	// ENG: Both ends outside the same band is out of measure: the theoretical success gets answered.
	if((x1 < 0 && x2 < 0) || (y1 < 0 && y2 < 0) || (x1 >= width && x2 >= width) || (y1 >= height && y2 >= height))
		return 1;

	// ESP: El paso a paso solo avanza hacia abajo a la derecha: los otros octantes hay que girarlos antes.
	// Un punto suelto se dibuja con la misma comprobacion de dentro de la medida.
	// ENG: The step by step only advances down to the right: the other octants have to be turned before.
	// A single dot gets drawn with the same check of being inside the measure.
	if(x1 == x2 && y1 == y2){
		if(x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) pixels[y1 * width + x1] = color;
		return 1;
	}
	// ESP: Cada pixel de la fila ocupa un entero en el algoritmo de Bresenham: la cuenta es y por ancho mas x.
	// ENG: Each pixel of the row takes one integer in Bresenham's algorithm: the count is y times width plus x.
	int dx = x2 - x1;
	int dy = y2 - y1;
	int d = 2 * dy - dx;
	while(x1 <= x2){
		if(x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) pixels[y1 * width + x1] = color;
		x1++;
		if(d < 0) d = d + 2 * dy;
		else{
			y1++;
			d = d + 2 * (dy - dx);
		}
	}
	return 1;
}

int main(){
	// ESP: El color entra como 0x00RRGGBB: aqui es azul con verde, o sea un morado claro.
	// ENG: The color comes in as 0x00RRGGBB: here it is blue with green, that is a light purple.
	void* wnd = window::create(nullptr, "test", 500, 500);
	draw_line(wnd, 0, 0, 499, 499, 0x00FF00FF);

	// ESP: paint es el unico vuelco manual: sin la llamada el buffer queda lleno y la pantalla vacia.
	// wait espera el cierre del usuario o del sistema y libera el handle al salir.
	// ENG: paint is the only manual dump: without the call the buffer stays full and the screen empty.
	// wait waits for the closing by the user or by the system and frees the handle on the way out.
	window::sendmessage(wnd, window_box::paint, 0, 0);
	window::wait(wnd);
	return 0;
}
