/* // ESP
	Ejemplo de la ventana de mensaje, la unica interfaz grafica sin modulo.
	Muestra: message() con las etiquetas de message_box y como se lee su respuesta.
	Compilar: g++ message_dialog.cpp -o message_dialog
	Bloquea hasta que el usuario pulsa un boton. En Linux necesita `zenity`;
	sin esa herramienta instalada la ventana no aparece y la respuesta no es valida.
	// ENG
	Message box example, the only graphical interface needing no module.
	Shows: message() with the message_box tags and how to read its answer.
	Build: g++ message_dialog.cpp -o message_dialog
	It blocks until the user presses a button. On Linux it needs `zenity`;
	without that tool installed the window never shows and the answer is not valid.
*/
#include "System_Tools.h"
using namespace System_Tools;

// ESP: Un 0 o un negativo nunca son una respuesta del usuario: son fallos.
// ENG: A 0 or a negative number is never a user answer: those are failures.
const char* answer(int code){
	switch(code){
		case 1: return "Aceptar / OK";
		case 2: return "Cancelar / Cancel";
		case 3: return "Abortar / Abort";
		case 4: return "Reintentar / Retry";
		case 5: return "Ignorar / Ignore";
		case 6: return "Si / Yes";
		case 7: return "No / No";
	}
	if(code == -1) return "no se resolvio la funcion de sistema / system function unresolved";
	if(code == -2) return "texto o titulo nulos / null text or title";
	return "ventana cerrada sin respuesta / window closed without an answer";
}

int main(){
	// ESP: Un boton de ok, con el icono de informacion.
	// ENG: A single ok button, with the information icon.
	int simple = message("System Tools funciona sin dependencias externas.", "Info",
		message_box::ok | message_box::info);
	printf("caja simple -> %s\n", answer(simple));

	// ESP: Las etiquetas se descomponen por mascara: 0x00F botones, 0x0F0 icono, 0xF00 boton por defecto.
	// ENG: The tags break down by mask: 0x00F buttons, 0x0F0 icon, 0xF00 default button.
	int asking = message("¿Continuar con el ejemplo? / Continue with the example?", "Question",
		message_box::yesno | message_box::question | message_box::defbutton2);
	printf("si/no -> %s\n", answer(asking));
	if(asking != 6){
		printf("el usuario rechazo seguir; se cierra sin abrir la tercera ventana\n");
		return 0;
	}

	// ESP: Tres botones: el boton por defecto elegido debe existir entre los mostrados.
	// ENG: Three buttons: the chosen default button must exist among those shown.
	int risky = message("Se va a escribir en disco. / About to write on disk.", "Warning",
		message_box::abortretryignore | message_box::warning | message_box::defbutton1);
	printf("abortar/reintentar/ignorar -> %s\n", answer(risky));

	return risky;
}
