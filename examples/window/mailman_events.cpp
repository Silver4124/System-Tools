/* // ESP
	Ejemplo del mensajero propio: quien recibe y filtra los eventos de una ventana.
	Muestra: window::create con mailman, window::defMailMan, window_box::accepted, window::sendmessage,
	window::yield, window::getsize, window::paint, window::close y window::wait.
	Compilar: g++ mailman_events.cpp -o mailman_events
	Requiere: #define ST_IMPORT_WINDOW antes de incluir la cabecera y una pantalla donde abrir la ventana.
	// ENG
	Example of the custom mailman: the one that receives and filters a window's events.
	Shows: window::create with mailman, window::defMailMan, window_box::accepted, window::sendmessage,
	window::yield, window::getsize, window::paint, window::close and window::wait.
	Build: g++ mailman_events.cpp -o mailman_events
	Needs: #define ST_IMPORT_WINDOW before including the header and a screen where to open the window.
*/
#define ST_IMPORT_WINDOW
#include "System_Tools.h"
using namespace System_Tools;

#define TRACKED 15

static const UWORD tracked[TRACKED] = {
	window_box::destroy, window_box::size, window_box::activate, window_box::paint, window_box::close,
	window_box::quit, window_box::key_down, window_box::key_up, window_box::mouse_move,
	window_box::lbutton_down, window_box::lbutton_up, window_box::lbutton_hold,
	window_box::rbutton_down, window_box::rbutton_up, window_box::rbutton_hold
};
static const char* tracked_name[TRACKED] = {
	"destroy", "size", "activate", "paint", "close", "quit", "key_down", "key_up", "mouse_move",
	"lbutton_down", "lbutton_up", "lbutton_hold", "rbutton_down", "rbutton_up", "rbutton_hold"
};
static UWORD hits[TRACKED] = {0};
static UWORD untracked = 0;
static int veto_left = 2;

static void unpack(WORD packed, int* low, int* high){
	// ESP: El empaquetado de 16 bits solo aguanta medidas positivas: lo negativo sale como numero enorme.
	// ENG: The 16 bit packing only holds positive measures: the negative comes out as an enormous number.
	*low  = (int)(packed & 0xFFFF);
	*high = (int)((packed >> 16) & 0xFFFF);
}

static UWORD mailman_events(void* wnd, UWORD msg, UWORD wp, WORD lp){
	// ESP: El mensajero corre en el hilo de la ventana, no en el del programa: escribir en consola
	// desde aqui se mezcla con lo que main imprime en el mismo instante.
	// ENG: The mailman runs on the window's thread, not on the program's: writing to the console
	// from here gets mixed with whatever main prints at the same moment.
	int slot = -1;
	for(int i = 0; i < TRACKED; ++i) if(tracked[i] == msg) slot = i;
	if(slot >= 0) ++hits[slot]; else ++untracked;

	switch(msg){
		case window_box::size: {
			int low = 0, high = 0, wide = 0, tall = 0;
			unpack((WORD)wp, &low, &high);
			unpack(lp, &wide, &tall);
			// ESP: En Windows el wParam del agrande es la bandera de restaurada o minimizada, no la posicion;
			// en Linux llega alli la x y la y empaquetadas. El lp si que es igual en los dos: ancho y alto.
			// ENG: On Windows the wParam of the resize is the restored or minimized flag, not the position;
			// on Linux the packed x and y arrive there. The lp is the same on both: width and height.
			printf("[size] wp=(%d, %d) lp=(%d x %d)\n", low, high, wide, tall);
			break;
		}
		case window_box::mouse_move: {
			int x = 0, y = 0;
			unpack(lp, &x, &y);
			printf("[move] (%d, %d)\n", x, y);
			break;
		}
		case window_box::key_down: {
			printf("[key] codigo " UWORD_PRINT "\n", wp);
			// ESP: ESC cierra desde aqui: 27 en Windows, 9 en el teclado de X. Los dos se aceptan.
			// Consumir el mensaje evita que el sistema lo siga procesando despues del rechazo.
			// ENG: ESC closes from here: 27 on Windows, 9 on the X keyboard. Both get accepted.
			// Consuming the message keeps the system from processing it further after the refusal.
			if(wp == 27 || wp == 9){
				printf("[key] ESC consumido, se pide el cierre\n");
				window::sendmessage(wnd, window_box::close, 0, 0);
				return window_box::accepted;
			}
			break;
		}
		case window_box::close: {
			// ESP: Devolver accepted sobre close solo veta en Windows: alli la llamada al mensajero
			// se hace antes de destruir la ventana. En Linux el cierre del administrador rompe el
			// bucle sea cual sea la respuesta, asi que el veto queda en nada.
			// ENG: Returning accepted over close only vetoes on Windows: there the call to the mailman
			// runs before destroying the window. On Linux the manager's closing breaks the loop
			// whatever the answer is, so the veto comes to nothing.
			if(veto_left > 0){
				--veto_left;
				printf("[close] vetado, quedan %d\n", veto_left);
				return window_box::accepted;
			}
			printf("[close] aceptado\n");
			break;
		}
		default: break;
	}

	// ESP: El resto se delega: defMailMan es el mensajero que create usa por defecto y siempre responde def.
	// Pintar no se consume nunca: accepted sobre paint se salta el vuelco del buffer y la ventana queda vacia.
	// ENG: The rest gets delegated: defMailMan is the mailman create uses by default and always answers def.
	// Painting never gets consumed: accepted over paint skips the buffer's dump and the window stays empty.
	return window::defMailMan(wnd, msg, wp, lp);
}

