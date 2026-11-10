/* // ESP
	Ejemplo del tour tranquilo por una ventana: geometria, titulo, color y foco.
	Muestra: window::create, window::getscreen, window::getsize, window::resize, window::clear,
	window::pixels, window::retitle, window::gettitle, window::show, window::focus,
	window::active, window::is_extern, window::release, window::father, window::yield y window::destroy.
	Compilar: g++ window_basics.cpp -o window_basics
	Requiere: #define ST_IMPORT_WINDOW antes de incluir la cabecera.
	// ENG
	Example of the quiet tour through a window: geometry, title, color and focus.
	Shows: window::create, window::getscreen, window::getsize, window::resize, window::clear,
	window::pixels, window::retitle, window::gettitle, window::show, window::focus,
	window::active, window::is_extern, window::release, window::father, window::yield and window::destroy.
	Build: g++ window_basics.cpp -o window_basics
	Needs: #define ST_IMPORT_WINDOW before including the header.
*/
#define ST_IMPORT_WINDOW
#include "System_Tools.h"
using namespace System_Tools;

int main(){
	// ESP: getscreen pregunta la pantalla sin necesitar ventana alguna: abre y cierra su propio contexto.
	// ENG: getscreen asks the screen with no window needed at all: it opens and shuts its own context.
	int screen_w = 0, screen_h = 0;
	window::getscreen(&screen_w, &screen_h);
	printf("pantalla: %d x %d\n", screen_w, screen_h);

	// ESP: create pide el ancho y el alto del area dibujable, no del marco del ventana.
	// El mailman se puede omitir: el por defecto no guarda nada y deja hacer al sistema.
	// ENG: create asks the width and the height of the drawable area, not of the window frame.
	// The mailman can be skipped: the default one stores nothing and lets the system work.
	void* wnd = window::create(nullptr, "basics", 320, 200);
	if(!wnd){ puts("sin ventana / no window"); return 1; }

	int x = 0, y = 0, width = 0, height = 0;
	window::getsize(wnd, &x, &y, &width, &height);
	printf("creada: %d x %d en (%d, %d)\n", width, height, x, y);

	// ESP: clear colorea el area entera y vuelca a pantalla ella sola: no hace falta paint extra.
	// El color entra como 0x00RRGGBB: el alfa del nibble alto se ignora.
	// ENG: clear colors the whole area and dumps to screen by itself: no extra paint is needed.
	// The color comes in as 0x00RRGGBB: the alpha of the high nibble gets ignored.
	window::clear(wnd, 0x00305080);

	// ESP: pixels entrega la direccion del bufer interno y su cantidad: es memoria del modulo.
	// Tras un resize la direccion puede cambiar: hay que volver a pedirla.
	// ENG: pixels hands over the address of the internal buffer and its amount: it is module memory.
	// After a resize the address can change: it has to be asked for again.
	unsigned int* pixels = nullptr;
	UWORD pixels_am = 0;
	window::pixels(wnd, &pixels, &pixels_am);
	printf("bufer: %p de " UWORD_PRINT " pixeles\n", (void*)pixels, pixels_am);

	// ESP: resize mueve y agranda segun los dos banderines: con ambos en false la llamada no hace nada.
	// Pedir un alto o un ancho de cero es rechazar: eso es una ventana minimizada, no un tamano.
	// ENG: resize moves and grows according to the two flags: with both at false the call does nothing.
	// Asking a zero height or width gets refused: that is a minimized window, not a size.
	if(window::resize(wnd, 40, 40, 480, 300, true, true)){
		window::getsize(wnd, &x, &y, &width, &height);
		window::pixels(wnd, &pixels, &pixels_am);
		printf("tras resize: %d x %d en (%d, %d), bufer %p de " UWORD_PRINT "\n",
			width, height, x, y, (void*)pixels, pixels_am);
	}

	// ESP: La superficie nueva llega en cero: hay que verter otra vez si se quiere el color viejo.
	// ENG: The new surface arrives in zero: the dump has to run again to want the old color.
	window::clear(wnd, 0x00805030);

	// ESP: gettitle reserva el texto con new cada llamada y libera el anterior: el ultimo es del programa.
	// La minimizada llega en el mismo paquete: es la que el sistema anoto al ocultar.
	// ENG: gettitle reserves the text with new each call and frees the previous: the last one is the program's.
	// The minimized flag arrives in the same package: the one the system wrote when hiding.
	char* title = nullptr;
	bool minimized = false;
	window::retitle(wnd, "basics redibujada");
	if(window::gettitle(wnd, &title, &minimized)) printf("titulo: [%s] minimizada %d\n", title, (int)minimized);

	// ESP: show con visible en false esconde la ventana sin cerrarla: yield sigue respondiendo viva.
	// El banderin de minimizar solo cuenta con visible en true: ocultar y minimizar son dos cosas.
	// ENG: show with visible at false hides the window without closing it: yield keeps answering alive.
	// The minimize flag only counts with visible at true: hiding and minimizing are two different things.
	window::show(wnd, false);
	printf("escondida, viva: %d\n", (int)window::yield(wnd));
	window::show(wnd, true, true);
	window::gettitle(wnd, &title, &minimized);
	printf("minimizada por el programa: %d\n", (int)minimized);
	window::show(wnd, true, false);

	// ESP: focus pide el primer plano y no comprueba el resultado: la espera de verdad es focuswait.
	// yield no mira el foco: solo dice si el handle sigue en pie y, con el banderin puesto, descansa 10 ms.
	// ENG: focus asks for the foreground and checks no result: the real waiting is focuswait.
	// yield looks at no focus: it only says if the handle still stands and, with the flag set, rests 10 ms.
	window::focus(wnd);
	printf("viva tras el foco: %d\n", (int)window::yield(wnd, true));

	// ESP: active entrega la ventana del primer plano, propia o de otra aplicacion, marcada como externa.
	// Un handle externo no admite dibujo y su ciclo de vida no es del modulo: la ventana ajena sigue a lo suyo.
	// destroy o wait le piden el cierre a la ventana ajena; release suelta solo el handle, que es lo que aqui hace falta.
	// ENG: active hands over the foreground window, its own or another application's, flagged as external.
	// An external handle takes no drawing and its life cycle is not the module's: the foreign window goes on with its business.
	// destroy or wait ask the closing to the foreign window; release lets go of the handle only, which is what is needed here.
	void* act = window::active();
	if(act){
		printf("activa es externa: %d, es la nuestra: %d\n",
			(int)window::is_extern(act), (int)(act == wnd));
		// ESP: getsize sobre una externa deja los destinos intactos: el modulo no conoce su geometria.
		// ENG: getsize over an external one leaves the targets intact: the module knows no geometry of it.
		int probe = 7;
		window::getsize(act, &probe, nullptr, nullptr, nullptr);
		printf("geometria de la externa: el probe sigue en %d\n", probe);
		// ESP: release rechaza el handle propio en silencio: ese lo destruye su hilo de eventos, nunca el programa.
		// ENG: release refuses one's own handle in silence: that one is destroyed by its event thread, never by the program.
		if(window::is_extern(act)) act = window::release(act);
	}

	// ESP: father responde nullptr en una ventana de primer nivel: no hay dueeno que la contenga.
	// ENG: father answers nullptr on a first level window: there is no owner holding it.
	printf("padre: %p, propio: %d\n", window::father(wnd), (int)window::is_extern(wnd));

	// ESP: Las consultas devuelven nullptr cuando no se pide nada: todos los punteros nulos es un rechazo.
	// ENG: The queries answer nullptr when nothing gets asked: every pointer null is a refusal.
	printf("raton sin punteros: %p\n", window::mouse(wnd, nullptr, nullptr, nullptr, nullptr));

	// ESP: destroy pide el cierre y espera al hilo del evento: despues el puntero ya no sirve para nada.
	// wait hace de espera sin pedir el cierre, para quien prefiera que el usuario sea quien la apague.
	// ENG: destroy asks for the closing and waits for the event thread: afterwards the pointer is no good for anything.
	// wait serves as the waiting without asking for the closing, for whoever prefers the user to be the one switching it off.
	window::destroy(wnd);
	printf("viva tras destroy: %d\n", (int)window::yield(wnd));
	if(title) delete[] title;
	puts("fin / done");
	return 0;
}
