#if !defined(__SYSTEM_TOOLS__H_FIRST__)
#define __SYSTEM_TOOLS__CPP_FIRST__
#include "System_Tools.h"
#undef __SYSTEM_TOOLS__CPP_FIRST__
#endif
#if !defined(ENDIAN_DETECTION_H)
#define ENDIAN_DETECTION_H
#if defined(IS_LITTLE_ENDIAN)
#undef IS_LITTLE_ENDIAN
#endif
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define IS_LITTLE_ENDIAN 1
#else
#define IS_LITTLE_ENDIAN 0
#endif
#elif defined(_WIN32) || defined(_WIN64) || defined(_M_IX86) || defined(_M_X64)
#define IS_LITTLE_ENDIAN 1
#elif defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(__i386__) || defined(__x86_64__) || defined(__amd64__)
#define IS_LITTLE_ENDIAN 1
#elif defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(__sparc__) || defined(_MIPSEB) || defined(__MIPSEB__) || defined(__MIPSEB__)
#define IS_LITTLE_ENDIAN 0
#else
#error "No se pudo detectar el endianness del procesador automaticamente."
#endif
#if defined(IS_BIG_ENDIAN)
#undef IS_BIG_ENDIAN
#endif
#define IS_BIG_ENDIAN (!IS_LITTLE_ENDIAN)
#endif
namespace System_Tools{
	#if (defined(__x86_64__) || defined(__x86_64)) && !defined(_WIN64)
	#define GET_VA_ARGS(prev_arg, va_name, arg_index) \
	struct va_list_linux{\
		unsigned int gp_offset;\
		unsigned int fp_offset;\
		void* overflow_arg_area;\
		void* reg_save_area;\
	} __attribute__((aligned(16))) vl;\
	typedef struct va_list_linux _VL_;\
	unsigned char reg_save[176] __attribute__((aligned(16)));\
	asm volatile(\
        "movq %%rdi, 0(%%rax)\n\t"\
        "movq %%rsi, 8(%%rax)\n\t"\
        "movq %%rdx, 16(%%rax)\n\t"\
        "movq %%rcx, 24(%%rax)\n\t"\
        "movq %%r8,  32(%%rax)\n\t"\
        "movq %%r9,  40(%%rax)\n\t"\
        "movaps %%xmm0, 48(%%rax)\n\t"\
        "movaps %%xmm1, 64(%%rax)\n\t"\
        "movaps %%xmm2, 80(%%rax)\n\t"\
        "movaps %%xmm3, 96(%%rax)\n\t"\
        "movaps %%xmm4, 112(%%rax)\n\t"\
        "movaps %%xmm5, 128(%%rax)\n\t"\
        "movaps %%xmm6, 144(%%rax)\n\t"\
        "movaps %%xmm7, 160(%%rax)\n\t"\
        : : "a"(reg_save) : "memory"\
    );\
    vl.gp_offset = (arg_index) << 3;\
    vl.fp_offset = 48;\
    vl.reg_save_area = reg_save;\
    vl.overflow_arg_area = ((char*)&prev_arg) + 8;\
    void* va_name = &vl
	#define GET_IV_ARG(va_name, idx) ((void*)((((((_VL_*)(va_name))->gp_offset)+((idx)<<3))<(((_VL_*)(va_name))->fp_offset))\
	    ?(((char*)(((_VL_*)(va_name))->reg_save_area))+(((_VL_*)(va_name))->gp_offset)+((idx)<<3))\
	    :(((char*)(((_VL_*)(va_name))->overflow_arg_area))+(((((_VL_*)(va_name))->gp_offset)+((idx)<<3))-(((_VL_*)(va_name))->fp_offset)))))
	#elif defined(_WIN64)
	#define GET_VA_ARGS(prev_arg, va_name, arg_index) void* va_name = (void*)(((char*)&prev_arg) + 8)
	#define GET_IV_ARG(va_name, idx) ((void*)(((char*)(va_name))+((idx)<<3)))
    #else
    #define GET_VA_ARGS(prev_arg, va_name, arg_index) void* va_name=((char*)&prev_arg)+sizeof(prev_arg)
	#define GET_IV_ARG(va_name, idx) ((void*)(((char*)(va_name))+((idx)*sizeof(void*))))
    #endif
    void* get_export(void* module_base, const char* func_name){
	    if(!module_base) return nullptr;
    	#if defined(_WIN32)
	    unsigned char* base = (unsigned char*)module_base;
	    if(*(unsigned short*)base != 0x5A4D) return nullptr;
	    unsigned int e_lfanew = *(unsigned int*)(base + 0x3C);
	    unsigned char* nt_header = base + e_lfanew;
	    if(*(unsigned int*)nt_header != 0x00004550) return nullptr;
	    #if defined(_WIN64) || defined(__x86_64__) || defined(__x86_64)
        unsigned char* data_dir_ptr = nt_header + 0x88;
	    #else
	    unsigned char* data_dir_ptr = nt_header + 0x78;
	    #endif
	    unsigned int export_dir_rva  = *(unsigned int*)(data_dir_ptr);
		unsigned int export_dir_size = *(unsigned int*)(data_dir_ptr + 4);
	    if(!export_dir_rva) return nullptr;
	    unsigned char* export_dir = base + export_dir_rva;
	    unsigned int num_names = *(unsigned int*)(export_dir + 0x18);
	    unsigned int names_rva = *(unsigned int*)(export_dir + 0x20);
	    unsigned int funcs_rva = *(unsigned int*)(export_dir + 0x1C);
	    unsigned int ords_rva  = *(unsigned int*)(export_dir + 0x24);
	    unsigned int* names = (unsigned int*)(base + names_rva);
	    unsigned int* funcs = (unsigned int*)(base + funcs_rva);
	    unsigned short* ords = (unsigned short*)(base + ords_rva);
	    for(unsigned int i = 0; i < num_names; i++){
	        const char* n = (const char*)(base + names[i]);
	        const char* f = func_name;
	        while(*f && *f == *n){ ++f; ++n; }
	        if(*f == *n){
	        	unsigned int func_rva = funcs[ords[i]];
				if(func_rva >= export_dir_rva && func_rva < (export_dir_rva + export_dir_size)){
					char* forward_str = (char*)(base + func_rva);
					static char lib_name[256];
					const char* func_name_ptr = nullptr;
					UWORD i = 0; while(forward_str[i] && forward_str[i] != '.'){
						lib_name[i] = forward_str[i];
						++i;
					}
					func_name_ptr = forward_str + i + 1;
					lib_name[i++] = '.'; lib_name[i++] = 'd'; lib_name[i++] = 'l'; 
					lib_name[i++] = 'l'; lib_name[i] = '\0';
					return get_system_function(func_name_ptr, lib_name);
				}
				return (void*)(base + func_rva);
			}
	    }
	    #else // Linux
	    struct Elf_Sym{
	    	unsigned int st_name;
	    	unsigned char st_info;
	    	unsigned char st_other;
	    	unsigned short st_shndx;
	    	UWORD st_value;
	    	UWORD st_size;
	    };
	    struct Elf_Dyn{
	    	WORD d_tag;
	    	UWORD d_val;
	    };
	    struct Link_Map{
	    	UWORD l_addr;
	    	char* l_name;
	    	Elf_Dyn* l_ld;
	    	Link_Map* l_next;
			Link_Map* l_prev;
	    };
	    Link_Map* base = (Link_Map*)module_base;
	    Elf_Dyn* dyn = base->l_ld;
	    Elf_Sym* symtab = nullptr;
	    char* strtab = nullptr;
	    while(dyn->d_tag != 0){
	    	if(dyn->d_tag == 6) symtab = (Elf_Sym*)(dyn->d_val); // DT_SYMTAB
	    	if(dyn->d_tag == 5) strtab = (char*)(dyn->d_val); // DT_STRTAB
	    	++dyn;
	    }
	    if(!symtab || !strtab) return nullptr;
	    UWORD i = 0;
	    while(true){
	    	if(i == 15000 || (symtab[i].st_name == 0 && i != 0)) break;
	    	const char* n = strtab + symtab[i].st_name;
	    	if((UWORD)n < base->l_addr) continue;
	    	const char* f = func_name;
	    	while(*f && *f == *n){ ++f; ++n; }
	    	if(*f == *n) return (void*)(base->l_addr + symtab[i].st_value);
	    	++i;
	    }
	    #endif
		return nullptr;
	}
	void* ensure_user(const char* lib_target){
    	#if defined(_WIN32)
    	void* k32 = nullptr;
		#if defined(_WIN64) || defined(__x86_64__) || defined(__x86_64)
		asm volatile(
			"movq %%gs:0x60, %%rax\n\t"
			"movq 0x18(%%rax), %%rax\n\t"
			"movq 0x10(%%rax), %%rax\n\t"
			"movq (%%rax), %%rax\n\t"
			"movq (%%rax), %%rax\n\t"
			"movq 0x30(%%rax), %0\n\t"
			: "=r"(k32) : : "rax", "memory"
		);
		#else
		asm volatile(
		    "movl %%fs:0x30, %%eax\n\t"
		    "movl 0x0c(%%eax), %%eax\n\t"
		    "movl 0x1c(%%eax), %%eax\n\t"
			"movl (%%eax), %%eax\n\t"
			"movl 0x08(%%eax), %0\n\t"
		    : "=r"(k32) : : "eax", "memory"
		);
		#endif
	    if(!lib_target) return k32;
	    struct UNICODE_STRING{
	    	unsigned short Length;
	    	unsigned short MaximumLength;
	    	wchar_t* Buffer;
	    	UNICODE_STRING() : Length(0), MaximumLength(0), Buffer(nullptr) {}
	    	~UNICODE_STRING(){ if(Buffer) delete[] Buffer; }
	    };
		struct LDR_DATA_TABLE_ENTRY{
		    void* Reserved1[2];
		    void* InMemoryOrderLinks[2];
		    void* Reserved2[2];
		    void* DllBase;
		    void* EntryPoint;
		    void* Reserved3;
		    UNICODE_STRING FullDllName;
		    unsigned char Reserved4[8];
		    void* Reserved5[3];
		    union{
		    	unsigned long CheckSum;
		    	void* Reserved6;
		    };
		    unsigned long TimeDateStamp;
		};
		UNICODE_STRING SEQ_LIB; {
			const char* _org = lib_target;
			while(*_org) SEQ_LIB.Length += (*_org < 0x80) ? ((++_org) ? 1 : 1)
				: (((*_org & 0xE0) == 0xC0) ? ((_org += 2) ? 1 : 1)
				: (((*_org & 0xF8) == 0xF0) ? ((_org += 3) ? 1 : 1)
				: (((*_org & 0xF8) == 0xF0) ? ((_org += 4) ? 1 : 1)
				:                                ((++_org) ? 0 : 0))));
			_org = lib_target;
			SEQ_LIB.Buffer = new wchar_t[SEQ_LIB.Length + 1];
			const wchar_t* w_org = SEQ_LIB.Buffer;
			while(*_org){
				if(*_org < 0x80) *(SEQ_LIB.Buffer)++ = (wchar_t)(*_org++);
				else if((*_org & 0xE0) == 0xC0) *(SEQ_LIB.Buffer)++ =
					(wchar_t)(((*_org++ & 0x1F) << 6) | (*_org++ & 0x3F));
				else if((*_org & 0xF0) == 0xE0) *(SEQ_LIB.Buffer)++ =
					(wchar_t)(((*_org++ & 0x0F) << 12) | ((*_org++ & 0x3F) << 6)
				  | (*_org++ & 0x3F));
				else if((*_org & 0xF8) == 0xF0) *(SEQ_LIB.Buffer)++ =
					(wchar_t)(((*_org++ & 0x07) << 18) | ((*_org++ & 0x3F) << 12)
				  | ((*_org++ & 0x3F) << 6) | (*_org++ & 0x3F));
				else ++_org;
			}
			*(SEQ_LIB.Buffer) = L'\0';
			SEQ_LIB.Buffer = (wchar_t*)w_org;
		}
		void* peb;
		#if defined(_WIN64)
		asm volatile(
			"movq %%gs:0x60, %0"
			: "=r"(peb)
		);
		void* ldr = *(void**)((unsigned char*)peb + 0x18);
		void* list_head = *(void**)((unsigned char*)ldr + 0x10);
		#else
		asm volatile(
			"movl %%fs:0x30, %0"
			: "=r"(peb)
		);
		void* ldr = *(void**)((unsigned char*)peb + 0x0C);
		void* list_head = *(void**)((unsigned char*)ldr + 0x0C);
		#endif
	    void* current_node = list_head;
		do{
		    LDR_DATA_TABLE_ENTRY* entry = (LDR_DATA_TABLE_ENTRY*)current_node;
		    if(entry->FullDllName.Buffer){
		    	const wchar_t* ws1 = entry->FullDllName.Buffer;
	    		if(*ws1 == L'\\'){
	    			while(*ws1) ++ws1;
	    			while(*ws1 != L'\\') --ws1;
	    			++ws1;
	    		}
	    		const wchar_t* ws2 = SEQ_LIB.Buffer;
	    		while(*ws1 && *ws2 && *ws1 == *ws2){ ++ws1; ++ws2; }
	    		if(*ws1 == *ws2) return entry->DllBase;
		    }
		    current_node = *(void**)current_node;
		}while(current_node != list_head);
	    typedef void*(__stdcall *LLF)(const char*);
	    LLF pLoadLibraryA = (LLF)get_export(k32, "LoadLibraryA");
	    if(pLoadLibraryA) return pLoadLibraryA(lib_target);
	    #else // Linux
	    struct Elf_Dyn{
	    	WORD d_tag;
	    	UWORD d_val;
	    };
	    struct Link_Map{
	    	UWORD l_addr;
	    	char* l_name;
	    	Elf_Dyn* l_ld;
	    	Link_Map* l_next;
			Link_Map* l_prev;
	    };
	    UWORD dynamic_addr;
	    #if defined(__x86_64__) || defined(__x86_64)
	    asm volatile(
	    	"lea _DYNAMIC(%%rip), %0" 
        	: "=r"(dynamic_addr)
	    );
	    #else
	    asm volatile(
		    "call 1f\n"
		    "1: pop %%eax\n"
		    "addl $_GLOBAL_OFFSET_TABLE_, %%eax\n"
		    "movl _DYNAMIC@GOT(%%eax), %0"
		    : "=r"(dynamic_addr) : : "eax"
		);
	    #endif
	    Elf_Dyn* dyn_ptr = (Elf_Dyn*)dynamic_addr;
	    Link_Map* map = nullptr;
	    for(UWORD i=0; dyn_ptr[i].d_tag != 0; ++i){
	    	if(dyn_ptr[i].d_tag == 21){ // DT_DEBUG
				struct r_debug{
					int r_version;
					Link_Map* r_map;
				} *debug_info = (struct r_debug*)(dyn_ptr[i].d_val);
				map = debug_info->r_map;
				break;
			}
	    }
	    Link_Map* base_map = map; bool base_founded = false;
	    while(map){
	    	if(map->l_name ? (*map->l_name != '\0') : false){
	    		const char* s1 = map->l_name;
	    		if(*s1 == '/'){
	    			while(*s1) ++s1;
	    			while(*s1 != '/') --s1;
	    			++s1;
	    		}
	    		const char* sl1 = s1; 
	    		const char* s2 = lib_target;
	    		while(*s1 && *s1 == *s2){ ++s1; ++s2; }
	    		if(*s1 == *s2) return (void*)map;
	    		if(!base_founded){
					const char* sl2 = "libc.so.6";
					while(*sl1 && *sl1 == *sl2){ ++sl1; ++sl2; }
					base_founded = *sl1 == *sl2;
	    		}
	    	}
	    	map = map->l_next;
	    	if(!base_founded) base_map = map;
	    }
	    if(base_founded){
             typedef void*(*DLOP)(const char*,int);
             static DLOP pDlOpen = (DLOP)get_export((void*)base_map, "dlopen");
             if(pDlOpen) return (void*)pDlOpen(lib_target, 1);
        }
	    #endif
	    return nullptr;
	}
	void* get_system_function(const char* func_name, const char* lib_target){
        return get_export(ensure_user(lib_target), func_name);
    }
    int system(const char* cmd){
    	if(!cmd) return -1;
    	typedef int(*SYS)(const char*);
    	#if defined(_WIN32)
		static SYS pSystem = (SYS)get_system_function("system", "msvcrt.dll");
		if(!pSystem){
			pSystem = (SYS)get_system_function("system", "ucrtbase.dll");
			if(!pSystem) return -1;
		}
    	#else // Linux
		static SYS pSystem = (SYS)get_system_function("system", "libc.so.6");
		if(!pSystem) return -1;
		#endif
		return pSystem(cmd);
    }
    void clear(){ system(
    	#if defined(_WIN32)
    	"cls"
    	#else // Linux
    	"clear"
    	#endif
    ); }
    int puts(const char* str){
    	if(!str) return -1;
        typedef int(*PUTS)(const char*);
        #if defined(_WIN32)
        static PUTS pPuts = (PUTS)get_system_function("puts", "msvcrt.dll");
		if(!pPuts){
			pPuts = (PUTS)get_system_function("puts", "ucrtbase.dll");
			if(!pPuts) return -1;
		}
        #else // Linux
        static PUTS pPuts = (PUTS)get_system_function("puts", "libc.so.6");
		if(!pPuts) return -1;
        #endif
        return pPuts(str);
    }
    int putchar(int chr){
        typedef int(*PUTCHAR)(int);
        #if defined(_WIN32)
        static PUTCHAR pPutchar = (PUTCHAR)get_system_function("putchar", "msvcrt.dll");
		if(!pPutchar){
			pPutchar = (PUTCHAR)get_system_function("putchar", "ucrtbase.dll");
			if(!pPutchar) return -1;
		}
        #else // Linux
        static PUTCHAR pPutchar = (PUTCHAR)get_system_function("putchar", "libc.so.6");
		if(!pPutchar) return -1;
        #endif
        return pPutchar(chr);
    }
    int printf(const char* format, ...){
        GET_VA_ARGS(format, args, 1);
    	if(!format) return -1;
    	typedef int(*VPRINTF)(const char*,void*);
    	#if defined(_WIN32)
	    static VPRINTF pVPrintf = (VPRINTF)get_system_function("vprintf", "msvcrt.dll");
		if(!pVPrintf){
			pVPrintf = (VPRINTF)get_system_function("vprintf", "ucrtbase.dll");
			if(!pVPrintf) return -1;
		}
	    #else
	    static VPRINTF pVPrintf = (VPRINTF)get_system_function("vprintf", "libc.so.6");
        if(!pVPrintf) return -1;
	    #endif
		return pVPrintf(format, args);
    }
    int sprintf(char* str, const char* format, ...){
        GET_VA_ARGS(format, args, 2);
    	if(!str || !format) return -1;
        typedef int(*VSPRINTF)(char*,const char*,void*);
        #if defined(_WIN32)
	    static VSPRINTF pVSprintf = (VSPRINTF)get_system_function("vsprintf", "msvcrt.dll");
		if(!pVSprintf){
			pVSprintf = (VSPRINTF)get_system_function("vsprintf", "ucrtbase.dll");
			if(!pVSprintf) return -1;
		}
	    #else
	    static VSPRINTF pVSprintf = (VSPRINTF)get_system_function("vsprintf", "libc.so.6");
	    if(!pVSprintf) return -1;
	    #endif
	    return pVSprintf(str, format, args);
    }
    int scanf(const char* format, ...){
        GET_VA_ARGS(format, args, 1);
    	if(!format) return -1;
        typedef int(*VSCANF)(const char*, void*);
        #if defined(_WIN32)
        static VSCANF pVScanf = (VSCANF)get_system_function("vscanf", "msvcrt.dll");
		if(!pVScanf){
			pVScanf = (VSCANF)get_system_function("vscanf", "ucrtbase.dll");
			if(!pVScanf) return -1;
		}
        #else // Linux
        static VSCANF pVScanf = (VSCANF)get_system_function("vscanf", "libc.so.6");
        if(!pVScanf) return -1;
        #endif
	    return pVScanf(format, args);
    }
    void sleep(unsigned int ms){
    	#if defined(_WIN32)
        typedef void(__stdcall *SFN)(unsigned int);
        static SFN psleep = (SFN)get_system_function("Sleep");
        if(!psleep) return;
        psleep(ms);
        #else // Linux
        typedef int(*SFN)(unsigned int);
        static SFN pUsleep = (SFN)get_system_function("usleep", "libc.so.6");
        if(!pUsleep) return;
        pUsleep((ms<<10)-(ms<<4)-(ms<<3));
        #endif
    }
	int message(const char* txt, const char* ttl, int type){
		if(!txt || !ttl) return -2;
		#if defined(_WIN32)
		typedef int(*MBT)(void*,const char*,const char*,unsigned int);
        static MBT pMessageBoxA = (MBT)get_system_function("MessageBoxA", "user32.dll");
        if(!pMessageBoxA) return -1;
        return pMessageBoxA(nullptr, txt, ttl, (unsigned int)type | 0x00250000);
		#else // Linux
		// We will actually use the console command 'zenity',
		// because it is an exact equivalent of 'MessageBoxW' in Linux.
		// We will modify the appearance and visual settings of the popup window,
		// so that it has functional fidelity with that of Windows.
		struct _ap_{
			static UWORD strlen(const char* s){
				if(s ? (!*s) : true) return 0;
				const char* ss = s; while(*ss) ++ss;
				return (UWORD)(ss - s);
			}
			static UWORD byte_count_escaped(const char* s){
				if(!s) return 0;
				UWORD len = 0;
				while(*s) len += (*s == '"' || *s++ == '\\') ? 2 : 1;
				return len;
			}
			static void append_escaped(char* dst, UWORD* pos, const char* src){
				while(*src){
				    if(*src == L'"' || *src == L'\\') dst[(*pos)++] = '\\';
				    dst[(*pos)++] = *src++;
				}
			}
		};
		int def_button = type & 0xF00;
		int icon_type  = type & 0x0F0;
		int buttons    = type & 0x00F;
	    const char* zenity_type = "--info";
		if(icon_type == 0x10) zenity_type = "--error";
		if(icon_type == 0x20) zenity_type = "--question";
		if(icon_type == 0x30) zenity_type = "--warning";
		const char* ok_label     = "Aceptar";
		const char* cancel_label = nullptr;
		const char* extra_label  = nullptr;
		int mb_default           = 1;
		#if defined(___mb__switch___case___1_2__3__)
		#undef ___mb__switch___case___1_2__3__
		#endif
		#if defined(___mb__switch___case___1_2__4__)
		#undef ___mb__switch___case___1_2__4__
		#endif
		#if defined(___mb__switch___case___1_2__5__)
		#undef ___mb__switch___case___1_2__5__
		#endif
		#define ___mb__switch___case___1_2__3__(variable, value, ok, cancel, extra, mb) \
		if((variable)==(value)){ok_label=(ok);cancel_label=(cancel);extra_label=(extra);mb_default=(mb);}
		#define ___mb__switch___case___1_2__4__(variable, value, mb) if((variable)==(value))mb_default=(mb)
		___mb__switch___case___1_2__3__(buttons, 0x2, "Abortar",    "Ignorar",  "Reintentar", 1);
		___mb__switch___case___1_2__3__(buttons, 0x3, "Si",         "No",       "Cancelar",   1);
		___mb__switch___case___1_2__3__(buttons, 0x4, "Si",         "No",       nullptr,      1);
		___mb__switch___case___1_2__3__(buttons, 0x5, "Reintentar", "Cancelar", nullptr,      1);
		___mb__switch___case___1_2__4__(def_button, 0x100, 2);
		___mb__switch___case___1_2__4__(def_button, 0x200, 3);
		#undef ___mb__switch___case___1_2__3__
		#undef ___mb__switch___case___1_2__4__
		UWORD pos = 0;
		if(ok_label)        pos += _ap_::strlen(ok_label)     + 20U;
		if(cancel_label)    pos += _ap_::strlen(cancel_label) + 20U;
		if(extra_label)     pos += _ap_::strlen(extra_label)  + 20U;
		if(mb_default == 2) pos += 20U;
		pos += _ap_::byte_count_escaped(txt) + _ap_::byte_count_escaped(ttl) + 300;
		char* cmd = new char[pos + 1];
		if(!cmd) return -1;
		pos = 0; cmd[0] = '\0';
		#define ___mb__switch___case___1_2__3__(str) _ap_::append_escaped(cmd,&pos,str)
		#define ___mb__switch___case___1_2__4__(var, val) if(var)pos+=sprintf(cmd+pos," --" val "=\"%s\"",var)
		pos += sprintf(cmd + pos, "zenity %s --title=\"", zenity_type);
		___mb__switch___case___1_2__3__(ttl);
		pos += sprintf(cmd + pos, "\" --text=\"");
		___mb__switch___case___1_2__3__(txt);
		cmd[pos++] = '\"';
		___mb__switch___case___1_2__4__(ok_label, "ok-label");
		___mb__switch___case___1_2__4__(cancel_label, "cancel-label");
		___mb__switch___case___1_2__4__(extra_label, "extra-button");
		if(mb_default == 2) pos += sprintf(cmd + pos, " --default-cancel");
		pos += sprintf(cmd + pos, " --no-wrap --ellipsize 2>/dev/null");
		#undef ___mb__switch___case___1_2__3__
		#undef ___mb__switch___case___1_2__4__
		int ret = system(cmd) >> 8;
		delete[] cmd;
		#define ___mb__switch___case___1_2__3__(value, vRet1, vRet2) if(buttons==(value))return((ret==0)?(vRet1):(vRet2))
		#define ___mb__switch___case___1_2__4__(_value, value1, vRet) if(buttons==(_value)&&ret==(value1))return(vRet)
		#define ___mb__switch___case___1_2__5__(value, vRet) if(buttons==(value))return(vRet)
		___mb__switch___case___1_2__3__(0x0, 1, 2);
		___mb__switch___case___1_2__3__(0x1, 1, 2);
		___mb__switch___case___1_2__3__(0x4, 6, 7);
		___mb__switch___case___1_2__3__(0x5, 4, 2);
		___mb__switch___case___1_2__4__(0x2, 0, 3);
		___mb__switch___case___1_2__4__(0x2, 1, 5);
		___mb__switch___case___1_2__5__(0x2, 4);
		___mb__switch___case___1_2__4__(0x3, 0, 6);
		___mb__switch___case___1_2__4__(0x3, 1, 7);
		___mb__switch___case___1_2__5__(0x3, 2);
		___mb__switch___case___1_2__3__(buttons, 1, 2);
		#undef ___mb__switch___case___1_2__3__
		#undef ___mb__switch___case___1_2__4__
		#undef ___mb__switch___case___1_2__5__
	    #endif
    }
	int message(const wchar_t* txt, const wchar_t* ttl, int type){
		if(!txt || !ttl) return -2;
		#if defined(_WIN32)
		typedef int(*MBT)(void*,const wchar_t*,const wchar_t*,unsigned int);
        static MBT pMessageBoxW = (MBT)get_system_function("MessageBoxW", "user32.dll");
        if(!pMessageBoxW) return -1;
        return pMessageBoxW(nullptr, txt, ttl, (unsigned int)type | 0x00250000);
		#else // Linux
		// We will actually use the console command 'zenity',
		// because it is an exact equivalent of 'MessageBoxW' in Linux.
		// We will modify the appearance and visual settings of the popup window,
		// so that it has functional fidelity with that of Windows.
		struct _ap_{
			static UWORD strlen(const char* s){
				if(s ? (!*s) : true) return 0;
				const char* ss = s; while(*ss) ++ss;
				return (UWORD)(ss - s);
			}
			static UWORD byte_count_escaped(const wchar_t* ws){
				if(!ws) return 0;
				UWORD len = 0;
				while(*ws){
				    wchar_t wc = *ws++;
				    if(wc == L'"' || wc == L'\\')
				        len += 2;
				    else if (wc < 0x80)
				         len += 1;
				    else if (wc < 0x800)
				         len += 2;
				    else len += 3;
				}
				return len;
			}
			static bool append_escaped(char* dst, UWORD* pos, const wchar_t* src){
				if(!src || !*src) return true;
				while(*src){
				    unsigned int wc = (unsigned int)*src++;
				    if(wc == L'"' || wc == L'\\'){
				        dst[(*pos)++] = '\\';
				        dst[(*pos)++] = (char)(wc & 0xFF);
				    }else if(wc < 0x80)
				        dst[(*pos)++] = (char)(wc & 0xFF);
				     else if(wc < 0x800){
				        dst[(*pos)++] = (char)(0xC0 | (wc >> 6));
				        dst[(*pos)++] = (char)(0x80 | (wc & 0x3F));
				    }else if(wc < 0x10000){
				        dst[(*pos)++] = (char)(0xE0 | (wc >> 12));
				        dst[(*pos)++] = (char)(0x80 | ((wc >> 6) & 0x3F));
				        dst[(*pos)++] = (char)(0x80 | (wc & 0x3F));
				    }else if(wc < 0x110000){
						dst[(*pos)++] = (char)(0xF0 | (wc >> 18));
						dst[(*pos)++] = (char)(0x80 | ((wc >> 12) & 0x3F));
						dst[(*pos)++] = (char)(0x80 | ((wc >> 6) & 0x3F));
						dst[(*pos)++] = (char)(0x80 | (wc & 0x3F));
					}
				}
				return true;
			}
		};
		int def_button = type & 0xF00;
		int icon_type  = type & 0x0F0;
		int buttons    = type & 0x00F;
	    const char* zenity_type = "--info";
		if(icon_type == 0x10) zenity_type = "--error";
		if(icon_type == 0x20) zenity_type = "--question";
		if(icon_type == 0x30) zenity_type = "--warning";
		const char* ok_label     = "Aceptar";
		const char* cancel_label = nullptr;
		const char* extra_label  = nullptr;
		int mb_default           = 1;
		#define ___mb__switch___case___1_2__3__(variable, value, ok, cancel, extra, mb) \
		if((variable)==(value)){ok_label=(ok);cancel_label=(cancel);extra_label=(extra);mb_default=(mb);}
		#define ___mb__switch___case___1_2__4__(variable, value, mb) if((variable)==(value))mb_default=(mb)
		___mb__switch___case___1_2__3__(buttons, 0x2, "Abortar",    "Ignorar",  "Reintentar", 1);
		___mb__switch___case___1_2__3__(buttons, 0x3, "Si",         "No",       "Cancelar",   1);
		___mb__switch___case___1_2__3__(buttons, 0x4, "Si",         "No",       nullptr,      1);
		___mb__switch___case___1_2__3__(buttons, 0x5, "Reintentar", "Cancelar", nullptr,      1);
		___mb__switch___case___1_2__4__(def_button, 0x100, 2);
		___mb__switch___case___1_2__4__(def_button, 0x200, 3);
		#undef ___mb__switch___case___1_2__3__
		#undef ___mb__switch___case___1_2__4__
		UWORD pos = 0;
		if(ok_label)        pos += _ap_::strlen(ok_label)     + 20U;
		if(cancel_label)    pos += _ap_::strlen(cancel_label) + 20U;
		if(extra_label)     pos += _ap_::strlen(extra_label)  + 20U;
		if(mb_default == 2) pos += 20U;
		pos += _ap_::byte_count_escaped(txt) + _ap_::byte_count_escaped(ttl) + 300;
		char* cmd = new char[pos + 1];
		if(!cmd) return -1;
		pos = 0; cmd[0] = '\0';
		#define ___mb__switch___case___1_2__3__(wstr) if(!_ap_::append_escaped(cmd,&pos,wstr)){delete[]cmd;return -1;}
		#define ___mb__switch___case___1_2__4__(var, val) if(var)pos+=sprintf(cmd+pos," --" val "=\"%s\"",var)
		pos += sprintf(cmd + pos, "zenity %s --title=\"", zenity_type);
		___mb__switch___case___1_2__3__(ttl);
		pos += sprintf(cmd + pos, "\" --text=\"");
		___mb__switch___case___1_2__3__(txt);
		cmd[pos++] = '\"';
		___mb__switch___case___1_2__4__(ok_label, "ok-label");
		___mb__switch___case___1_2__4__(cancel_label, "cancel-label");
		___mb__switch___case___1_2__4__(extra_label, "extra-button");
		if(mb_default == 2) pos += sprintf(cmd + pos, " --default-cancel");
		pos += sprintf(cmd + pos, " --no-wrap --ellipsize 2>/dev/null");
		#undef ___mb__switch___case___1_2__3__
		#undef ___mb__switch___case___1_2__4__
		int ret = system(cmd) >> 8;
		delete[] cmd;
		#define ___mb__switch___case___1_2__3__(value, vRet1, vRet2) if(buttons==(value))return((ret==0)?(vRet1):(vRet2))
		#define ___mb__switch___case___1_2__4__(_value, value1, vRet) if(buttons==(_value)&&ret==(value1))return(vRet)
		#define ___mb__switch___case___1_2__5__(value, vRet) if(buttons==(value))return(vRet)
		___mb__switch___case___1_2__3__(0x0, 1, 2);
		___mb__switch___case___1_2__3__(0x1, 1, 2);
		___mb__switch___case___1_2__3__(0x4, 6, 7);
		___mb__switch___case___1_2__3__(0x5, 4, 2);
		___mb__switch___case___1_2__4__(0x2, 0, 3);
		___mb__switch___case___1_2__4__(0x2, 1, 5);
		___mb__switch___case___1_2__5__(0x2, 4);
		___mb__switch___case___1_2__4__(0x3, 0, 6);
		___mb__switch___case___1_2__4__(0x3, 1, 7);
		___mb__switch___case___1_2__5__(0x3, 2);
		___mb__switch___case___1_2__3__(buttons, 1, 2);
		#undef ___mb__switch___case___1_2__3__
		#undef ___mb__switch___case___1_2__4__
		#undef ___mb__switch___case___1_2__5__
	    #endif
    }
    namespace memory{
		void* alloc(unsigned int size){
			if(size == 0) return nullptr;
		    #if defined(_WIN32)
		    typedef void*(__stdcall *H_ALC)(void*,unsigned int,unsigned int);
		    static H_ALC pAlloc = (H_ALC)get_system_function("HeapAlloc");
		    typedef void*(__stdcall *G_HP)();
		    static G_HP pGetProcessHeap = (G_HP)get_system_function("GetProcessHeap");
		    if(!pAlloc || !pGetProcessHeap) return nullptr;
		    static void* hHeap = nullptr; if(!hHeap) hHeap = pGetProcessHeap();
		    return pAlloc(hHeap, 0x00000008, size);
		    #else
		    typedef void*(*M_ALC)(unsigned int);
		    static M_ALC pMalloc = (M_ALC)get_system_function("malloc", "libc.so.6");
		    typedef void*(*M_SET)(void*,int,unsigned int);
		    static M_SET pMemset = (M_SET)get_system_function("memset", "libc.so.6");
		    if(!pMalloc || !pMemset) return nullptr;
		    void* ptr = pMalloc(size);
		    if(ptr) pMemset(ptr, 0, size);
		    return ptr;
		    #endif
		}
		void* clloc(void* dest, const void* src, unsigned int size){
		    if(!dest || !src || size == 0) return nullptr;
		    #if defined(_WIN32)
		    typedef void*(__stdcall *M_CPY)(void*,const void*,unsigned int);
		    static M_CPY pMemcpy = (M_CPY)get_system_function("memcpy", "msvcrt.dll");
		    #else
		    typedef void*(*M_CPY)(void*,const void*,unsigned int);
		    static M_CPY pMemcpy = (M_CPY)get_system_function("memcpy", "libc.so.6");
		    #endif
		    if(!pMemcpy) return nullptr;
		    pMemcpy(dest, src, size);
		    return dest;
		}
		void* realloc(void* ptr, unsigned int size){
			if(!ptr) return memory::alloc(size);
		    if(size == 0){ memory::free(ptr); return nullptr; }
		    #if defined(_WIN32)
		    typedef void*(__stdcall *H_RAL)(void*,unsigned int,void*,unsigned int);
		    static H_RAL pRealloc = (H_RAL)get_system_function("HeapReAlloc");
		    if(!pRealloc) return nullptr;
		    return pRealloc(nullptr, 0x00000008, ptr, size);
		    #else
		    typedef void*(*M_RAL)(void*,unsigned int);
		    static M_RAL pRealloc = (M_RAL)get_system_function("realloc", "libc.so.6");
		    if(!pRealloc) return nullptr;
		    return pRealloc(ptr, size);
		    #endif
		}
		void* free(void* ptr){
			if(!ptr) return nullptr;
		    #if defined(_WIN32)
		    typedef int(__stdcall *H_FRE)(void*,unsigned int,void*);
		    static H_FRE pFree = (H_FRE)get_system_function("HeapFree");
		    typedef void* (__stdcall *G_HP)();
		    static G_HP pGetProcessHeap = (G_HP)get_system_function("GetProcessHeap");
		    if(!pFree || !pGetProcessHeap) return nullptr;
		    static void* hHeap = nullptr; if(!hHeap) hHeap = pGetProcessHeap();
		    pFree(hHeap, 0, ptr);
		    #else
		    typedef void(*M_FRE)(void*);
		    static M_FRE pFree = (M_FRE)get_system_function("free", "libc.so.6");
		    if(!pFree) return nullptr;
		    pFree(ptr);
		    #endif
		    return ptr;
		}
		void* slloc(void* dest, unsigned char value, unsigned int size){
			if(!dest || size == 0) return nullptr;
		    #if defined(_WIN32)
		    typedef void*(__stdcall *M_SET)(void*,int,unsigned int);
		    static M_SET pMemset = (M_SET)get_system_function("memset", "msvcrt.dll");
		    #else
		    typedef void*(*M_SET)(void*,int,unsigned int);
		    static M_SET pMemset = (M_SET)get_system_function("memset", "libc.so.6");
		    #endif
		    if(!pMemset) return nullptr;
		    pMemset(dest, value, size);
		    return dest;
		}
		void* cnrlloc(const void* p1, const void* p2, unsigned int size){
		    if(!p1 || !p2 || size == 0) return nullptr;
		    if(p1 == p2) return (void*)p1;
		    #if defined(_WIN32)
		    typedef int(__stdcall *M_CMP)(const void*,const void*,unsigned int);
		    static M_CMP pMemcmp = (M_CMP)get_system_function("memcmp", "msvcrt.dll");
		    #else
		    typedef int(*M_CMP)(const void*,const void*,unsigned int);
		    static M_CMP pMemcmp = (M_CMP)get_system_function("memcmp", "libc.so.6");
		    #endif
		    if(!pMemcmp) return nullptr;
		    int ret = pMemcmp(p1, p2, size);
		    return (void*)((ret == 0) ? ((void*)((UWORD)p1 ^ (UWORD)p2)) : ((ret < 0) ? p1 : p2));
		}
		void* cmplloc(const void* p1, const void* p2, unsigned int size){
			if(!p1 || !p2 || size == 0) return nullptr;
		    if(p1 == p2) return (void*)p1;
		    void* ret = memory::cnrlloc(p1, p2, size);
		    return ret ? ((ret == p1 || ret == p2) ? nullptr : ret) : nullptr;
		}
		void* mlloc(void* dest, const void* src, unsigned int size){
            if(!dest || !src || size == 0) return dest;
            #if defined(_WIN32)
            typedef void*(__stdcall *M_MOV)(void*,const void*,unsigned int);
            static M_MOV pMemmove = (M_MOV)get_system_function("memmove", "msvcrt.dll");
            #else
            typedef void*(*M_MOV)(void*,const void*,unsigned int);
            static M_MOV pMemmove = (M_MOV)get_system_function("memmove", "libc.so.6");
            #endif
            if(!pMemmove) return nullptr;
            pMemmove(dest, src, size);
            return dest;
        }
	}
    namespace thread{
    	void* create(void*(*func)(void*), void* arg){
		    if(!func) return nullptr;
		    #if defined(_WIN32)
    		#define THR_HANDLE_DECL struct THR_HANDLE{\
    			UWORD magic;\
		        void* native_handle;\
		        unsigned long id;\
		        THR_HANDLE() : magic(0x54485244), native_handle(nullptr), id(0) {}\
		        ~THR_HANDLE(){ magic = 0x00; }\
		    }
		    THR_HANDLE_DECL;
		    typedef void*(__stdcall *CTH)(void*,UWORD,void*(*)(void*),void*,unsigned int,unsigned long*);
            static CTH pCreateThread = (CTH)get_system_function("CreateThread");
            if(!pCreateThread) return nullptr;
            THR_HANDLE* h = new THR_HANDLE;
            h->native_handle = pCreateThread(nullptr, 0, func, arg, 0, &(h->id));
            if(h->native_handle) return (void*)h;
            #else // Linux
            #define THR_HANDLE_DECL struct THR_HANDLE{\
    			UWORD magic;\
		        UWORD id;\
		        THR_HANDLE() : magic(0x54485244), id(0) {}\
		        ~THR_HANDLE(){ magic = 0x00; }\
		    }
            THR_HANDLE_DECL;
            typedef int(*PTH)(UWORD*,void*,void*(*)(void*),void*);
            static PTH pPthreadCreate = (PTH)get_system_function("pthread_create", "libc.so.6");
            if(!pPthreadCreate){
            	pPthreadCreate = (PTH)get_system_function("pthread_create", "libpthread.so.0");
            	if(!pPthreadCreate) return nullptr;
            }
            THR_HANDLE* h = new THR_HANDLE;
            if(pPthreadCreate(&(h->id), nullptr, func, arg) == 0) return (void*)h;
            #endif
            delete h;
            return nullptr;
    	}
    	template<typename R, typename... Args> inline void* create(R(*func)(Args...), void* arg){ return create((void*(*)(void*))func, arg); }
    	void* join(void* thr){
    		#define THR_HANDLE_VALID(thr) ((thr)?(*(UWORD*)(thr)!=0x54485244):true)
    		if(THR_HANDLE_VALID(thr)) return nullptr;
    		THR_HANDLE_DECL;
            THR_HANDLE* h = (THR_HANDLE*)thr;
    		#if defined(_WIN32)
		    typedef unsigned int(__stdcall *WSO)(void*, unsigned int);
		    static WSO pWait = (WSO)get_system_function("WaitForSingleObject");
		    if(!pWait || !(h->native_handle)) return nullptr;
			pWait(h->native_handle, 0xFFFFFFFF);
		    #else // Linux
		    typedef int(*PTH)(unsigned long,void**);
		    static PTH pJoin = (PTH)get_system_function("pthread_join", "libc.so.6");
            if(!pJoin){
            	pJoin = (PTH)get_system_function("pthread_join", "libpthread.so.0");
            	if(!pJoin) return nullptr;
            }
		    if(!(h->id)) return nullptr;
			pJoin(h->id, nullptr);
		    #endif
			delete h;
		    return thr;
		}
    	void* kill(void* thr){
    		if(THR_HANDLE_VALID(thr)) return nullptr;
    		THR_HANDLE_DECL;
            THR_HANDLE* h = (THR_HANDLE*)thr;
    		#if defined(_WIN32)
            typedef int(__stdcall *TTH)(void*,unsigned int);
            static TTH pTerminateThread = (TTH)get_system_function("TerminateThread");
            if(!pTerminateThread || !(h->native_handle)) return nullptr;
            pTerminateThread(h->native_handle, 0);
            #else // Linux
            typedef int(*PTH)(UWORD);
            static PTH pPthreadCancel = (PTH)get_system_function("pthread_cancel", "libc.so.6");
            if(!pPthreadCancel){
            	pPthreadCancel = (PTH)get_system_function("pthread_cancel", "libpthread.so.0");
            	if(!pPthreadCancel) return nullptr;
            }
		    if(!(h->id)) return nullptr;
            pPthreadCancel(h->id);
            #endif
            delete h;
            return thr;
    	}
    	inline bool yiel(void* thr, bool _wait){
    		if(THR_HANDLE_VALID(thr)) return false;
    		if(_wait) sleep(10);
    		return true;
    	}
    }
    #if defined(ST_IMPORT_SOUND)
    namespace sound{
		void* create(unsigned int buffer_size){
			#define SND_HANDLE_VALID(ptr) ((ptr)?((*(UWORD*)(ptr))!=0x534E444F):true)
			#if defined(_WIN32)
			#define SND_HANDLE_DECL struct WAVEHDR{\
			    char* lpData; unsigned int dwBufferLength; unsigned int dwBytesRecorded;\
			    void* dwUser; unsigned int dwFlags; unsigned int dwLoops;\
			    void* lpNext; void* reserved;\
			};struct SND_HANDLE{\
			    UWORD magic;\
			    void* os_handle;\
			    unsigned int sample_rate;\
			    unsigned char channels;\
			    unsigned char bits;\
			    float volume;\
			    float pitch;\
			    float* master_volume;\
			    bool playing;\
				unsigned int buffers_size;\
			    struct Buffer{ char* data; bool busy; WAVEHDR* meta; } buffers[2];\
			    int current_buffer;\
			    SND_HANDLE() : magic(0x534E444F), os_handle(nullptr),\
			                   sample_rate(44100), channels(2), bits(16), volume(1.0f), pitch(1.0f),\
			                   master_volume(nullptr), playing(false), buffers_size(0), current_buffer(0) {\
			        buffers[0] = {nullptr, false, nullptr};\
			        buffers[1] = {nullptr, false, nullptr};\
			    }\
			    ~SND_HANDLE(){\
			        magic = 0x00;\
			        if(buffers[0].data) delete[] buffers[0].data;\
			        if(buffers[0].meta) delete buffers[0].meta;\
			        if(buffers[1].data) delete[] buffers[1].data;\
			        if(buffers[1].meta) delete buffers[1].meta;\
			    }\
			}
			SND_HANDLE_DECL;
		    SND_HANDLE* h = new SND_HANDLE();
		    h->buffers_size = buffer_size;
		    h->buffers[0].data = new char[buffer_size];
		    h->buffers[0].meta = new WAVEHDR();
			h->buffers[0].meta->lpData = h->buffers[0].data;
			h->buffers[0].meta->dwBufferLength = buffer_size;
			h->buffers[0].meta->dwFlags = 0;
		    h->buffers[1].data = new char[buffer_size];
		    h->buffers[1].meta = new WAVEHDR();
			h->buffers[1].meta->lpData = h->buffers[1].data;
			h->buffers[1].meta->dwBufferLength = buffer_size;
			h->buffers[1].meta->dwFlags = 0;
		    return (void*)h;
			#else // Linux
			#define SND_HANDLE_DECL struct SND_HANDLE{\
		        UWORD magic;\
		        void* os_handle;\
		        unsigned int sample_rate;\
		        unsigned char channels;\
		        unsigned char bits;\
		        float volume;\
		        float pitch;\
		        float* master_volume;\
		        bool playing;\
				unsigned int buffers_size;\
		        struct Buffer{ char* data; bool busy; } buffers[2];\
		        int current_buffer;\
		        SND_HANDLE() : magic(0x534E444F), os_handle(nullptr),\
		                       sample_rate(44100), channels(2), bits(16), volume(1.0f), pitch(1.0f),\
		                       master_volume(nullptr), playing(false), buffers_size(0), current_buffer(0) {\
		            buffers[0] = {nullptr, false};\
		            buffers[1] = {nullptr, false};\
		        }\
		        ~SND_HANDLE(){\
		            magic = 0x00;\
		            if(buffers[0].data) delete[] buffers[0].data;\
		            if(buffers[1].data) delete[] buffers[1].data;\
		        }\
		    };
		    SND_HANDLE_DECL;
		    SND_HANDLE* h = new SND_HANDLE();
		    h->buffers_size = buffer_size;
		    h->buffers[0].data = new char[buffer_size];
		    h->buffers[1].data = new char[buffer_size];
		    return (void*)h;
			#endif
			#define TSN_HANDLE_DECL struct TSN_HANDLE{\
				UWORD magic;\
				void* snd;\
				void* file;\
				void* thr;\
				bool canIcloser;\
				bool autodestruction;\
				float target_volume;\
				float fade_step;\
				TSN_HANDLE() : magic(0x4E534854), snd(nullptr), file(nullptr), thr(nullptr), canIcloser(false), autodestruction(false),\
				               target_volume(1.0f), fade_step(0.0f) {}\
				~TSN_HANDLE(){ magic = 0x00; }\
			}
			#define TSN_HANDLE_UNVALID(ptr) ((ptr)?((*(UWORD*)(ptr))==0x4E534854):false)
		}
		void* create(unsigned int buffer_size, float* mst_vol){
			SND_HANDLE_DECL;
			SND_HANDLE* ret = (SND_HANDLE*)sound::create(buffer_size);
			if(mst_vol) ret->master_volume = mst_vol;
			return (void*)ret;
		}
		UWORD stream(void* snd, void* external_data, UWORD size){
		    if(SND_HANDLE_VALID(snd) || !external_data) return 0;
			SND_HANDLE_DECL;
		    SND_HANDLE* h = (SND_HANDLE*)snd;
		    if(!(h->os_handle)) return 0;
            float vol = (h->master_volume) ? ((h->volume) * (*(h->master_volume))) : (h->volume);
		    int target = -1;
		    if(!(h->buffers[0].busy)) target = 0;
		    else if(!(h->buffers[1].busy)) target = 1;
		    else return 0;
		    UWORD to_copy = (size > (h->buffers_size)) ? (h->buffers_size) : size;
		    char* dst = h->buffers[target].data;
		    UWORD _jump = (h->bits) >> 3;
		    float _volt_;
		    #if defined(_WIN32)
			typedef int(__stdcall *WPH)(void*,void*,unsigned int);
			static WPH pPrepare = (WPH)get_system_function("waveOutPrepareHeader", "winmm.dll");
			typedef int(__stdcall *WOW)(void*,void*,unsigned int);
			static WOW pWrite = (WOW)get_system_function("waveOutWrite", "winmm.dll");
		    if(!pPrepare || !pWrite) return 0;
			#else // Linux
			typedef long(*SPW)(void*,const void*,unsigned long);
            static SPW pPcmWrite = (SPW)get_system_function("snd_pcm_writei", "libasound.so.2");
            typedef int(*SPR)(void*);
            static SPR pPcmPrepare = (SPR)get_system_function("snd_pcm_prepare", "libasound.so.2");
            if(!pPcmWrite || !pPcmPrepare) return 0;
			#endif
		    for(UWORD i=0; i < to_copy; i += _jump){
		    	if(_jump == 1){
		    		_volt_ = (float)(*((char*)external_data + i)) * vol;
					*(dst + i) = (char)((_volt_ > -128) ? ((_volt_ < 127) ? _volt_ : 127) : -128);
				}
		    	if(_jump == 2){
		    		_volt_ = (float)(*(short*)((char*)external_data + i)) * vol;
					*((short*)(dst + i)) = (short)((_volt_ > -32768) ? ((_volt_ < 32767) ? _volt_ : 32767) : -32768);

				}
		    	if(_jump == 4){
		    		_volt_ = (float)(*(int*)((char*)external_data + i)) * vol;
					*((int*)(dst + i)) = (int)((_volt_ > -2147483648) ? ((_volt_ < 2147483647) ? _volt_ : 2147483647) : -2147483648);
				}
			}
		    #if defined(_WIN32)
		    h->buffers[target].busy = true;
			WAVEHDR* hdr = (WAVEHDR*)(h->buffers[target].meta);
			pPrepare(h->os_handle, hdr, sizeof(WAVEHDR));
			pWrite(h->os_handle, hdr, sizeof(WAVEHDR));
			#else // Linux
            unsigned long frames = to_copy / (h->channels * _jump);
            long written = pPcmWrite(h->os_handle, dst, frames);
            if(written < 0){
                pPcmPrepare(h->os_handle);
                h->buffers[target].busy = false;
                return 0;
            }
            h->buffers[target].busy = true;
			#endif
		    return to_copy;
		}
		bool play(void* snd){
			TSN_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)) return sound::play(((TSN_HANDLE*)snd)->snd);
			if(SND_HANDLE_VALID(snd)) return false;
			SND_HANDLE_DECL;
		    SND_HANDLE* h = (SND_HANDLE*)snd;
		    if(h->playing) return false;
		    #if defined(_WIN32)
		    struct _wfx_{
		        unsigned short format; unsigned short channels; unsigned int samplerate;
		        unsigned int bytespersec; unsigned short align; unsigned short bits; unsigned short size;
		    };
		    typedef int(__stdcall *WOO)(void**,unsigned int,void*,void*,void*,unsigned int);
		    static WOO pWaveOutOpen = (WOO)get_system_function("waveOutOpen", "winmm.dll");
		    if(!pWaveOutOpen) return false;
		    unsigned int effective_rate = (unsigned int)((float)h->sample_rate * h->pitch);
            unsigned short block_align = (unsigned short)(h->channels * (h->bits >> 3));
            _wfx_ wfx = {
                1,                                  // WAVE_FORMAT_PCM
                h->channels, 
                effective_rate,                     // Frecuencia alterada
                effective_rate * block_align,       // Bytes por segundo ajustados
                block_align,                        // Alineacion de bloque
                h->bits,
                0
            };
		    if(!pWaveOutOpen(&(h->os_handle), 0xFFFFFFFF, &wfx, nullptr, nullptr, 0)) return ((h->playing) = true);
		    return false;
			#else // Linux
			typedef int (*SPO)(void**,const char*,int,int);
			static SPO pPcmOpen = (SPO)get_system_function("snd_pcm_open", "libasound.so.2");
			typedef int (*SPS)(void*,int,int,int,unsigned int,int,unsigned int);
			static SPS pPcmSetParams = (SPS)get_system_function("snd_pcm_set_params", "libasound.so.2");
			if(!pPcmOpen || !pPcmSetParams) return false;
			if(pPcmOpen(&(h->os_handle), "default", 0, 0) < 0) return false;
			int alsa_fmt = (h->bits == 8) ? 1 : (h->bits == 16) ? 2 : (h->bits == 32) ? 6 : 2;
			unsigned int effective_rate = (unsigned int)((float)h->sample_rate * h->pitch);
			if(pPcmSetParams(h->os_handle, alsa_fmt, 3, h->channels, effective_rate, 1, 150000) < 0){
				typedef int (*SPC)(void*);
				static SPC pPcmClose = (SPC)get_system_function("snd_pcm_close", "libasound.so.2");
				if(pPcmClose) pPcmClose(h->os_handle);
				h->os_handle = nullptr;
				return false;
			}
			return (h->playing = true);
		    #endif
		}
		void stop(void* snd){
			TSN_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)){
				sound::stop(((TSN_HANDLE*)snd)->snd);
				return;
			}
			if(SND_HANDLE_VALID(snd)) return;
			SND_HANDLE_DECL;
		    SND_HANDLE* h = (SND_HANDLE*)snd;
		    if(!(h->playing)) return;
		    #if defined(_WIN32)
		    typedef int(__stdcall *WOC)(void*);
		    static WOC pWaveOutClose = (WOC)get_system_function("waveOutClose", "winmm.dll");
		    if(!pWaveOutClose) return;
			pWaveOutClose(h->os_handle);
		    h->playing = false;
		    h->os_handle = nullptr;
			#else // Linux
			typedef int(*SPC)(void*);
			static SPC pPcmClose = (SPC)get_system_function("snd_pcm_close", "libasound.so.2");
			static SPC pPcmDrop  = (SPC)get_system_function("snd_pcm_drop", "libasound.so.2");
			if(!pPcmDrop || !pPcmClose) return;
			if(h->os_handle){ pPcmDrop(h->os_handle); pPcmClose(h->os_handle); }
			h->playing = false;
			h->os_handle = nullptr;
			h->buffers[0].busy = false;
			h->buffers[1].busy = false;
		    #endif
		}
		void destroy(void* snd){
			TSN_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)){
				TSN_HANDLE* tsn_h = (TSN_HANDLE*)snd;
				tsn_h->autodestruction = true;
				delete tsn_h;
				return;
			}
			if(SND_HANDLE_VALID(snd)) return;
		    sound::stop(snd);
			SND_HANDLE_DECL;
		    delete (SND_HANDLE*)snd;
		}
		bool sync(void* snd){
			TSN_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)) return sound::sync(((TSN_HANDLE*)snd)->snd);
		    if(SND_HANDLE_VALID(snd)) return false;
			SND_HANDLE_DECL;
		    SND_HANDLE* h = (SND_HANDLE*)snd;
		    if(!(h->playing)) return true;
		    #if defined(_WIN32)
		    WAVEHDR* hdr = (WAVEHDR*)(h->buffers[0].meta);
		    if((h->buffers[0].busy) && ((hdr->dwFlags) & 0x00000001)){
		        h->buffers[0].busy = false;
		        hdr->dwFlags &= ~0x00000001;
		    }
		    hdr = (WAVEHDR*)(h->buffers[1].meta);
		    if((h->buffers[1].busy) && ((hdr->dwFlags) & 0x00000001)){
		        h->buffers[1].busy = false;
		        hdr->dwFlags &= ~0x00000001;
		    }
			#else // Linux
			typedef long(*SFA)(void*);
            static SFA pPcmAvail = (SFA)get_system_function("snd_pcm_avail_update", "libasound.so.2");
			if(!pPcmAvail || !(h->os_handle)) return true;
            long avail = pPcmAvail(h->os_handle);
            unsigned int bytes_per_frame = h->channels * (h->bits >> 3);
            unsigned int avail_bytes = (unsigned int)(avail * bytes_per_frame);
            if((h->buffers[0].busy) && avail_bytes >= (h->buffers[0].size)) h->buffers[0].busy = false;
            if((h->buffers[1].busy) && avail_bytes >= (h->buffers[1].size)) h->buffers[1].busy = false;
		    #endif
            return (!(h->buffers[0].busy) || !(h->buffers[1].busy));
		}
		void set(void* snd, unsigned int rate, unsigned char channels, unsigned char bits){
			TSN_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)) return sound::set(((TSN_HANDLE*)snd)->snd, rate, channels, bits);
            if(SND_HANDLE_VALID(snd)) return;
            SND_HANDLE_DECL;
            SND_HANDLE* h = (SND_HANDLE*)snd;
            if(h->playing) return; 
            if(rate)     h->sample_rate = rate;
            if(channels) h->channels    = channels;
            if(bits)     h->bits        = bits;
        }
		bool playing(void* snd){
			SND_HANDLE_DECL;
			TSN_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)) return ((SND_HANDLE*)(((TSN_HANDLE*)snd)->snd))->playing;
			if(SND_HANDLE_VALID(snd)) return false;
			return ((SND_HANDLE*)snd)->playing;
		}
		bool is_threaded(void* snd){
			if(TSN_HANDLE_UNVALID(snd)) return true;
			return false;
		}
		bool set_volume(void* snd, float vol){
			TSN_HANDLE_DECL;
			SND_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)){
				((SND_HANDLE*)(((TSN_HANDLE*)snd)->snd))->volume = vol;
				((TSN_HANDLE*)snd)->target_volume = vol;
				((TSN_HANDLE*)snd)->fade_step = 0.0f;
				return true;
			}
			if(SND_HANDLE_VALID(snd)) return false;
			((SND_HANDLE*)snd)->volume = vol;
			return true;
		}
		bool volume(void* snd, float* vol){
			if(!vol) return false;
			*vol = 0.0f;
			TSN_HANDLE_DECL;
			SND_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)){
				*vol = ((SND_HANDLE*)(((TSN_HANDLE*)snd)->snd))->volume;
				return true;
			}
			if(SND_HANDLE_VALID(snd)) return false;

			*vol = ((SND_HANDLE*)snd)->volume;
			return true;
		}
		float volume(void* snd){
			float vol;
			volume(snd, &vol);
			return vol;
		}
		void set_pitch(void* snd, float pitch){
			TSN_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)){
				set_pitch(((TSN_HANDLE*)snd)->snd, pitch);
				return;
			}
		    if(SND_HANDLE_VALID(snd)) return;
		    SND_HANDLE_DECL;
		    SND_HANDLE* h = (SND_HANDLE*)snd;
		    h->pitch = pitch;
		    if((h->playing) && (h->os_handle)){
		    	#if defined(_WIN32)
		        typedef int(__stdcall *WSP)(void*, unsigned int);
		        static WSP pSetPlaybackRate = (WSP)get_system_function("waveOutSetPlaybackRate", "winmm.dll");
		        if(!pSetPlaybackRate) return;
		        pSetPlaybackRate(h->os_handle, (unsigned int)(pitch * 0x00010000));
		        #else // Linux
                typedef int (*SPS)(void*, int, int, int, unsigned int, int, unsigned int);
                static SPS pPcmSetParams = (SPS)get_system_function("snd_pcm_set_params", "libasound.so.2");
                if(!pPcmSetParams) return;
                int alsa_fmt = (h->bits == 8) ? 1 : (h->bits == 16) ? 2 : (h->bits == 32) ? 6 : 2;
                unsigned int effective_rate = (unsigned int)((float)h->sample_rate * h->pitch);
                pPcmSetParams(h->os_handle, alsa_fmt, 3, h->channels, effective_rate, 1, 100000);
		        #endif
			}
		}
		bool pitch(void* snd, float* pit){
			if(!pit) return false;
			*pit = 0.0f;
			TSN_HANDLE_DECL;
			SND_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)){
				*pit = ((SND_HANDLE*)(((TSN_HANDLE*)snd)->snd))->pitch;
				return true;
			}
			if(SND_HANDLE_VALID(snd)) return false;
			*pit = ((SND_HANDLE*)snd)->pitch;
			return true;
		}
		float pitch(void* snd){
			float pit;
			pitch(snd, &pit);
			return pit;
		}
		bool set_master_volume(void* snd, float* mst_vol){
			TSN_HANDLE_DECL;
			SND_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)){
				((SND_HANDLE*)(((TSN_HANDLE*)snd)->snd))->master_volume = mst_vol;
				return true;
			}
			if(SND_HANDLE_VALID(snd)) return false;
			((SND_HANDLE*)snd)->master_volume = mst_vol;
			return true;
		}
		bool master_volume(void* snd, float** mst_vol){
			if(!mst_vol) return false;
			*mst_vol = nullptr;
			TSN_HANDLE_DECL;
			SND_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)){
				*mst_vol = ((SND_HANDLE*)(((TSN_HANDLE*)snd)->snd))->master_volume;
				return true;
			}
			if(SND_HANDLE_VALID(snd)) return false;
			*mst_vol = ((SND_HANDLE*)snd)->master_volume;
			return true;
		}
		float* master_volume(void* snd){
			float* mst_vol;
			master_volume(snd, &mst_vol);
			return mst_vol;
		}
		#if defined(ST_IMPORT_FILE)
		bool reload(void* th_snd){
			TSN_HANDLE_DECL;
			if(!TSN_HANDLE_UNVALID(th_snd)) return false;
			void* file_h = ((TSN_HANDLE*)th_snd)->file;
			file::seek(file_h, 44, seek_mode::begin);
			return true;
		}
		void* file_stream(const char* filepath, bool looping, float pitch){
			void* file_h = file::open(filepath, file_mode::read);
			void* ret = file_stream(file_h, looping, pitch, true);
			if(!ret) file::close(file_h);
			return ret;
		}
		bool fade(void* th_snd, float target_volume, UWORD ms){
			if(!TSN_HANDLE_UNVALID(th_snd)) return false;
			TSN_HANDLE_DECL;
		    SND_HANDLE_DECL;
			TSN_HANDLE* h = (TSN_HANDLE*)th_snd;
		    SND_HANDLE* hs = (SND_HANDLE*)(h->snd);
			h->target_volume = target_volume;
			float diff = target_volume - hs->volume;
		    float steps = (float)ms / 50.0f;
		    if(steps < 1.0f) steps = 1.0f;
		    h->fade_step = diff / steps;
			return true;
		}
		float get_position(void* th_snd){
		    if(!TSN_HANDLE_UNVALID(th_snd)) return 0.0f;
			TSN_HANDLE_DECL;
			SND_HANDLE_DECL;
		    TSN_HANDLE* thsn = (TSN_HANDLE*)th_snd;
		    void* wav = thsn->file;
		    SND_HANDLE* h = (SND_HANDLE*)(thsn->snd);
		    if(!wav || !h) return 0.0f;
		    long current_pos = file::tell(wav);
		    long data_pos = (current_pos > 44) ? (current_pos - 44) : 0;
		    float bytes_per_sec = (float)(h->sample_rate * h->channels * (h->bits >> 3));
		    float position = (float)data_pos / bytes_per_sec;
			float buffer_latency = (float)(h->buffers_size << 1) / bytes_per_sec;
			return position - buffer_latency;
		}
		float get_duration(void* th_snd){
		    if(!TSN_HANDLE_UNVALID(th_snd)) return 0.0f;
		    TSN_HANDLE_DECL;
			SND_HANDLE_DECL;
		    TSN_HANDLE* thsn = (TSN_HANDLE*)th_snd;
		    SND_HANDLE* h = (SND_HANDLE*)(thsn->snd);
		    unsigned long total_size = file::size(thsn->file);
		    unsigned long data_size = (total_size > 44) ? (total_size - 44) : 0;
		    float bytes_per_sec = (float)(h->sample_rate * h->channels * (h->bits >> 3));
		    return (float)data_size / bytes_per_sec;
		}
		bool seek(void* th_snd, float seconds){
			if(!TSN_HANDLE_UNVALID(th_snd)) return false;
			TSN_HANDLE_DECL;
			SND_HANDLE_DECL;
		    TSN_HANDLE* thsn = (TSN_HANDLE*)th_snd;
		    void* wav = thsn->file;
		    SND_HANDLE* h = (SND_HANDLE*)(thsn->snd);
		    if(!wav || !h) return false;
		    unsigned int bytes_per_sec = h->sample_rate * h->channels * (h->bits >> 3);
		    unsigned long target_byte = 44 + (unsigned long)(seconds * (float)bytes_per_sec);
		    if(target_byte >= file::size(wav)) return false;
		    file::seek(wav, target_byte, seek_mode::begin);
		    return true;
		}
		bool get_spectrum(void* th_snd, float** buffer, UWORD* samples, float* average){
			if(!TSN_HANDLE_UNVALID(th_snd) || !buffer || !samples) return false;
			TSN_HANDLE_DECL;
			SND_HANDLE_DECL;
		    TSN_HANDLE* thsn = (TSN_HANDLE*)th_snd;
		    void* wav = thsn->file;
		    SND_HANDLE* h = (SND_HANDLE*)(thsn->snd);
		    if(!wav || !h) return false;
		    int current = h->current_buffer;
		    char _jump = (h->bits) >> 3;
		    char* raw_data = (char*)(h->buffers[current].data);
		    UWORD buffer_size = h->buffers_size;
		    UWORD total_samples = buffer_size / _jump;
		    if(*buffer) delete[] *buffer; *buffer = new float[total_samples];
		    *samples = total_samples;
		    float _avrg = 0.0f;
		    for(UWORD i = 0, j = 0; i < buffer_size; i+=_jump){
		    	if(_jump == 1) (*buffer)[j] = (float)raw_data[i] / 127.0f;
		    	if(_jump == 2) (*buffer)[j] = (float)(*(short*)(raw_data + i)) / 32767.0f;
		    	if(_jump == 4) (*buffer)[j] = (float)(*(int*)(raw_data + i)) / 2147483647.0f;
		    	if(_avrg < (*buffer)[j]) _avrg = (*buffer)[j];
		    	++j;
			}
			if(average) *average = _avrg;
		    return true;
		}
		#endif
	}
    #endif
    #if defined(ST_IMPORT_WINDOW)
    namespace window{
		void* create(void* father, const char* title, unsigned int width, unsigned int height, UWORD(*mailman)(void*,UWORD,UWORD,WORD)){
			#define WND_HANDLE_VALID(wnd) ((wnd)?(*(UWORD*)(wnd)!=0x574E444F):true)
			if(!width || !height || !mailman || (father && WND_HANDLE_VALID(father))) return nullptr;
			#if defined(_WIN32)
			#define WND_HANDLE_DECL struct WNDCLASS{\
	            unsigned int style;\
				void* lpfnWndProc;\
				int cbClsExtra;\
				int cbWndExtra;\
	            void* hInstance;\
				void* hIcon;\
				void* hCursor;\
				void* hbrBackground;\
	            const char* lpszMenuName;\
				const char* lpszClassName;\
	        };struct WND_HANDLE{\
	        	UWORD magic;\
	        	WNDCLASS wc;\
	        	const char* className;\
	        	void* hwnd;\
	        	WND_HANDLE* father;\
	        	void* MSGthr;\
	        	int x, y, width, height;\
	        	unsigned int* pixels;\
	        	UWORD pixels_amont;\
	        	bool minimized;\
	        	bool invisible;\
	        	bool ext;\
	        	WND_HANDLE() : magic(0x574E444F), wc({}),\
					className(nullptr), hwnd(nullptr), father(nullptr), MSGthr(nullptr), x(0), y(0), width(0), height(0),\
					pixels(nullptr), pixels_amont(0), minimized(false), invisible(false), ext(false) {}\
            	~WND_HANDLE(){ magic = 0x00; }\
			}
            #define WND_HANDLE_STATE(handle) ((handle)->hwnd != nullptr)
			WND_HANDLE_DECL;
			struct BITMAPINFO{
	            struct{
	                unsigned long  biSize;
	                long           biWidth;
	                long           biHeight;
	                unsigned short biPlanes;
	                unsigned short biBitCount;
	                unsigned long  biCompression;
	                unsigned long  biSizeImage;
	                long           biXPelsPerMeter;
	                long           biYPelsPerMeter;
	                unsigned long  biClrUsed;
	                unsigned long  biClrImportant;
	            } bmiHeader;
	            unsigned long bmiColors[1];
	        };
			struct RECT{ int left, top, right, bottom; };
			struct _ap_{
				struct MetaWin{
					UWORD(*mailman)(void*,UWORD,UWORD,WORD);
					void* ret;
				};
				static long long __stdcall st_InternalWndProc(void* hwnd, unsigned int msg, unsigned long long wp, long long lp){
				    typedef WORD(__stdcall *GWL)(void*,int);
				    #if defined(_WIN64) || defined(__x86_64__) || defined(__x86_64)
				    static GWL pGetLong = (GWL)get_system_function("GetWindowLongPtrA", "user32.dll");
				    #else
				    static GWL pGetLong = (GWL)get_system_function("GetWindowLongA", "user32.dll");
				    #endif
					MetaWin* mail = pGetLong ? (MetaWin*)pGetLong(hwnd, -21) : nullptr;
				    typedef long long(__stdcall *DWP)(void*,unsigned int,unsigned long long,long long);
				    static DWP pDef = (DWP)get_system_function("DefWindowProcA", "user32.dll");
				    typedef int(__stdcall *DWF)(void*);
				    static DWF pDestroyWindow = (DWF)get_system_function("DestroyWindow", "user32.dll");
				    typedef void*(__stdcall *GDC)(void*);
				    static GDC pGetDC = (GDC)get_system_function("GetDC", "user32.dll");
				    typedef int(__stdcall *RDC)(void*,void*);
				    static RDC pReleaseDC = (RDC)get_system_function("ReleaseDC", "user32.dll");
				    typedef int(__stdcall *SDIB)(void*,int,int,int,int,int,int,int,int,const void*,const void*,unsigned int,unsigned long);
				    static SDIB pStretchDIBits = (SDIB)get_system_function("StretchDIBits", "gdi32.dll");
				    typedef int(__stdcall *GCR)(void*,RECT*);
					static GCR pGetClientRect = (GCR)get_system_function("GetClientRect", "user32.dll");
				    typedef long long(__stdcall *SM)(void*,unsigned int,unsigned long long,long long);
		    		static SM pSendMessage = (SM)get_system_function("SendMessageA", "user32.dll");
				    typedef long long(__stdcall *VR)(void*,void*);
		    		static VR pValidateRect = (VR)get_system_function("ValidateRect", "user32.dll");
					if(!mail) return pDef ? pDef(hwnd, msg, wp, lp) : -1;
					WND_HANDLE* handle = (WND_HANDLE*)(mail->ret);
				    if(msg == 0x0010){ if(pDestroyWindow) pDestroyWindow(hwnd); return 0; }
				    if(msg == 0x0002){
				    	THR_HANDLE_DECL;
				    	if(handle->MSGthr) delete (THR_HANDLE*)(handle->MSGthr); handle->MSGthr = nullptr;
				    	return 0;
				    }
				    if(msg == 0x0005 && (lp & 0xFFFF) != 0 && ((lp >> 16) & 0xFFFF) != 0){
				        int _old_width  = handle->width;
				        int _old_height = handle->width;
				        handle->width  = (int)(lp & 0xFFFF);
				        handle->height = (int)((lp >> 16) & 0xFFFF);
				        if(handle->pixels && (_old_width != handle->width || _old_height != handle->height)){
					        UWORD _new_pixels_amont = (handle->width) * (handle->height);
					        unsigned int* _new_pixels = new unsigned int[_new_pixels_amont]{0};
					        int copy_w = (_old_width < handle->width) ? _old_width : handle->width;
							int copy_h = (_old_height < handle->height) ? _old_height : handle->height;
							unsigned int* _n_p = _new_pixels;
							unsigned int* _o_p = handle->pixels;
							while(copy_h--){
								for(int col = 0; col < copy_w; ++col) _n_p[col] = _o_p[col];
								_n_p += handle->width;
								_o_p += _old_width;
							}
							delete[] handle->pixels;
							handle->pixels = _new_pixels;
							handle->pixels_amont = _new_pixels_amont;
						}
						if(handle->minimized){
							pSendMessage(handle->hwnd, 0x000F, 0, 0);
							handle->minimized = false;
						}
				    }
				    if(msg == 0x0005 && ((lp & 0xFFFF) == 0 || ((lp >> 16) & 0xFFFF) == 0)) handle->minimized = true;
				    if(msg == 0x0003){
			            handle->x = (int)(lp & 0xFFFF);
			            handle->y = (int)((lp >> 16) & 0xFFFF);
			        }
				    if(msg == 0x0014) return 1;
					UWORD ret = mail->mailman(mail->ret, (UWORD)msg, (UWORD)wp, (WORD)lp);
					if(msg == 0x000F && pGetDC && pReleaseDC && pStretchDIBits && pGetClientRect){
						if(!handle->pixels) return 0;
						void* hdc = pGetDC(handle->hwnd);
						RECT client; pGetClientRect(handle->hwnd, &client);
						BITMAPINFO bmi = {};
						bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
						bmi.bmiHeader.biWidth = handle->width;
						bmi.bmiHeader.biHeight = -(long)(handle->height); 
						bmi.bmiHeader.biPlanes = 1;
						bmi.bmiHeader.biBitCount = 32;
						bmi.bmiHeader.biCompression = 0;
						pStretchDIBits(
							hdc,
							0, 0, client.right, client.bottom,
						    0, 0, handle->width, handle->height,
						    handle->pixels,
							&bmi, 0, 0x00CC0020
						);
						pReleaseDC(handle->hwnd, hdc);
						if(pValidateRect) pValidateRect(hwnd, nullptr);
						return 0;
					}
					return (ret == 0) ? (pDef ? pDef(hwnd, msg, wp, lp) : -1) : ((long long)ret);
				}
				struct MetaMessage{
					WND_HANDLE* ret;
					WND_HANDLE* father;
					volatile bool ready;
					const char* title;
					int width, height;
					UWORD(*mailman)(void*,UWORD,UWORD,WORD);
				};
				static void dispatch(MetaMessage* meta){
				    THR_HANDLE_DECL;
				    struct MSG{
				        void* hwnd;
				        UWORD  message;
				        UWORD  wParam;
				        WORD   lParam;
				        unsigned long time;
				        struct{ int x, y; } pt;
				    } msg;
				    typedef int(__stdcall *PMF)(MSG*,void*,UWORD,UWORD,UWORD);
					static PMF pPeekMessage = (PMF)get_system_function("PeekMessageA", "user32.dll");
				    typedef int(__stdcall *TMF)(const MSG*);
				    static TMF pTranslateMessage = (TMF)get_system_function("TranslateMessage", "user32.dll");
				    typedef int(__stdcall *DMF)(const MSG*);
				    static DMF pDispatchMessage = (DMF)get_system_function("DispatchMessageA", "user32.dll");
					typedef void*(__stdcall* GMH)(const char*);
					static GMH pGetModuleHandle = (GMH)get_system_function("GetModuleHandleA");
					typedef unsigned short(__stdcall *REG)(const WNDCLASS*);
				    static REG pRegisterClass = (REG)get_system_function("RegisterClassA", "user32.dll");
				    typedef unsigned long(__stdcall *GLE)();
					static GLE pGetLastError = (GLE)get_system_function("GetLastError");
					typedef void*(__stdcall *CWE)(unsigned long,const char*,const char*,unsigned long,int,int,int,int,void*,void*,void*,void*);
		            static CWE pCreate = (CWE)get_system_function("CreateWindowExA", "user32.dll");
				    typedef WORD(__stdcall *SWL)(void*,int,WORD);
				    #if defined(_WIN64)
				    static SWL pSetLong = (SWL)get_system_function("SetWindowLongPtrA", "user32.dll");
				    #else
				    static SWL pSetLong = (SWL)get_system_function("SetWindowLongA", "user32.dll");
				    #endif
				    typedef int(__stdcall *SW)(void*,int);
				    static SW pShow = (SW)get_system_function("ShowWindow", "user32.dll");
				    typedef int(__stdcall *UW)(void*);
				    static UW pUpdate = (UW)get_system_function("UpdateWindow", "user32.dll");
				    typedef int(__stdcall *AWR)(struct RECT*, unsigned long, int);
					static AWR pAdjustWindowRect = (AWR)get_system_function("AdjustWindowRect", "user32.dll");
					if(!pGetModuleHandle || !pRegisterClass || !pGetLastError || !pCreate || !pSetLong || !pShow || !pUpdate || !pAdjustWindowRect) return;
					meta->ret = new WND_HANDLE;
					WND_HANDLE* hWnd = meta->ret;
		            int w = meta->width;
					int h = meta->height;
					const char* t = meta->title ? meta->title : "";
					WND_HANDLE* f = meta->father;
					RECT rect = { 0, 0, w, h };
					void* hInst = pGetModuleHandle(nullptr);
					hWnd->className = "ST_WINDOW";
					hWnd->wc.lpfnWndProc = (void*)(_ap_::st_InternalWndProc);
				    hWnd->wc.hInstance = hInst;
				    hWnd->wc.lpszClassName = hWnd->className;
				    hWnd->wc.hbrBackground = (void*)8; // BLACK
				    pRegisterClass(&(hWnd->wc));
					if(pAdjustWindowRect) pAdjustWindowRect(&rect, 0x00CF0000, 0);
					int final_w = rect.right - rect.left;
					int final_h = rect.bottom - rect.top;
					hWnd->hwnd = pCreate(
					    0,
					    hWnd->className,
					    t,
					    0x00CF0000,
					    0x80000000, 0x80000000, final_w, final_h,
					    nullptr,
					    nullptr,
					    hInst,
					    nullptr
					);
		            if(!(hWnd->hwnd)){
				    	delete hWnd; meta->ret = nullptr;
						meta->ready = true;
		            	return;
		            }
					hWnd->pixels_amont = w * h;
					hWnd->pixels = new unsigned int[hWnd->pixels_amont]();
					hWnd->father = f;
					MetaWin* mema = new MetaWin{ meta->mailman, (void*)hWnd };
				    pSetLong(hWnd->hwnd, -21, (WORD)mema);
					pShow(hWnd->hwnd, 1);
					pUpdate(hWnd->hwnd);
					hWnd->x = 100;
					hWnd->y = 100;
					hWnd->width = meta->width;
					hWnd->height = meta->height;
					meta->ready = true;
					while(!(hWnd->MSGthr)) sleep(10);
					while(true){
					    if(pPeekMessage(&msg, hWnd->hwnd, 0, 0, 0x0001)){
					    	if(msg.message == 0x0012) break;
					        pTranslateMessage(&msg);
					        pDispatchMessage(&msg);
					    }else{
							if(!(hWnd->MSGthr)) break;
							if(hWnd->father ? (!(hWnd->father->MSGthr)) : false) break;
							sleep(10);
						}
					}
				    pSetLong(hWnd->hwnd, -21, 0);
					delete mema;
				    if(hWnd->pixels) delete[] hWnd->pixels; hWnd->pixels = nullptr;
					if(hWnd->MSGthr) delete (THR_HANDLE*)(hWnd->MSGthr); hWnd->MSGthr = nullptr;
				    hWnd->hwnd = nullptr;
				}
			};
			_ap_::MetaMessage* msg = new _ap_::MetaMessage{
				nullptr,
				(WND_HANDLE*)father,
				false,
				title,
				(int)width,
				(int)height,
				mailman
			};
			void* thr = thread::create(_ap_::dispatch, msg);
			while(!(msg->ready)) sleep(10);
			if(msg->ret) msg->ret->MSGthr = thr;
			else         thread::join(thr);
			WND_HANDLE* handle = msg->ret;
			delete msg;
            return (void*)handle;
            #else // Linux
            #define WND_HANDLE_DECL struct WND_HANDLE{\
            	UWORD magic;\
            	void* display;\
            	int scr;\
            	unsigned long root;\
            	void* visual;\
            	unsigned long window;\
            	void* gc;\
            	void* image;\
            	WND_HANDLE* father;\
	        	void* MSGthr;\
	        	int x, y, width, height;\
	        	unsigned int* pixels;\
	        	UWORD pixels_amont;\
	        	bool minimized;\
	        	bool invisible;\
	        	bool ext;\
            	WND_HANDLE() : magic(0x574E444F), display(nullptr), scr(0), root(0), visual(nullptr),\
            		window(0), gc(nullptr), image(nullptr), father(nullptr), MSGthr(nullptr), x(0), y(0),\
            		width(0), height(0), pixels(nullptr), pixels_amont(0), minimized(false),\
            		invisible(false), ext(false) {}\
            	~WND_HANDLE(){ magic = 0x00; }\
            }
            #define WND_HANDLE_STATE(handle) ((handle)->display && (handle)->window)
            WND_HANDLE_DECL;
            typedef void*(*XOD)(const char*);
            static XOD pXOpenDisplay = (XOD)get_system_function("XOpenDisplay", "libX11.so.6");
            typedef unsigned long(*XCW)(void*,unsigned long,int,int,unsigned int,unsigned int,unsigned int,int,unsigned int,void*,unsigned long,void*);
            static XCW pXCreateWindow = (XCW)get_system_function("XCreateWindow", "libX11.so.6");
            typedef int(*XSM)(void*,unsigned long,unsigned long);
            static XSM pXSelectInput = (XSM)get_system_function("XSelectInput", "libX11.so.6");
            typedef int(*XMAP)(void*,unsigned long);
            static XMAP pXMapWindow = (XMAP)get_system_function("XMapWindow", "libX11.so.6");
            typedef void*(*CXGC)(void*,unsigned long,unsigned long,void*);
			static CXGC pXCreateGC = (CXGC)get_system_function("XCreateGC", "libX11.so.6");
			typedef int(*XDS)(void*);
			static XDS pXDefaultScreen = (XDS)get_system_function("XDefaultScreen", "libX11.so.6");
			typedef int(*DDM)(void*,int);
			static DDM pXSync = (DDM)get_system_function("XSync", "libX11.so.6");
			typedef int(*XST)(void*, unsigned long, const char*);
			static XST pXStoreName = (XST)get_system_function("XStoreName", "libX11.so.6");
			typedef unsigned long(*XRS)(void*);
			static XRS pXRootWindow = (XRS)get_system_function("XDefaultRootWindow", "libX11.so.6");
			typedef void*(*XDV)(void*,int);
			static XDV pXDefaultVisual = (XDV)get_system_function("XDefaultVisualOfScreen", "libX11.so.6");
			typedef int(*SXWP)(void*,unsigned long,unsigned long*,int);
			static SXWP pXSetWMProtocols = (SXWP)get_system_function("XSetWMProtocols", "libX11.so.6");
			typedef unsigned long(*XIA)(void*,const char*,int);
			static XIA pXInternAtom = (XIA)get_system_function("XInternAtom", "libX11.so.6");
            if(!pXOpenDisplay || !pXCreateWindow || !pXSelectInput || !pXMapWindow || !pXCreateGC
            	|| !pXDefaultScreen || !pXRootWindow || !pXDefaultVisual || !pXSync || !pXStoreName || !pXInternAtom) return nullptr;
            WND_HANDLE* handle = new WND_HANDLE;
            handle->display = pXOpenDisplay(nullptr);
			if(!(handle->display)){
				delete handle;
				return nullptr;
			}
			handle->x = 100;
			handle->y = 100;
			handle->width  = width;
			handle->height = height;
			handle->father = (WND_HANDLE*)father;
			handle->scr = pXDefaultScreen(handle->display);
			handle->root = pXRootWindow(handle->display);
			handle->visual = pXDefaultVisual(handle->display, handle->scr);
			handle->window = pXCreateWindow(
				handle->display, handle->root, 
				handle->x, handle->y, handle->width, handle->height,
				0, 0, 1, nullptr, 0, nullptr
			);
			handle->gc = pXCreateGC(handle->display, handle->window, 0, nullptr);
			handle->pixels_amont = width * height;
			handle->pixels = new unsigned int[handle->pixels_amont];
			struct _ap_{
				#define XEvent_DECL struct MSGpack{\
					WND_HANDLE* handle;\
					UWORD(*mailman)(void*,UWORD,UWORD,WORD);\
					bool ready;\
				};struct XAnyEvent{\
					int type;\
					unsigned long serial;\
					int send_event;\
					void* display;\
					unsigned long window;\
				};struct XConfigureEvent{\
					int type;\
					unsigned long serial;\
					int send_event;\
					void* display;\
					unsigned long event;\
					unsigned long window;\
					int x, y;\
					int width, height;\
					int border_width;\
					unsigned long above;\
					int override_redirect;\
				};struct XClientMessageEvent{\
					int type;\
					unsigned long serial;\
					int send_event;\
					void* display;\
					unsigned long window;\
					unsigned long message_type;\
					int format;\
					union{\
						char b[20];\
						short s[10];\
						long l[5];\
					} data;\
				};struct XKeyEvent{\
					int type; unsigned long serial; int send_event; void* display;\
					unsigned long window; unsigned long root; unsigned long subwindow;\
					unsigned long time; int x, y; int x_root, y_root;\
					unsigned int state; unsigned int keycode; int same_screen;\
				};struct XButtonEvent{\
					int type; unsigned long serial; int send_event; void* display;\
					unsigned long window; unsigned long root; unsigned long subwindow;\
					unsigned long time; int x, y; int x_root, y_root;\
					unsigned int state; unsigned int button; int same_screen;\
				};struct XMotionEvent{\
					int type; unsigned long serial; int send_event; void* display;\
					unsigned long window; unsigned long root; unsigned long subwindow;\
					unsigned long time; int x, y; int x_root, y_root;\
					unsigned int state; char is_hint; int same_screen;\
				};struct XPropertyEvent{\
					int type; unsigned long serial; int send_event; void* display;\
					unsigned long window; unsigned long atom; unsigned long time; int state;\
				};struct XEvent{union{\
					int type;\
					XAnyEvent xany;\
					XKeyEvent xkey;\
					XButtonEvent xbutton;\
					XMotionEvent xmotion;\
					XConfigureEvent xconfigure;\
					XClientMessageEvent xclient;\
					XPropertyEvent xproperty;\
					long pad[24];\
				};}
				XEvent_DECL;
				static void dispach(MSGpack* msg){
					WND_HANDLE* handle = msg->handle;
					UWORD(*mailman)(void*,UWORD,UWORD,WORD) = msg->mailman;
					typedef int(*XNE)(void*,XEvent*);
					static XNE pXNextEvent = (XNE)get_system_function("XNextEvent", "libX11.so.6");
				    typedef void*(*XCI)(void*,void*,unsigned int,int,int,char*,unsigned int,unsigned int,int,int);
					static XCI pXCreateImage = (XCI)get_system_function("XCreateImage", "libX11.so.6");
					typedef int(*XPI)(void*,unsigned long,void*,void*,int,int,int,int,unsigned int,unsigned int);
					static XPI pXPutImage = (XPI)get_system_function("XPutImage", "libX11.so.6");
					typedef int(*XFR)(void*);
					static XFR pXFree = (XFR)get_system_function("XFree", "libX11.so.6");
					typedef int(*XFRGC)(void*,void*);
					static XFRGC pXFreeGC = (XFRGC)get_system_function("XFreeGC", "libX11.so.6");
					typedef unsigned long(*XIA)(void*,const char*,int);
					static XIA pXInternAtom = (XIA)get_system_function("XInternAtom", "libX11.so.6");
					typedef int(*XDW)(void*,unsigned long);
					static XDW pXDestroyWindow = (XDW)get_system_function("XDestroyWindow", "libX11.so.6");
					typedef int(*XCD)(void*);
					static XCD pXCloseDisplay = (XCD)get_system_function("XCloseDisplay", "libX11.so.6");
					typedef int(*XGP)(void*,unsigned long,unsigned long,long,long,int,unsigned long,unsigned long*,
						int*,unsigned long*,unsigned long*,unsigned char**);
					static XGP pXGetWindowProperty = (XGP)get_system_function("XGetWindowProperty", "libX11.so.6");
					typedef int(*XWP)(void*, unsigned long, unsigned long, int, int, unsigned int, unsigned int, int, int);
					static XWP pXWarpPointer = (XWP)get_system_function("XWarpPointer", "libX11.so.6");
					typedef int(*XFL)(void*);
					static XFL pXFlush = (XFL)get_system_function("XFlush", "libX11.so.6");
					if(!pXNextEvent || !pXCreateImage || !pXPutImage || !pXFree || !pXInternAtom
						|| !pXDestroyWindow || !pXCloseDisplay || !pXFreeGC || !pXGetWindowProperty
						|| !pXWarpPointer || !pXFlush){
						if(handle->pixels) delete[] handle->pixels; handle->pixels = nullptr;
						delete handle; msg->handle = nullptr;
						msg->ready = true;
						return;
					}
					msg->ready = true;
					unsigned long wmProc      = pXInternAtom(handle->display, "WM_PROTOCOLS", 0);
					unsigned long wmStateAtom = pXInternAtom(handle->display, "WM_STATE", 0);
					unsigned long wmDelete    = pXInternAtom(handle->display, "WM_DELETE_WINDOW", 0);
					UWORD msgSend, wp, lp;
					while(true){
						XEvent ev = {0};
						if(handle->father ? (!(handle->father->MSGthr)) : false) break;
						pXNextEvent(handle->display, &ev);
						if(handle->father ? (!(handle->father->MSGthr)) : false) break;
						msgSend = window_box::def; wp = 0; lp = 0;
						if(ev.type == 2){ // KeyPress
							msgSend = window_box::key_down;
							wp = ev.xkey.keycode;
						}
						if(ev.type == 3){ // KeyRelease
							msgSend = window_box::key_up;
							wp = ev.xkey.keycode;
						}
						if(ev.type == 4){ // ButtonPress
							if(ev.xbutton.button == 1) msgSend = window_box::lbutton_down;
							if(ev.xbutton.button == 3) msgSend = window_box::rbutton_down;
							lp = (ev.xbutton.y << 16) | (ev.xbutton.x & 0xFFFF);
						}
						if(ev.type == 5){ // ButtonRelease
							if(ev.xbutton.button == 1) msgSend = window_box::lbutton_up;
							if(ev.xbutton.button == 3) msgSend = window_box::rbutton_up;
							lp = (ev.xbutton.y << 16) | (ev.xbutton.x & 0xFFFF);
						}
						if(ev.type == 6){ // MotionNotify
							msgSend = window_box::mouse_move;
							lp = (ev.xmotion.y << 16) | (ev.xmotion.x & 0xFFFF);
						}
						if(ev.type == 9){ // FocusIn
							msgSend = window_box::activate;
							wp = 1; // Active
						}
						if(ev.type == 10){ // FocusOut
							msgSend = window_box::activate;
							wp = 0; // Inactive
						}
						if(ev.type == 12){ // PAINT
							msgSend = window_box::paint;
							void* ximg = pXCreateImage(
								handle->display, 
								nullptr,
								24, 2, 0, 
								(char*)(handle->pixels), 
								handle->width, handle->height, 
								32, 0
							);
							if(ximg){
								pXPutImage(
									handle->display, handle->window, handle->gc, ximg,
									0, 0, 0, 0, handle->width, handle->height
								);
								pXFree(ximg); 
							}
						}
						if(ev.type == 22){ // RESIZE
							msgSend = window_box::size;
							wp = (ev.xconfigure.x     & 0xFFFF) | ((ev.xconfigure.y      & 0xFFFF) << 16);
							lp = (ev.xconfigure.width & 0xFFFF) | ((ev.xconfigure.height & 0xFFFF) << 16);
							handle->x      = ev.xconfigure.x;
							handle->y      = ev.xconfigure.y;
							int oldWidth   = handle->width;
							int oldHeight  = handle->height;
							handle->width  = ev.xconfigure.width;
							handle->height = ev.xconfigure.height;
							if(handle->width != 0 && handle->height != 0 && handle->pixels && (oldWidth != handle->width || oldHeight != handle->height)){
								UWORD _new_pixels_amont = (handle->width) * (handle->height);
								unsigned int* _new_pixels = new unsigned int[_new_pixels_amont]{0};
								int copy_w = (oldWidth  < handle->width)  ? oldWidth  : handle->width;
								int copy_h = (oldHeight < handle->height) ? oldHeight : handle->height;
								unsigned int* _n_p = _new_pixels;
								unsigned int* _o_p = handle->pixels;
								while(copy_h--){
									for(int col = 0; col < copy_w; ++col) _n_p[col] = _o_p[col];
									_n_p += handle->width;
									_o_p += oldWidth;
								}
								delete[] handle->pixels;
								handle->pixels = _new_pixels;
								handle->pixels_amont = _new_pixels_amont;
							}
						}
						if(ev.type == 28){
							if(ev.xproperty.atom == wmStateAtom){
								unsigned long nitems, bytes_after;
								unsigned char* prop = nullptr;
								unsigned long actual_type;
								int actual_format;
								if(pXGetWindowProperty(handle->display, handle->window, wmStateAtom, 0, 2, 0, wmStateAtom, 
												       &actual_type, &actual_format, &nitems, &bytes_after, &prop) == 0){
									if(prop){
										handle->minimized = *(unsigned long*)prop == 1;
										pXFree(prop);
									}
								}
							}
						}
						if(ev.type == 33){
							if(ev.xclient.message_type == wmProc) {
								if((unsigned long)(ev.xclient.data.l[0]) == wmDelete) msgSend = window_box::close; // CLOSE
							}
						}
						if(mailman) msgSend = mailman(handle, msgSend, wp, lp);
						if(ev.type == 33 ? (ev.xclient.message_type == wmProc ? (ev.xclient.data.l[0] == wmDelete) : false) : false) break;
						if(msgSend == window_box::accepted) continue;
					}
					if(handle->display && handle->gc) pXFreeGC(handle->display, handle->gc);
					if(handle->display && handle->window) pXDestroyWindow(handle->display, handle->window);
					if(handle->display) pXCloseDisplay(handle->display);
					handle->gc = nullptr;
					handle->window = 0;
					handle->display = nullptr;
					if(handle->pixels) delete[] handle->pixels; handle->pixels = nullptr;
					handle->pixels_amont = 0;
					THR_HANDLE_DECL;
					if(handle->MSGthr) delete (THR_HANDLE*)(handle->MSGthr); handle->MSGthr = nullptr;
					delete handle;
				}
			};
			pXSelectInput(handle->display, handle->window, 0x62804F);
			unsigned long wmDelete = pXInternAtom(handle->display, "WM_DELETE_WINDOW", 0);
			pXSetWMProtocols(handle->display, handle->window, &wmDelete, 1);
			pXMapWindow(handle->display, handle->window);
			pXSync(handle->display, 0);
			if(title) pXStoreName(handle->display, handle->window, title);
			_ap_::MSGpack* msg = new _ap_::MSGpack{
				handle,
				mailman,
				false
			};
			handle->MSGthr = thread::create((void*(*)(void*))_ap_::dispach, msg);
			while(!(msg->ready)) sleep(10);
			if(!(msg->handle)){
				handle->MSGthr = nullptr;
				handle = nullptr;
			}
			delete msg;
			return (void*)handle;
            #endif
		}
		void* sendmessage(void* wnd, UWORD msg, UWORD wp, WORD lp){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			#if defined(_WIN32)
			typedef long long(__stdcall *SM)(void*,unsigned int,unsigned long long,long long);
		    static SM pSendMessage = (SM)get_system_function("SendMessageA", "user32.dll");
		    if(!pSendMessage) return nullptr;
		    pSendMessage(handle->hwnd, (unsigned int)msg, (unsigned long long)wp, (long long)lp);
			#else // Linux
			XEvent_DECL;
			typedef int(*XSE)(void*,unsigned long,int,long,void*);
			static XSE pXSendEvent = (XSE)get_system_function("XSendEvent", "libX11.so.6");
			typedef unsigned long(*XIA)(void*,const char*,int);
			static XIA pXInternAtom = (XIA)get_system_function("XInternAtom", "libX11.so.6");
			typedef int(*XFL)(void*);
			static XFL pXFlush = (XFL)get_system_function("XFlush", "libX11.so.6");
			if(!pXSendEvent || !pXInternAtom || !pXFlush) return nullptr;
			XEvent ev = {0};
			 if(msg == window_box::close){
				ev.type = 33; // ClientMessage
				ev.xclient.window = handle->window;
				ev.xclient.message_type = pXInternAtom(handle->display, "WM_PROTOCOLS", 0);
				ev.xclient.format = 32;
				ev.xclient.data.l[0] = pXInternAtom(handle->display, "WM_DELETE_WINDOW", 0);
			}if(msg == window_box::paint){
				ev.type = 12; // Expose
			}if(msg == window_box::key_down || msg == window_box::key_up){
				ev.type = (msg == window_box::key_down) ? 2 : 3; // KeyPress : KeyRelease
				ev.xkey.keycode = wp;
				ev.xkey.state = 0;
			}if(msg == window_box::mouse_move){
				ev.type = 6; // MotionNotify
				ev.xmotion.x = (short)(lp & 0xFFFF);
				ev.xmotion.y = (short)((lp >> 16) & 0xFFFF);
			}if(msg >= window_box::lbutton_down && msg <= window_box::rbutton_up){
				// Simple mapping: Left=1, Right=3. 
				// 0x0201=LDown, 0x0202=LUp, 0x0204=RDown, 0x0205=RUp
				ev.type = (msg == window_box::lbutton_down || msg == window_box::rbutton_down) ? 4 : 5;
				ev.xbutton.button = (msg == window_box::lbutton_down || msg == window_box::lbutton_up) ? 1 : 3;
				ev.xbutton.x = (short)(lp & 0xFFFF);
				ev.xbutton.y = (short)((lp >> 16) & 0xFFFF);
			}if(msg == window_box::activate){
				ev.type = (wp > 0) ? 9 : 10; // FocusIn : FocusOut
			}
			pXSendEvent(handle->display, handle->window, 1, 0, &ev);
			pXFlush(handle->display);
			#endif
			return wnd;
		}
		void* wait(void* wnd){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(handle->ext){
				#if defined(_WIN32)
				typedef unsigned long (__stdcall *WFSO)(void*, unsigned long);
				static WFSO pWaitForSingleObject = (WFSO)get_system_function("WaitForSingleObject");
				typedef int (__stdcall *CH)(void*);
				static CH pCloseHandle = (CH)get_system_function("CloseHandle");
				typedef int (__stdcall *IW)(void*);
				static IW pIsWindow = (IW)get_system_function("IsWindow", "user32.dll");
				while(pIsWindow && pIsWindow(handle->hwnd)) sleep(50);
				#else // Linux
				typedef int (*XSI)(void*, unsigned long, unsigned long);
				static XSI pXSelectInput = (XSI)get_system_function("XSelectInput", "libX11.so.6");
				typedef int (*XNE)(void*, void*);
				static XNE pXNextEvent = (XNE)get_system_function("XNextEvent", "libX11.so.6");
				if(handle->display && handle->window){
					pXSelectInput(handle->display, handle->window, 0x20000);
				    XEvent_DECL;
				    XEvent ev;
				    while(true){
				        pXNextEvent(handle->display, &ev);
				        if(ev.type == 17 && ev.xany.window == handle->window) break;
				    }
				}
				#endif
				delete handle;
				return wnd;
			}
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			while(handle->MSGthr != nullptr) sleep(10);
			return wnd;
		}
		void* destroy(void* wnd, bool _wait){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
		    WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(handle->ext){
				window::sendmessage(wnd, window_box::close, 0, 0);
				delete handle;
				return wnd;
			}
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			window::sendmessage(wnd, window_box::close, 0, 0);
			if(_wait) while(handle->MSGthr != nullptr) sleep(5);
		    return wnd;
		}
		void* draw(void* wnd, int offsetX, int offsetY, unsigned int* _pixels, unsigned int _width, unsigned int _height){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
		    WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle) || handle->ext) return nullptr;
		    if(offsetX >= handle->width || offsetY >= handle->height || !handle->pixels || !_pixels || !_width || !_height) return wnd;
		    int targetX = offsetX;
		    int targetY = offsetY;
		    int targetW = (int)_width;
		    int targetH = (int)_height;
		    int srcX = 0, srcY = 0;
		    if(targetX < 0){
		        srcX = -targetX;
		        targetW -= srcX;
		        targetX = 0;
		    }
		    if(targetY < 0){
		        srcY = -targetY;
		        targetH -= srcY;
		        targetY = 0;
		    }
		    if(targetX + targetW > handle->width)  targetW = handle->width  - targetX;
		    if(targetY + targetH > handle->height) targetH = handle->height - targetY;
		    if(targetW <= 0 || targetH <= 0 || srcX >= (int)_width || srcY >= (int)_height) return wnd;
			unsigned int* dest = handle->pixels + (targetY * handle->width) + targetX;
		    unsigned int* src = _pixels + (srcY * _width) + srcX;
		    for(int y = 0; y < targetH; ++y){
		        for(int x = 0; x < targetW; ++x) dest[x] = src[x];
		        dest += handle->width;
		        src  += _width;
		    }
		    return window::sendmessage(wnd, window_box::paint, 0, 0);
		}
		void  pixels(void* wnd, unsigned int** _px, UWORD* _px_am){
			if(WND_HANDLE_VALID(wnd) || (!_px && !_px_am)) return;
		    WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle) || handle->ext) return;
		    if(_px)    *_px    = nullptr;
		    if(_px_am) *_px_am = 0;
		    if(!(handle->pixels) || !(handle->pixels_amont)) return;
		    if(_px)    *_px    = handle->pixels;
		    if(_px_am) *_px_am = handle->pixels_amont;
		}
		void* clear(void* wnd, unsigned int _color){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
		    WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle) || handle->ext) return nullptr;
		    if(!handle->pixels || !(handle->pixels_amont)) return wnd;
		    unsigned int* _i_p = handle->pixels + handle->pixels_amont;
		    while(_i_p-- != handle->pixels) *_i_p = _color;
			return window::sendmessage(wnd, window_box::paint, 0, 0);
		}
		void* resize(void* wnd, int _x, int _y, int _width, int _height, bool SetPosition, bool SetSize){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			if(!SetPosition && !SetSize) return wnd; // (SetPosition == false && SetSize == false) => "Its equivalent to doing nothing".
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			#if defined(_WIN32)
			struct RECT{ int left, top, right, bottom; };
			typedef int(__stdcall *SWP)(void*,void*,int,int,int,int,unsigned int);
		    static SWP pSetWindowPos = (SWP)get_system_function("SetWindowPos", "user32.dll");
		    typedef int(__stdcall *AWR)(RECT*, unsigned long, int);
		    static AWR pAdjustWindowRect = (AWR)get_system_function("AdjustWindowRect", "user32.dll");
		    typedef long (__stdcall *GWL)(void*, int);
		    #if defined(_WIN64)
		    static GWL pGetWindowLong = (GWL)get_system_function("GetWindowLongPtrA", "user32.dll");
		    #else
		    static GWL pGetWindowLong = (GWL)get_system_function("GetWindowLongA", "user32.dll");
		    #endif
			if(!pSetWindowPos || !pAdjustWindowRect || !pGetWindowLong) return nullptr;
		    if(SetSize && pAdjustWindowRect && pGetWindowLong){
		        unsigned long style = (unsigned long)pGetWindowLong(handle->hwnd, -16);
		        RECT rect = { 0, 0, _width, _height };
		        pAdjustWindowRect(&rect, style, 0);
		        _width = rect.right - rect.left;
		        _height = rect.bottom - rect.top;
		    }
			pSetWindowPos(
				handle->hwnd,
				nullptr,
				_x, _y, _width, _height,
				0x14 | (SetPosition ? (SetSize ? 0x00 : 0x01) : 0x02)
			);
			#else // Linux
			typedef int(*XMRW)(void*,unsigned long,int,int,unsigned int,unsigned int);
			static XMRW pXMoveResizeWindow = (XMRW)get_system_function("XMoveResizeWindow", "libX11.so.6");
			typedef int(*XMW)(void*,unsigned long,int,int);
			static XMW pXMoveWindow = (XMW)get_system_function("XMoveWindow", "libX11.so.6");
			typedef int(*XRW)(void*,unsigned long,unsigned int,unsigned int);
			static XRW pXResizeWindow = (XRW)get_system_function("XResizeWindow", "libX11.so.6");
			typedef int(*XFL)(void*);
			static XFL pXFlush = (XFL)get_system_function("XFlush", "libX11.so.6");
			if(!pXMoveResizeWindow || !pXMoveWindow || !pXResizeWindow || !pXFlush) return nullptr;
			if(SetPosition && SetSize) pXMoveResizeWindow(handle->display, handle->window, _x, _y, _width, _height);
			else if(SetPosition)       pXMoveWindow(handle->display, handle->window, _x, _y);
			else                       pXResizeWindow(handle->display, handle->window, _width, _height);
			pXFlush(handle->display);
			#endif
			return wnd;
		}
		void  getsize(void* wnd, int* _x, int* _y, int* _width, int* _height){
			if(WND_HANDLE_VALID(wnd) || (!_x && !_y && !_width && !_height)) return;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle) || handle->ext) return;
			if(_x)      *_x      = handle->x;
			if(_y)      *_y      = handle->y;
			if(_width)  *_width  = handle->width;
			if(_height) *_height = handle->height;
		}
		void* father(void* wnd){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			return (void*)(handle->father);
		}
		void* focus(void* wnd){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			#if defined(_WIN32)
			typedef int(__stdcall* SFW)(void*);
			static SFW pSetForegroundWindow = (SFW)get_system_function("SetForegroundWindow", "user32.dll");
			if(!pSetForegroundWindow) return nullptr;
			pSetForegroundWindow(handle->hwnd);
			#else // Linux
			typedef unsigned long(*XIA)(void*,const char*,int);
			static XIA pXInternAtom = (XIA)get_system_function("XInternAtom", "libX11.so.6");
			typedef int(*XSE)(void*,unsigned long,int,long,void*);
			static XSE pXSendEvent = (XSE)get_system_function("XSendEvent", "libX11.so.6");
			typedef unsigned long(*XRS)(void*);
			static XRS pXRootWindow = (XRS)get_system_function("XDefaultRootWindow", "libX11.so.6");
			typedef int(*XDS)(void*);
			static XDS pXDefaultScreen = (XDS)get_system_function("XDefaultScreen", "libX11.so.6");
			typedef int(*XFL)(void*);
			static XFL pXFlush = (XFL)get_system_function("XFlush", "libX11.so.6");
			typedef int(*XRM)(void*,unsigned long);
			static XRM pXRaiseWindow = (XRM)get_system_function("XRaiseWindow", "libX11.so.6");
			if(!pXInternAtom || !pXSendEvent || !pXRootWindow || !pXFlush || !pXRaiseWindow) return nullptr;
			pXRaiseWindow(handle->display, handle->window);
			XEvent_DECL;
			XEvent ev = {0};
			unsigned long root = pXRootWindow(handle->display);
			ev.type = 33;
			ev.xclient.display = handle->display;
			ev.xclient.window = handle->window;
			ev.xclient.message_type = pXInternAtom(handle->display, "_NET_ACTIVE_WINDOW", 0);
			ev.xclient.format = 32;
			ev.xclient.data.l[0] = 1;
			pXSendEvent(handle->display, root, 0, 0x180000, &ev);
			pXFlush(handle->display);
			#endif
			return wnd;
		}
		void* retitle(void* wnd, const char* title){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			#if defined(_WIN32)
			typedef int(_stdcall* SWT)(void*,const char*);
			static SWT pSetWindowTextA = (SWT)get_system_function("SetWindowTextA", "user32.dll");
			if(!pSetWindowTextA) return nullptr;
			pSetWindowTextA(handle->hwnd, title ? title : "");
			#else // Linux
			typedef int(*XST)(void*,unsigned long,const char*);
			static XST pXStoreName = (XST)get_system_function("XStoreName", "libX11.so.6");
			if(!pXStoreName) return nullptr;
			pXStoreName(handle->display, handle->window, title ? title : "");
			#endif
			return wnd;
		}
		void* gettitle(void* wnd, char** title, bool* minimized){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			if(!title && !minimized) return wnd;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			#if defined(_WIN32)
			typedef int(__stdcall* GWT)(void*,char*,int);
			static GWT pGetWindowTextA = (GWT)get_system_function("GetWindowTextA", "user32.dll");
			if(!pGetWindowTextA) return nullptr;
			#else // Linux
			typedef int(*XFN)(void*,unsigned long,char**);
			static XFN pXFetchName = (XFN)get_system_function("XFetchName", "libX11.so.6");
			typedef int(*XFR)(void*);
			static XFR pXFree = (XFR)get_system_function("XFree", "libX11.so.6");
			typedef int(*XSY)(void*, int);
			static XSY pXSync = (XSY)get_system_function("XSync", "libX11.so.6");
			if(!pXFetchName || !pXFree || !pXSync) return nullptr;
			pXSync(handle->display, 0);
			#endif
			if(title){
				#if defined(_WIN32)
				char* name = new char[256];
				if(!pGetWindowTextA(handle->hwnd, name, 256)) return nullptr;
				#else // Linux
				char* name = nullptr;
				if(!pXFetchName(handle->display, handle->window, &name)) return nullptr;
				#endif
				if(*title) delete[] *title;
				int len = 0; while(name[len]) ++len;
				*title = new char[len + 1]; (*title)[len] = '\0';
				while(len--) (*title)[len] = name[len];
				#if defined(_WIN32)
				delete[] name;
				#else // Linux
				pXFree(name);
				#endif
			}
			if(minimized && !(handle->ext)) *minimized = handle->minimized;
			return wnd;
		}
		void  getscreen(int* _width, int* _height){
			if(!_width && !_height) return;
			#if defined(_WIN32)
			typedef int(__stdcall* GSM)(int);
			static GSM pGetSystemMetrics = (GSM)get_system_function("GetSystemMetrics", "user32.dll");
			if(!pGetSystemMetrics) return;
			if(_width)  *_width  = pGetSystemMetrics(0);
			if(_height) *_height = pGetSystemMetrics(1);
			#else // Linux
			typedef void*(*XOD)(const char*);
			static XOD pXOpenDisplay = (XOD)get_system_function("XOpenDisplay", "libX11.so.6");
			typedef int(*XDS)(void*);
			static XDS pXDefaultScreen = (XDS)get_system_function("XDefaultScreen", "libX11.so.6");
			typedef int(*XDW)(void*,int);
			static XDW pXDisplayWidth = (XDW)get_system_function("XDisplayWidth", "libX11.so.6");
			typedef int(*XDH)(void*,int);
			static XDH pXDisplayHeight = (XDH)get_system_function("XDisplayHeight", "libX11.so.6");
			typedef int(*XCD)(void*);
			static XCD pXCloseDisplay = (XCD)get_system_function("XCloseDisplay", "libX11.so.6");
			if(!pXOpenDisplay || !pXDefaultScreen || !pXDisplayWidth || !pXDisplayHeight || !pXCloseDisplay) return;
			void* d = pXOpenDisplay(nullptr);
			if(!d) return;
			int s = pXDefaultScreen(d);
			if(_width)  *_width  = pXDisplayWidth (d, s);
			if(_height) *_height = pXDisplayHeight(d, s);
			pXCloseDisplay(d);
			#endif
		}
		void* show(void* wnd, bool visible, bool minimize){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			#if defined(_WIN32)
			typedef int(__stdcall* SW)(void*,int);
			static SW pShowWindow = (SW)get_system_function("ShowWindow", "user32.dll");
			if(!pShowWindow) return nullptr;
			pShowWindow(handle->hwnd, visible ? (minimize ? 6 : 1) : 0);
			#else // Linux
			typedef int(*XMAP)(void*,unsigned long);
			static XMAP pXMapWindow = (XMAP)get_system_function("XMapWindow", "libX11.so.6");
			typedef int(*XUNP)(void*,unsigned long);
			static XUNP pXUnmapWindow = (XUNP)get_system_function("XUnmapWindow", "libX11.so.6");
			typedef unsigned long(*XIA)(void*,const char*,int);
			static XIA pXInternAtom = (XIA)get_system_function("XInternAtom", "libX11.so.6");
			typedef int(*XSE)(void*,unsigned long,int,long,void*);
			static XSE pXSendEvent = (XSE)get_system_function("XSendEvent", "libX11.so.6");
			typedef unsigned long(*XRS)(void*);
			static XRS pXRootWindow = (XRS)get_system_function("XDefaultRootWindow", "libX11.so.6");
			typedef int(*XFL)(void*);
			static XFL pXFlush = (XFL)get_system_function("XFlush", "libX11.so.6");
			if(!pXMapWindow || !pXUnmapWindow || !pXInternAtom || !pXSendEvent || !pXRootWindow || !pXFlush) return nullptr;
			if(visible){
				pXMapWindow(handle->display, handle->window);
				if(minimize){
				    XEvent_DECL;
				    XEvent ev = {0};
				    ev.type = 33; // ClientMessage
				    ev.xclient.window = handle->window;
				    ev.xclient.message_type = pXInternAtom(handle->display, "WM_CHANGE_STATE", 0);
				    ev.xclient.format = 32;
				    ev.xclient.data.l[0] = 3;
				    pXSendEvent(handle->display, pXRootWindow(handle->display), 0, 0x180000, &ev);
				}else window::focus(wnd);
			}else pXUnmapWindow(handle->display, handle->window);
			pXFlush(handle->display);
			#endif
			handle->invisible = !visible; handle->minimized = visible ? minimize : false;
			return wnd;
		}
		void* focuswait(void* wnd){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			#if defined(_WIN32)
			typedef void* (__stdcall *GFW)();
			static GFW pGetForegroundWindow = (GFW)get_system_function("GetForegroundWindow", "user32.dll");
			if(!pGetForegroundWindow) return nullptr;
			while(pGetForegroundWindow() != handle->hwnd && handle->hwnd) sleep(10);
			if(!(handle->hwnd)) return nullptr;
			#else // Linux
			typedef int(*XGIF)(void*,unsigned long*,int*);
			static XGIF pXGetInputFocus = (XGIF)get_system_function("XGetInputFocus", "libX11.so.6");
			if(!pXGetInputFocus) return nullptr;
			unsigned long current_focus; int revert;
			while(({pXGetInputFocus(handle->display, &current_focus, &revert); current_focus != handle->window;})) sleep(10);
			sleep(10);
			#endif
			return wnd;
		}
		void* active(){
			WND_HANDLE_DECL;
			#if defined(_WIN32)
			typedef void* (__stdcall *GFW)();
			static GFW pGetForegroundWindow = (GFW)get_system_function("GetForegroundWindow", "user32.dll");
			if(!pGetForegroundWindow) return nullptr;
			void* hwnd = pGetForegroundWindow();
			if(!hwnd) return nullptr;
			WND_HANDLE* handle = new WND_HANDLE;
			// Nota: Al marcar 'ext'(-ern) como 'true',
			// hara que la variable pierda privilegios
			// con algunas funciones, como forma de
			// mecanismo de seguridad a la memoria.
			handle->ext = true;
			handle->hwnd = hwnd;
			#else // Linux
			struct _ap_{
				void* globalDpy;
				_ap_(){
					typedef void*(*XOD)(const char*);
					static XOD pXOpenDisplay = (XOD)get_system_function("XOpenDisplay", "libX11.so.6");
					globalDpy = pXOpenDisplay ? pXOpenDisplay(nullptr) : nullptr;
				}
				~_ap_(){
					typedef int(*XCD)(void*);
					static XCD pXCloseDisplay = (XCD)get_system_function("XCloseDisplay", "libX11.so.6");
					if(globalDpy && pXCloseDisplay) pXCloseDisplay(globalDpy); globalDpy = nullptr;
				}
			};
			static _ap_ _ba_;
			typedef int(*XGIF)(void*, unsigned long*, int*);
			static XGIF pXGetInputFocus = (XGIF)get_system_function("XGetInputFocus", "libX11.so.6");
			if(!pXGetInputFocus || !(_ba_.globalDpy)) return nullptr;
			unsigned long focus_win = 0;
			int revert = 0;
			pXGetInputFocus(_ba_.globalDpy, &focus_win, &revert);
			if(focus_win <= 2) return nullptr;
			WND_HANDLE* handle = new WND_HANDLE;
			handle->ext = true;
			handle->display = _ba_.globalDpy;
			handle->window = focus_win;
			#endif
			return (void*)handle;
		}
		bool is_extern(void* wnd){
			if(WND_HANDLE_VALID(wnd)) return false;
		    WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			return handle->ext;
		}
    	inline bool yiel(void* wnd, bool _wait){
    		if(WND_HANDLE_VALID(wnd)) return false;
    		if(_wait) sleep(10);
    		return true;
    	}
	}
	#endif
    #if defined(ST_IMPORT_INPUT)
	namespace input{
		void mouse(void* wnd, int* _x, int* _y, int* lclick, int* rclick){
			if((wnd
			#if defined(ST_IMPORT_WINDOW)
			&& WND_HANDLE_VALID(wnd)
			#endif
			) || (!_x && !_y && !lclick && !rclick)) return;
			#if defined(ST_IMPORT_WINDOW)
			WND_HANDLE_DECL;
		    WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(wnd ? (!WND_HANDLE_STATE(handle)) : false) return;
			#endif
			bool currl = false;
			bool currr = false;
			#if defined(_WIN32)
		    struct POINT{ long x, y; } pt;
		    typedef int(__stdcall *GCP)(POINT*);
		    static GCP pGetCursorPos = (GCP)get_system_function("GetCursorPos", "user32.dll");
		    typedef int(__stdcall *STC)(void*,POINT*);
		    static STC pScreenToClient = (STC)get_system_function("ScreenToClient", "user32.dll");
		    typedef short(__stdcall *GAKS)(int);
		    static GAKS pGetAsyncKeyState = (GAKS)get_system_function("GetAsyncKeyState", "user32.dll");
		    if(pGetCursorPos){
		        pGetCursorPos(&pt);
				#if defined(ST_IMPORT_WINDOW)
		        if(wnd && pScreenToClient) pScreenToClient(handle->hwnd, &pt);
				#endif
		        if(_x) *_x = (int)(pt.x);
		        if(_y) *_y = (int)(pt.y);
		    }
		    if(pGetAsyncKeyState){
		        currl = (pGetAsyncKeyState(0x01) & 0x8000) != 0;
		        currr = (pGetAsyncKeyState(0x02) & 0x8000) != 0;
		    }
			#else // Linux
			struct _ap_{
			    struct _MA_{
					bool l_down;
					bool r_down;
					bool ready;
					bool running;
			    };
				void* globalDpy;
				unsigned long root;
			    void* thr;
			    _MA_ ms;
				_ap_(){
					typedef void*(*XOD)(const char*);
					static XOD pXOpenDisplay = (XOD)get_system_function("XOpenDisplay", "libX11.so.6");
					typedef unsigned long(*XDRW)(void*);
					static XDRW pXDefaultRootWindow = (XDRW)get_system_function("XDefaultRootWindow", "libX11.so.6");
					globalDpy = pXOpenDisplay ? pXOpenDisplay(nullptr) : nullptr;
					root = (globalDpy && pXDefaultRootWindow) ? pXDefaultRootWindow(globalDpy) : 0;
					if(!globalDpy || !root) return;
					ms = { false, false, false, true };
					thr = thread::create((void*(*)(void*))get_events, &ms);
					while(!(ms.ready)) sleep(10);
					if(!(ms.running)){ thread::join(thr); thr = nullptr; }
				}
				~_ap_(){
					typedef int(*CLOSE)(int);
					static CLOSE pClose = (CLOSE)get_system_function("close", "libc.so.6");
					typedef int(*XCD)(void*);
					static XCD pXCloseDisplay = (XCD)get_system_function("XCloseDisplay", "libX11.so.6");
					if(thr){
						ms.running = false;
						thread::join(thr);
						thr = nullptr;
					}
					if(globalDpy && pXCloseDisplay) pXCloseDisplay(globalDpy);
				}
				static void get_events(_MA_* ms){
					struct input_event{
						long tv_sec;
						long tv_usec;
						unsigned short type;
						unsigned short code;
						int value;
					};
			    	typedef int(*POPEN)(const char*,int);
					static POPEN pOpen = (POPEN)get_system_function("open", "libc.so.6");
					typedef WORD(*PREAD)(int,void*,UWORD);
					static PREAD pRead = (PREAD)get_system_function("read", "libc.so.6");
					typedef int(*CLOSE)(int);
					static CLOSE pClose = (CLOSE)get_system_function("close", "libc.so.6");
					if(!pOpen || !pRead || !pClose){
						ms->running = false;
						ms->ready = true;
						return;
					}
					int fd = -1;
					bool using_mice = false;
					for(int i = 0; i < 20 && fd < 0; ++i){
						char path[32];
						sprintf(path, "/dev/input/event%d", i);
						fd = pOpen(path, 0x800);
						if (fd < 0) continue;
						struct input_event buf[4] = {0};
						if(pRead(fd, buf, sizeof(buf)) > 0) for (int j = 0; j < 4; ++j)
							if(buf[j].type == 1 && (buf[j].code == 272 || buf[j].code == 273))
								goto found_mouse;
						pClose(fd);
						fd = -1;
					}
					if(fd < 0){
						fd = pOpen("/dev/input/mice", 0x800);
						if(fd >= 0) using_mice = true;
					}
					if(fd < 0){
						puts("\
[Mouse] ERROR: Could not open any mouse device.\n\
               Make sure your user is in 'input's group.\n\
        Command: sudo usermod -aG input $USER   (then logout/reset)\n\
--------------------------------");
						ms->running = false;
						ms->ready = true;
						return;
					}
					found_mouse:
					ms->ready = true;
					if(using_mice){
						unsigned char packet[3];
						while(ms->running){
							WORD n = pRead(fd, packet, 3);
							if(n == 3){
								ms->l_down = (packet[0] & 1) != 0;
								ms->r_down = (packet[0] & 2) != 0;
							}
							if(n <= 0) sleep(5);
						}
					}else{
						input_event ev = {0};
						while(ms->running){
							WORD n = pRead(fd, &ev, sizeof(ev));
							if(n == sizeof(ev) && ev.type == 1){
								if (ev.code == 272) ms->l_down = (ev.value != 0);
								if (ev.code == 273) ms->r_down = (ev.value != 0);
							}
							if(n <= 0) sleep(5);
						}
					}
					if(fd >= 0) pClose(fd);
				}
			};
			static _ap_ _ba_;
			typedef int(*XQP)(void*,unsigned long,unsigned long*,unsigned long*,int*,int*,int*,int*,unsigned int*);
			static XQP pXQueryPointer = (XQP)get_system_function("XQueryPointer", "libX11.so.6");
			unsigned long root_ret, child_ret; int rx, ry, wx, wy; unsigned int mask;
			if(pXQueryPointer ? !pXQueryPointer(_ba_.globalDpy,
					#if defined(ST_IMPORT_WINDOW)
					wnd ? handle->window : _ba_.root
					#else
					_ba_.root
					#endif
					, &root_ret, &child_ret, &rx, &ry, &wx, &wy, &mask) : true) return;
			if(_x) *_x = (wnd ? wx : rx);
			if(_y) *_y = (wnd ? wy : ry);
			currl = wnd ? ((mask & 0x100) != 0) : (_ba_.ms.l_down);
			currr = wnd ? ((mask & 0x400) != 0) : (_ba_.ms.r_down);
			#undef XEvent_DECL
			#endif
			#if defined(ST_IMPORT_WINDOW)
			#if defined(_____mouse___set_orientation___1__2_3___)
			#undef _____mouse___set_orientation___1__2_3___
			#endif
			#define _____mouse___set_orientation___1__2_3___(letter) if(letter##click) *letter##click = (int)((curr##letter\
				? (((UWORD)*letter##click == window_box::letter##button_up || (UWORD)*letter##click == window_box::def)\
					? window_box::letter##button_down\
					: window_box::letter##button_hold)\
				: (((UWORD)*letter##click == window_box::letter##button_down || (UWORD)*letter##click == window_box::letter##button_hold)\
					? window_box::letter##button_up\
					: window_box::def)) & 0xFFFFFFFF)
			_____mouse___set_orientation___1__2_3___(l);
			_____mouse___set_orientation___1__2_3___(r);
			#undef _____mouse___set_orientation___1__2_3___
			#else
			if(lclick) *lclick = (currl?((*lclick==0x202||*lclick==0)?0x201:0x203):((*lclick==0x201||*lclick==0x203)?0x202:0));
			if(rclick) *rclick = (currr?((*rclick==0x205||*rclick==0)?0x204:0x206):((*rclick==0x204||*rclick==0x206)?0x205:0));
			#endif
		}
		void keyboard(void* wnd, char* key, char* ascii_key, int* outstanding_amount){
			#if defined(ST_IMPORT_WINDOW)
			if((wnd && WND_HANDLE_VALID(wnd)) || (!key && !ascii_key && !outstanding_amount)) return;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(wnd ? (!WND_HANDLE_STATE(handle)) : false) return;
			#else
			if(wnd || (!key && !ascii_key && !outstanding_amount)) return;
			#endif
			int i = (key ? (*key != '\0') : false) ? ((int)(unsigned char)*key) : 7;
			char k = '\0'; char as = '\0'; int count = 0;
			if(key) *key = '\0';
			if(ascii_key) *ascii_key = '\0';
		    if(outstanding_amount) *outstanding_amount = 0;
			#if defined(_WIN32)
		    typedef short(__stdcall *GAKS)(int);
		    static GAKS pGetAsyncKeyState = (GAKS)get_system_function("GetAsyncKeyState", "user32.dll");
		    typedef int(__stdcall *GKS)(unsigned char*);
		    static GKS pGetKeyboardState = (GKS)get_system_function("GetKeyboardState", "user32.dll");
		    typedef int(__stdcall *TVA)(unsigned int,unsigned int,unsigned char*,unsigned short*,unsigned int);
		    static TVA pToAscii = (TVA)get_system_function("ToAscii", "user32.dll");
		    typedef void*(__stdcall *GFW)();
		    static GFW pGetForegroundWindow = (GFW)get_system_function("GetForegroundWindow", "user32.dll");
		    typedef int(__stdcall *MVK)(unsigned int,unsigned int);
		    static MVK pMapVirtualKey = (MVK)get_system_function("MapVirtualKeyA", "user32.dll");
		    #if defined(ST_IMPORT_WINDOW)
		    if((wnd && pGetForegroundWindow) ? (pGetForegroundWindow() != handle->hwnd) : false) return;
		    #endif
		    if(!pGetAsyncKeyState || !pGetKeyboardState || !pToAscii) return;
		    unsigned char kState[256];
		    pGetKeyboardState(kState);
		    while(++i != 256) if(pGetAsyncKeyState(i) & 0x8000){
			    ++count;
			    if(k) continue;
			    k = (char)i;
			    unsigned short translated = 0;
			    unsigned int scanCode = pMapVirtualKey ? pMapVirtualKey(i, 0) : 0;
			    if(pToAscii(i, scanCode, kState, &translated, 0) > 0) as = (char)(translated & 0xFF);
			}
		    #else // Linux
			struct _ap_{
				void* globalDpy;
				_ap_(){
					typedef void*(*XOD)(const char*);
					static XOD pXOpenDisplay = (XOD)get_system_function("XOpenDisplay", "libX11.so.6");
					globalDpy = pXOpenDisplay ? pXOpenDisplay(nullptr) : nullptr;
				}
				~_ap_(){
					typedef int(*XCD)(void*);
					static XCD pXCloseDisplay = (XCD)get_system_function("XCloseDisplay", "libX11.so.6");
					if(globalDpy && pXCloseDisplay) pXCloseDisplay(globalDpy); globalDpy = nullptr;
				}
			};
			static _ap_ _ba_;
		    struct XKeyEvent{
				int type; unsigned long serial; int send_event; void* display;
				unsigned long window; unsigned long root; unsigned long subwindow;
				unsigned long time; int x, y; int x_root, y_root;
				unsigned int state; unsigned int keycode; int same_screen;
			};
			typedef int(*XQKM)(void*,char*);
			static XQKM pXQueryKeymap = (XQKM)get_system_function("XQueryKeymap", "libX11.so.6");
			typedef int(*XLS)(void*,void*,int,unsigned long*,void*);
			static XLS pXLookupString = (XLS)get_system_function("XLookupString", "libX11.so.6");
			if(!(_ba_.globalDpy) || !pXQueryKeymap || !pXLookupString) return;
			#if defined(ST_IMPORT_WINDOW)
			if(wnd){
				typedef int(*XGIF)(void*, unsigned long*, int*);
				static XGIF pXGetInputFocus = (XGIF)get_system_function("XGetInputFocus", "libX11.so.6");
				if(!pXGetInputFocus) return;
				unsigned long focus_win = 0;
				int revert = 0;
				pXGetInputFocus(_ba_.globalDpy, &focus_win, &revert);
				if(focus_win <= 2) return;
				if(handle->window != focus_win) return;
			}
			#endif
			char keys_return[32];
			pXQueryKeymap(_ba_.globalDpy, keys_return);
			while(++i != 256) if(keys_return[i >> 3] & (1 << (i & 0x00000007))){
				++count;
				if(k) continue;
				k = (char)i;
				XKeyEvent ev = {0};
				ev.display = _ba_.globalDpy;
				ev.keycode = i;
				char buf[8];
				if(pXLookupString(&ev, buf, sizeof(buf), nullptr, nullptr) > 0) as = buf[0];
			}
			#endif
			if(key) *key = k;
			if(ascii_key) *ascii_key = as;
			if(outstanding_amount) *outstanding_amount = count;
		}
		void mousewait(void* wnd, bool buttonDirection, bool unPress){
			if(wnd
			#if defined(ST_IMPORT_WINDOW)
			&& WND_HANDLE_VALID(wnd)
			#endif
			) return;
			int lclick, rclick;
			#if defined(ST_IMPORT_WINDOW)
			int bclick = (int)((buttonDirection
				? (unPress ? window_box::rbutton_up : window_box::rbutton_down)
				: (unPress ? window_box::lbutton_up : window_box::lbutton_down)) & 0xFFFFFFFF);
			#else
			int bclick = (buttonDirection?(unPress?0x205:0x204):(unPress?0x202:0x201));
			#endif
			while(true){
				mouse(wnd, nullptr, nullptr, &lclick, &rclick);
				if((buttonDirection ? rclick : lclick) == bclick) break;
		        sleep(10);
			}
		}
		char keywait(void* wnd, char key, bool dontPress){
			#if defined(ST_IMPORT_WINDOW)
			if(wnd && WND_HANDLE_VALID(wnd)) return '\0';
			#else
			if(wnd) return '\0';
			#endif
		    char currentKey = '\0', asciiKey = '\0';
		    while(true){
		        keyboard(wnd, &currentKey, &asciiKey, nullptr);
		        if(dontPress ? (currentKey == '\0') : (key == '\0' || asciiKey == key)) break;
		        sleep(10);
		    }
		    return currentKey;
		}
		UWORD language(){
			char _lang[2] = { '?', '?' };
			char _region[2] = { '?', '?' };
			#if defined(___language__set_the_ret__1_2__3__)
			#undef ___language__set_the_ret__1_2__3__
			#endif
			#define ___language__set_the_ret__1_2__3__ (((UWORD)(_lang[0]))|(((UWORD)(_lang[1]))<<8)|((((UWORD)(_region[0]))|(((UWORD)(_region[1]))<<8))<<16))
			#if defined(_WIN32)
		    typedef unsigned short(__stdcall *GUL)(void);
		    static GUL pGetUserDefaultLangID = (GUL)get_system_function("GetUserDefaultLangID");
		    typedef int(__stdcall *GLI)(unsigned int,unsigned int,char*,int);
		    static GLI pGetLocaleInfoA = (GLI)get_system_function("GetLocaleInfoA");
		    if(!pGetUserDefaultLangID || !pGetLocaleInfoA) return ___language__set_the_ret__1_2__3__;
		    unsigned short id = pGetUserDefaultLangID();
		    pGetLocaleInfoA(id, 0x59, _lang, 2);
		    pGetLocaleInfoA(id, 0x5A, _region, 2);
		    #else // Linux
		    typedef char*(*GENV)(const char*);
		    static GENV pGetenv = (GENV)get_system_function("getenv", "libc.so.6");
		    if(!pGetenv) return ___language__set_the_ret__1_2__3__;
		    char* lang = pGetenv("LANG");
		    if(lang){
		        _lang[0] = lang[0]; _lang[1] = lang[1];
		        _region[0] = lang[3]; _region[1] = lang[4];
		    }
		    #endif
		    return ___language__set_the_ret__1_2__3__;
		    #undef ___language__set_the_ret__1_2__3__
		}
		bool set_console_layout(bool UTF){
		    #if defined(_WIN32)
		    typedef int(__stdcall *SCP)(unsigned int);
		    static SCP pSetOutputCP = (SCP)get_system_function("SetConsoleOutputCP");
		    static SCP pSetCP = (SCP)get_system_function("SetConsoleCP");
		    if(!pSetOutputCP || !pSetCP) return false;
		    unsigned int cp = UTF ? 65001 : 437;
		    return pSetOutputCP(cp) && pSetCP(cp);
		    #else
		    return UTF;
		    #endif
		}
		bool gotoxy(UWORD _x, UWORD _y){
			#if defined(_WIN32)
		    struct _COORD { short X; short Y; };
		    typedef int(__stdcall *SCCP)(void*, _COORD);
		    static SCCP pSetPos = (SCCP)get_system_function("SetConsoleCursorPosition");
		    typedef void*(__stdcall *GSH)(unsigned int);
		    static GSH pGetHandle = (GSH)get_system_function("GetStdHandle");
		    static void* hOut = nullptr;
		    if(!pSetPos) return false;
		    if(!hOut) hOut = pGetHandle((unsigned int)-11);
		    _COORD pos = { (short)_x, (short)_y };
		    return pSetPos(hOut, pos) != 0;
		    #else
		    printf("\x1B[%d;%dH", _y + 1, _x + 1);
		    return true;
		    #endif
		}
		#if defined(ST_IMPORT_WINDOW)
		void* mouse_show(void* wnd, bool visible){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
		    #if defined(_WIN32)
		    typedef int(__stdcall *SCS)(int);
		    static SCS pShowCursor = (SCS)get_system_function("ShowCursor", "user32.dll");
		    if(!pShowCursor) return nullptr;
		    int res = pShowCursor(visible);
		    if( visible && res <  0) while(pShowCursor(true)  <  0);
		    if(!visible && res >= 0) while(pShowCursor(false) >= 0);
		    #else // Linux (X11)
		    struct _ap_{
				void* globalDpy;
				_ap_(){
					typedef void*(*XOD)(const char*);
					static XOD pXOpenDisplay = (XOD)get_system_function("XOpenDisplay", "libX11.so.6");
					globalDpy = pXOpenDisplay ? pXOpenDisplay(nullptr) : nullptr;
				}
				~_ap_(){
					typedef int(*XCD)(void*);
					static XCD pXCloseDisplay = (XCD)get_system_function("XCloseDisplay", "libX11.so.6");
					if(globalDpy && pXCloseDisplay) pXCloseDisplay(globalDpy);
				}
			};
			static _ap_ _ba_;
		    typedef int(*XFC)(void*,unsigned long);
			static XFC pXFixesHideCursor = (XFC)get_system_function("XFixesHideCursor", "libXfixes.so.3");
			static XFC pXFixesShowCursor = (XFC)get_system_function("XFixesShowCursor", "libXfixes.so.3");
			if(!_ba_.globalDpy || !pXFixesHideCursor || !pXFixesShowCursor) return nullptr;
			unsigned long target_win = ((WND_HANDLE*)wnd)->window;
			if(!target_win) return nullptr;
			if(visible) pXFixesShowCursor(_ba_.globalDpy, target_win);
			else pXFixesHideCursor(_ba_.globalDpy, target_win);
		    #endif
		    return wnd;
		}
		void* mouse_capture(void* wnd, bool lock){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
		    if(!WND_HANDLE_STATE(handle)) return nullptr;
		    #if defined(_WIN32)
		    typedef int(__stdcall *CC)(void*);
		    static CC pClipCursor = (CC)get_system_function("ClipCursor", "user32.dll");
		    typedef void*(__stdcall *GDW)(void);
		    static GDW pGetDesktopWindow = (GDW)get_system_function("GetDesktopWindow", "user32.dll");
		    typedef int(__stdcall *GWR)(void*, void*);
		    static GWR pGetWindowRect = (GWR)get_system_function("GetWindowRect", "user32.dll");
		    if(!pClipCursor || !pGetWindowRect || !pGetDesktopWindow) return nullptr;
		    struct{ int l, t, r, b; } rect;
		    if(lock){
		        void* target_h = (wnd ? WND_HANDLE_STATE(handle) : false) ? (handle->hwnd) : pGetDesktopWindow();
		        if(target_h ? pGetWindowRect(target_h, &rect) : false) pClipCursor(&rect);
		    }else pClipCursor(nullptr);
		    #else
		    struct _ap_{
				void* globalDpy;
				_ap_(){
					typedef void*(*XOD)(const char*);
					static XOD pXOpenDisplay = (XOD)get_system_function("XOpenDisplay", "libX11.so.6");
					globalDpy = pXOpenDisplay ? pXOpenDisplay(nullptr) : nullptr;
				}
				~_ap_(){
					typedef int(*XCD)(void*);
					static XCD pXCloseDisplay = (XCD)get_system_function("XCloseDisplay", "libX11.so.6");
					if(globalDpy && pXCloseDisplay) pXCloseDisplay(globalDpy);
				}
			};
			static _ap_ _ba_;
		    typedef int(*XGP)(void*,unsigned long,int,unsigned long,int,int,unsigned long,unsigned long,unsigned long);
			static XGP pXGrabPointer = (XGP)get_system_function("XGrabPointer", "libX11.so.6");
			typedef int(*XUGP)(void*,unsigned long);
			static XUGP pXUngrabPointer = (XUGP)get_system_function("XUngrabPointer", "libX11.so.6");
			if(!(_ba_.globalDpy) || !pXGrabPointer || !pXUngrabPointer) return nullptr;
			if(lock){ if(pXGrabPointer(_ba_.globalDpy, handle->window, false, 0x8C, 1, 1, handle->window, 0, 0)) return nullptr; }
			else pXUngrabPointer(_ba_.globalDpy, handle->window);
		    #endif
		    return wnd;
		}
		#endif
	}
    #endif
	#if defined(ST_IMPORT_FILE)
	namespace file{
		void* open(const char* filepath, UWORD openMode){
			if(!filepath || openMode == file_mode::none) return nullptr;
			#define FLE_HANDLE_VALID(ptr) ((ptr)?(*(UWORD*)(ptr)!=0x46494C45):true)
			#if defined(_WIN32)
			#define FLE_HANDLE_DECL struct FLE_HANDLE{\
				UWORD magic;\
				void* os_handle;\
				UWORD open_mode;\
				bool is_dir;\
				char* full_path;\
				UWORD full_spath;\
				FLE_HANDLE() : magic(0x46494C45), os_handle((void*)-1), open_mode(0), is_dir(false), full_path(nullptr), full_spath(0) {}\
				~FLE_HANDLE(){ magic = 0x00; if(full_path) delete[] full_path; }\
			}
			FLE_HANDLE_DECL;
			typedef void*(__stdcall *CFA)(const char*,unsigned int,unsigned int,void*,unsigned int,unsigned int,void*);
		    static CFA pCreateFileA = (CFA)get_system_function("CreateFileA");
		    typedef UWORD(__stdcall *GFP)(const char*,UWORD,char*,UWORD*);
		    static GFP pGetFullPathNameA = (GFP)get_system_function("GetFullPathNameA");
		    if(!pCreateFileA || !pGetFullPathNameA) return nullptr;
		    unsigned int access = 0;
		    unsigned int disp = 3;
		    if(openMode & file_mode::read)   access = 0x80000000;
		    if(openMode & file_mode::write)  access = 0x40000000;
		    if(openMode & file_mode::create) disp = 2;
		    void* h = pCreateFileA(filepath, access, 1 | 2, nullptr, disp, 128, nullptr);
		    if(h == (void*)-1) return nullptr;
		    UWORD req = pGetFullPathNameA(filepath, 0, nullptr, nullptr);
		    char* abs_path = new char[req];
		    pGetFullPathNameA(filepath, (unsigned int)req, abs_path, nullptr);
		    FLE_HANDLE* handle = new FLE_HANDLE();
		    handle->os_handle  = h;
		    handle->open_mode  = openMode;
		    handle->full_path  = abs_path;
		    handle->full_spath = req - 1;
		    handle->is_dir     = false;
			#else // Linux
			#define FLE_HANDLE_DECL struct FLE_HANDLE{\
				UWORD magic;\
				int fd;\
				UWORD open_mode;\
				bool is_dir;\
				char* full_path;\
				UWORD full_spath;\
				FLE_HANDLE() : magic(0x46494C45), fd(0), open_mode(0), is_dir(false), full_path(nullptr), full_spath(0) {}\
				~FLE_HANDLE(){ magic = 0x00; if(full_path) delete[] full_path; }\
			}
			FLE_HANDLE_DECL;
			typedef int(*POPEN)(const char*,int,...);
		    static POPEN pOpen = (POPEN)get_system_function("open", "libc.so.6");
			typedef char*(*RPATH)(const char*,char*);
			static RPATH pRealpath = (RPATH)get_system_function("realpath", "libc.so.6");
		    typedef void(*FREE)(void*);
		    static FREE pFree = (FREE)get_system_function("free", "libc.so.6");
		    if(!pOpen || !pRealpath || !pFree) return nullptr;
		    int flags = 0;
		    if(openMode & file_mode::read)   flags = 2;
		    if(openMode & file_mode::write)  flags = 1;
		    if(openMode & file_mode::create) flags |= 0100;
		    int fd = pOpen(filepath, flags, 0664);
		    if(fd < 0) return nullptr;
		    char* abs_path_tmp = pRealpath(filepath, nullptr); 
		    char* abs_path = nullptr;
		    UWORD abs_plen = 0;
		    if(abs_path_tmp){
		        while(abs_path_tmp[abs_plen]) abs_plen++;
		        abs_path = new char[abs_plen + 1];
		        for(UWORD i=0; i<=abs_plen; i++) abs_path[i] = abs_path_tmp[i];
		        pFree(abs_path_tmp);
		    }
		    FLE_HANDLE* handle = new FLE_HANDLE();
		    handle->fd         = fd;
		    handle->open_mode  = openMode;
		    handle->full_path  = abs_path;
		    handle->full_spath = abs_plen;
		    handle->is_dir     = false;
		    if(openMode & file_mode::create) file::clear(handle);
			#endif
			return (void*)handle;
		}
		void* open(void* folder, const char* subFilepath){
			if(!file::is_folder(folder) || (subFilepath ? (!*subFilepath) : true)) return nullptr;
			FLE_HANDLE_DECL;
			FLE_HANDLE* handle_folder = (FLE_HANDLE*)folder;
			UWORD full_child_spath = handle_folder->full_spath;
			while(*subFilepath++) ++full_child_spath;
			subFilepath -= full_child_spath - (handle_folder->full_spath) + 1;
			++full_child_spath;
			char* full_child_path = new char[full_child_spath + 1];
			sprintf(full_child_path,
			#if defined(_WIN32)
			"%s\\%s"
			#else // Linux
			"%s/%s"
			#endif
			, handle_folder->full_path, subFilepath);
			void* ret = file::open(full_child_path, handle_folder->open_mode);
			delete[] full_child_path;
			return ret;
		}
		void* close(void* file){
			if(FLE_HANDLE_VALID(file)) return nullptr;
			FLE_HANDLE_DECL;
			FLE_HANDLE* handle = (FLE_HANDLE*)file;
			#if defined(_WIN32)
			typedef int(__stdcall *CH)(void*);
			static CH pCloseHandle = (CH)get_system_function("CloseHandle");
			if(!pCloseHandle || handle->os_handle == (void*)-1) return nullptr;
			pCloseHandle(handle->os_handle); handle->os_handle = (void*)-1;
			#else // Linux
			typedef int(*CLOSE)(int);
			static CLOSE pClose = (CLOSE)get_system_function("close", "libc.so.6");
			if(!pClose || !(handle->fd)) return nullptr;
			pClose(handle->fd); handle->fd = 0;
			#endif
			delete handle;
			return file;
		}
		const char* copy(const char* source, const char* dest, bool overwrite){
			if(!source || !dest) return nullptr;
			if(!*source || !*dest) return nullptr;
			if(!file::exist(source)) return nullptr;
			#if defined(_WIN32)
		    typedef unsigned int(__stdcall *GFA)(const char*);
		    static GFA pGetFileAttributesA = (GFA)get_system_function("GetFileAttributesA");
		    unsigned int attr = pGetFileAttributesA ? pGetFileAttributesA(source) : 0xFFFFFFFF;
		    if(attr == 0xFFFFFFFF) return nullptr;
		    bool is_dir = (attr & 0x10);
		    #else
		    struct _stat_linux{
				unsigned long  st_dev;     unsigned long  st_ino;
				unsigned long  st_nlink;   unsigned int   st_mode;
				unsigned int   st_uid;     unsigned int   st_gid;
				unsigned long  st_rdev;    long           st_size;
				long           st_blksize; long           st_blocks;
				unsigned long  st_atime;   unsigned long  st_atime_nsec;
				unsigned long  st_mtime;   unsigned long  st_mtime_nsec;
				unsigned long  st_ctime;   unsigned long  st_ctime_nsec;
			} st;
			struct _dirent_linux{
				unsigned long  d_ino;      long           d_off;
				unsigned short d_reclen;   unsigned char  d_type;
				char           d_name[256];
			};
			typedef int(*STAT_FN)(const char*,void*);
        	static STAT_FN pStat = (STAT_FN)get_system_function("stat", "libc.so.6");
        	if(!pStat) return nullptr;
		    if(pStat(source, &st)) return nullptr;
		    bool is_dir = ((st.st_mode) & 170000) == 40000;
		    #endif
		    if(is_dir){
		        file::close(file::folder(dest, file_mode::create | file_mode::write));
		        #if defined(_WIN32)
		        struct WIN32_FIND_DATAA{
			        unsigned int attr;
					WORD t[3];
					unsigned int s[2], r0, r1;
			        char name[260], aname[14];
			    } fData;
		        static void*(__stdcall *pFindFirst)(const char*,void*) = (void*(__stdcall*)(const char*,void*))get_system_function("FindFirstFileA");
		        static int(__stdcall *pFindNext)(void*,void*) = (int(__stdcall*)(void*,void*))get_system_function("FindNextFileA");
		        static int(__stdcall *pFindClose)(void*) = (int(__stdcall*)(void*))get_system_function("FindClose");
		        char path_buffer[1024];
		        sprintf(path_buffer, "%s\\*", source);
		        void* hFind = pFindFirst(path_buffer, &fData);
		        if(hFind != (void*)-1){
		            do{
		            	const char* fname = fData.name - 4;
		                if(fname[0] == '.' && (fname[1] == '\0' || (fname[1] == '.' && fname[2] == '\0'))) continue;
		                char src_child[1024], dst_child[1024];
		                sprintf(src_child, "%s\\%s", source, fname);
		                sprintf(dst_child, "%s\\%s", dest, fname);
		                file::copy(src_child, dst_child, overwrite);
		            }while(pFindNext(hFind, &fData));
		            pFindClose(hFind);
		        }
		        #else
		        typedef void*(*OPENDIR_FN)(const char*);
		        static OPENDIR_FN pOpenDir = (OPENDIR_FN)get_system_function("opendir", "libc.so.6");
		        typedef _dirent_linux*(*READDIR_FN)(void*);
		        static READDIR_FN pReadDir = (READDIR_FN)get_system_function("readdir", "libc.so.6");
		        typedef int(*CLOSEDIR_FN)(void*);
		        static CLOSEDIR_FN pCloseDir = (CLOSEDIR_FN)get_system_function("closedir", "libc.so.6");
		        if(!pOpenDir || !pReadDir || !pCloseDir) return nullptr;
		        void* dir = pOpenDir(source);
		        if(dir){
		            _dirent_linux* entry = nullptr;
		            while((entry = pReadDir(dir)) != nullptr){
		                const char* fname = entry->d_name;
		                if(fname[0] == '.' && (fname[1] == '\0' || (fname[1] == '.' && fname[2] == '\0'))) continue;
		                char src_child[1024], dst_child[1024];
		                sprintf(src_child, "%s/%s", source, fname);
		                sprintf(dst_child, "%s/%s", dest, fname);
		                file::copy(src_child, dst_child, overwrite);
		            }
		            pCloseDir(dir);
		        }
		        #endif
		        return dest;
		    } 
		    #if defined(_WIN32)
		    typedef int(__stdcall *CFA)(const char*,const char*,int);
		    static CFA pCopyFileA = (CFA)get_system_function("CopyFileA");
		    if(!pCopyFileA) return nullptr;
		    if(pCopyFileA(source, dest, !overwrite)) return dest;
		    #else
		    typedef int(*OPEN_FN)(const char*,int,...);
		    static OPEN_FN pOpen = (OPEN_FN)get_system_function("open", "libc.so.6");
		    typedef int(*CLOSE_FN)(int);
		    static CLOSE_FN pClose = (CLOSE_FN)get_system_function("close", "libc.so.6");
		    typedef long(*SENDFILE_FN)(int,int,long*,unsigned long);
		    static SENDFILE_FN pSendFile = (SENDFILE_FN)get_system_function("sendfile", "libc.so.6");
		    if(!pOpen || !pClose || !pSendFile) return nullptr;
		    int src_fd = pOpen(source, 0);
		    int dst_fd = pOpen(dest, 101 | (overwrite ? 1000 : 200), 664);
		    if(src_fd >= 0 && dst_fd >= 0) pSendFile(dst_fd, src_fd, nullptr, st.st_size);
		    if(src_fd >= 0) pClose(src_fd);
		    if(dst_fd >= 0) pClose(dst_fd);
		    #endif
		    return dest;
		}
		const char* move(const char* old_path, const char* new_path, bool overwrite){
            if(!old_path || !new_path) return nullptr;
			if(!*old_path || !*new_path) return nullptr;
			if(!file::exist(old_path)) return nullptr;
			if(!overwrite && file::exist(new_path)) return nullptr;
			#if defined(_WIN32)
            typedef int(__stdcall *MFE)(const char*, const char*, unsigned int);
            static MFE pMoveFileExA = (MFE)get_system_function("MoveFileExA");
            if(!pMoveFileExA) return nullptr;
            if(pMoveFileExA(old_path, new_path, 0x2 | (overwrite ? 0x1 : 0x0))) return new_path;
            #else // Linux
            typedef int(*RENAME)(const char*, const char*);
            static RENAME pRename = (RENAME)get_system_function("rename", "libc.so.6");
            if(!pRename) return nullptr;
            if(!pRename(old_path, new_path)) return new_path;
            #endif
            if(file::copy(old_path, new_path, overwrite)){
                file::remove(old_path);
                return new_path;
            }
            return nullptr;
		}
		void* parent(void* file_ptr){
            if(!FLE_HANDLE_VALID(file_ptr)) return nullptr;
            FLE_HANDLE_DECL;
            FLE_HANDLE* h = (FLE_HANDLE*)file_ptr;
            char* path_copy = new char[h->full_spath + 1];
            for(UWORD i=0; i <= h->full_spath; i++) path_copy[i] = h->full_path[i];
            char* p = path_copy + h->full_spath;
            if(*p == '/' || *p == '\\' || *p == '\0') p--;
            while(p > path_copy && *p != '/' && *p != '\\') p--;
            void* parent_h = nullptr;
            if(p > path_copy){
                *p = '\0';
                parent_h = file::folder(path_copy, h->open_mode);
            }
            delete[] path_copy;
            return parent_h;
        }
        UWORD timestamp(void* file, UWORD type){
        	if(FLE_HANDLE_VALID(file)) return 0;
        	FLE_HANDLE_DECL;
            FLE_HANDLE* h = (FLE_HANDLE*)file;
            #if defined(_WIN32)
            unsigned long long ft[3];
            typedef int(__stdcall *GFT)(void*,void*,void*,void*);
            static GFT pGetFileTime = (GFT)get_system_function("GetFileTime");
            if(!pGetFileTime) return 0;
            if(pGetFileTime(h->os_handle, ft, ft+1, ft+2)){
            	int idx = (type == time_mode::creation) ? 0 : ((type == time_mode::access) ? 1 : 2);
                return (UWORD)((ft[idx] / 10000000ULL) - 11644473600ULL);
			}
            #else // Linux
            struct S_stat{
                unsigned long dev, ino, nlink;
                unsigned int mode, uid, gid, __pad0;
                unsigned long rdev, size, blksize, blocks;
                long atime, atime_nsec;
                long mtime, mtime_nsec;
                long ctime, ctime_nsec;
                long __unused[3];
            } st;
            typedef int(*FSTAT)(int,void*);
            static FSTAT pFstat = (FSTAT)get_system_function("fstat", "libc.so.6");
            if(!pFstat) return 0;
            if(!pFstat(h->fd, &st)){
                if(type == time_mode::creation) return (UWORD)st.ctime;
                if(type == time_mode::access) return (UWORD)st.atime;
                return (UWORD)st.mtime;
            }
            #endif
            return 0;
		}
		UWORD timestamp(const char* filepath, UWORD type){
			void* _h = file::open(filepath, file_mode::read);
			if(!_h) _h = file::folder(filepath, file_mode::read);
			if(!_h) return 0;
			UWORD ret = timestamp(_h, type);
			file::close(_h);
			return ret;
		}
		UWORD read(void* file, void* buffer, UWORD bytes){
			if(FLE_HANDLE_VALID(file) || !buffer || !bytes) return 0;
			FLE_HANDLE_DECL;
			FLE_HANDLE* handle = (FLE_HANDLE*)file;
			if(!((handle->open_mode) & file_mode::read)) return 0;
			#if defined(_WIN32)
			typedef int(__stdcall *RF)(void*,void*,unsigned int,unsigned int*,void*);
            static RF pReadFile = (RF)get_system_function("ReadFile");
            if(!pReadFile || handle->os_handle == (void*)-1) return 0;
            unsigned int readOut = 0;
            pReadFile(handle->os_handle, buffer, bytes, &readOut, nullptr);
			#else // Linux
			typedef long (*PREAD)(int,void*,unsigned long);
            static PREAD pRead = (PREAD)get_system_function("read", "libc.so.6");
			if(!pRead || !(handle->fd)) return 0;
            long readOut = pRead(handle->fd, buffer, (unsigned long)bytes);
            readOut = (readOut >= 0) ? readOut : 0;
			#endif
			return (UWORD)readOut;
		}
		UWORD write(void* file, const void* buffer, UWORD bytes){
			if(FLE_HANDLE_VALID(file) || !buffer || !bytes) return 0;
			FLE_HANDLE_DECL;
			FLE_HANDLE* handle = (FLE_HANDLE*)file;
			if(!((handle->open_mode) & file_mode::write)) return 0;
			#if defined(_WIN32)
			typedef int(__stdcall *WF)(void*,const void*,unsigned int,unsigned int*,void*);
            static WF pWriteFile = (WF)get_system_function("WriteFile");
			if(!pWriteFile || handle->os_handle == (void*)-1) return 0;
            unsigned int writtenOut = 0;
            pWriteFile(handle->os_handle, buffer, bytes, &writtenOut, nullptr);
            UWORD writeIn = (UWORD)writtenOut;
			#else // Linux
			typedef long (*PWRITE)(int,const void*,unsigned long);
            static PWRITE pWrite = (PWRITE)get_system_function("write", "libc.so.6");
			if(!pWrite || !(handle->fd)) return 0;
            long n = pWrite(handle->fd, buffer, (unsigned long)bytes);
            UWORD writeIn = (n >= 0) ? ((UWORD)n) : 0;
			#endif
			return writeIn;
		}
		UWORD size(void* file){
            if(FLE_HANDLE_VALID(file)) return 0;
            FLE_HANDLE_DECL;
            FLE_HANDLE* handle = (FLE_HANDLE*)file;
            if(is_folder(file)){
		        UWORD count = 0;
				#if defined(_WIN32)
		        char search_path[260];
		        sprintf(search_path, "%s\\*", handle->full_path);
		        struct WIN32_FIND_DATAA{
		            unsigned int dwFileAttributes;
		            long long ftCreationTime, ftLastAccessTime, ftLastWriteTime;
		            unsigned int nFileSizeHigh, nFileSizeLow, dwReserved0, dwReserved1;
		            char cFileName[260], cAlternateFileName[14];
		        } findData;
		        typedef void*(__stdcall *FFF)(const char*,void*);
		        static FFF pFindFirstFileA = (FFF)get_system_function("FindFirstFileA");
		        typedef int(__stdcall *FNF)(void*,void*);
		        static FNF pFindNextFileA  = (FNF)get_system_function("FindNextFileA");
		        typedef int(__stdcall *FC)(void*);
		        static FC pFindClose = (FC)get_system_function("FindClose");
		        void* hFind = pFindFirstFileA ? pFindFirstFileA(search_path, &findData) : (void*)-1;
		        if(hFind != (void*)-1){
		            do ++count; while(pFindNextFileA(hFind, &findData));
		            pFindClose(hFind);
		        	count -= 2;
		        }
				#else // Linux
				struct Sdir{
					unsigned long d_ino; unsigned long d_off;
					unsigned short d_reclen; unsigned char d_type;
					char d_name[256];
				};
		        typedef void*(*ODIR)(const char*);
		        static ODIR pOpendir = (ODIR)get_system_function("opendir", "libc.so.6");
		        typedef Sdir*(*RDIR)(void*);
		        static RDIR pReaddir = (RDIR)get_system_function("readdir", "libc.so.6");
		        typedef int(*CDIR)(void*);
		        static CDIR pClosedir = (CDIR)get_system_function("closedir", "libc.so.6");
		        void* dir = pOpendir ? pOpendir(handle->full_path) : nullptr;
		        if(dir){
		            Sdir* entry = nullptr;
		            while(entry = pReaddir(dir)) if(entry->d_name[0] != '.') ++count;
		            pClosedir(dir);
		        }
				#endif
		        return count;
		    }
            #if defined(_WIN32)
            typedef int(__stdcall *GSF)(void*,unsigned int*);
            static GSF pGetFileSize = (GSF)get_system_function("GetFileSize");
            if(!pGetFileSize || handle->os_handle == (void*)-1) return 0;
            unsigned int high = 0;
            unsigned int low = pGetFileSize(handle->os_handle, &high);
            return (low == 0xFFFFFFFF) ? 0 : (((UWORD)high << 32) | low);
            #else // Linux
            struct stat_buf{
                unsigned long dev, ino, nlink;
                unsigned int mode, uid, gid, __pad0;
                unsigned long rdev, size, blksize, blocks;
                long atime, atime_nsec, mtime, mtime_nsec, ctime, ctime_nsec;
            };
            typedef int(*FSTAT)(int,void*);
            static FSTAT pFstat = (FSTAT)get_system_function("fstat", "libc.so.6");
            if(!pFstat || !(handle->fd)) return 0;
            stat_buf st = {0};
            if(pFstat(handle->fd, &st) == 0) return (UWORD)(st.size);
            return 0;
            #endif
        }
        UWORD tell(void* file){ return seek(file, 0, seek_mode::current); }
        UWORD seek(void* file, WORD length, UWORD mode){
            if(FLE_HANDLE_VALID(file)) return 0;
            FLE_HANDLE_DECL;
            FLE_HANDLE* handle = (FLE_HANDLE*)file;
            #if defined(_WIN32)
            typedef unsigned int(__stdcall *SFP)(void*,long,long*,unsigned int);
            static SFP pSetFilePointer = (SFP)get_system_function("SetFilePointer");
            if (!pSetFilePointer || handle->os_handle == (void*)-1) return 0;
            long high = (sizeof(WORD) > 4) ? ((long)(length >> 32)) : 0;
            unsigned int low = pSetFilePointer(handle->os_handle, (long)(length & 0xFFFFFFFF), &high, (unsigned int)mode);
            return (low == 0xFFFFFFFF) ? 0 : (((UWORD)high << 32) | low);
            #else // Linux
            typedef long(*LSEEK)(int,long,int);
            static LSEEK pLseek = (LSEEK)get_system_function("lseek", "libc.so.6");
            if(!pLseek || !(handle->fd)) return 0;
            long res = pLseek(handle->fd, (long)length, (int)mode);
            return (res >= 0) ? (UWORD)res : 0;
            #endif
        }
        UWORD clear(const char* filepath){
        	if(!file::exist(filepath)) return 0;
        	void* file = file::open(filepath, file_mode::write);
        	if(!file) file = file::folder(filepath, file_mode::write);
        	if(!file) return 0;
            UWORD ret = file::clear(file);
			file::close(file);
            return ret;
		}
        UWORD clear(void* file){
        	if(FLE_HANDLE_VALID(file)) return 0;
            UWORD _sz = file::size(file);
        	if(file::is_folder(file)){
        		UWORD idx = _sz;
        		while(idx--) file::remove(inside(file, idx));
        		return _sz;
			}
            FLE_HANDLE_DECL;
            FLE_HANDLE* handle = (FLE_HANDLE*)file;
            #if defined(_WIN32)
		    typedef int(__stdcall *SEF)(void*);
		    static SEF pSetEndOfFile = (SEF)get_system_function("SetEndOfFile");
		    file::seek(file, 0, seek_mode::begin);
		    if(pSetEndOfFile) pSetEndOfFile(handle->os_handle);
			#else
		    typedef int(*FTRUNC)(int,long);
		    static FTRUNC pFtruncate = (FTRUNC)get_system_function("ftruncate", "libc.so.6");
		    if(pFtruncate) pFtruncate(handle->fd, 0);
			#endif
            return _sz;
		}
		bool exist(const char* filepath){
		    if(!filepath) return false;
		    #if defined(_WIN32)
		    typedef unsigned int(__stdcall *GFA)(const char*);
		    static GFA pGetFileAttributesA = (GFA)get_system_function("GetFileAttributesA");
		    return pGetFileAttributesA ? (pGetFileAttributesA(filepath) != 0xFFFFFFFF) : false;
		    #else
		    struct _stat_linux {
				unsigned long  st_dev;     unsigned long  st_ino;
				unsigned long  st_nlink;   unsigned int   st_mode;
				unsigned int   st_uid;     unsigned int   st_gid;
				unsigned long  st_rdev;    long           st_size;
				long           st_blksize; long           st_blocks;
				unsigned long  st_atime;   unsigned long  st_atime_nsec;
				unsigned long  st_mtime;   unsigned long  st_mtime_nsec;
				unsigned long  st_ctime;   unsigned long  st_ctime_nsec;
			} st;
		    typedef int(*XSTAT)(const char*,void*);
		    static XSTAT pStat = (XSTAT)get_system_function("stat", "libc.so.6");
		    return pStat ? (pStat(filepath, &st) == 0) : false;
		    #endif
		}
		const char* remove(const char* filepath){
			if(!file::exist(filepath)) return nullptr;
			#if defined(_WIN32)
		    typedef int(__stdcall *DF)(const char*);
		    static DF pDeleteFileA = (DF)get_system_function("DeleteFileA");
		    static DF pRemoveDirectoryA = (DF)get_system_function("RemoveDirectoryA");
		    if(!pDeleteFileA || !pRemoveDirectoryA) return nullptr;
		    if(pDeleteFileA(filepath)) return filepath;
		    struct WIN32_FIND_DATAA{
		        unsigned int attr; long long t[3]; unsigned int s[2], r[2];
		        char name[260], aname[14];
		    } fData;
		    typedef void*(__stdcall *FFF)(const char*,void*);
		    static FFF pFindFirst = (FFF)get_system_function("FindFirstFileA");
		    typedef int(__stdcall *FNF)(void*,void*);
		    static FNF pFindNext = (FNF)get_system_function("FindNextFileA");
		    typedef int(__stdcall *FC)(void*);
		    static FC pFindClose = (FC)get_system_function("FindClose");
		    if(!pFindFirst || !pFindNext || !pFindClose) return nullptr;
		    UWORD filepath_size = 0; { const char* _l = filepath; while(*_l++) ++filepath_size; }
			char* pattern = new char[filepath_size + 3];
		    sprintf(pattern, "%s\\*", filepath);
		    void* hFind = pFindFirst(pattern, &fData);
		    delete[] pattern;
		    if(hFind != (void*)-1){
		        do{
		            if(fData.name[0] == '\0'
					|| (fData.name[0] == '.' && (fData.name[1] == '\0'
					                         || (fData.name[1] == '.' && fData.name[2] == '\0')))) continue;
		            char* child_name = new char[260]; UWORD child_sname = 0;
		            pattern = fData.name - 4;
		            while(*pattern && child_sname != 259) child_name[child_sname++] = *pattern++;
		            child_name[child_sname] = '\0';
		            char* full_child_path = new char[filepath_size + child_sname + 2];
				    sprintf(full_child_path, "%s\\%s", filepath, child_name);
					delete[] child_name;
					file::remove(full_child_path);
				    delete[] full_child_path;
		        }while(pFindNext(hFind, &fData));
		        pFindClose(hFind);
		    }
		    return pRemoveDirectoryA(filepath) ? filepath : nullptr;
			#else
		    typedef int(*UNLINK)(const char*);
		    static UNLINK pUnlink = (UNLINK)get_system_function("unlink", "libc.so.6");
		    typedef int(*RMDIR)(const char*);
		    static RMDIR pRmdir = (RMDIR)get_system_function("rmdir", "libc.so.6");
		    if(!pUnlink || !pRmdir) return nullptr;
		    if(!pUnlink(filepath)) return filepath;
		    return pRmdir(filepath) ? nullptr : filepath;
			#endif
		}
		void* remove(void* file){
			if(FLE_HANDLE_VALID(file)) return nullptr;
		    FLE_HANDLE_DECL;
            FLE_HANDLE* handle = (FLE_HANDLE*)file;
            char* path = handle->full_path; handle->full_path = nullptr;
            file::close(file);
			bool Removed = (file::remove(path) == path);
			delete[] path;
			return Removed ? file : nullptr;
		}
		void* folder(const char* filepath, UWORD openMode){
		    if(!filepath || openMode == file_mode::none) return nullptr;
		    if(openMode & file_mode::create){
				#if defined(_WIN32)
				typedef int(__stdcall *CDA)(const char*, void*);
				static CDA pCreateDirectoryA = (CDA)get_system_function("CreateDirectoryA");
				if(pCreateDirectoryA) pCreateDirectoryA(filepath, nullptr);
				#else // Linux
				typedef int(*MKDIR)(const char*, unsigned int);
				static MKDIR pMkdir = (MKDIR)get_system_function("mkdir", "libc.so.6");
				if(pMkdir) pMkdir(filepath, 0775);
				#endif
			}
		    #if defined(_WIN32)
		    typedef void*(__stdcall *CFA)(const char*,unsigned int,unsigned int,void*,unsigned int,unsigned int,void*);
			static CFA pCreateFileA = (CFA)get_system_function("CreateFileA");
		    typedef UWORD(__stdcall *GFP)(const char*,UWORD,char*,char**);
		    static GFP pGetFullPathNameA = (GFP)get_system_function("GetFullPathNameA");
		    if(!pGetFullPathNameA || !pCreateFileA) return nullptr;
		    void* h = pCreateFileA(filepath, 0x80000000, 7, nullptr, 3, 0x02000080, nullptr);
		    UWORD req = pGetFullPathNameA(filepath, 0, nullptr, nullptr);
		    char* abs_path = new char[req];
		    pGetFullPathNameA(filepath, (unsigned int)req, abs_path, nullptr);
		    FLE_HANDLE_DECL;
		    FLE_HANDLE* handle = new FLE_HANDLE();
		    handle->os_handle  = h;
		    handle->open_mode  = openMode;
		    handle->full_path  = abs_path;
		    handle->full_spath = req - 1;
		    handle->is_dir     = true;
		    #else // Linux
		    typedef int(*POPEN)(const char*,int,...);
			static POPEN pOpen = (POPEN)get_system_function("open", "libc.so.6");
			typedef int(*CLOSE)(int);
			static CLOSE pClose = (CLOSE)get_system_function("close", "libc.so.6");
		    typedef char*(*RPATH)(const char*,char*);
		    static RPATH pRealpath = (RPATH)get_system_function("realpath", "libc.so.6");
		    typedef void(*FREE)(void*);
		    static FREE pFree = (FREE)get_system_function("free", "libc.so.6");
		    if(!pOpen || !pClose || !pRealpath || !pFree) return nullptr;
		    int fd = pOpen(filepath, 200000);
		    char* tmp = pRealpath(filepath, nullptr);
		    if(!tmp){ if(fd) pClose(fd); return nullptr; }
		    UWORD len = 0; while(tmp[len]) len++;
		    char* abs_path = new char[len + 1];
		    for(UWORD i=0; i<=len; i++) abs_path[i] = tmp[i];
		    pFree(tmp);
		    FLE_HANDLE_DECL;
		    FLE_HANDLE* handle = new FLE_HANDLE();
		    handle->fd = fd;
		    handle->full_path = abs_path;
		    handle->is_dir = true;
		    handle->open_mode = openMode;
		    #endif
		    return (void*)handle;
		}
		void* inside(void* folder, UWORD idx){
		    if(!file::is_folder(folder)) return nullptr;
		    FLE_HANDLE_DECL;
		    FLE_HANDLE* h = (FLE_HANDLE*)folder;
		    char* child_name = nullptr;
		    UWORD child_sname = 0;
		    bool is_a_folder = false;
			#if defined(_WIN32)
		    struct WIN32_FIND_DATAA{
		        unsigned int attr; long long t[3]; unsigned int s[2], r[2];
		        char name[260], aname[14];
		    } fData;
		    typedef void*(__stdcall *FFF)(const char*,void*);
		    static FFF pFindFirst = (FFF)get_system_function("FindFirstFileA");
		    typedef int(__stdcall *FNF)(void*,void*);
		    static FNF pFindNext = (FNF)get_system_function("FindNextFileA");
		    typedef int(__stdcall *FC)(void*);
		    static FC pFindClose = (FC)get_system_function("FindClose");
		    if(!pFindFirst || !pFindNext || !pFindClose) return nullptr;
			char* pattern = new char[h->full_spath + 3];
		    sprintf(pattern, "%s\\*", h->full_path);
		    void* hFind = pFindFirst(pattern, &fData);
		    delete[] pattern;
		    if(hFind != (void*)-1){
		        do{
		            if(fData.name[0] == '\0'
					|| (fData.name[0] == '.' && (fData.name[1] == '\0'
					                         || (fData.name[1] == '.' && fData.name[2] == '\0')))) continue;
		            if(!idx--){
		            	child_name = new char[260];
		            	pattern = fData.name - 4;
		            	while(*pattern && child_sname != 259) child_name[child_sname++] = *pattern++;
		            	child_name[child_sname] = '\0';
		            	is_a_folder = (fData.attr & 0x10);
		                break;
		            }
		        }while(pFindNext(hFind, &fData));
		        pFindClose(hFind);
		    }
			#else // Linux
			struct Sdir{
				unsigned long i, o;
				unsigned short r;
				unsigned char t;
				char name[256];
			};
		    typedef void*(*ODIR)(const char*);
		    static ODIR pOpendir = (ODIR)get_system_function("opendir", "libc.so.6");
		    typedef Sdir*(*RDIR)(void*);
		    static RDIR pReaddir = (RDIR)get_system_function("readdir", "libc.so.6");
		    typedef int(*CDIR)(void*);
		    static CDIR pClosedir = (CDIR)get_system_function("closedir", "libc.so.6");
		    if(!pOpendir || !pReaddir || !pClosedir) return nullptr;
		    void* dir = pOpendir(h->full_path);
		    if(dir){
		        Sdir* entry = nullptr;
		        while(entry = pReaddir(dir)){
		            if(entry->name[0] == '.' && (entry->name[1] == '\0' || (entry->name[1] == '.' && entry->name[2] == '\0'))) continue;
		            if(!idx--){
		            	child_name = new char[256];
		            	char* analized = entry->name;
		                while(*analized && child_sname != 255) child_name[child_sname++] = *analized++;
		            	child_name[child_sname] = '\0';
		            	is_a_folder = (entry->t == 0x4);
		                break;
		            }
		        }
		        pClosedir(dir);
		    }
			#endif
			void* ret = nullptr;
		    if(child_name){
		        char* full_child_path = new char[h->full_spath + child_sname + 2];
		        sprintf(full_child_path,
		        #if defined(_WIN32)
				"%s\\%s"
		        #else
		        "%s/%s"
		        #endif
				, h->full_path, child_name);
				delete[] child_name;
		        ret = (is_a_folder ? file::folder(full_child_path, h->open_mode) : file::open(full_child_path, h->open_mode));
		        delete[] full_child_path;
		    }
		    return ret;
		}
		bool is_folder(void* file){
		    if(FLE_HANDLE_VALID(file)) return false;
		    FLE_HANDLE_DECL;
		    return ((FLE_HANDLE*)file)->is_dir;
		}
		void* name(void* file, char** fname, UWORD* fnsize, bool with_extension){
			if(FLE_HANDLE_VALID(file) || (!fname || !fnsize)) return nullptr;
		    FLE_HANDLE_DECL;
		    FLE_HANDLE* h = (FLE_HANDLE*)file;
			char* start = h->full_path + h->full_spath;
		    while(start > h->full_path && *(start - 1) != '/' && *(start - 1) != '\\') --start;
		    char* dot = h->full_path + h->full_spath;
		    while(dot > start && *dot != '.') --dot;
		    char* end = h->full_path + h->full_spath;
		    if(!with_extension && *dot == '.') end = dot;
		    UWORD final_size = (UWORD)(end - start);
		    if(fnsize) *fnsize = final_size;
		    if(fname){
		        if(*fname) delete[] *fname;
		        *fname = new char[final_size + 1];
		        for(UWORD i = 0; i < final_size; i++) (*fname)[i] = start[i];
		        (*fname)[final_size] = '\0';
		    }
		    return file;
		}
		void* extension(void* file, char** fname, UWORD* fnsize, bool with_dot){
			if(FLE_HANDLE_VALID(file) || (!fname && !fnsize)) return nullptr;
		    FLE_HANDLE_DECL;
            FLE_HANDLE* handle = (FLE_HANDLE*)file;
			char* Hname = handle->full_path;
		    UWORD Sname = handle->full_spath;
		    Hname += Sname;
			while((Hname < handle->full_path) ? false : (*Hname != '/' || *Hname != '\\' || *Hname != '.')) --Hname;
			if((Hname < handle->full_path) ? true : (!with_dot && *Hname == '.')) ++Hname;
			Sname = Sname - (UWORD)(Hname - (handle->full_path));
			if(fnsize) *fnsize = Sname;
		    if(fname){
		    	if(*fname) delete[] *fname;
				*fname = new char[Sname + 1];
				Hname = handle->full_path;
		    	char* name = *fname;
		    	while(*Hname) *name++ = *Hname++;
		    	*name = '\0';
			}
			return file;
		}
		void* path(void* file, char** fname, UWORD* fnsize){
			if(FLE_HANDLE_VALID(file) || (!fname && !fnsize)) return nullptr;
			FLE_HANDLE_DECL;
            FLE_HANDLE* handle = (FLE_HANDLE*)file;
            if(fnsize) *fnsize = handle->full_spath;
            if(fname){
		    	if(*fname) delete[] *fname;
				*fname = new char[handle->full_spath + 1];
				char* Hname = handle->full_path;
		    	char* name  = *fname;
		    	while(*Hname) *name++ = *Hname++;
		    	*name = '\0';
			}
			return file;
		}
		void root_this(char** fname, UWORD* fnsize){
			if(!fname && !fnsize) return;
            char* root_path = nullptr;
            UWORD root_len = 0;
            #if defined(_WIN32)
            typedef unsigned int(__stdcall *GMF)(void*, char*, unsigned int);
            static GMF pGetModuleFileNameA = (GMF)get_system_function("GetModuleFileNameA");
            if(!pGetModuleFileNameA) return;
            char temp[260];
            root_len = pGetModuleFileNameA(nullptr, temp, 260);
            if(fname){
                if(*fname) delete[] *fname;
                *fname = new char[root_len + 1];
                root_path = *fname;
                pGetModuleFileNameA(nullptr, root_path, (unsigned int)root_len + 1);
            }
            #else // Linux
            typedef int(*LSTAT)(const char*, void*);
            static LSTAT pLstat = (LSTAT)get_system_function("lstat", "libc.so.6");
            typedef long (*RDLNK)(const char*, char*, unsigned long);
            static RDLNK pReadlink = (RDLNK)get_system_function("readlink", "libc.so.6");
            if(!pLstat || !pReadlink) return;
            struct{ unsigned long long st[18]; } st_buf;
            if(pLstat("/proc/self/exe", &st_buf) == 0) root_len = 1024; 
            if(fname && root_len){
                if (*fname) delete[] *fname;
                *fname = new char[root_len + 1];
                root_path = *fname;
                long r = pReadlink("/proc/self/exe", root_path, (unsigned long)root_len);
                if(r > 0){
                    root_len = (UWORD)r;
                    root_path[root_len] = '\0';
                }
            }
            #endif
            if(root_path && root_len > 0){
                char* p = root_path + root_len;
                while (p > root_path && *p != '/' && *p != '\\') p--;
                if(p > root_path){
                    *(p + 1) = '\0';
                    root_len = (UWORD)(p - root_path) + 1;
                }
            }
            if(fnsize) *fnsize = root_len;
        }
		char* normalize(char* path, UWORD* path_size){
			if(!path) return nullptr;
			char* s = path;
			char* ss = s;
			#if defined(___norm_char_to_search__1_2__3__)
			#undef ___norm_char_to_search__1_2__3__
			#endif
			#if defined(___norm_char_to_replace__1_2__3__)
			#undef ___norm_char_to_replace__1_2__3__
			#endif
			#if defined(_WIN32)
			#define ___norm_char_to_search__1_2__3__  '/'
			#define ___norm_char_to_replace__1_2__3__ '\\'
			#else // Linux
			#define ___norm_char_to_search__1_2__3__  '\\'
			#define ___norm_char_to_replace__1_2__3__ '/'
			#endif
			while(*s){
				if (*s == ___norm_char_to_search__1_2__3__
				|| (*s == ___norm_char_to_replace__1_2__3__ && *(s + 1) == ___norm_char_to_replace__1_2__3__)
				|| (*s == ___norm_char_to_replace__1_2__3__ && *(s + 1) == ___norm_char_to_search__1_2__3__))
					*ss = ___norm_char_to_replace__1_2__3__;
				else *ss++ = *s;
				++s;
			}
			#undef ___norm_char_to_search__1_2__3__
			#undef ___norm_char_to_replace__1_2__3__
			*ss = '\0';
			if(path_size) *path_size = (UWORD)(ss - path);
			return path;
		}
		void to_date(UWORD ts, char* sec, char* min, char* hour, char* day, char* month, int* year){
		    if(!sec && !min && !hour && !day && !month && !year) return;
		    if(sec)  *sec  = (char)(ts % 60); ts /= 60;
		    if(min)  *min  = (char)(ts % 60); ts /= 60;
		    if(hour) *hour = (char)(ts % 24); ts /= 24;
		    UWORD z = ts + 719468;
		    UWORD era = z / 146097;
		    UWORD doe = z - era * 146097;
		    UWORD yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;
		    int y = (int)(yoe) + era * 400;
		    UWORD doy = doe - (365*yoe + yoe/4 - yoe/100);
		    UWORD mp = (5*doy + 2)/153;
		    UWORD d = doy - (153*mp + 2)/5 + 1;
		    UWORD m = mp < 10 ? mp + 3 : mp - 9;
		    y += (m > 2) ? 0 : 1;
		    if(year)  *year = y;
		    if(month) *month = (char)m;
		    if(day)   *day = (char)d;
		}
		void to_time(UWORD* ts, char sec, char min, char hour, char day, char month, int year){
			if(!ts) return;
		    *ts = 0;
		    for(int y = 1970; y < year; y++) *ts += (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) ? 31622400 : 31536000;
		    static const char mdays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		    bool biyear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
		    for(char m = 0; m < month - 1; m++)
		        *ts += (UWORD)(mdays[m] + ((m == 1 && biyear)?1:0)) * 86400;
		    *ts += (day - 1) * 86400 + hour * 3600 + min * 60 + sec;
		}
		UWORD days_until_2000(){
			UWORD now_ts = 0;
            #if defined(_WIN32)
            unsigned long long ft;
            typedef void(__stdcall *GST)(void*);
            static GST pGetSystemTime = (GST)get_system_function("GetSystemTimeAsFileTime");
            if(!pGetSystemTime) return 0;
            pGetSystemTime(&ft);
            now_ts = (ft / 10000000ULL) - 11644473600ULL;
            #else // Linux
            struct{ long sec; long nsec; } ts;
            typedef int(*CGT)(int,void*);
            static CGT pClockGetTime = (CGT)get_system_function("clock_gettime", "libc.so.6");
            if(!pClockGetTime) return 0;
            if(!pClockGetTime(0, &ts)) now_ts = (UWORD)ts.sec;
            #endif
            static const UWORD y2k_ts = 946684800ULL;
            if(now_ts < y2k_ts) return 0; // Por si acaso alguien viaja al pasado
            return (now_ts - y2k_ts);
		}
		UWORD apends(void* file, const char* str){
			if(FLE_HANDLE_VALID(file) || !str) return 0;
			static const char _end = '\0';
			const char* _s = str; while(*_s) ++_s;
			return file::write(file, str, (UWORD)(_s - str)) + file::write(file, &_end, 1);
		}
		UWORD apendc(void* file, int chr){
			if(FLE_HANDLE_VALID(file) || !chr) return 0;
			char len = 0;
			char str[4];
			if(chr < 0x80) str[len++] = (char)(chr & 0xFF);
			else if(chr < 0x800){
			    str[len++] = (char)(0xC0 | (chr >> 6));
			    str[len++] = (char)(0x80 | (chr & 0x3F));
			}else if(chr < 0x10000){
			    str[len++] = (char)(0xE0 | (chr >> 12));
			    str[len++] = (char)(0x80 | ((chr >> 6) & 0x3F));
			    str[len++] = (char)(0x80 | (chr & 0x3F));
			}else if(chr < 0x110000){
				str[len++] = (char)(0xF0 | (chr >> 18));
				str[len++] = (char)(0x80 | ((chr >> 12) & 0x3F));
				str[len++] = (char)(0x80 | ((chr >> 6) & 0x3F));
				str[len++] = (char)(0x80 | (chr & 0x3F));
			}
			if(!len) return 0;
			return file::write(file, str, len);
		}
		UWORD apendws(void* file, const wchar_t* wstr){
			if(FLE_HANDLE_VALID(file) || !wstr) return 0;
			static const char _end = '\0';
			UWORD len = 0;
			while(*wstr) len += apendc(file, (int)*wstr++);
			return len + file::write(file, &_end, 1);
		}
		UWORD gets(void* file, char** str){
			if(FLE_HANDLE_VALID(file) || !str) return 0;
			if(*str) delete[] *str;
			char _c; UWORD _start = file::tell(file), len = 0;
			while(file::read(file, &_c, 1) ? _c : 0) ++len;
			*str = new char[++len]; file::seek(file, (WORD)_start, seek_mode::begin);
			return file::read(file, *str, len);
		}
		UWORD getc(void* file, int* chr){
			if(FLE_HANDLE_VALID(file) || !chr) return 0;
			unsigned char b;
		    if(!file::read(file, &b, 1)) return 0;
		    if(b < 0x80){ // 1 byte (ASCII)
		        *chr = b;
		        return 1;
		    }else if((b & 0xE0) == 0xC0){ // 2 bytes
		        unsigned char b2;
		        if (!file::read(file, &b2, 1)) return 0;
		        *chr = ((b & 0x1F) << 6) | (b2 & 0x3F);
		        return 2;
		    }else if((b & 0xF0) == 0xE0){ // 3 bytes
		        unsigned char b2, b3;
		        if(file::read(file, &b2, 1) && file::read(file, &b3, 1)){
		            *chr = ((b & 0x0F) << 12) | ((b2 & 0x3F) << 6) | (b3 & 0x3F);
		            return 3;
		        }
		    }else if((b & 0xF8) == 0xF0){ // 4 bytes
		        unsigned char b2, b3, b4;
		        if(file::read(file, &b2, 1) && file::read(file, &b3, 1) && file::read(file, &b4, 1)){
		            *chr = ((b & 0x07) << 18) | ((b2 & 0x3F) << 12) | ((b3 & 0x3F) << 6) | (b4 & 0x3F);
		            return 4;
		        }
		    }
		    return 0;
		}
		UWORD getws(void* file, wchar_t** wstr){
			if(FLE_HANDLE_VALID(file) || !wstr) return 0;
			if(*wstr) delete[] *wstr;
		    int tmp_chr; UWORD start = file::tell(file), count = 0;
		    while(file::getc(file, &tmp_chr) ? tmp_chr : 0) ++count;
		    *wstr = new wchar_t[count + 1];
		    file::seek(file, (WORD)start, seek_mode::begin);
		    (*wstr)[count] = L'\0';
		    while(count--){
		        file::getc(file, &tmp_chr);
		        (*wstr)[count] = (wchar_t)tmp_chr;
		    }
		    file::getc(file, &tmp_chr);
		    return count;
		}
		UWORD apendo(void* file, void* Hobj){
			if(FLE_HANDLE_VALID(file) || !Hobj) return 0;
			UWORD magic = *(UWORD*)Hobj, len = 0;
			if(magic == 0x54485244) return 0; // THREADs are not allowed (... Yet?)
			#if defined(____apendo_bit_size__text__1_2__3__)
			#undef ____apendo_bit_size__text__1_2__3__
			#endif
			#if defined(____apendo_bit_size__ordr__1_2__3__)
			#undef ____apendo_bit_size__ordr__1_2__3__
			#endif
			#if defined(____apendo_bit_size__WORD__1_2__3__)
			#undef ____apendo_bit_size__WORD__1_2__3__
			#endif
			#if defined(_WIN64) || defined(__x86_64__) || defined(__x86_64)
			#define ____apendo_bit_size__text__1_2__3__ "@"
			#define ____apendo_bit_size__WORD__1_2__3__ 8
			#else // x32
			#define ____apendo_bit_size__text__1_2__3__ " "
			#define ____apendo_bit_size__WORD__1_2__3__ 4
			#endif
			#if IS_BIG_ENDIAN == 1
			#define ____apendo_bit_size__ordr__1_2__3__ "<"
			#else
			#define ____apendo_bit_size__ordr__1_2__3__ ">"
			#endif
			FLE_HANDLE_DECL;
			if(magic == 0x46494C45){ // File
				FLE_HANDLE* handle = (FLE_HANDLE*)Hobj;
				len = file::write(file, "FILE" ____apendo_bit_size__text__1_2__3__ ____apendo_bit_size__ordr__1_2__3__, 6)
				    + file::write(file, &(handle->open_mode), 4)
					+ file::write(file, handle->full_path, handle->full_spath + 1);
			}
			#if defined(ST_IMPORT_WINDOW)
			WND_HANDLE_DECL;
			if(magic == 0x574E444F){ // Window
				WND_HANDLE* handle = (WND_HANDLE*)Hobj;
				if(!WND_HANDLE_STATE(handle)) return 0;
				char* title = nullptr; if(!window::gettitle(Hobj, &title, nullptr)){
					if(title) delete[] title;
					return 0;
				}
				char config = ((handle->minimized)?0x02:0) | ((handle->invisible)?0x01:0);
				len = file::write(file, "WIND" ____apendo_bit_size__text__1_2__3__ ____apendo_bit_size__ordr__1_2__3__, 6)
				    + file::apends(file, title)
					+ file::write(file, &(handle->width), 4)
				    + file::write(file, &(handle->height), 4)
					+ file::write(file, &(handle->x), 4)
					+ file::write(file, &(handle->y), 4)
					+ file::write(file, &config, 1)
					+ file::write(file, &(handle->pixels_amont), ____apendo_bit_size__WORD__1_2__3__)
					+ file::write(file, handle->pixels, (handle->pixels_amont) << 2);
				delete[] title;
			}
			#endif
			#if defined(ST_IMPORT_SOUND)
			SND_HANDLE_DECL;
			if(magic == 0x534E444F){
				SND_HANDLE* handle = (SND_HANDLE*)Hobj;
				char config = ((handle->buffers[0].busy) ? 0x01 : 0x00) | ((handle->buffers[1].busy) ? 0x02 : 0x00);
				len = file::write(file, "SOND" ____apendo_bit_size__text__1_2__3__ ____apendo_bit_size__ordr__1_2__3__, 6)
				    + file::write(file, &(handle->sample_rate), 4)
				    + file::write(file, &(handle->channels), 1) + file::write(file, &(handle->bits), 1)
				    + file::write(file, &(handle->volume), 4) + file::write(file, &(handle->pitch), 4)
				    + file::write(file, (handle->master_volume)?"M":" ", 1)
					+ ((handle->master_volume) ? file::write(file, handle->master_volume, 4) : 0)
					+ file::write(file, &(handle->buffers_size), 4) + file::write(file, &config, 1)
					+ file::write(file, &(handle->current_buffer), 4);
				if(handle->buffers_size){
					if(handle->buffers[0].data) len += file::write(file, "D", 1) + file::write(file, handle->buffers[0].data, handle->buffers_size);
					else                        len += file::write(file, " ", 1);
					if(handle->buffers[1].data) len += file::write(file, "D", 1) + file::write(file, handle->buffers[1].data, handle->buffers_size);
					else                        len += file::write(file, " ", 1);
				}
			}
			TSN_HANDLE_DECL;
			if(magic == 0x4E534854){
				TSN_HANDLE* handle = (TSN_HANDLE*)Hobj;
				len = file::write(file, "TSND" ____apendo_bit_size__text__1_2__3__ ____apendo_bit_size__ordr__1_2__3__, 6)
				    + file::write(file, (handle->snd) ?"S":" ", 1) + ((handle->snd)  ? file::apendo(file, handle->snd)  : 0)
				    + file::write(file, (handle->file)?"F":" ", 1) + ((handle->file) ? file::apendo(file, handle->file) : 0)
				    + file::write(file, (handle->canIcloser)?"C":" ", 1) + file::write(file, &(handle->target_volume), 4)
				    + file::write(file, &(handle->fade_step), 4);
			}
			#endif
			return len;
		}
		UWORD geto(void* file, void** Hobj){
			if(FLE_HANDLE_VALID(file) || !Hobj) return 0;
			struct _ap_{
				static int read_int(void* file, char _inv){
					int _ret; file::read(file, &_ret, 4);
					if(_inv != ____apendo_bit_size__ordr__1_2__3__[0]) _ret =
						((_ret & 0x000000FF) << 24) |
						((_ret & 0x0000FF00) <<  8) |
						((_ret & 0x00FF0000) >>  8) |
						((_ret & 0xFF000000) >> 24) ;
					return _ret;
				}
				static float read_float(void* file, char _inv){
					int i_ret = read_int(file, _inv);
					return *(float*)&i_ret;
				}
				static UWORD read_word(void* file, char _n_s, char _inv){
					UWORD _ret;
					#if ____apendo_bit_size__WORD__1_2__3__ == 4
					if(_n_s == ' ') file::read(file, &_ret, 4);
					if(_n_s == '@'){
						if(_inv == '>'){
							file::read(file, &_1_u, 4);
							file::seek(file, 2, seek_mode::current);
						}
						if(_inv == '<'){
							file::seek(file, 2, seek_mode::current);
							file::read(file, &_1_u, 4);
						}
					}
					if(_inv != ____apendo_bit_size__ordr__1_2__3__[0]) _ret =
						((_ret & 0x000000FF) << 24) |
						((_ret & 0x0000FF00) <<  8) |
						((_ret & 0x00FF0000) >>  8) |
						((_ret & 0xFF000000) >> 24) ;
					#else // == 8
					if(_n_s == ' ') file::read(file, &_ret, 4);
					if(_n_s == '@') file::read(file, &_ret, 8);
					if(_inv != ____apendo_bit_size__ordr__1_2__3__[0]) _ret =
						((_ret & 0x00000000000000FF) << 56) |
						((_ret & 0x000000000000FF00) << 52) |
						((_ret & 0x0000000000FF0000) << 48) |
						((_ret & 0x00000000FF000000) << 44) |
						((_ret & 0x000000FF00000000) >> 44) |
						((_ret & 0x0000FF0000000000) >> 48) |
						((_ret & 0x00FF000000000000) >> 52) |
						((_ret & 0xFF00000000000000) >> 56) ;
					#endif
					return _ret;
				}
			};
			if(*Hobj){
				UWORD magic = *(UWORD*)(*Hobj);
				if(magic == 0x54485244) thread::kill(*Hobj);
				if(magic == 0x46494C45) file::close(*Hobj);
				#if defined(ST_IMPORT_WINDOW)
				if(magic == 0x574E444F) window::destroy(*Hobj, false);
				#endif
				#if defined(ST_IMPORT_SOUND)
				if(magic == 0x534E444F || magic == 0x4E534854) sound::destroy(*Hobj);
				#endif
				*Hobj = nullptr;
			}
			char head[6];
			UWORD _org_pos = file::tell(file);
		    if(file::read(file, head, 6) != 6){
		    	file::seek(file, _org_pos, seek_mode::begin);
				return 0;
			}
		    if(head[0] == 'F' && head[1] == 'I' && head[2] == 'L' && head[3] == 'E'){
		        UWORD mode; file::read(file, &mode, 4);
		        char* path = nullptr; file::gets(file, &path);
		        *Hobj = file::open(path, mode);
		        if(path) delete[] path;
		    }
			#if defined(ST_IMPORT_WINDOW)
		    else if(head[0] == 'W' && head[1] == 'I' && head[2] == 'N' && head[3] == 'D'){
		        char* title = nullptr;
		        file::gets(file, &title);
		        int w = _ap_::read_int(file, head[5]);
				int h = _ap_::read_int(file, head[5]);
				int x = _ap_::read_int(file, head[5]);
				int y = _ap_::read_int(file, head[5]);
		        char config; file::read(file, &config, 1);
		        *Hobj = window::create(nullptr, title, w, h);
		        if(title) delete[] title;
		        window::resize(*Hobj, x, y, 0, 0, true, false);
		        window::show(*Hobj, config & 0x01, config & 0x02);
		        UWORD pixels_amont = _ap_::read_word(file, head[4], head[5]);
		        unsigned int* pixels = new unsigned int[pixels_amont];
		        file::read(file, pixels, pixels_amont << 2);
		        window::draw(*Hobj, 0, 0, pixels, w, h);
		        delete[] pixels;
		    }
			#endif
			#if defined(ST_IMPORT_SOUND)
			else if(head[0] == 'S' && head[1] == 'O' && head[2] == 'N' && head[3] == 'D'){
				char config;
				SND_HANDLE_DECL;
				SND_HANDLE* handle = new SND_HANDLE();
				handle->sample_rate = (unsigned int)_ap_::read_int(file, head[5]);
				file::read(file, &(handle->channels), 1); file::read(file, &(handle->bits), 1);
				handle->volume = _ap_::read_float(file, head[5]);
				handle->pitch = _ap_::read_float(file, head[5]);
				file::read(file, &config, 1); if(config == 'M') handle->master_volume = new float(_ap_::read_float(file, head[5]));
				handle->buffers_size = (unsigned int)_ap_::read_int(file, head[5]);
				file::read(file, &config, 1); handle->buffers[0].busy = config & 0x01, handle->buffers[1].busy = config & 0x02;
				handle->current_buffer = _ap_::read_int(file, head[5]);
				if(handle->buffers_size){
					file::read(file, &config, 1); if(config == 'D'){
						handle->buffers[0].data = new char[handle->buffers_size];
						file::read(file, handle->buffers[0].data, handle->buffers_size);
					}
					file::read(file, &config, 1); if(config == 'D'){
						handle->buffers[1].data = new char[handle->buffers_size];
						file::read(file, handle->buffers[1].data, handle->buffers_size);
					}
				}
				*Hobj = (void*)handle;
			}
			else if(head[0] == 'T' && head[1] == 'S' && head[2] == 'N' && head[3] == 'D'){
				char config;
				TSN_HANDLE_DECL;
				TSN_HANDLE* handle = new TSN_HANDLE();
				file::read(file, &config, 1); if(config == 'S') file::geto(file, &(handle->snd));
				file::read(file, &config, 1); if(config == 'F') file::geto(file, &(handle->file));
				file::read(file, &config, 1); handle->canIcloser = config == 'C';
				handle->target_volume = _ap_::read_float(file, head[5]);
				handle->fade_step = _ap_::read_float(file, head[5]);
				*Hobj = (void*)handle;
			}
			#endif
		    else file::seek(file, _org_pos, seek_mode::begin);
		    #undef ____apendo_bit_size__text__1_2__3__
			#undef ____apendo_bit_size__WORD__1_2__3__
			#undef ____apendo_bit_size__ordr__1_2__3__
			return file::tell(file) - _org_pos;
		}
		UWORD opened_like(void* file){
			if(FLE_HANDLE_VALID(file)) return -1;
			FLE_HANDLE_DECL;
			return ((FLE_HANDLE*)file)->open_mode;
		}
	}
	#if defined(ST_IMPORT_SOUND)
	namespace sound{
    	void* file_stream(void* file, bool looping, float pitch, bool canIcloser){
			if(FLE_HANDLE_VALID(file)) return nullptr;
			if(file::opened_like(file) != file_mode::read) return nullptr;
			SND_HANDLE_DECL;
			TSN_HANDLE_DECL;
			THR_HANDLE_DECL;
			struct _ap_{
				struct _MSG_{
					TSN_HANDLE* thsn;
					bool looping;
					bool canIcloser;
					float pitch;
					bool ready;
				};
				static void _exec_(_MSG_* msg){
					TSN_HANDLE* thsn = msg->thsn;
					bool looping = msg->looping;
					bool canIcloser = msg->canIcloser;
				    float current_pitch = msg->pitch;
					void* wav = thsn->file;
					unsigned short channels, bits;
				    unsigned int rate;
				    file::seek(wav, 22, seek_mode::begin);
				    file::read(wav, &channels, 2);
				    file::read(wav, &rate, 4);
				    file::seek(wav, 34, seek_mode::begin);
				    file::read(wav, &bits, 2);
				    unsigned short block_align = (channels * (bits >> 3));
					UWORD raw_size = (UWORD)((float)rate * (float)block_align * current_pitch) / 10;
					UWORD frame_stream_size = (raw_size / block_align) * block_align;
					void* snd = sound::create(frame_stream_size);
					if(!snd){
						msg->thsn = nullptr;
						msg->ready = true;
				    	return;
					}
					SND_HANDLE* snd_h = (SND_HANDLE*)snd;
					snd_h->pitch = current_pitch;
					thsn->snd = snd;
				    sound::set(snd, rate, (unsigned char)channels, (unsigned char)bits);
				    void* thr_h = thsn->thr;
				    char* temp_buffer = new char[frame_stream_size];
				    sound::play(snd);
					msg->ready = true;
					msg = nullptr;
				    do{
					    file::seek(wav, 44, seek_mode::begin);
					    bool end_of_file = false;
					    while(!end_of_file){
					    	if(thsn->autodestruction) break;
					    	if(snd_h->volume != thsn->target_volume){
							    snd_h->volume += thsn->fade_step;
							    if((thsn->fade_step > 0 && snd_h->volume > thsn->target_volume) ||
							        (thsn->fade_step < 0 && snd_h->volume < thsn->target_volume)){
							        snd_h->volume = thsn->target_volume;
							        thsn->fade_step = 0.0f;
							    }
							}
					    	if(current_pitch != snd_h->pitch){
					    		thsn->snd = nullptr;
								sound::stop(snd);
					    		current_pitch = snd_h->pitch;
							    unsigned short block_align = (channels * (bits >> 3));
								UWORD raw_size = (UWORD)((float)rate * (float)block_align * current_pitch) / 10;
								UWORD frame_stream_size = (raw_size / block_align) * block_align;
					    		delete[] temp_buffer; temp_buffer = new char[frame_stream_size];
					    		float vol = volume(snd); float* mst_vol = master_volume(snd);
					    		sound::destroy(snd); snd = sound::create(frame_stream_size);
				    			sound::set(snd, rate, (unsigned char)channels, (unsigned char)bits);
					    		snd_h = (SND_HANDLE*)snd;
					    		snd_h->pitch = current_pitch;
					    		snd_h->volume = vol;
					    		snd_h->master_volume = mst_vol;
								thsn->snd = snd;
								sound::play(snd);
							}
					    	if(!sound::playing(snd)){
					    		sleep(10);
					    		continue;
							}
					        if(sound::sync(snd)){
					            UWORD readed = file::read(wav, temp_buffer, frame_stream_size);
					            if(readed > 0) sound::stream(snd, temp_buffer, readed);
					            else           end_of_file = true;
					        }
					        unsigned int i = *(unsigned int*)&current_pitch;
						    i = 0x7F000000 - i;
						    float recip = *(float*)&i;
						    recip = recip * (2.0f - current_pitch * recip);
							unsigned int dynamic_sleep = (unsigned int)(50.0f * recip);
	                        if(dynamic_sleep < 5) dynamic_sleep = 5;
	                        #if !defined(_WIN32)
	                        dynamic_sleep -= 5;
	                        #endif
	                        sleep(dynamic_sleep);
					    }
					    while(snd_h->buffers[0].busy || snd_h->buffers[1].busy){
						    sound::sync(snd);
						    sleep(10);
						}
					}while(looping && !(thsn->autodestruction));
				    delete[] temp_buffer;
				    delete (THR_HANDLE*)thr_h; thsn->thr = nullptr;
				    sound::destroy(snd);
				    if(canIcloser) file::close(wav);
				}
			};
			_ap_::_MSG_* msg = new _ap_::_MSG_{ new TSN_HANDLE(), looping, canIcloser, pitch, false };
			TSN_HANDLE* thsn = msg->thsn;
			thsn->file = file;
			thsn->thr = thread::create((void*(*)(void*))_ap_::_exec_, msg);
			if(!(thsn->thr)){
				delete thsn;
				delete msg;
				return nullptr;
			}
			while(!(msg->ready)) sleep(10);
			if(!(msg->thsn)){
				thread::kill(thsn->thr);
				delete thsn;
				delete msg;
				return nullptr;
			}
			delete msg;
			thsn->canIcloser = canIcloser;
			return (void*)thsn;
		}
	}
	#endif
	#endif
	#if defined(ST_IMPORT_SOUND)
	#undef SND_HANDLE_DECL
	#undef SND_HANDLE_VALID
	#undef TSN_HANDLE_DECL
	#undef TSN_HANDLE_UNVALID
	#endif
	#if defined(ST_IMPORT_WINDOW)
	#undef WND_HANDLE_DECL
	#undef WND_HANDLE_VALID
	#undef WND_HANDLE_STATE
	#endif
	#if defined(ST_IMPORT_FILE)
	#undef FLE_HANDLE_DECL
	#undef FLE_HANDLE_VALID
	#endif
}
#if !defined(_INC_STDIO) || !defined(_INC_STDLIB)
namespace System_Tools{ namespace System_Tools = System_Tools; }
#if !defined(_INC_STDIO)
#define _INC_STDIO
#define system(str)                        System_Tools::system(str)
#define puts(str)                          System_Tools::puts(str)
#define putchar(chr)                       System_Tools::putchar(chr)
#define printf(str, ...)                   System_Tools::printf(str, ##__VA_ARGS__)
#define sprintf(dest_str, format_str, ...) System_Tools::sprintf(dest_str, format_str, ##__VA_ARGS__)
#define scanf(str, ...)                    System_Tools::scanf(str, ##__VA_ARGS__)
#endif
#if !defined(_INC_STDLIB)
#define _INC_STDLIB
#define sleep(miliseconds)                 System_Tools::sleep(miliseconds)
#define alloc(size)                        System_Tools::memory::alloc(size)
#define clloc(dest, src, size)             System_Tools::memory::clloc(dest, src, size)
#define slloc(dest, value, size)           System_Tools::memory::slloc(dest, value, size)
#define mlloc(dest, src, size)             System_Tools::memory::mlloc(dest, src, size)
#define realloc(ptr, size)                 System_Tools::memory::realloc(ptr, size)
#define free(ptr)                          System_Tools::memory::free(ptr)
#define cmplloc(p1, p2, size)              System_Tools::memory::cmplloc(p1, p2, size)
#define cnrlloc(p1, p2, size)              System_Tools::memory::cnrlloc(p1, p2, size)
#endif
#endif
