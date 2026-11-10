/* // ESP
	Ejemplo de orden de terminal, espera y limpieza de pantalla.
	Muestra: system(), sleep() y clear().
	Compilar: g++ run_system.cpp -o run_system
	`clear` borra la terminal entera, asi que aqui solo se invoca si pasas
	un argumento: `./run_system limpio`.
	// ENG
	Terminal command, waiting and screen clearing example.
	Shows: system(), sleep() and clear().
	Build: g++ run_system.cpp -o run_system
	`clear` wipes the whole terminal, so it only runs here when you pass an
	argument: `./run_system limpio`.
*/
#include "System_Tools.h"
using namespace System_Tools;

int main(int argc, char** argv){
	// ESP: El comando corre en el interprete de la plataforma: cmd en Windows, sh en Linux.
	// ENG: The command runs in the platform interpreter: cmd on Windows, sh on Linux.
	// ESP: "echo" existe en los dos, asi que el ejemplo sigue siendo un unico fuente portable.
	// ENG: "echo" exists in both, so the example stays a single portable source.
	int code = system("echo hola desde system");
	printf("system devolvio el codigo %d\n", code);

	// ESP: Un codigo distinto de cero viene del comando, no de la libreria.
	// ENG: A non-zero code comes from the command, not from the library.
	int failing = system("this_command_should_not_exist_42");
	printf("comando inexistente -> %d\n", failing);

	// ESP: sleep bloquea el hilo que lo invoca, en milisegundos.
	// ENG: sleep blocks the calling thread, in milliseconds.
	printf("esperando 300 ms...");
	sleep(300);
	printf(" listo\n");

	// ESP: La espera nunca sale mas corta de lo pedido, pero un sistema ocupado la alarga.
	// ENG: The wait never comes out shorter than asked, though a busy system can lengthen it.
	UWORD start_counter = 0;
	for(unsigned int i = 0; i < 3; ++i){
		sleep(100);
		printf("tic " UWORD_PRINT "\n", ++start_counter);
	}

	// ESP: clear solo si el programador lo pide expresamente en la linea de comandos.
	// ENG: clear only when the programmer asks for it on the command line.
	if(argc > 1){
		puts("se limpia la pantalla / clearing the screen");
		clear();
	}
	return code;
}
