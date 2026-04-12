# 🛠️ System_Tools v1.0
**The No-CRT, Multiplatform System Library for C++**

`System_Tools` es una librería de bajo nivel diseñada para aplicaciones que requieren control total sobre el hardware sin el "bloat" de la STL o la CRT estándar.
### ✨ Características Clave
* **Zero Standard Dependency**: Carga dinámica de símbolos (`winmm.dll`, `libc.so.6`, `libasound.so.2`).
* **Multiplataforma Nativa**: Código idéntico para Windows y Linux.
* **Sistema de Permiciones**: Solo compila lo que necesitas mediante `ST_IMPORT_XXXX`.
* **Audio de Alta Fidelidad**: Motor de streaming con soporte para pitch, volumen y espectro.
### 🚀 Quick Start
```cpp
#define ST_IMPORT_SOUND
#define ST_IMPORT_FILE
#include "System_Tools.h"
using namespace System_Tools;
int main() {
    void* th_snd = sound::file_stream("My Rawwest Song.wav", false, 1.0f);
    sleep(5000);
    sound::destroy(snd);
    return 0;
}
```