/* // ESP
	Ejemplo de las variantes funcionales de message: las firmas que no estan declaradas
	en el encabezado pero si definidas en el modulo, y lo que cada una realmente hace.
	Muestra: message de texto ancho, message con const void* en cada lado, message con
	nullptr como texto o como titulo, y el valor por defecto del tipo de ventana.
	Compilar: g++ message_variants.cpp -o message_variants
	Requiere: nada adicional, message vive en las funciones por defecto.
	Este ejemplo abre ventanas modales y espera al usuario: se compueba compilando y se
	ejecuta cuando el usuario quiera verlas.
	Las mezclas de un lado ancho con un literal del otro NO compilan: son ambiguas.
	// ENG
	Example of the functional variants of message: the signatures that are not declared
	in the header but are defined in the module, and what each one really does.
	Shows: message over wide text, message with const void* on either side, message with
	nullptr as the text or as the title, and the default value of the window type.
	Build: g++ message_variants.cpp -o message_variants
	Needs: nothing extra, message lives in the default functions.
	This example opens modal windows and waits for the user: it gets checked by compiling
	and runs when the user wants to see them.
	The mixtures of one wide side with a literal on the other do NOT compile: they are ambiguous.
*/
#include "System_Tools.h"
using namespace System_Tools;

int main(){
	// ESP: La firma del encabezado: los dos lados de bytes y el tipo con valor por defecto.
	// El retorno es la respuesta del usuario: 1 aceptar, 2 cancelar, 6 si, 7 no.
	// ENG: The header signature: both sides in bytes and the type with its default value.
	// The return is the user's answer: 1 ok, 2 cancel, 6 yes, 7 no.
	int r = message("texto de bytes, ventana de un boton", "base", message_box::ok);
	printf("base bytes: %d\n", r);

	// ESP: Sin tipo: el defecto trae la ventana de un solo boton, igual que escribir message_box::ok.
	// ENG: Without a type: the default brings the one-button window, same as writing message_box::ok.
	r = message("sin tercer argumento", "defecto");
	printf("defecto de tipo: %d\n", r);

	// ESP: La ruta ancha pide los DOS lados como wchar*: va directo al MessageBoxW del sistema.
	// Con caracteres fuera del ASCII la escritura sale limpia sin tocar ninguna pagina de codigo.
	// Aca se escriben con escape universal para que el fichero fuente siga siendo puro de ASCII.
	// ENG: The wide road asks BOTH sides as wchar*: it goes straight to the system MessageBoxW.
	// With characters outside ASCII the writing comes out clean without touching any code page.
	// Here they get written with the universal escape so the source file stays pure ASCII.
	r = message(L"texto ancho con \u00f1 y \u00e1", L"titulo ancho", message_box::yesno);
	printf("ancho si/no: %d\n", r);

	// ESP: const void* en el texto con el titulo ancho: la eleccion cae en la firma (void*, wchar*).
	// Esa firma reinterpreta el void* como wchar*, asi que el texto llega bien: el cast no estropea nada.
	// ENG: const void* on the text with the title wide: the pick lands on the (void*, wchar*) signature.
	// That signature reinterprets the void* as wchar*, so the text arrives fine: the cast spoils nothing.
	const wchar* wide_text = L"texto via const void*";
	r = message((const void*)wide_text, L"titulo ancho", message_box::question);
	printf("texto como void* con titulo ancho: %d\n", r);

	// ESP: const void* en el titulo con el texto de bytes: la firma (char*, void*) convierte el titulo a char*.
	// Un titulo ancho por ese camino se lee como bytes y se corta en el primer nulo: sale una sola letra.
	// El cast neutro NO es neutro: solo sirve con buffers que ya sean del tipo que la firma espera.
	// ENG: const void* on the title with the text in bytes: the (char*, void*) signature turns the title into char*.
	// A wide title through that road gets read as bytes and cut at the first null: one single letter comes out.
	// The neutral cast is NOT neutral: it only works with buffers already holding the type the signature expects.
	r = message("cuidado con el cast", (const void*)L"ancho mal leido", message_box::info);
	printf("titulo ancho por char*,void*: %d (el titulo sale cortado)\n", r);

	// ESP: Los dos lados como const void* bajan a la firma de bytes: vale para punteros a texto normal.
	// Es la via para buffers opacos que ya se saben de un byte por unidad.
	// ENG: Both sides as const void* go down to the byte signature: it works for plain text pointers.
	// It is the road for opaque buffers already known to hold one byte per unit.
	const void* opaque_text = "texto desde puntero opaco";
	const void* opaque_ttl  = "opaco";
	r = message(opaque_text, opaque_ttl, message_box::warning);
	printf("los dos opacos: %d\n", r);

	// ESP: nullptr en el texto: la firma dedicada lo cambia por una cadena vacia y la ventana sale igual.
	// No es un rechazo: el retorno sigue siendo la respuesta del usuario.
	// ENG: nullptr on the text: the dedicated signature swaps it for an empty string and the window comes out anyway.
	// It is not a refusal: the return stays the user's answer.
	r = message(nullptr, "sin texto", message_box::ok);
	printf("texto nulo: %d\n", r);

	// ESP: nullptr en el titulo: ventana sin cabecera, y con los dos nulos ventana vacia pero visible.
	// ENG: nullptr on the title: window with no header, and with both nulls an empty but visible window.
	r = message("sin titulo", nullptr);
	printf("titulo nulo: %d\n", r);
	r = message(nullptr, nullptr);
	printf("los dos nulos: %d\n", r);

	// ESP: Con un literal ancho y un literal de bytes la llamada es AMBIGUA y no compila:
	// message(L"texto", "titulo") deja al compilador elegir entre la firma ancha y la de void*.
	// La solucion es escribir los dos lados del mismo tipo o pasar el ancho por const void*.
	// ENG: With one wide literal and one byte literal the call is AMBIGUOUS and does not compile:
	// message(L"texto", "titulo") leaves the compiler choosing between the wide signature and the void* one.
	// The fix is writing both sides in the same type, or passing the wide side through const void*.
	const wchar* answer_note = L"los valores negativos no son respuesta: -1 sin funcion, -2 con un lado nulo";
	r = message((const void*)answer_note, L"nota", message_box::ok);
	printf("nota final: %d\n", r);
	return 0;
}
