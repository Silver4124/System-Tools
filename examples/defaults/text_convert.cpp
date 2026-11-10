/* // ESP
	Ejemplo de conversion de texto entre UTF y ASCII, y de lectura del idioma.
	Muestra: mbstowcs y wcstombs en sus dos versiones (buffer propio y memoria
	asignada por la libreria), la probes de longitud sin escribir, y language().
	Compilar: g++ text_convert.cpp -o text_convert
	// ENG
	Text conversion between UTF and ASCII, plus reading the user language.
	Shows: mbstowcs and wcstombs in both versions (your own buffer and memory
	allocated by the library), length probing without writing, and language().
	Build: g++ text_convert.cpp -o text_convert
*/
#include "System_Tools.h"
using namespace System_Tools;

int main(){
	const char* text = "System Tools 1.5";
	UWORD text_len = 0;
	while(text[text_len]) ++text_len;

	// ESP: Version de buffer propio: los tamanos excluyen el terminador nulo.
	// ENG: Own-buffer version: the sizes exclude the null terminator.
	wchar wide[64];
	UWORD wide_len = mbstowcs(wide, text, sizeof(wide)/sizeof(wchar) - 1, text_len);
	printf("mbstowcs ocupo " UWORD_PRINT " wchar\n", wide_len);

	// ESP: Sin escribir nada, entregar dst nulo en la version de memoria pregunta la longitud.
	// ENG: Without writing anything, handing a null dst to the memory version asks the length.
	UWORD needed = mbstowcs(nullptr, text, text_len, false);
	printf("la conversion pedira " UWORD_PRINT " wchar\n", needed);

	// ESP: La version que asigna deja el buffer en tu puntero: con stacked=false se libera con memory::free.
	// ENG: The allocating version leaves the buffer in your pointer: with stacked=false release it with memory::free.
	wchar* heap_wide = nullptr;
	UWORD heap_len = mbstowcs(&heap_wide, text, text_len, false);
	printf("buffer asignado: " UWORD_PRINT " wchar, puntero %p\n", heap_len, (void*)heap_wide);

	// ESP: De vuelta a ASCII con la misma pareja de versiones.
	// ENG: Back to ASCII with the same pair of versions.
	char back[64];
	UWORD back_len = wcstombs(back, heap_wide, sizeof(back) - 1, heap_len);
	printf("wcstombs devolvio " UWORD_PRINT " char: %s\n", back_len, back);

	// ESP: El idioma y la region vienen empaquetados en un solo entero: nada que liberar.
	// ENG: Language and region arrive packed in a single integer: nothing to release.
	UWORD id = language();
	char lang_lo = (char)( id        & 0xFF);
	char lang_hi = (char)((id >>  8) & 0xFF);
	char reg_lo  = (char)((id >> 16) & 0xFF);
	char reg_hi  = (char)((id >> 24) & 0xFF);
	printf("idioma " UWORD_PRINT " -> %c%c-%c%c\n", id, lang_lo, lang_hi, reg_lo, reg_hi);

	memory::free(heap_wide);
	return 0;
}
