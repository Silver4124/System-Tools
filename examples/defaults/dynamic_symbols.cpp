/* // ESP
	Ejemplo del corazon de la libreria: resolver simbolos del sistema en tiempo de
	ejecucion, sin enlazar ninguna biblioteca.
	Muestra: get_system_function con y sin libreria destino, get_system_variable
	para leer una variable de entorno, y el modo de llamar al puntero obtenido.
	Compilar: g++ dynamic_symbols.cpp -o dynamic_symbols
	El tipo de funcion declarado debe coincidir con la firma real del sistema y con
	su convencion de llamada (__stdcall en Windows de 32 bits); un prototipo
	equivocado corrompe la pila y produce un fallo en tiempo de ejecucion, no en
	compilacion.
	// ENG
	Example of the library core: resolving system symbols at runtime, with no
	library linked in at all.
	Shows: get_system_function with and without a target library, get_system_variable
	reading an environment entry, and how to call the pointer you obtained.
	Build: g++ dynamic_symbols.cpp -o dynamic_symbols
	The declared function type must match the real system signature
	and its calling convention (__stdcall on 32-bit Windows); a wrong
	prototype corrupts the stack and fails at run time, not at
	compile time.
*/
#include "System_Tools.h"
using namespace System_Tools;

// ESP: La convencion de llamada de kernel32 es __stdcall en Windows de 32 bits.
// WINCALL es una macro interna de la libreria, no la exporta el encabezado.
// ENG: The kernel32 calling convention is __stdcall on 32-bit Windows.
// WINCALL is an internal macro of the library; the header does not export it.
#if defined(_WIN32) && !defined(_WIN64)
	#define ST_STDCALL __stdcall
#else
	#define ST_STDCALL
#endif

int main(){
	// ESP: Sin libreria destino, Windows busca en kernel32.dll y Linux en libc.
	// ENG: With no target library, Windows searches kernel32.dll and Linux searches libc.
	#if defined(_WIN32)
		typedef unsigned int (ST_STDCALL *tick_t)();
		tick_t uptime = (tick_t)get_system_function("GetTickCount");
		const char* from = "kernel32.dll!GetTickCount";
		unsigned int ms = uptime ? uptime() : 0;
		printf("%s -> " UWORD_PRINT " ms de sistema\n", from, (UWORD)ms);
	#else
		typedef int (*clock_now_t)(int, void*);
		struct { long sec; long nsec; } ts;
		clock_now_t now = (clock_now_t)get_system_function("clock_gettime");
		const char* from = "libc!clock_gettime";
		long seconds = (now && now(0, &ts) == 0) ? ts.sec : 0;
		printf("%s -> " UWORD_PRINT " segundos\n", from, (UWORD)seconds);
	#endif

	// ESP: Un fallo se nota siempre igual: el puntero viene en nullptr.
	// ENG: A failure always shows the same way: the pointer comes back as nullptr.
	void* missing = get_system_function("funcion_que_no_existe_42");
	printf("simbolo inexistente -> %p\n", missing);
	if(!missing) puts("no se cargo nada, y nada se ejecuto");

	// ESP: Pedir una libreria concreta tambien sirve: user32.dll solo existe en Windows.
	// ENG: Asking for a concrete library works too: user32.dll only exists on Windows.
	#if defined(_WIN32)
		typedef int (ST_STDCALL *msgbox_t)(void*, const char*, const char*, unsigned int);
		msgbox_t sys_beep = (msgbox_t)get_system_function("MessageBoxTimeoutA", "user32.dll");
		printf("MessageBoxTimeoutA en user32.dll -> %p\n", (void*)sys_beep);
	#endif

	// ESP: Sin libreria, get_system_variable busca en el entorno del proceso.
	// La direccion devuelta apunta a la tabla del sistema: se lee, no se libera.
	// ENG: Without a library, get_system_variable searches the process environment.
	// The returned address points into the system table: read it, never release it.
	char* temp_dir = (char*)get_system_variable("TEMP");
	if(!temp_dir) temp_dir = (char*)get_system_variable("TMP");
	if(!temp_dir) temp_dir = (char*)get_system_variable("HOME");
	if(temp_dir) printf("carpeta temporal -> %s\n", temp_dir);
	else puts("el entorno no expone ninguna carpeta temporal conocida");

	// ESP: Una variable protegida entrega puntero igualmente: escribir en ella es tu riesgo.
	// ENG: A protected variable still hands over a pointer: writing into it is your risk.
	void* readonly_probe = get_system_variable("WINDIR");
	printf("WINDIR -> %p\n", readonly_probe);

	return missing != nullptr;
}
