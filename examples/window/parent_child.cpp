/* // ESP
	Ejemplo del vinculo entre ventanas: padre, hijo y la regla de quien arrastra a quien.
	Muestra: window::create con padre, window::father, window::is_extern, window::resize,
	window::clear, window::retitle, window::getsize, window::yield, window::destroy y window::wait.
	Compilar: g++ parent_child.cpp -o parent_child
	Requiere: #define ST_IMPORT_WINDOW antes de incluir la cabecera y una pantalla donde abrir la ventana.
	// ENG
	Example of the link between windows: father, child and the rule of who drags whom.
	Shows: window::create with father, window::father, window::is_extern, window::resize,
	window::clear, window::retitle, window::getsize, window::yield, window::destroy and window::wait.
	Build: g++ parent_child.cpp -o parent_child
	Needs: #define ST_IMPORT_WINDOW before including the header and a screen where to open the window.
*/
#define ST_IMPORT_WINDOW
#define ST_IMPORT_CONSOLE
#include "System_Tools.h"
using namespace System_Tools;

// ESP: Descansar vueltas mientras el handle siga vivo: es la forma de esperar sin bloquearse.
// ENG: Resting rounds while the handle still lives: it is the way to wait without blocking.
static void pump(void* wnd, int rounds){
	for(int i = 0; i < rounds && window::yield(wnd, true); ++i);
}

static void geometry(const char* tag, void* wnd){
	int x = 0, y = 0, width = 0, height = 0;
	window::getsize(wnd, &x, &y, &width, &height);
	printf("%-8s en (%d, %d) de %d x %d\n", tag, x, y, width, height);
}

int main(){
	// ESP: create abre las ventanas en el mismo sitio por defecto: hay que separarlas a mano.
	// El padre entra como primer argumento y vale nullptr para una ventana de primer nivel.
	// ENG: create opens the windows at the same place by default: they have to be split by hand.
	// The father comes in as the first argument and nullptr serves for a first level window.
	void* father = window::create(nullptr, "padre", 300, 200);
	if(!father){ puts("sin padre / no father"); return 1; }
	window::resize(father, 60, 60, 300, 200, true, true);
	window::clear(father, 0x00603010);

	// ESP: El hijo nace atado al handle del padre: el modulo no usa el ventana hija del sistema,
	// solo guarda la direccion del padre para saber cuando tiene que morir el hilo del hijo.
	// Por eso el hijo no se recorta contra el padre ni se mueve con el: solo le sigue la vida.
	// ENG: The child is born tied to the father's handle: the module uses no system child window,
	// it only stores the father's address to know when the child's thread has to die.
	// That is why the child gets no clipping against the father nor moves with him: it only follows his life.
	void* child = window::create(father, "hijo", 160, 120);
	if(!child){ puts("sin hijo / no child"); return 1; }
	window::resize(child, 380, 80, 160, 120, true, true);
	window::clear(child, 0x00106030);

	// ESP: El nieto hereda el vinculo del hijo: la cadena entera cae cuando cae la cabecera.
	// ENG: The grandchild inherits the link from the child: the whole chain falls when the head falls.
	void* grand = window::create(child, "nieto", 120, 90);
	if(grand){
		window::resize(grand, 380, 230, 120, 90, true, true);
		window::clear(grand, 0x00301060);
	}

	// ESP: father responde la direccion exacta del handle padre, no una copia ni un numero de ventana.
	// En una de primer nivel la respuesta es nullptr y en ninguna de las tres hay externa.
	// ENG: father answers the exact address of the father's handle, not a copy nor a window number.
	// On a first level one the answer is nullptr and in none of the three is there an external.
	printf("padre del hijo es el padre: %d\n", (int)(window::father(child) == father));
	printf("padre del nieto es el hijo: %d\n", (int)(window::father(grand) == child));
	printf("padre del padre: %p\n", window::father(father));
	printf("externas: %d %d %d\n",
		(int)window::is_extern(father), (int)window::is_extern(child), (int)window::is_extern(grand));

	// ESP: El handle de la consola tambien vale como padre: create lo traduce a la ventana del terminal.
	// La conversion entrega un handle de ventana incrustado en la consola, no la direccion de la consola.
	// Como la consola vive todo el programa, esta ventana no cae sola: hay que cerrarla a mano.
	// ENG: The console's handle serves as father too: create translates it to the terminal's window.
	// The translation hands over a window handle embedded in the console, not the console's address.
	// Since the console lives the whole program, this window falls no one: it has to be shut by hand.
	void* console = console::get_handle();
	void* on_top = window::create(console, "sobre la consola", 200, 100);
	if(on_top){
		// ESP: La comparacion contra la consola falla a proposito: el padre guardado ya es la ventana convertida.
		// ENG: The compare against the console fails on purpose: the father stored is already the turned window.
		void* cnsl_wnd = window::father(on_top);
		printf("padre terminal igual a la consola: %d\n", (int)(cnsl_wnd == console));
		printf("padre terminal: %p, consola: %p\n", cnsl_wnd, console);
		geometry("terminal", cnsl_wnd);
		window::retitle(on_top, "hija de la consola");
		pump(on_top, 30);
		window::destroy(on_top);
	}

	geometry("padre", father);
	geometry("hijo", child);

	// ESP: Matar al padre arrastra al hijo y, con el, al nieto: los hilos ven el magic muerto y salen.
	// destroy libera el handle del padre y el hilo del hijo se borra a si mismo: self_destroy esta en true.
	// Queda pendiente quien libera el handle del hijo: wait responde nullptr si el hilo ya se lo comio.
	// ENG: Killing the father drags the child and, with him, the grandchild: the threads see the magic
	// dead and leave. destroy frees the father's handle and the child's thread erases itself: self_destroy
	// sits at true. Left pending is who frees the child's handle: wait answers nullptr if the thread ate it.
	window::destroy(father);

	int waited = 0;
	while(window::yield(grand, true) && waited < 200){ ++waited; }
	printf("el nieto se apago en %d vueltas, vivo: %d\n", waited, (int)window::yield(grand, false));

	waited = 0;
	while(window::yield(child, true) && waited < 200){ ++waited; }
	printf("el hijo se apago en %d vueltas, vivo: %d\n", waited, (int)window::yield(child, false));

	// ESP: wait es la salida documentada de la liberacion: si el handle ya murio responde nullptr sin tocar nada.
	// El orden importa: primero el nieto, luego el hijo, para que nadie mire un padre ya liberado.
	// ENG: wait is the documented exit of the release: if the handle already died it answers nullptr untouched.
	// The order matters: first the grandchild, then the child, so no one looks at an already freed father.
	window::wait(grand);
	window::wait(child);
	puts("fin / done");
	return 0;
}