int main(){
	void* wnd = window::create(nullptr, "mensajero", 300, 200, mailman_events);
	if(!wnd){ puts("sin ventana / no window"); return 1; }

	// ESP: Los mensajes tambien se pueden inyectar desde el programa: sendmessage reproduce el camino
	// del sistema y el mensajero los ve llegar igual que si vinieran del usuario.
	// En Linux los ids close, destroy y quit se traducen todos al mismo cierre del administrador.
	// ENG: The messages can also get injected by the program: sendmessage replays the system's path
	// and the mailman sees them arrive just as if they came from the user.
	// On Linux the ids close, destroy and quit all get translated to the same manager's closing.
	window::sendmessage(wnd, window_box::size, 0, (WORD)((240 << 16) | 320));
	window::sendmessage(wnd, window_box::mouse_move, 0, (WORD)((50 << 16) | 70));
	window::sendmessage(wnd, window_box::key_down, 'a', 0);
	window::sendmessage(wnd, window_box::paint, 0, 0);
	window::sendmessage(wnd, window_box::close, 0, 0);

	// ESP: La espera activa: yield descansa y avisa cuando el handle ya no sirve.
	// Aqui el bucle dura un rato corto para que el ejemplo termine solo sin tocar nada.
	// ENG: The waiting on hold: yield rests and warns when the pointer no longer serves.
	// Here the loop lasts a short while so the example ends on its own without touching anything.
	for(int round = 0; round < 300 && window::yield(wnd, true); ++round);

	int x = 0, y = 0, width = 0, height = 0;
	window::getsize(wnd, &x, &y, &width, &height);
	printf("geometria final: %d x %d\n", width, height);

	// ESP: El recuento del mensajero, vivido o inyectado, junto a los ids que no entran en la lista.
	// En Windows asoman codigos propios del sistema que el modulo no traduce nunca.
	// ENG: The mailman's count, lived or injected, next to the ids outside the list.
	// On Windows the system's own codes show up that the module never translates.
	for(int i = 0; i < TRACKED; ++i) if(hits[i]) printf("%-13s %d\n", tracked_name[i], (int)hits[i]);
	printf("sin catalogar: " UWORD_PRINT "\n", untracked);

	// ESP: Queda un veto pendiente de gastar: destroy lo gasta y luego ya puede cerrar.
	// Si el veto fuese definitivo, destroy y wait se quedarian esperando un cierre que nunca llega.
	// ENG: One veto is left unspent: destroy spends it and only then can it close.
	// Were the veto definitive, destroy and wait would stay waiting for a closing that never arrives.
	veto_left = 0;
	window::destroy(wnd);
	puts("fin / done");
	return 0;
}
