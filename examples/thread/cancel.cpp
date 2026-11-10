/* // ESP
	Ejemplo de terminacion abrupta de un hilo y del sondeo previo al corte.
	Muestra: thread::create, thread::kill y el uso de thread::yield como chequeo de vida.
	Compilar: g++ cancel.cpp -o cancel
	No hace falta definir ninguna macro ST_IMPORT_*: thread es una funcion base.
	// ENG
	Example of abrupt thread termination and the poll that should precede the cut.
	Shows: thread::create, thread::kill and thread::yield used as a liveness check.
	Build: g++ cancel.cpp -o cancel
	No ST_IMPORT_* macro is needed: thread is a base function.
*/
#include "System_Tools.h"
using namespace System_Tools;

// ESP: Un cuerpo largo y por etapas: el corte abrupta puede llegar en cualquier punto.
// Por eso todo lo acumulado es perdido, aqui el contador nunca llega a escribir su informe.
// volatile obliga a releer la variable: sin ella el compilador puede sostener un valor viejo.
// Por eso al entregar la direccion hace falta el cast (void*): un puntero volatile no es un void*.
// ENG: A long body built in stages: the abrupt cut may land on any point of it.
// That is why everything accumulated is lost, here the counter never reaches its report.
// volatile forces the variable to be re-read: without it the compiler may hold a stale value.
// So handing the address needs the (void*) cast: a volatile pointer is not a void*.
void* worker(void* arg){
	volatile UWORD* ticks = (volatile UWORD*)arg;
	for(UWORD i = 0; i < 1000; ++i){
		sleep(50);
		*ticks = *ticks + 1;
	}
	return nullptr;
}

int main(){
	volatile UWORD ticks = 0;
	void* thr = thread::create(worker, (void*)&ticks);
	if(!thr){ puts("no se pudo crear el hilo / thread could not be created"); return 1; }

	// ESP: Dejar trabajar al hilo un rato antes de cortarlo.
	// ENG: Let the thread work a while before cutting it.
	sleep(400);
	printf("vivo tras 400 ms: %d, ticks = " UWORD_PRINT "\n", thread::yield(thr, false), ticks);

	// ESP: kill devuelve el propio handle ya liberado: verdadero significa "corte hecho".
	// ENG: kill returns the own handle already freed: true means "the cut was done".
	void* cut = thread::kill(thr);
	if(cut) puts("hilo cortado y handle destruido / thread cut and handle destroyed");
	else puts("fallo al cortar / cut failed");
	// ESP: La direccion devuelta no vuelve a usarse como puntero.
	// ENG: The returned address is never used as a pointer again.
	(void)cut;

	UWORD after = ticks;
	sleep(400);
	// ESP: El contador queda congelado: el cuerpo murio junto al hilo.
	// ENG: The counter stays frozen: the body died together with the thread.
	printf("ticks antes = " UWORD_PRINT ", tras 400 ms = " UWORD_PRINT "\n", after, ticks);

	// ESP: kill sobre nullptr o sobre un handle ya gastado devuelve nullptr.
	// Un handle ya liberado no es un handle invalido detectable: es memoria destruida.
	// ENG: kill over nullptr or over a spent handle returns nullptr.
	// A already freed handle is not a detectable invalid handle: it is destroyed memory.
	printf("kill(nullptr) = %p\n", thread::kill(nullptr));

	return 0;
}
