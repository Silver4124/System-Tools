/* // ESP
	Ejemplo de creacion de hilos, espera por sondeo y union.
	Muestra: thread::create, thread::join y thread::yield.
	Compilar: g++ run_join.cpp -o run_join
	No hace falta definir ninguna macro ST_IMPORT_*: thread es una funcion base.
	// ENG
	Example of thread creation, polling wait and join.
	Shows: thread::create, thread::join and thread::yield.
	Build: g++ run_join.cpp -o run_join
	No ST_IMPORT_* macro is needed: thread is a base function.
*/
#include "System_Tools.h"
using namespace System_Tools;

// ESP: La firma del cuerpo es fija: void* nombre(void*).
// El valor devuelto se descarta, el unico canal de salida es la memoria apuntada por Arg.
// volatile obliga a releer esa memoria: sin ella el compilador puede devolver un valor viejo.
// Por eso al entregar la direccion hace falta el cast (void*): un puntero volatile no es un void*.
// ENG: The body signature is fixed: void* name(void*).
// The returned value is thrown away, the only output channel is the memory Arg points to.
// volatile forces that memory to be re-read: without it the compiler may hand back a stale value.
// So handing the address needs the (void*) cast: a volatile pointer is not a void*.
void* counter(void* arg){
	volatile UWORD* total = (volatile UWORD*)arg;
	for(UWORD i = 0; i < 5; ++i){
		sleep(60);
		*total = *total + 1;
	}
	return nullptr;
}

int main(){
	// ESP: create devuelve nullptr si el cuerpo es nulo o si el sistema rechaza el hilo.
	// ENG: create answers nullptr if the body is null or the system refuses the thread.
	volatile UWORD total = 0;
	void* thr = thread::create(counter, (void*)&total);
	if(!thr){ puts("no se pudo crear el hilo / thread could not be created"); return 1; }

	// ESP: yield con _wait en false pregunta sin dormir: sirve para sondear.
	// Ojo: el propio yield destruye el handle cuando descubre el fin, ese false agota el handle.
	// ENG: yield with _wait as false asks without sleeping: it works as a poll.
	// Note: yield itself destroys the handle when it finds the end, that false spends the handle.
	printf("sondeando");
	while(thread::yield(thr, false)){
		printf(".");
		sleep(100);
	}
	// ESP: Desde aqui thr es memoria liberada: ni join, ni kill, ni otro yield.
	// ENG: From here thr is freed memory: no join, no kill, no other yield.
	printf("\ntotal = " UWORD_PRINT " (5 esperado)\n", total);

	// ESP: join bloquea hasta el fin y también destruye el handle.
	// Su retorno no nulo afirma que el hilo fue destruido, no que el puntero sigue sirviendo.
	// ENG: join blocks until the end and also destroys the handle.
	// Its non null return asserts the thread was destroyed, not that the pointer still works.
	volatile UWORD total2 = 0;
	void* thr2 = thread::create(counter, (void*)&total2);
	void* joined = thread::join(thr2);
	if(joined) puts("hilo unido y handle destruido / thread joined and handle destroyed");
	printf("total2 = " UWORD_PRINT " (5 esperado)\n", total2);
	// ESP: joined y thr2 son la misma direccion ya invalida: nunca se vuelve a usar.
	// ENG: joined and thr2 are the same already invalid address: never used again.
	(void)joined;

	// ESP: yield con _wait en true duerme unos 10 ms por vuelta, incluso en la del fin.
	// ENG: yield with _wait as true sleeps about 10 ms per round, even on the ending one.
	volatile UWORD total3 = 0;
	void* thr3 = thread::create(counter, (void*)&total3);
	while(thread::yield(thr3)) sleep(50);
	printf("total3 tras yield bloqueante = " UWORD_PRINT "\n", total3);

	// ESP: Un handle nullptr o de otro modulo no es handle: yield responde false y join nullptr.
	// ENG: A nullptr handle or one from another module is not a handle: yield answers false and join nullptr.
	printf("yield(nullptr) = %d, join(nullptr) = %p\n", thread::yield(nullptr, false), thread::join(nullptr));

	return 0;
}
