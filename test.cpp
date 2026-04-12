#define ST_IMPORT_INPUT
#include "System_Tools.h"
using namespace System_Tools;
int main(){
	clear();
	UWORD lang = input::language();
	printf("lang = %.2s_%.2s\n", &lang, (char*)&lang + 2);
    return 0;
}
