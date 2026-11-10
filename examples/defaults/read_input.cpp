/* // ESP
	Ejemplo de lectura formateada por consola.
	Muestra: sscanf sobre una cadena ya en memoria y scanf sobre lo que
	escribe el usuario, con limites de ancho para no desbordar nunca.
	Compilar: g++ read_input.cpp -o read_input
	scanf y sscanf reenvian el va_list a la runtime C del sistema.
	// ENG
	Formatted console input example.
	Shows: sscanf over a string already in memory and scanf over what the
	user types, using width limits so nothing can ever overflow.
	Build: g++ read_input.cpp -o read_input
	scanf and sscanf forward the va_list to the system C runtime.
*/
#include "System_Tools.h"
using namespace System_Tools;

int main(){
	// ESP: sscanf no bloquea: lee de una cadena que ya tienes.
	// ENG: sscanf never blocks: it reads from a string you already hold.
	const char* record = "ada 36 1.5";
	char name[16];
	int age = 0;
	float height = 0.0f;

	// ESP: El ancho (%15s) es el maximo de caracteres leidos, sin contar el terminador.
	// ENG: The width (%15s) is the maximum characters read, not counting the terminator.
	int done = sscanf(record, "%15s %d %f", name, &age, &height);
	printf("sscanf asigno %d valores: %s, %d, %.2f\n", done, name, age, height);

	// ESP: El retorno cuenta conversiones asignadas, no caracteres consumidos.
	// ENG: The return counts assigned conversions, not consumed characters.
	int partial = sscanf("solo_texto 99", "%d", &age);
	printf("sscanf sobre un entero inexistente devolvio %d y no toco age\n", partial);

	// ESP: scanf lee de la consola y bloquea hasta que pulsas Enter.
	// ENG: scanf reads from the console and blocks until you press Enter.
	// ESP: La libreria no expone flush: en algunas runtime el prompt aparece detras
	// de la linea leida, y eso no es un fallo del ejemplo.
	// ENG: The library exposes no flush: on some runtimes the prompt shows up after
	// the line you typed, and that is not a bug in this example.
	printf("Escribe un numero y una palabra / type a number and a word: ");
	int number = 0;
	char word[32];
	done = scanf("%d %31s", &number, word);
	printf("scanf asigno %d valores: %d y %s\n", done, number, word);

	return 0;
}
