/* // ESP
	Ejemplo del bucle de animacion sobre el buffer de pixeles de una ventana.
	Muestra: window::create, window::getsize, window::pixels, window::clear, window::draw,
	window::sendmessage, window::resize, window::mouse, window::keyboard, window::yield y window::wait.
	Compilar: g++ pixel_animation.cpp -o pixel_animation
	Requiere: #define ST_IMPORT_WINDOW antes de incluir la cabecera y una pantalla donde abrir la ventana.
	// ENG
	Example of the animation loop over the pixel buffer of a window.
	Shows: window::create, window::getsize, window::pixels, window::clear, window::draw,
	window::sendmessage, window::resize, window::mouse, window::keyboard, window::yield and window::wait.
	Build: g++ pixel_animation.cpp -o pixel_animation
	Needs: #define ST_IMPORT_WINDOW before including the header and a screen where to open the window.
*/
#define ST_IMPORT_WINDOW
#include "System_Tools.h"
using namespace System_Tools;

#define TILE   32
#define FRAMES 240

// ESP: Un mosaico del programa que se vuelca sobre el buffer de la ventana con draw.
// ENG: A program's mosaic that gets dumped over the window buffer with draw.
static void build_tile(unsigned int* tile){
	for(int y = 0; y < TILE; ++y){
		for(int x = 0; x < TILE; ++x){
			int border = x == 0 || y == 0 || x == TILE - 1 || y == TILE - 1;
			int dx = x - TILE / 2, dy = y - TILE / 2;
			int ring = (dx * dx + dy * dy) / 24;
			tile[y * TILE + x] = border ? 0x00FFFFFFu : (ring & 1 ? 0x0020A060u : 0x00104030u);
		}
	}
}

// ESP: La casilla que sigue al raton, del mismo tamano que el mosaico grande.
// ENG: The box that follows the mouse, of the same size as the big mosaic.
static void build_box(unsigned int* box){
	for(int y = 0; y < TILE; ++y)
		for(int x = 0; x < TILE; ++x)
			box[y * TILE + x] = (x == 15 || y == 15) ? 0x00FF4040u : 0x00000000u;
}

static const char* click_name(int code){
	switch(code){
		case window_box::lbutton_down: case window_box::rbutton_down: return "pulsado / pressed";
		case window_box::lbutton_hold: case window_box::rbutton_hold: return "mantenido / held";
		case window_box::lbutton_up:   case window_box::rbutton_up:   return "soltado / released";
	}
	return "libre / free";
}

int main(){
	// ESP: create entrega nullptr solo cuando el sistema no levanta la ventana: el alto y el ancho
	// que pide son los del area dibujable, asi que el marco queda fuera de la cuenta.
	// ENG: create hands over nullptr only when the system raises no window: the height and width it
	// asks for are those of the drawable area, so the frame stays out of the count.
	void* wnd = window::create(nullptr, "animacion", 360, 260);
	if(!wnd){ puts("sin ventana / no window"); return 1; }

	unsigned int tile[TILE * TILE], box[TILE * TILE];
	build_tile(tile);
	build_box(box);

	// ESP: mouse y keyboard escriben sobre destinos del programa: los del raton tambien leen lo previo.
	// ENG: mouse and keyboard write over the program's targets: the mouse ones also read the previous.
	int mx = 0, my = 0, lclick = 0, rclick = 0, last_l = -1, last_r = -1;
	int frames_done = 0, resizes = 0;

	for(int frame = 0; frame < FRAMES; ++frame){
		// ESP: yield sin banderin no descansa: dice si el handle sigue en pie y nada mas.
		// Cuando responde false la ventana ya se cerro y el handle apunta a memoria liberada.
		// ENG: yield without the flag rests nothing: it only says if the handle still stands.
		// When it answers false the window already closed and the handle points to freed memory.
		if(!(window::yield(wnd, false))) break;

		// ESP: El usuario puede arrastrar el borde: el tamano se pregunta en cada vuelta, no una vez.
		// ENG: The user can drag the border: the size gets asked on every round, not once.
		int x = 0, y = 0, width = 0, height = 0;
		window::getsize(wnd, &x, &y, &width, &height);

		// ESP: pixels entrega la direccion del buffer del modulo y su cantidad de enteros.
		// Tras cada resize la direccion cambia: hay que volver a pedirla en cada vuelta.
		// Con la ventana minimizada no hay superficie: la funcion deja el destino en nullptr.
		// ENG: pixels hands over the address of the module buffer and its count of integers.
		// After each resize the address changes: it has to be asked for again on every round.
		// With the window minimized there is no surface: the function leaves the target at nullptr.
		unsigned int* pixels = nullptr;
		UWORD pixels_am = 0;
		window::pixels(wnd, &pixels, &pixels_am);
		if(!pixels || width < 1 || height < 1){ sleep(16); continue; }

		// ESP: Escribir en el buffer no dibuja nada por si solo: el vuelco lo pide paint.
		// El color entra como 0x00RRGGBB y cada fila ocupa width enteros.
		// ENG: Writing the buffer draws nothing on its own: the dump gets asked by paint.
		// The color comes in as 0x00RRGGBB and each row takes width integers.
		for(int row = 0; row < height; ++row){
			unsigned int green = (unsigned int)(row * 120 / height);
			unsigned int blue  = (unsigned int)((height - row) * 90 / height);
			unsigned int shift = (unsigned int)(frame % width);
			unsigned int color = (green << 8) | blue;
			unsigned int* dst = pixels + row * width;
			for(int col = 0; col < width; ++col)
				dst[col] = ((col + shift) % width) < 2 ? 0x00FFFFFFu : color;
		}

		// ESP: sendmessage con paint es el unico vuelco manual: clear y draw lo piden solas al terminar.
		// El mensaje viaja el mismo camino del sistema, asi que el mailman lo ve y puede negar el vuelco.
		// ENG: sendmessage with paint is the only manual dump: clear and draw ask for it by themselves on finishing.
		// The message travels the system's same path, so the mailman sees it and can deny the dump.
		window::sendmessage(wnd, window_box::paint, 0, 0);

		// ESP: draw recorta: lo que sale del area se tira y lo que entra se copia.
		// Con el origen pasado de ancho o de alto no copia nada y aun asi responde el handle.
		// ENG: draw clips: what leaves the area gets thrown and what enters gets copied.
		// With the origin past the width or the height nothing copies and yet the handle still answers.
		int off = (frame * 3) % (width + TILE * 2) - TILE;
		window::draw(wnd, off, 24, tile, TILE, TILE);
		if(frame == 60) printf("draw fuera del area: %s\n",
			window::draw(wnd, width + 400, 0, tile, TILE, TILE) ? "aceptado sin efecto" : "rechazado");

		// ESP: El segundo mosaico entra con desplazamiento negativo: se ve solo el trozo que cabe.
		// ENG: The second mosaic comes in with a negative offset: only the piece that fits shows.
		window::draw(wnd, off - TILE / 2, height - TILE - 8, tile, TILE, TILE);

		// ESP: mouse pregunta el cursor relativo a la ventana y el estado de los dos botones.
		// Los destinos de boton son una maquina de estados: leen lo anterior y entregan la transicion.
		// ENG: mouse asks for the cursor relative to the window and the state of the two buttons.
		// The button targets are a state machine: they read the previous and hand over the transition.
		if(window::mouse(wnd, &mx, &my, &lclick, &rclick)){
			if(lclick != last_l){ printf("izquierdo: %s\n", click_name(lclick)); last_l = lclick; }
			if(rclick != last_r){ printf("derecho: %s\n", click_name(rclick)); last_r = rclick; }
		}

		// ESP: keyboard rellena un mapa de bits propio: cada bit es un codigo virtual pulsado.
		// En Windows exige el primer plano: sin foco la llamada devuelve nullptr y no toca el mapa.
		// ENG: keyboard fills its own bit map: each bit is one pressed virtual keycode.
		// On Windows it demands the foreground: without focus the call returns nullptr and leaves the map.
		BYTE keys[32];
		if(window::keyboard(wnd, keys, 32)){
			UWORD pressed = 0;
			for(UWORD byte = 0; byte < 32; ++byte)
				for(int bit = 0; bit < 8; ++bit) if(keys[byte] & (1 << bit)) ++pressed;
			if(pressed) printf("teclas pulsadas: %d\n", (int)pressed);
		}

		// ESP: La casilla del raton se copia encima, con la cruz como centro del dibujo.
		// ENG: The mouse box gets copied over, with the cross as the center of the drawing.
		window::draw(wnd, mx - TILE / 2, my - TILE / 2, box, TILE, TILE);

		// ESP: A mitad del ejemplo la ventana crece: el buffer viejo deja de valer en el mismo instante.
		// resize con SetPosition en false conserva el sitio y solo cambia la medida.
		// ENG: Halfway through the example the window grows: the old buffer stops being valid right then.
		// resize with SetPosition at false keeps the place and changes only the measure.
		if(frame == 120){
			unsigned int* before = pixels;
			window::resize(wnd, x, y, width + 60, height + 30, false, true);
			++resizes;
			window::pixels(wnd, &pixels, &pixels_am);
			printf("bufer %p -> %p tras el agrande\n", (void*)before, (void*)pixels);
		}

		++frames_done;
		sleep(16);
	}

	printf("vueltos dibujadas: %d, agrandes: %d\n", frames_done, resizes);

	// ESP: La variante con BYTE** reparte el buffer: stacked en false usa realloc y manda liberar
	// con memory::free; stacked en true reserva con new y la vuelta siguiente suelta el anterior.
	// ENG: The BYTE** variant hands the buffer out: stacked at false uses realloc and asks to release
	// with memory::free; stacked at true reserves with new and the next round drops the previous one.
	BYTE* big_map = nullptr;
	UWORD map_size = 0;
	if(window::keyboard(wnd, &big_map, &map_size)){
		printf("mapa " UWORD_PRINT " bytes en %p\n", map_size, (void*)big_map);
		big_map = (BYTE*)memory::free(big_map);
	}

	// ESP: wait es el dueño documentado de la liberacion: espera el cierre y suelta el handle.
	// Si el usuario ya cerro durante el bucle, yield respondio false y la ventana sigue pidiendo espera.
	// Despues de wait el puntero no vuelve a usarse: ni para yield, ni para destroy.
	// ENG: wait is the documented owner of the release: it waits for the closing and frees the handle.
	// If the user already closed during the loop, yield answered false and the window still asks waiting.
	// After wait the pointer gets used no more: not for yield, not for destroy.
	window::wait(wnd);
	puts("ventana cerrada / window closed");
	return 0;
}
