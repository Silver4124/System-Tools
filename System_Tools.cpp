#if !defined(__SYSTEM_TOOLS__AUTO_IMPORT_FIRST__) && !defined(__SYSTEM_TOOLS__DECL_TOOLS_FIRST__) && !defined(__SYSTEM_TOOLS__CPP_FIRST__)
	#define __SYSTEM_TOOLS__CPP_FIRST__ 1
	#if defined(__SYSTEM_TOOLS__H_FIRST__)
		#undef __SYSTEM_TOOLS__CPP_FIRST__
		#define __SYSTEM_TOOLS__CPP_FIRST__ 2
	#endif
		#include "System_Tools.h"
	#if __SYSTEM_TOOLS__H_FIRST__ == __SYSTEM_TOOLS__CPP_FIRST__
		#undef __SYSTEM_TOOLS__H_FIRST__
		#define __SYSTEM_TOOLS__H_FIRST__ 2
	#endif
#if defined(WINCALL)
#undef WINCALL
#endif
#if defined(_WIN32) && !defined(_WIN64) && (defined(__GNUC__) || defined(__clang__))
	#define WINCALL __attribute__((__stdcall__))
#elif defined(_WIN32) && !defined(_WIN64)
	#define WINCALL __stdcall
#else
	#define WINCALL
#endif
// Avoid Intellisense errors relationated with 'asm volatile': //
// Note for myself: I hate Interllissense.
#if defined(__asm_volatile__)
#undef __asm_volatile__
#endif
#if defined(__GNUC__) || defined(__clang__) || defined(__INTELLISENSE__)
	#define __asm_volatile__(...) __asm__ __volatile__(__VA_ARGS__)
#elif defined(_MSC_VER)
	#define __asm_volatile__(...) __asm{ __VA_ARGS__ }
#else
	#define __asm_volatile__(...) asm volatile(__VA_ARGS__)
#endif
// /////////////////////////////////////////////////////////// //
namespace System_Tools{
	#if defined(FUNCDECL)
	#undef FUNCDECL
	// Personal development note:
	// If you're working within 'System_Tools.cpp', try using the 'FUNCDECL' macro instead of writing the usual function definition when creating it.
	// This way, only the signatures created in 'System_Tools.h' will be visible to the editor,
	// instead of all of them, resulting in a cleaner and less redundant symbol tree built by the editor.
	#endif
	// 'inline' is what lets two .cpp files of one project include the header without the
	// linker answering 'multiple definition': without it the library demands a single TU.
	#define FUNCDECL(signature, ...) inline signature(__VA_ARGS__)
	FUNCDECL(void* get_system_function, const char* RESTRICT func_name, const char* RESTRICT lib_target){
		if(!func_name) return nullptr;
		struct _ap_{
			#if !defined(_WIN32)
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
			#endif
			static void* get_export(void* RESTRICT module_base, const char* RESTRICT func_name){
			    if(!module_base) return nullptr;
		    	#if defined(_WIN32)
			    BYTE* base = (BYTE*)module_base;
			    if(*(unsigned short*)base != 0x5A4D) return nullptr;
			    unsigned int e_lfanew = *(unsigned int*)(base + 0x3C);
			    BYTE* nt_header = base + e_lfanew;
			    if(*(unsigned int*)nt_header != 0x00004550) return nullptr;
			    BYTE* data_dir_ptr = nt_header + 0x78
			    #if WORD_SIZE == 8
		        	+ 0x10
			    #endif
			    ;
			    unsigned int export_dir_rva  = *(unsigned int*)(data_dir_ptr);
				unsigned int export_dir_size = *(unsigned int*)(data_dir_ptr + 4);
			    if(!export_dir_rva) return nullptr;
			    BYTE* export_dir = base + export_dir_rva;
			    unsigned int num_names = *(unsigned int*)(export_dir + 0x18);
			    unsigned int names_rva = *(unsigned int*)(export_dir + 0x20);
			    unsigned int funcs_rva = *(unsigned int*)(export_dir + 0x1C);
			    unsigned int ords_rva  = *(unsigned int*)(export_dir + 0x24);
			    unsigned int* names = (unsigned int*)(base + names_rva);
			    unsigned int* funcs = (unsigned int*)(base + funcs_rva);
			    unsigned short* ords = (unsigned short*)(base + ords_rva);
				char lib_name[256];
			    for(unsigned int i = 0; i < num_names; i++){
			        const char* n = (const char*)(base + names[i]);
			        const char* f = func_name;
			        while(*f && *f == *n){ ++f; ++n; }
			        if(*f == *n){
			        	unsigned int func_rva = funcs[ords[i]];
						if(func_rva >= export_dir_rva && func_rva < (export_dir_rva + export_dir_size)){
							char* forward_str = (char*)(base + func_rva);
							const char* func_name_ptr = nullptr;
							UWORD i = 0; while(forward_str[i] && forward_str[i] != '.' && i != 250){
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
			    	BYTE st_info;
			    	BYTE st_other;
			    	unsigned short st_shndx;
			    	UWORD st_value;
			    	UWORD st_size;
			    };
			    Link_Map* base = (Link_Map*)module_base;
			    if(base ? (!(base->l_ld)) : true) return nullptr;
			    Elf_Dyn* dyn = base->l_ld;
				Elf_Sym* symtab = nullptr;
				char* strtab = nullptr;
				unsigned int* hash_ptr = nullptr;
				unsigned int* gnu_hash_ptr = nullptr;
			    while(dyn->d_tag){
			        if(dyn->d_tag == 6)          symtab       = (Elf_Sym*)(dyn->d_val);      // DT_SYMTAB
			        if(dyn->d_tag == 5)          strtab       = (char*)(dyn->d_val);         // DT_STRTAB
			        if(dyn->d_tag == 4)          hash_ptr     = (unsigned int*)(dyn->d_val); // DT_HASH
			        if(dyn->d_tag == 0x6FFFFEF5) gnu_hash_ptr = (unsigned int*)(dyn->d_val); // DT_GNU_HASH
			        ++dyn;
			    }
			    if(!symtab || !strtab) return nullptr;
			    if((UWORD)symtab < base->l_addr) symtab = (Elf_Sym*)((UWORD)symtab + base->l_addr);
				if((UWORD)strtab < base->l_addr) strtab = (char*)((UWORD)strtab + base->l_addr);
				unsigned int num_symbols = 2048;
				if(gnu_hash_ptr){
					if((UWORD)gnu_hash_ptr < base->l_addr) gnu_hash_ptr = (unsigned int*)((UWORD)gnu_hash_ptr + base->l_addr);
					unsigned int nbuckets   = gnu_hash_ptr[0];
					unsigned int symoffset  = gnu_hash_ptr[1];
					unsigned int bloom_size = gnu_hash_ptr[2];
					UWORD* bloom = (UWORD*)(gnu_hash_ptr + 4);
					unsigned int* buckets = (unsigned int*)(bloom + bloom_size);
					unsigned int* chain = buckets + nbuckets;
					unsigned int max_chain = 0;
					for(unsigned int i = 0; i < nbuckets; ++i) if(buckets[i] > max_chain) max_chain = buckets[i];
					num_symbols = symoffset + 1000;
					if(max_chain >= symoffset){
						while(!(chain[max_chain - symoffset] & 1)) max_chain++;
						num_symbols = max_chain + 1;
					}
				}
				else if(hash_ptr){
					if((UWORD)hash_ptr < base->l_addr) hash_ptr = (unsigned int*)((UWORD)hash_ptr + base->l_addr);
					num_symbols = hash_ptr[1];
				}
			    for(unsigned int i = 0; i < num_symbols; ++i){
			        if(!(symtab[i].st_name)) continue;
			        const char* n = strtab + symtab[i].st_name;
			        const char* f = func_name;
			        while(*f && *n && *f == *n){ ++f; ++n; }
			        if(!*f && !*n && symtab[i].st_value) return (void*)(base->l_addr + symtab[i].st_value);
			    }
			    #endif
				return nullptr;
			}
			static void* ensure_user(const char* RESTRICT lib_target){
		    	#if defined(_WIN32)
		    	void* k32 = nullptr;
				#if WORD_SIZE == 8
				#if defined(_MSC_VER) && !defined(__clang__)
		        k32 = *(void**)((***(BYTE****)((*(BYTE**)(((BYTE*)(__readgsqword(0x60))) + 0x18)) + 0x10)) + 0x30);
				#else
				__asm_volatile__(
					"movq %%gs:0x60, %%rax\n\t"
					"movq 0x18(%%rax), %%rax\n\t"
					"movq 0x10(%%rax), %%rax\n\t"
					"movq (%%rax), %%rax\n\t"
					"movq (%%rax), %%rax\n\t"
					"movq 0x30(%%rax), %0\n\t"
					: "=r" (k32) : : "rax", "memory"
				);
				#endif
				#else
				#if defined(_MSC_VER) && !defined(__clang__)
		        k32 = *(void**)((***(BYTE****)((*(BYTE**)(((BYTE*)(UWORD)(__readfsdword(0x30))) + 0x0C)) + 0x0C)) + 0x18);
		    	#else
		        __asm_volatile__(
		            "movl %%fs:0x30, %%eax\n\t"
		            "movl 0x0c(%%eax), %%eax\n\t"
		            "movl 0x0c(%%eax), %%eax\n\t"
		            "movl (%%eax), %%eax\n\t"
		            "movl (%%eax), %%eax\n\t"
		            "movl 0x18(%%eax), %0\n\t"
		            : "=r" (k32) : : "eax", "memory"
		        );
		    	#endif
				#endif
			    if(!lib_target) return k32;
			    struct UNICODE_STRING{
			    	unsigned short Length;
			    	unsigned short MaximumLength;
			    	wchar* Buffer;
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
				    BYTE Reserved4[8];
				    void* Reserved5[3];
				    union{
				    	unsigned long CheckSum;
				    	void* Reserved6;
				    };
				    unsigned long TimeDateStamp;
				};
				UNICODE_STRING SEQ_LIB; {
					const char* _org = lib_target;
					unsigned short char_count = 0;
					while(*_org){
				        if((*_org & 0xF8) == 0xF0){ char_count += 2; _org += 4; }
				        else{
				            char_count += ((*_org < 0x80) || ((*_org & 0xE0) == 0xC0) || ((*_org & 0xF0) == 0xE0));
				            if(*_org < 0x80) ++_org;
				            else if((*_org & 0xE0) == 0xC0) _org += 2;
				            else if((*_org & 0xF0) == 0xE0) _org += 3;
				            else ++_org;
				        }
				    }
				    SEQ_LIB.Length = char_count * sizeof(wchar);
				    SEQ_LIB.MaximumLength = SEQ_LIB.Length;
					_org = lib_target;
					SEQ_LIB.Buffer = new wchar[char_count];
					wchar* w_org = SEQ_LIB.Buffer;
					while(*_org){
				        if(*_org < 0x80) *w_org++ = (wchar)(*_org++);
				        else if((*_org & 0xE0) == 0xC0){
				            *w_org++ = (wchar)(((*_org & 0x1F) << 6) | (_org[1] & 0x3F));
				            _org += 2;
				        }
				        else if((*_org & 0xF0) == 0xE0){
				            *w_org++ = (wchar)(((*_org & 0x0F) << 12) | ((_org[1] & 0x3F) << 6) | (_org[2] & 0x3F));
				            _org += 3;
				        }
				        else if((*_org & 0xF8) == 0xF0){
				            unsigned int cp = (((*_org & 0x07) << 18) | ((_org[1] & 0x3F) << 12) | ((_org[2] & 0x3F) << 6) | (_org[3] & 0x3F));
				            cp -= 0x10000;
				            *w_org++ = (wchar)((cp >> 10) + 0xD800);
				            *w_org++ = (wchar)((cp & 0x3FF) + 0xDC00);
				            _org += 4;
				        }
				        else ++_org;
				    }
				}
				void* peb;
				#if WORD_SIZE == 8
				#if defined(_MSC_VER) && !defined(__clang__)
		        peb = (void*)__readgsqword(0x60);
		    	#else
		        __asm_volatile__(
		            "movq %%gs:0x60, %0"
		            : "=r"(peb)
		        );
			    #endif
			    void* ldr = *(void**)((BYTE*)peb + 0x18);
			    void* list_sentinel = (void*)((BYTE*)ldr + 0x10);
			    void* list_head     = *(void**)list_sentinel;
				#else
				#if defined(_MSC_VER) && !defined(__clang__)
			    peb = (void*)(UWORD)__readfsdword(0x30);
			    #else
			    __asm_volatile__(
			        "movl %%fs:0x30, %0"
			        : "=r"(peb)
			    );
			    #endif
			    void* ldr = *(void**)((BYTE*)peb + 0x0C);
			    void* list_sentinel = (void*)((BYTE*)ldr + 0x0C);
			    void* list_head     = *(void**)list_sentinel;
				#endif
			    void* current_node = list_head;
				while(current_node && current_node != list_sentinel){
				    LDR_DATA_TABLE_ENTRY* entry = (LDR_DATA_TABLE_ENTRY*)current_node;
				    if(entry->FullDllName.Buffer){
				    	const wchar* ws1 = entry->FullDllName.Buffer;
				    	unsigned short wSs1 = (entry->FullDllName.Length) >> 1;
			    		if(wSs1 > 1 && (*ws1 == L'\\' || *(ws1 + 1) == L':')){
			    			ws1 += wSs1 - 1;
			    			while(ws1 != entry->FullDllName.Buffer && *ws1 != L'\\') --ws1;
			    			if(*ws1 == L'\\') ++ws1;
			    			wSs1 = (unsigned short)((entry->FullDllName.Buffer + wSs1) - ws1);
			    		}
			    		const wchar* ws2 = SEQ_LIB.Buffer;
				        unsigned short wSs2 = SEQ_LIB.Length >> 1;
				        if(wSs1 == wSs2){
				            const wchar *wE1 = ws1 + wSs1;
				            while(ws1 != wE1){
				            	wchar c1 = *ws1, c2 = *ws2;
				            	if(c1 >= L'A' && c1 <= L'Z') c1 += 32;
				            	if(c2 >= L'A' && c2 <= L'Z') c2 += 32;
				            	if(c1 != c2) break;
				            	++ws1; ++ws2;
				            }
				            if(ws1 == wE1) return entry->DllBase;
				        }
				    }
				    current_node = *(void**)current_node;
				}
			    typedef void*(WINCALL *LLF)(const char*);
			    static LLF pLoadLibraryA = (LLF)get_export(k32, "LoadLibraryA");
			    if(pLoadLibraryA) return pLoadLibraryA(lib_target);
			    #else // Linux
			    struct r_debug{
					int r_version;
					Link_Map* r_map;
				};
				const char* libbase = "libc.so.6";
				if(!lib_target) lib_target = libbase;
			    UWORD dynamic_addr = 0;
			    #if WORD_SIZE == 8
			    __asm_volatile__(
			        "lea _DYNAMIC(%%rip), %0" 
			        : "=r"(dynamic_addr)
			    );
			    #else
			    __asm_volatile__(
			        "call 1f\n\t"
			        "1: pop %%eax\n\t"
			        "addl $_GLOBAL_OFFSET_TABLE_, %%eax\n\t"
			        "movl _DYNAMIC@GOT(%%eax), %0"
			        : "=r"(dynamic_addr) : : "eax"
			    );
			    #endif
			    Elf_Dyn* dyn_ptr = (Elf_Dyn*)dynamic_addr;
			    UWORD* plt_got = nullptr;
			    for(UWORD i = 0; dyn_ptr[i].d_tag; ++i){
			        if(dyn_ptr[i].d_tag == 3){ // DT_PLTGOT
			            plt_got = (UWORD*)(dyn_ptr[i].d_val);
			            break;
			        }
			    }
			    if(!plt_got) return nullptr;
			    UWORD self_base = 0;
			    if((UWORD)plt_got < dynamic_addr){
			        UWORD* temp_got = (UWORD*)((UWORD)plt_got + (dynamic_addr - (UWORD)plt_got));
			        self_base = dynamic_addr - (UWORD)plt_got;
			        plt_got = (UWORD*)((UWORD)plt_got + self_base);
			    }
			    Link_Map* map = nullptr;
				if(plt_got ? plt_got[1] : false) map = (Link_Map*)(plt_got[1]); else
					for(UWORD i = 0; dyn_ptr[i].d_tag; ++i) if(dyn_ptr[i].d_tag == 21){
						r_debug* rdbg = (r_debug*)(dyn_ptr[i].d_val);
						if(rdbg) map = rdbg->r_map;
						break;
					}
				if(map) while(map && map->l_prev) map = map->l_prev;
			    Link_Map* libc_map = nullptr;
			    while(map){
			        if((map->l_name) ? (*(map->l_name)) : false){
			            const char* s1 = map->l_name;
			            const char* last_slash = nullptr;
			            for(const char* p = s1; *p != '\0'; ++p) if(*p == '/') last_slash = p;
			            if(last_slash) s1 = last_slash + 1;
			            const char* sl1 = s1;
			            const char* s2 = lib_target;
			            while(*sl1 && *sl1 == *s2){ ++sl1; ++s2; }
			            if(*sl1 == *s2) return (void*)map;
			            if(!libc_map){
			                const char* check_libc = s1;
			                const char* target_libc = libbase;
			                while(*check_libc && *check_libc == *target_libc){ ++check_libc; ++target_libc; }
			                if(*check_libc == *target_libc || (*target_libc == '\0' && *check_libc == '.')) libc_map = map;
			            }
			        }
			        map = map->l_next;
			    }
			    if(libc_map){
			        typedef void*(*DLOP)(const char*,int);
			        static DLOP pDlOpen = (DLOP)get_export((void*)libc_map, "dlopen");
			        if(pDlOpen) return pDlOpen(lib_target, 1);
			    }
			    #endif
			    return nullptr;
			}
		};
        return _ap_::get_export(_ap_::ensure_user(lib_target), func_name);
    }
    FUNCDECL(void* get_system_variable, const char* RESTRICT var_name, const char* RESTRICT lib_target){
    	if(!var_name) return nullptr;
    	#if defined(_WIN32)
		typedef void*(WINCALL *LLL)(const char*);
		static LLL pLoadLibraryA = (LLL)get_system_function("LoadLibraryA");
		typedef void*(WINCALL *GPA)(void*,const char*);
		static GPA pGetProcAddress = (GPA)get_system_function("GetProcAddress");
		typedef char*(WINCALL *GESA)();
		static GESA pGetEnvironmentStringsA = (GESA)get_system_function("GetEnvironmentStringsA");
		if (!pLoadLibraryA || !pGetProcAddress || !pGetEnvironmentStringsA) return nullptr;
		#else
		typedef void*(*DLO)(const char*,int);
		static DLO pDlOpen = (DLO)get_system_function("dlopen", "libdl.so.2");
		if(!pDlOpen) pDlOpen = (DLO)get_system_function("dlopen");
		typedef void*(*DLS)(void*,const char*);
		static DLS pDlSym = (DLS)get_system_function("dlsym", "libdl.so.2");
		if(!pDlSym) pDlSym = (DLS)get_system_function("dlsym");
		typedef char*(*GE)(const char*);
		static GE pGetenv = (GE)get_system_function("getenv");
		if(!pDlOpen || !pDlSym || !pGetenv) return nullptr;
		#endif
    	if(lib_target ? (*lib_target != '\0') : false){
    		#if defined(_WIN32)
		    void* hModule = pLoadLibraryA(lib_target);
        	if(hModule) return pGetProcAddress(hModule, var_name);
			#else
			void* handle = pDlOpen(lib_target, 1);
		    if(handle) return pDlSym(handle, var_name);
			#endif
		}
		UWORD name_len = 0; while(var_name[name_len]) ++name_len;
    	#if defined(_WIN32)
		char*** pEnvTable = (char***)get_system_function("_environ", "ucrtbase.dll");
		if(!pEnvTable) pEnvTable = (char***)get_system_function("_environ", "msvcrt.dll");
		if(pEnvTable ? (*pEnvTable != nullptr) : false){
		    char** env = *pEnvTable;
		    while(*env){
		        char* entry = *env;
		        for(UWORD i = 0; i < name_len; ++i)
					if(((entry[i] >= 'a' && entry[i] <= 'z') ? (entry[i] - 'a' + 'A') : (entry[i]))
						!=
					   ((var_name[i] >= 'a' && var_name[i] <= 'z') ? (var_name[i] - 'a' + 'A') : (var_name[i])))
			            goto _get_system_variable__jump__noMatch1;
		        if(entry[name_len] == '=') return (void*)((BYTE*)entry + name_len + 1);
		        _get_system_variable__jump__noMatch1:
		        env++;
		    }
		}
		char* env_block = pGetEnvironmentStringsA();
		if(env_block) while(*env_block != '\0'){
		    for(UWORD i = 0; i < name_len; ++i)
		    	if(((env_block[i] >= 'a' && env_block[i] <= 'z') ? (env_block[i] - 'a' + 'A') : (env_block[i]))
					!=
				   ((var_name[i] >= 'a' && var_name[i] <= 'z') ? (var_name[i] - 'a' + 'A') : (var_name[i])))
		        	goto _get_system_variable__jump__noMatch2;
		    if(env_block[name_len] == '=') return (void*)((BYTE*)env_block + name_len + 1);
		    _get_system_variable__jump__noMatch2:
		    while(*env_block) ++env_block;
		    ++env_block;
		}
		return nullptr;
		#else
		void* sym = pDlSym((void*)-1l, var_name);
		if(!sym) sym = pDlSym((void*)-1l, "__environ");
		if(!sym) sym = pDlSym((void*)-1l, "environ");
    	bool is_env_symbol = (name_len == 7 && var_name[0]=='e' && var_name[1]=='n' && var_name[2]=='v' && var_name[3]=='i'
    	                     && var_name[4]=='r' && var_name[5]=='o' && var_name[6]=='n') ||
                             (name_len == 9 && var_name[0]=='_' && var_name[1]=='_' && var_name[2]=='e' && var_name[3]=='n'
                             && var_name[4]=='v' && var_name[5]=='i' && var_name[6]=='r' && var_name[7]=='o' && var_name[8]=='n');
		if(is_env_symbol && sym) return sym;
        if(!sym){
			void* handle = (void*)pGetenv(var_name);
			if(handle) return handle;
		}
		char** env = *(char***)sym;
		if(env) while(*env){
		    char* entry = *env;
		    for(UWORD i = 0; i < name_len; ++i)
		    	if(((entry[i] >= 'a' && entry[i] <= 'z') ? (entry[i] - 'a' + 'A') : (entry[i]))
					!=
				   ((var_name[i] >= 'a' && var_name[i] <= 'z') ? (var_name[i] - 'a' + 'A') : (var_name[i])))
			        goto _get_system_variable__jump__noMatch1;
		    if(entry[name_len] == '=') return (void*)((BYTE*)entry + name_len + 1);
		    _get_system_variable__jump__noMatch1:
		    env++;
		}
		return (void*)pGetenv(var_name);
		#endif
    }
    FUNCDECL(int system, const char* cmd){
    	if(!cmd) return -1;
    	typedef int(*SYS)(const char*);
    	#if defined(_WIN32)
		static SYS pSystem = (SYS)get_system_function("system", "msvcrt.dll");
		if(!pSystem){
			pSystem = (SYS)get_system_function("_system", "msvcrt.dll");
			if(!pSystem){
				pSystem = (SYS)get_system_function("system", "ucrtbase.dll");
				if(!pSystem) return -1;
			}
		}
    	#else // Linux
		static SYS pSystem = (SYS)get_system_function("system");
		if(!pSystem) return -1;
		#endif
		return pSystem(cmd);
    }
    #if defined(_WIN32)
    FUNCDECL(ALWAYSINLINE void clear){ system("cls"); }
	#else
	FUNCDECL(ALWAYSINLINE void clear){ system("clear"); }
	#endif
    FUNCDECL(int puts, const char* str){
    	if(!str) return -1;
        typedef int(*PUTS)(const char*);
        #if defined(_WIN32)
        static PUTS pPuts = (PUTS)get_system_function("puts", "msvcrt.dll");
		if(!pPuts){
			pPuts = (PUTS)get_system_function("_puts", "msvcrt.dll");
			if(!pPuts){
				pPuts = (PUTS)get_system_function("puts", "ucrtbase.dll");
				if(!pPuts) return -1;
			}
		}
        #else // Linux
        static PUTS pPuts = (PUTS)get_system_function("puts");
		if(!pPuts) return -1;
        #endif
        return pPuts(str);
    }
    FUNCDECL(int putchar, int chr){
        typedef int(*PUTCHAR)(int);
        #if defined(_WIN32)
        static PUTCHAR pPutchar = (PUTCHAR)get_system_function("putchar", "msvcrt.dll");
		if(!pPutchar){
			pPutchar = (PUTCHAR)get_system_function("_putchar", "msvcrt.dll");
			if(!pPutchar){
				pPutchar = (PUTCHAR)get_system_function("putchar", "ucrtbase.dll");
				if(!pPutchar) return -1;
			}
		}
        #else // Linux
        static PUTCHAR pPutchar = (PUTCHAR)get_system_function("putchar");
		if(!pPutchar) return -1;
        #endif
        return pPutchar(chr);
    }
    // NOINLINE: the VA_START scaffolding on Windows walks the stack by hand, and that is
    // only valid while the variadic function keeps its own frame.
    FUNCDECL(NOINLINE int printf, const char* RESTRICT format, ...){
        VA_START(format, args);
    	if(!format) return -1;
    	typedef int(*VPRINTF)(const char* RESTRICT,void*);
    	#if defined(_WIN32)
	    typedef void*(*FN_acrt_iob_func)(unsigned int);
	    typedef int(*FN_stdio_common_vfprintf)(unsigned long long,void*,const char* RESTRICT,void*,void*);
	    static VPRINTF pVPrintf = (VPRINTF)get_system_function("vprintf", "msvcrt.dll");
	    static FN_acrt_iob_func pAcrtIobFunc = (FN_acrt_iob_func)get_system_function("__acrt_iob_func", "ucrtbase.dll");
	    static FN_stdio_common_vfprintf pUcrtVfprintf = (FN_stdio_common_vfprintf)get_system_function("__stdio_common_vfprintf", "ucrtbase.dll");
	    static void* pStdout = nullptr;
	    if(!pVPrintf && pAcrtIobFunc && pUcrtVfprintf){
	    	if(!pStdout) pStdout = pAcrtIobFunc(1);
	    	return pUcrtVfprintf(0ULL, pStdout, format, nullptr, args);
		}
		if(!pVPrintf) return -1;
		return pVPrintf(format, args);
	    #else
	    static VPRINTF pVPrintf = (VPRINTF)get_system_function("vprintf");
        if(!pVPrintf){
			VA_END(args);
			return -1;
		}
		int _ret = pVPrintf(format, args);
		VA_END(args);
		return _ret;
	    #endif
    }
    FUNCDECL(NOINLINE int sprintf, char* RESTRICT str, const char* RESTRICT format, ...){
        VA_START(format, args);
    	if(!str || !format) return -1;
        typedef int(*VSPRINTF)(char* RESTRICT,const char* RESTRICT,void*);
        #if defined(_WIN32)
        typedef int(*FN_stdio_common_vsprintf)(unsigned long long,char* RESTRICT,UWORD,const char* RESTRICT,void*,void*);
	    static VSPRINTF pVSprintf = (VSPRINTF)get_system_function("_vsprintf", "msvcrt.dll");
	    static FN_stdio_common_vsprintf pUcrtVsprintf = (FN_stdio_common_vsprintf)get_system_function("__stdio_common_vsprintf", "ucrtbase.dll");
	    if(!pVSprintf && pUcrtVsprintf) return pUcrtVsprintf(0ULL, str, (UWORD)-1, format, nullptr, args);
		if(!pVSprintf){
			pVSprintf = (VSPRINTF)get_system_function("vsprintf", "msvcrt.dll");
			if(!pVSprintf) return -1;
		}
	    return pVSprintf(str, format, args);
	    #else
	    static VSPRINTF pVSprintf = (VSPRINTF)get_system_function("vsprintf");
	    if(!pVSprintf){
			VA_END(args);
			return -1;
		}
		int _ret = pVSprintf(str, format, args);
		VA_END(args);
		return _ret;
	    #endif
    }
    FUNCDECL(NOINLINE int scanf, const char* RESTRICT format, ...){
        VA_START(format, args);
    	if(!format) return -1;
        typedef int(*VSCANF)(const char* RESTRICT,void*);
        #if defined(_WIN32)
        typedef void*(*FN_acrt_iob_func)(unsigned int);
	    typedef int(*FN_stdio_common_vfscanf)(unsigned long long,void*,const char* RESTRICT,void*,void*);
        static VSCANF pVScanf = (VSCANF)get_system_function("vscanf", "msvcrt.dll");
	    static FN_acrt_iob_func pAcrtIobFunc = (FN_acrt_iob_func)get_system_function("__acrt_iob_func", "ucrtbase.dll");
	    static FN_stdio_common_vfscanf pUcrtVfscanf = (FN_stdio_common_vfscanf)get_system_function("__stdio_common_vfscanf", "ucrtbase.dll");
	    static void* pStdin = nullptr;
	    if(!pVScanf && pAcrtIobFunc && pUcrtVfscanf){
	        if(!pStdin) pStdin = pAcrtIobFunc(0);
	        return pUcrtVfscanf(0ULL, pStdin, format, nullptr, args);
	    }
	    if(!pVScanf){
	        pVScanf = (VSCANF)get_system_function("vscanf", "ucrtbase.dll");
	        if(!pVScanf) return -1;
	    }
	    return pVScanf(format, args);
		#else // Linux
        static VSCANF pVScanf = (VSCANF)get_system_function("vscanf");
        if(!pVScanf){
			VA_END(args);
			return -1;
		}
		int _ret = pVScanf(format, args);
		VA_END(args);
		return _ret;
        #endif
    }
    FUNCDECL(NOINLINE int sscanf, const char* RESTRICT str, const char* RESTRICT format, ...){
        VA_START(format, args);
    	if(!str || !format) return -1;
        typedef int(*VSSCANF)(const char* RESTRICT,const char* RESTRICT,void*);
        #if defined(_WIN32)
        typedef int(*FN_stdio_common_vsscanf)(unsigned long long,const char* RESTRICT,UWORD,const char* RESTRICT,void*,void*);
        static VSSCANF pVSscanf = (VSSCANF)get_system_function("vsscanf", "msvcrt.dll");
        static FN_stdio_common_vsscanf pUcrtVsscanf = (FN_stdio_common_vsscanf)get_system_function("__stdio_common_vsscanf", "ucrtbase.dll");
        if(!pVSscanf && pUcrtVsscanf) return pUcrtVsscanf(0ULL, str, (UWORD)-1, format, nullptr, args);
		if(!pVSscanf){
			pVSscanf = (VSSCANF)get_system_function("vsscanf", "ucrtbase.dll");
			if(!pVSscanf) return -1;
		}
        return pVSscanf(str, format, args);
		#else // Linux
        static VSSCANF pVSscanf = (VSSCANF)get_system_function("vsscanf");
        if(!pVSscanf){
			VA_END(args);
			return -1;
		}
		int _ret = pVSscanf(str, format, args);
		VA_END(args);
		return _ret;
        #endif
    }
    FUNCDECL(void sleep, unsigned int ms){
    	#if defined(_WIN32)
        typedef void(WINCALL *SFN)(unsigned int);
        static SFN psleep = (SFN)get_system_function("Sleep");
        if(!psleep) return;
        psleep(ms);
        #else // Linux
        typedef int(*SFN)(unsigned int);
        static SFN pUsleep = (SFN)get_system_function("usleep");
        if(!pUsleep) return;
        pUsleep((ms<<10)-(ms<<4)-(ms<<3));
        #endif
    }
	namespace memory{
		FUNCDECL(void* alloc, UWORD size){
			if(size == 0) return nullptr;
		    #if defined(_WIN32)
		    typedef void*(WINCALL *H_ALC)(void*,unsigned int,UWORD);
		    static H_ALC pAlloc = (H_ALC)get_system_function("HeapAlloc");
		    typedef void*(WINCALL *G_HP)();
		    static G_HP pGetProcessHeap = (G_HP)get_system_function("GetProcessHeap");
		    if(!pAlloc || !pGetProcessHeap) return nullptr;
		    void* hHeap = pGetProcessHeap();
			if(!hHeap) return nullptr;
		    return pAlloc(hHeap, 0, size);
		    #else
		    typedef void*(*M_ALC)(UWORD);
		    static M_ALC pMalloc = (M_ALC)get_system_function("malloc");
		    typedef void*(*M_SET)(void*,int,UWORD);
		    static M_SET pMemset = (M_SET)get_system_function("memset");
		    if(!pMalloc || !pMemset) return nullptr;
		    void* ptr = pMalloc(size);
		    if(ptr) pMemset(ptr, 0, size);
		    return ptr;
		    #endif
		}
		FUNCDECL(void* clloc, void* RESTRICT dest, const void* RESTRICT src, UWORD size){
		    if(!dest || !src || size == 0) return nullptr;
		    if(dest == src) return dest;
		    #if defined(_WIN32)
		    typedef void*(*M_CPY)(void* RESTRICT,const void* RESTRICT,UWORD);
		    static M_CPY pMemcpy = (M_CPY)get_system_function("memcpy", "msvcrt.dll");
		    #else
		    typedef void*(*M_CPY)(void* RESTRICT,const void* RESTRICT,UWORD);
		    static M_CPY pMemcpy = (M_CPY)get_system_function("memcpy");
		    #endif
		    if(!pMemcpy) return nullptr;
		    pMemcpy(dest, src, size);
		    return dest;
		}
		FUNCDECL(void* realloc, void* ptr, UWORD size){
			if(!ptr) return memory::alloc(size);
		    if(size == 0) return memory::free(ptr);
		    #if defined(_WIN32)
		    typedef void*(WINCALL *H_RAL)(void*,unsigned long,void*,UWORD);
		    static H_RAL pRealloc = (H_RAL)get_system_function("HeapReAlloc");
		    typedef void*(WINCALL *GPH)();
		    static GPH pGetProcessHeap = (GPH)get_system_function("GetProcessHeap");
		    if(!pRealloc || !pGetProcessHeap) return nullptr;
		    void* hHeap = pGetProcessHeap();
			if(!hHeap) return nullptr;
		    return pRealloc(hHeap, 0, ptr, size);
		    #else
		    typedef void*(*M_RAL)(void*,UWORD);
		    static M_RAL pRealloc = (M_RAL)get_system_function("realloc");
		    if(!pRealloc) return nullptr;
		    return pRealloc(ptr, size);
		    #endif
		}
		FUNCDECL(void* free, void* ptr){
			if(!ptr) return nullptr;
		    #if defined(_WIN32)
		    typedef int(WINCALL *H_FRE)(void*,unsigned int,void*);
		    static H_FRE pFree = (H_FRE)get_system_function("HeapFree");
		    typedef void*(WINCALL *G_HP)();
		    static G_HP pGetProcessHeap = (G_HP)get_system_function("GetProcessHeap");
		    if(!pFree || !pGetProcessHeap) return ptr;
		    void* hHeap = pGetProcessHeap();
			if(!hHeap) return ptr;
		    pFree(hHeap, 0, ptr);
		    #else
		    typedef void(*M_FRE)(void*);
		    static M_FRE pFree = (M_FRE)get_system_function("free");
		    if(!pFree) return ptr;
		    pFree(ptr);
		    #endif
		    return nullptr;
		}
		FUNCDECL(void* slloc, void* dest, BYTE value, UWORD size){
			if(!dest || size == 0) return nullptr;
		    typedef void*(*M_SET)(void*,int,UWORD);
		    #if defined(_WIN32)
		    static M_SET pMemset = (M_SET)get_system_function("memset", "msvcrt.dll");
		    #else
		    static M_SET pMemset = (M_SET)get_system_function("memset");
		    #endif
		    if(!pMemset) return nullptr;
		    pMemset(dest, value, size);
		    return dest;
		}
		FUNCDECL(void* cnrlloc, const void* p1, const void* p2, UWORD size){
		    if(!p1 || !p2 || size == 0) return nullptr;
		    if(p1 == p2) return (void*)p1;
		    #if defined(_WIN32)
		    typedef int(*M_CMP)(const void*,const void*,UWORD);
		    static M_CMP pMemcmp = (M_CMP)get_system_function("memcmp", "msvcrt.dll");
		    #else
		    typedef int(*M_CMP)(const void*,const void*,UWORD);
		    static M_CMP pMemcmp = (M_CMP)get_system_function("memcmp");
		    #endif
		    if(!pMemcmp) return nullptr;
		    int ret = pMemcmp(p1, p2, size);
		    return (void*)((ret == 0) ? ((void*)((UWORD)p1 ^ (UWORD)p2)) : ((ret < 0) ? p1 : p2));
		}
		FUNCDECL(void* cmplloc, const void* p1, const void* p2, UWORD size){
			if(!p1 || !p2 || size == 0) return nullptr;
		    if(p1 == p2) return (void*)p1;
		    void* ret = memory::cnrlloc(p1, p2, size);
		    return ret ? ((ret == p1 || ret == p2) ? nullptr : ret) : nullptr;
		}
		FUNCDECL(void* mlloc, void* dest, const void* src, UWORD size){
            if(!dest || !src || size == 0 || dest == src) return dest;
            #if defined(_WIN32)
            typedef void*(*M_MOV)(void*,const void*,UWORD);
            static M_MOV pMemmove = (M_MOV)get_system_function("memmove", "msvcrt.dll");
            #else
            typedef void*(*M_MOV)(void*,const void*,UWORD);
            static M_MOV pMemmove = (M_MOV)get_system_function("memmove");
            #endif
            if(!pMemmove) return nullptr;
            pMemmove(dest, src, size);
            return dest;
        }
		FUNCDECL(UWORD szlen, void* ptr){
			if(!ptr) return 0;
			if(memory::on_stack(ptr)) return 0;
			#if defined(_WIN32)
			typedef UWORD(WINCALL *H_SZE)(void*,unsigned long,const void*);
			static H_SZE pHeapSize = (H_SZE)get_system_function("HeapSize");
			typedef void*(WINCALL *GPH)();
			static GPH pGetProcessHeap = (GPH)get_system_function("GetProcessHeap");
			if(!pHeapSize || !pGetProcessHeap) return 0;
			void* hHeap = pGetProcessHeap();
			if(!hHeap) return 0;
			UWORD res = pHeapSize(hHeap, 0, ptr);
			return (res == (UWORD)-1) ? 0 : res;
			#else // POSIX (Linux)
			typedef UWORD(*M_SZE)(void*);
			static M_SZE pMallocUsableSize = (M_SZE)get_system_function("malloc_usable_size");
			if(!pMallocUsableSize) return 0;
			return pMallocUsableSize(ptr);
			#endif
		}
		FUNCDECL(bool on_stack, void* ptr){
			if(!ptr) return false;
			char *stack_low, *stack_high;
			#if defined(_WIN32)
			#if WORD_SIZE == 8
			__asm_volatile__(
			    "movq %%gs:0x08, %0\n\t"
			    "movq %%gs:0x10, %1\n\t"
			    : "=r" (stack_high), "=r" (stack_low) : : "memory"
			);
			#else
			__asm_volatile__(
		        "movl %%fs:0x04, %0\n\t"
		        "movl %%fs:0x08, %1\n\t"
		        : "=r" (stack_high), "=r" (stack_low) : : "memory"
		    );
			#endif
			#else // Linux
			long long attr_buffer[12] = {0}; 
			typedef int(*THGANP)(void*,void*);
			static THGANP p_pthread_getattr_np = (THGANP)get_system_function("pthread_getattr_np");
			if(!p_pthread_getattr_np) p_pthread_getattr_np = (THGANP)get_system_function("pthread_getattr_np", "libpthread.so.0");
			typedef UWORD(*THSLF)();
			static THSLF p_pthread_self = (THSLF)get_system_function("pthread_self");
			if(!p_pthread_self) p_pthread_self = (THSLF)get_system_function("pthread_self", "libpthread.so.0");
			typedef int(*THAGSTK)(const void*,void**,UWORD*);
			static THAGSTK p_pthread_attr_getstack = (THAGSTK)get_system_function("pthread_attr_getstack");
			if(!p_pthread_attr_getstack) p_pthread_attr_getstack = (THAGSTK)get_system_function("pthread_attr_getstack", "libpthread.so.0");
			typedef int(*THAD)(void*);
			static THAD p_pthread_attr_destroy = (THAD)get_system_function("pthread_attr_destroy");
			if(!p_pthread_attr_destroy) p_pthread_attr_destroy = (THAD)get_system_function("pthread_attr_destroy", "libpthread.so.0");
			if(!p_pthread_getattr_np || !p_pthread_self || !p_pthread_attr_getstack || !p_pthread_attr_destroy) return false;
			void* stack_addr = nullptr; UWORD stack_size = 0;
			p_pthread_getattr_np((void*)p_pthread_self(), (void*)attr_buffer);
			p_pthread_attr_getstack((void*)attr_buffer, &stack_addr, &stack_size);
			p_pthread_attr_destroy((void*)attr_buffer);
			stack_low = (char*)stack_addr; stack_high = stack_low + stack_size;
			#endif
			return ((char*)ptr >= stack_low && (char*)ptr < stack_high);
		}
	}
    FUNCDECL(UWORD mbstowcs, wchar** dst, const char* src, UWORD size, bool stacked){
	    if(!src) return ~(UWORD)0;
	    if(!size){
			if(dst){
				if(*dst){
					if(stacked) delete[] *dst;
					else        memory::free(*dst);
				}
				*dst = nullptr;
			}
			return 0;
		}
	    #if defined(___mb__switch___case___1_2__3__)
		#undef ___mb__switch___case___1_2__3__
		#endif
		#if defined(___mb__switch___case___1_2__4__)
		#undef ___mb__switch___case___1_2__4__
		#endif
	    #if defined(_WIN32)
		#define ___mb__switch___case___1_2__4__(dst, dst_limit, buffer_size, code_point, __read_only) \
		wchar high_surrogate = (wchar)(code_point & 0xFFFF);\
	    wchar low_surrogate  = L'\0';\
		if(code_point > 0xFFFF){\
	        code_point -= 0x10000;\
	        high_surrogate = (wchar)((code_point >> 10) + 0xD800);\
	        low_surrogate  = (wchar)((code_point & 0x3FF) + 0xDC00);\
	    }\
		if(__read_only){\
			buffer_size += (low_surrogate == L'\0') ? 1 : 2;\
			if(buffer_size >= (dst_limit) - 1) buffer_size = (dst_limit) - 1;\
		}\
		else{\
			(*dst)[buffer_size++] = high_surrogate;\
	        if(low_surrogate != L'\0' && buffer_size < (dst_limit) - 1) (*dst)[buffer_size++] = low_surrogate;\
		}
	    #else
		// We dont need to handle surrogate pairs on Linux, as wchar_t is typically 32 bits and can represent all Unicode code points directly.
		// The existence of the dst_limit parameter is just for safety autocomplete text.
		#define ___mb__switch___case___1_2__4__(dst, dst_limit, buffer_size, code_point, __read_only) if(__read_only) ++buffer_size; else (*dst)[buffer_size++] = (wchar)code_point
	    #endif
		#define ___mb__switch___case___1_2__3__(dst, dst_limit, buffer_size, code_point, i, ptr, src, size, bytes_read, __read_only) \
		buffer_size = 0; i = 0; ptr = (const BYTE*)src;\
	    while(i < size && buffer_size < (dst_limit)){\
	    	bytes_read = 1; code_point = 0xFFFD;\
	    	if(!*ptr) code_point = 0;\
		    else if((*ptr & 0x80) == 0x00) code_point = ptr[0];\
		    else if((*ptr & 0xE0) == 0xC0){\
		        bytes_read = 2;\
		    	if(i + 1 < size) code_point = ((ptr[0] & 0x1F) << 6) | (ptr[1] & 0x3F);\
		    }\
		    else if((*ptr & 0xF0) == 0xE0){\
		        bytes_read = 3;\
		        if(i + 2 < size) code_point = ((ptr[0] & 0x0F) << 12) | ((ptr[1] & 0x3F) << 6) | (ptr[2] & 0x3F);\
		    }\
		    else if((*ptr & 0xF8) == 0xF0){\
		        bytes_read = 4;\
		        if(i + 3 < size) code_point = ((ptr[0] & 0x07) << 18) | ((ptr[1] & 0x3F) << 12) | ((ptr[2] & 0x3F) << 6) | (ptr[3] & 0x3F);\
		    }\
	        if((i + bytes_read - 1) >= size) i = size;\
			else                             i += bytes_read;\
			ptr += bytes_read;\
	        ___mb__switch___case___1_2__4__(dst, dst_limit, buffer_size, code_point, __read_only);\
		}
		UWORD buffer_size, bytes_read, i; unsigned int code_point; const BYTE* ptr;
		___mb__switch___case___1_2__3__(dst, ~(UWORD)0, buffer_size, code_point, i, ptr, src, size, bytes_read, 1);
		if(!dst) return buffer_size;
		if(*dst && stacked) delete[] *dst;
		if(stacked) *dst = new wchar[buffer_size + 1];
		else        *dst = (wchar*)memory::realloc(*dst, sizeof(wchar) * (buffer_size + 1));
		___mb__switch___case___1_2__3__(dst, ~(UWORD)0, buffer_size, code_point, i, ptr, src, size, bytes_read, 0);
		(*dst)[buffer_size] = L'\0';
	    return buffer_size;
	}
	FUNCDECL(UWORD mbstowcs, wchar* dst, const char* src, UWORD dst_size, UWORD src_size){
		if(!src || !dst || !dst_size) return ~(UWORD)0;
		if(!src_size){
			if(dst) *dst = L'\0';
			return 0;
		}
		UWORD buffer_size, bytes_read, i; unsigned int code_point; const BYTE* ptr;
		// The '&dst' is used to avoid parsing errors in the macro.
		// The compiler just replace any sentences '*&dst' resulting in 'dst' and it will work as expected.
		___mb__switch___case___1_2__3__(&dst, dst_size, buffer_size, code_point, i, ptr, src, src_size, bytes_read, 0);
		dst[buffer_size] = L'\0';
		return buffer_size;
		#undef ___mb__switch___case___1_2__3__
		#undef ___mb__switch___case___1_2__4__
	}
	FUNCDECL(UWORD wcstombs, char** dst, const wchar* src, UWORD size, bool stacked){
	    if(!src) return ~(UWORD)0;
	    if(!size){
	        if(dst){
	            if(*dst){
					if(stacked) delete[] *dst;
					else        memory::free(*dst);
				}
	            *dst = nullptr;
	        }
	        return 0;
	    }
	    #if defined(_WIN32)
		#define ___mb__switch___case___1_2__4__(src, i, size, code_point) \
		wchar c1 = src[i];\
	    if(c1 >= 0xD800 && c1 <= 0xDBFF && (i + 1) < size){\
	        wchar c2 = src[i + 1];\
	        if(c2 >= 0xDC00 && c2 <= 0xDFFF){\
	            code_point = 0x10000 + (((c1 & 0x3FF) << 10) | (c2 & 0x3FF));\
	            i += 2;\
	        }\
			else{ code_point = c1; ++i; }\
	    }\
		else{ code_point = c1; ++i; }
		#else
		#define ___mb__switch___case___1_2__4__(src, i, size, code_point) code_point = (unsigned int)src[i++]
		#endif
		#define ___mb__switch___case___1_2__3__(dst, dst_limit, buffer_size, code_point, i, src, size, __read_only)\
	    buffer_size = 0; i = 0;\
	    while(i < size && buffer_size < (dst_limit)){\
	        ___mb__switch___case___1_2__4__(src, i, size, code_point);\
			if(__read_only){\
				if(code_point <= 0x7F)          buffer_size += 1;\
				else if(code_point <= 0x7FF)    buffer_size += 2;\
				else if(code_point <= 0xFFFF)   buffer_size += 3;\
				else if(code_point <= 0x10FFFF) buffer_size += 4;\
				else                            buffer_size += 3;\
				if(buffer_size >= (dst_limit)) buffer_size = (dst_limit);\
			}\
			else{\
				if(code_point <= 0x7F) (*dst)[buffer_size++] = (char)code_point;\
				else if(code_point <= 0x7FF){\
					(*dst)[buffer_size++] = (char)(0xC0 | ((code_point >> 6) & 0x1F));\
					if(buffer_size >= (dst_limit)) break;\
					(*dst)[buffer_size++] = (char)(0x80 | (code_point & 0x3F));\
				}\
				else if(code_point <= 0xFFFF){\
					(*dst)[buffer_size++] = (char)(0xE0 | ((code_point >> 12) & 0x0F));\
					if(buffer_size >= (dst_limit)) break;\
					(*dst)[buffer_size++] = (char)(0x80 | ((code_point >> 6) & 0x3F));\
					if(buffer_size >= (dst_limit)) break;\
					(*dst)[buffer_size++] = (char)(0x80 | (code_point & 0x3F));\
				}\
				else if(code_point <= 0x10FFFF){\
					(*dst)[buffer_size++] = (char)(0xF0 | ((code_point >> 18) & 0x07));\
					if(buffer_size >= (dst_limit)) break;\
					(*dst)[buffer_size++] = (char)(0x80 | ((code_point >> 12) & 0x3F));\
					if(buffer_size >= (dst_limit)) break;\
					(*dst)[buffer_size++] = (char)(0x80 | ((code_point >> 6) & 0x3F));\
					if(buffer_size >= (dst_limit)) break;\
					(*dst)[buffer_size++] = (char)(0x80 | (code_point & 0x3F));\
				}\
				else{\
					(*dst)[buffer_size++] = (char)0xEF;\
					if(buffer_size >= (dst_limit)) break;\
					(*dst)[buffer_size++] = (char)0xBF;\
					if(buffer_size >= (dst_limit)) break;\
					(*dst)[buffer_size++] = (char)0xBD;\
				}\
			}\
	    }
		UWORD buffer_size = 0, i = 0; unsigned int code_point;
		___mb__switch___case___1_2__3__(dst, ~(UWORD)0, buffer_size, code_point, i, src, size, 1);
	    if(!dst) return buffer_size;
	    if(*dst && stacked) delete[] *dst;
	    if(stacked) *dst = new char[buffer_size + 1];
	    else        *dst = (char*)memory::realloc(*dst, buffer_size + 1);
		___mb__switch___case___1_2__3__(dst, ~(UWORD)0, buffer_size, code_point, i, src, size, 0);
	    (*dst)[buffer_size] = '\0';
	    return buffer_size;
	}
	FUNCDECL(UWORD wcstombs, char* dst, const wchar* src, UWORD dst_size, UWORD src_size){
		if(!src || !dst || !dst_size) return ~(UWORD)0;
		if(!src_size){
			if(dst) *dst = '\0';
			return 0;
		}
		UWORD buffer_size, i; unsigned int code_point;
		___mb__switch___case___1_2__3__(&dst, dst_size, buffer_size, code_point, i, src, src_size, 0);
		dst[buffer_size] = '\0';
		return buffer_size;
		#undef ___mb__switch___case___1_2__3__
		#undef ___mb__switch___case___1_2__4__
	}
	FUNCDECL(UWORD language){
		struct _ap_{
			static FORCEINLINE UWORD language_set_ret(const char* lang, const char* region){
				BYTE l0 = (BYTE)((lang   && lang[0]               ) ? (lang[0])   : '?');
			    BYTE l1 = (BYTE)((lang   && lang[0]   &&   lang[1]) ? (lang[1])   : '?');
			    BYTE r0 = (BYTE)((region && region[0]             ) ? (region[0]) : '?');
			    BYTE r1 = (BYTE)((region && region[0] && region[1]) ? (region[1]) : '?');
				return ((UWORD)l0) | ((UWORD)l1 << 8) | ((UWORD)r0 << 16) | ((UWORD)r1 << 24);
			}
		};
		#if defined(_WIN32)
	    typedef unsigned short(WINCALL *GUL)(void);
	    static GUL pGetUserDefaultLangID = (GUL)get_system_function("GetUserDefaultLangID");
	    typedef int(WINCALL *GLI)(unsigned int,unsigned int,char*,int);
	    static GLI pGetLocaleInfoA = (GLI)get_system_function("GetLocaleInfoA");
	    if(!pGetUserDefaultLangID || !pGetLocaleInfoA) return _ap_::language_set_ret("ER", "R!");
	    unsigned short id = pGetUserDefaultLangID();
	    char l_buf[3] = "??", r_buf[3] = "??";
	    pGetLocaleInfoA(id, 0x59, l_buf, 2);
	    pGetLocaleInfoA(id, 0x5A, r_buf, 2);
	    return _ap_::language_set_ret(l_buf, r_buf);
	    #else // Linux
	    const char* lang = (const char*)get_system_variable("LANG");
    	if(!lang || !*lang) lang = (const char*)get_system_variable("LC_ALL");
    	if(!lang || !*lang) return _ap_::language_set_ret("ER", "R!");
    	const char* region_ptr = nullptr;
        for(const char* p = lang; *p && *p != '.'; ++p) if(*p == '_' || *p == '-'){
            region_ptr = p + 1;
            break;
        }
        return _ap_::language_set_ret(lang, region_ptr);
	    #endif
	}
	FUNCDECL(int message, const char* RESTRICT txt, const char* RESTRICT ttl, int type){
		if(!txt || !ttl) return -2;
		int def_button = type & 0xF00;
		int icon_type  = type & 0x0F0;
		int buttons    = type & 0x00F;
		#if defined(_WIN32)
		typedef int(WINCALL *MBT)(void*,const char* RESTRICT,const char* RESTRICT,unsigned int);
        static MBT pMessageBoxA = (MBT)get_system_function("MessageBoxA", "user32.dll");
        if(!pMessageBoxA) return -1;
        if(def_button == 0x200
		&& buttons != 0x002
		&& buttons != 0x003)
			def_button = 0x100;
		if(def_button == 0x100
		&& buttons != 0x001
		&& buttons != 0x004
		&& buttons != 0x005)
			def_button = 0x000;
        type = def_button | icon_type | buttons;
        return pMessageBoxA(nullptr, txt, ttl, (unsigned int)type | 0x00250000);
		#else // Linux
		// We will actually use the console command 'zenity',
		// because it is an exact equivalent of 'MessageBoxW' in Linux.
		// We will modify the appearance and visual settings of the popup window,
		// so that it has functional fidelity with that of Windows.
		#if defined(___mb__switch___case___1_2__3__)
		#undef ___mb__switch___case___1_2__3__
		#endif
		#if defined(___mb__switch___case___1_2__4__)
		#undef ___mb__switch___case___1_2__4__
		#endif
		#if defined(___mb__switch___case___1_2__5__)
		#undef ___mb__switch___case___1_2__5__
		#endif
		char* _str_txt = nullptr; char* _str_ttl = nullptr;
		UWORD _len_txt = 0, _qua_txt = 0, _len_ttl = 0, _qua_ttl = 0;
		while(txt[_len_txt]){ if(txt[_len_txt] == '"' || txt[_len_txt] == '\\') ++_qua_txt; ++_len_txt; }
		while(ttl[_len_ttl]){ if(ttl[_len_ttl] == '"' || ttl[_len_ttl] == '\\') ++_qua_ttl; ++_len_ttl; }
		#define ___mb__switch___case___1_2__3__(variable) if(_qua_##variable){\
			UWORD i = _len_##variable + _qua_##variable, j = _len_##variable, k = 0;\
			_str_##variable = (char*)memory::alloc(i + 1);\
			memory::clloc(_str_##variable, variable, j + 1);\
			while(k <= _len_##variable && _qua_##variable){\
				_str_##variable[i] = _str_##variable[j]; --i;\
				if(_str_##variable[j] == '"' || _str_##variable[j] == '\\'){\
					_str_##variable[i] = '\\';\
					--i; --_qua_##variable;\
				}\
				++k; --j;\
			}\
			_len_##variable += _qua_##variable;\
		}
		___mb__switch___case___1_2__3__(txt);
		___mb__switch___case___1_2__3__(ttl);
		#undef ___mb__switch___case___1_2__3__
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
		UWORD cmd_size = _len_txt + _len_ttl + 300;
		#define ___mb__switch___case___1_2__3__(str) ({UWORD _sze=0;while(str[_sze])++_sze;_sze;})
		if(ok_label)     cmd_size += ___mb__switch___case___1_2__3__(ok_label)     + 20;
		if(cancel_label) cmd_size += ___mb__switch___case___1_2__3__(cancel_label) + 20;
		if(extra_label)  cmd_size += ___mb__switch___case___1_2__3__(extra_label)  + 20;
		#undef ___mb__switch___case___1_2__3__
		if(mb_default == 2 || mb_default == 3) cmd_size += 20;
		char* cmd = (char*)memory::alloc(cmd_size + 1);
		int wr_offset = sprintf(cmd, "zenity %s --title=\"%s\" --text=\"%s\"",
			zenity_type,
			_str_ttl ? _str_ttl : ttl,
			_str_txt ? _str_txt : txt
		);
		if(_str_txt) memory::free(_str_txt);
		if(_str_ttl) memory::free(_str_ttl);
		if(ok_label) wr_offset += sprintf(cmd + wr_offset, " --ok-label=\"%s\"", ok_label);
		if(cancel_label) wr_offset += sprintf(cmd + wr_offset, " --cancel-label=\"%s\"", cancel_label);
		if(extra_label) wr_offset += sprintf(cmd + wr_offset, " --extra-label=\"%s\"", extra_label);
		__message_box__retry_with_2nd_button:
		if(cancel_label && mb_default == 2) wr_offset += sprintf(cmd + wr_offset, " --default-cancel");
		if(extra_label && mb_default == 3) wr_offset += sprintf(cmd + wr_offset, " --default-extra");
		else if(mb_default == 3){ mb_default = 2; goto __message_box__retry_with_2nd_button; }
		sprintf(cmd + wr_offset, " --no-wrap --ellipsize 2>/dev/null");
		int ret = system(cmd) >> 8;
		memory::free(cmd);
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
		return ret;
	    #endif
    }
	FUNCDECL(int message, const wchar* RESTRICT txt, const wchar* RESTRICT ttl, int type = message_box::ok){
		if(!txt || !ttl) return -2;
		#if defined(_WIN32)
		typedef int(WINCALL *MBT)(void*,const wchar* RESTRICT,const wchar* RESTRICT,unsigned int);
        static MBT pMessageBoxW = (MBT)get_system_function("MessageBoxW", "user32.dll");
        if(!pMessageBoxW) return -1;
        return pMessageBoxW(nullptr, txt, ttl, (unsigned int)type | 0x00250000);
		#else // Linux
		// We delegate the functionality to the char* version of the function, since we need to convert the wide strings to multibyte strings for the zenity command.
		#define ___mb__switch___case___1_2__3__(variable, delproc) \
		UWORD _len_##variable = 0; while(variable[_len_##variable]) ++_len_##variable;\
		if(_len_##variable == 0) delproc;\
		char* _str_##variable = (char*)memory::alloc(_len_##variable + 1);\
		wcstombs(_str_##variable, variable, _len_##variable, _len_##variable);\
		if(_len_##variable){\
			UWORD quant_of_marks = 0, i = 0, j = _len_##variable, k = 0;\
			while(i < _len_##variable){\
				if(_str_##variable[i] == '"' || _str_##variable[i] == '\\') ++quant_of_marks;\
				++i;\
			}\
			if(quant_of_marks){\
				_str_##variable = (char*)memory::realloc(_str_##variable, _len_##variable + quant_of_marks + 1);\
				i = _len_##variable + quant_of_marks;\
				while(k <= _len_##variable && quant_of_marks){\
					_str_##variable[i] = _str_##variable[j]; --i;\
					if(_str_##variable[j] == '"' || _str_##variable[j] == '\\'){\
						_str_##variable[i] = '\\';\
						--i; --quant_of_marks;\
					}\
					++k; --j;\
				}\
			}\
		}
		int _ret;
		___mb__switch___case___1_2__3__(txt, {
			___mb__switch___case___1_2__3__(ttl, return message("", "", type));
			_ret = message("", _str_ttl, type);
			memory::free(_str_ttl);
			return _ret;
		});
		___mb__switch___case___1_2__3__(ttl, {
			_ret = message(_str_txt, "", type);
			memory::free(_str_txt);
			return _ret;
		});
		#undef ___mb__switch___case___1_2__3__
		_ret = message(_str_txt, _str_ttl, type);
		memory::free(_str_txt); memory::free(_str_ttl);
		return _ret;
	    #endif
    }
	FUNCDECL(ALWAYSINLINE int message, const void* RESTRICT txt, const char* RESTRICT ttl, int type = message_box::ok){
		return message((const char*)txt, ttl, type);
	}
	FUNCDECL(ALWAYSINLINE int message, decltype(nullptr), const char* RESTRICT ttl, int type = message_box::ok){
		return message("", ttl, type);
	}
	FUNCDECL(ALWAYSINLINE int message, const char* RESTRICT txt, const void* RESTRICT ttl, int type = message_box::ok){
		return message(txt, (const char*)ttl, type);
	}
	FUNCDECL(ALWAYSINLINE int message, const char* RESTRICT txt, decltype(nullptr), int type = message_box::ok){
		return message(txt, "", type);
	}
	FUNCDECL(ALWAYSINLINE int message, const void* RESTRICT txt, const wchar* RESTRICT ttl, int type = message_box::ok){
		return message((const wchar*)txt, ttl, type);
	}
	FUNCDECL(ALWAYSINLINE int message, decltype(nullptr), const wchar* RESTRICT ttl, int type = message_box::ok){
		return message(L"", ttl, type);
	}
	FUNCDECL(ALWAYSINLINE int message, const wchar* RESTRICT txt, const void* RESTRICT ttl, int type = message_box::ok){
		return message(txt, (const wchar*)ttl, type);
	}
	FUNCDECL(ALWAYSINLINE int message, const wchar* RESTRICT txt, decltype(nullptr), int type = message_box::ok){
		return message(txt, L"", type);
	}
	FUNCDECL(ALWAYSINLINE int message, const void* RESTRICT txt, const void* RESTRICT ttl, int type = message_box::ok){
		return message((const char*)txt, (const char*)ttl, type);
	}
	FUNCDECL(ALWAYSINLINE int message, decltype(nullptr), decltype(nullptr), int type = message_box::ok){
		return message("", "", type);
	}
	namespace thread{
    	FUNCDECL(void* create, void*(*func)(void*), void* arg){
		    if(!func) return nullptr;
		    #if defined(_WIN32)
    		#define THR_HANDLE_DECL struct THR_HANDLE{\
    			UWORD magic;\
		        void* native_handle;\
		        unsigned long id;\
		        THR_HANDLE() : magic(0x54485244), native_handle(nullptr), id(0) {}\
		        ~THR_HANDLE(){ magic = 0x00; native_handle = nullptr; id = 0; }\
		    }
		    THR_HANDLE_DECL;
		    struct THR_ENTRY{
		    	void*(*fn)(void*);
		    	void* fn_arg;
		    	static unsigned long WINCALL entry(void* p){
		    		THR_ENTRY* e = (THR_ENTRY*)p;
		    		void*(*fn)(void*) = e->fn; void* fn_arg = e->fn_arg;
		    		delete e;
		    		if(fn) fn(fn_arg);
		    		return 0;
		    	}
		    };
		    typedef void*(WINCALL *CTH)(void*,UWORD,unsigned long(WINCALL *)(void*),void*,unsigned int,unsigned long*);
            static CTH pCreateThread = (CTH)get_system_function("CreateThread");
            if(!pCreateThread) return nullptr;
            THR_HANDLE* h = new THR_HANDLE;
            THR_ENTRY* e = new THR_ENTRY; e->fn = func; e->fn_arg = arg;
            h->native_handle = pCreateThread(nullptr, 0, THR_ENTRY::entry, e, 0, &(h->id));
            if(h->native_handle) return (void*)h;
            delete e;
            #else // Linux
            #define THR_HANDLE_DECL struct THR_HANDLE{\
    			UWORD magic;\
		        UWORD id;\
		        THR_HANDLE() : magic(0x54485244), id(0) {}\
		        ~THR_HANDLE(){ magic = 0x00; id = 0; }\
		    }
            THR_HANDLE_DECL;
            typedef int(*PTH)(UWORD*,void*,void*(*)(void*),void*);
            static PTH pPthreadCreate = (PTH)get_system_function("pthread_create");
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
    	template<typename R> FUNCDECL(ALWAYSINLINE void* create, R(*func)()){ return thread::create((void*(*)(void*))func, nullptr); }
    	template<typename R, typename... Args> FUNCDECL(ALWAYSINLINE void* create, R(*func)(Args...)){ return thread::create((void*(*)(void*))func, nullptr); }
    	template<typename R> FUNCDECL(ALWAYSINLINE void* create, R(*func)(), void* arg){ return thread::create((void*(*)(void*))func, arg); }
    	template<typename R, typename... Args> FUNCDECL(ALWAYSINLINE void* create, R(*func)(Args...), void* arg){ return thread::create((void*(*)(void*))func, arg); }
    	FUNCDECL(void* join, void* thr){
    		#define THR_HANDLE_VALID(thr) ((thr)?(*(UWORD*)(thr)!=0x54485244):true)
    		if(THR_HANDLE_VALID(thr)) return nullptr;
    		THR_HANDLE_DECL;
            THR_HANDLE* h = (THR_HANDLE*)thr;
    		#if defined(_WIN32)
		    typedef unsigned int(WINCALL *WSO)(void*, unsigned int);
		    static WSO pWait = (WSO)get_system_function("WaitForSingleObject");
		    if(!pWait || !(h->native_handle)) return nullptr;
			pWait(h->native_handle, 0xFFFFFFFF);
		    #else // Linux
		    typedef int(*PTH)(unsigned long,void**);
		    static PTH pJoin = (PTH)get_system_function("pthread_join");
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
    	FUNCDECL(void* kill, void* thr){
    		if(THR_HANDLE_VALID(thr)) return nullptr;
    		THR_HANDLE_DECL;
            THR_HANDLE* h = (THR_HANDLE*)thr;
    		#if defined(_WIN32)
            typedef int(WINCALL *TTH)(void*,unsigned int);
            static TTH pTerminateThread = (TTH)get_system_function("TerminateThread");
            if(!pTerminateThread || !(h->native_handle)) return nullptr;
            pTerminateThread(h->native_handle, 0);
            #else // Linux
            typedef int(*PTH)(UWORD);
            static PTH pPthreadCancel = (PTH)get_system_function("pthread_cancel");
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
    	FUNCDECL(bool yield, void* thr, bool _wait){
    		if(THR_HANDLE_VALID(thr)) return false;
    		THR_HANDLE_DECL;
    		#if defined(_WIN32)
	        typedef int(WINCALL *GECT)(void*, unsigned long*);
	        static GECT pGetExitCodeThread = (GECT)get_system_function("GetExitCodeThread");
	        THR_HANDLE* handle = (THR_HANDLE*)thr;
	        if(!(handle->native_handle) || !pGetExitCodeThread) return false;
	        unsigned long exit_code = 0;
	        if(pGetExitCodeThread(handle->native_handle, &exit_code)){
	            if(exit_code == 259){
	                if(_wait) sleep(10);
	                return true;
	            }else{
	                delete handle;
	                if(_wait) sleep(10);
	            }
	        }
	        return false;
	        #else // Linux
	        typedef int(*PKILL)(unsigned long, int);
		    static PKILL pPthreadKill = (PKILL)get_system_function("pthread_kill");
		    THR_HANDLE* handle = (THR_HANDLE*)thr;
		    if(!(handle->id) || !pPthreadKill) return false;
		    if(pPthreadKill((unsigned long)(handle->id), 0)){
		    	delete handle;
		    	if(_wait) sleep(10);
			}else{
		    	if(_wait) sleep(10);
		    	return true;
		    }
		    return false;
	        #endif
    	}
    }
    #if defined(ST_IMPORT_JSON)
	#undef ST_IMPORT_JSON
    #define ST_IMPORT_JSON 1
	namespace json{
    	FUNCDECL(void* init, BYTE type){
    		#define JSN_DEFAULT_MEMSIZE(type) ((((((type)&0x0F)==(json_type::num))&&(!(((type)&0xF0)&(~(json_type::uns))))))\
				?WORD_SIZE:((((type)==(json_type::ary))||((type)==(json_type::obj)))?2:(((type)==(json_type::flt))?4\
				:(((type)==(json_type::str))?1:0))))
			(void)((void*)0); // IntelliSense error parsing fix.
			#define JSN_HANDLE_DECL struct JSN_HANDLE{\
				UWORD magic;\
				BYTE** base;\
				BYTE* data;\
				JSN_HANDLE* array_handle;\
				BYTE* index_ptr;\
				JSN_HANDLE() : magic(0x4E4F534A), base(nullptr), data(nullptr), array_handle(nullptr), index_ptr(nullptr) {}\
				~JSN_HANDLE(){ magic = 0; base = nullptr; data = index_ptr = nullptr; array_handle = nullptr; }\
				BYTE* expand(UWORD idx_cut, UWORD idx_dst){\
					if(base ? (!*base) : true) return nullptr;\
					UWORD old_size = memory::szlen(*base);\
					if(idx_cut < idx_dst){\
						BYTE* old_base = *base; *base = (BYTE*)memory::realloc(*base, old_size + idx_dst - idx_cut);\
						WORD ptr_pos = (*base) - old_base;\
						if(ptr_pos){\
							if(data) data += ptr_pos;\
							if(index_ptr) index_ptr += ptr_pos;\
							JSN_HANDLE* array_ptr = array_handle;\
							while(array_ptr){\
								if(array_ptr->data) array_ptr->data += ptr_pos;\
								if(array_ptr->index_ptr) array_ptr->index_ptr += ptr_pos;\
								array_ptr = array_ptr->array_handle;\
							}\
						}\
						if(old_size - idx_cut) memory::mlloc((*base) + idx_dst, (*base) + idx_cut, old_size - idx_cut);\
					}\
					if(idx_cut > idx_dst){\
						if(old_size - idx_cut) memory::mlloc((*base) + idx_dst, (*base) + idx_cut, old_size - idx_cut);\
						BYTE* old_base = *base; *base = (BYTE*)memory::realloc(*base, old_size + idx_dst - idx_cut);\
						WORD ptr_pos = (*base) - old_base;\
						if(ptr_pos){\
							if(data) data += ptr_pos;\
							if(index_ptr) index_ptr += ptr_pos;\
							JSN_HANDLE* array_ptr = array_handle;\
							while(array_ptr){\
								if(array_ptr->data) array_ptr->data += ptr_pos;\
								if(array_ptr->index_ptr) array_ptr->index_ptr += ptr_pos;\
								array_ptr = array_ptr->array_handle;\
							}\
						}\
					}\
					return (*base) + idx_dst;\
				}\
				BYTE* new_elem(BYTE type, unsigned int offset){\
					if(!base || !*base) return nullptr;\
					UWORD old_size = (UWORD)memory::szlen(*base), memsize = JSN_DEFAULT_MEMSIZE(type) + offset;\
					BYTE* old_base = *base; *base = (BYTE*)memory::realloc(*base, old_size + 1 + memsize);\
					WORD ptr_pos = (*base) - old_base;\
					if(ptr_pos){\
						if(data) data += ptr_pos;\
						if(index_ptr) index_ptr += ptr_pos;\
						JSN_HANDLE* array_ptr = array_handle;\
						while(array_ptr){\
							if(array_ptr->data) array_ptr->data += ptr_pos;\
							if(array_ptr->index_ptr) array_ptr->index_ptr += ptr_pos;\
							array_ptr = array_ptr->array_handle;\
						}\
					}\
					*((*base) + old_size) = type;\
					if(memsize > 0) memory::slloc((*base) + old_size + 1, 0, memsize);\
					return (*base) + old_size;\
				}\
				static BYTE* nav(BYTE* start, unsigned short offset){\
					while(offset){\
						--offset;\
						char type = *start;\
						if((type & 0x0F) == json_type::bll) ++start;\
						if((type & 0x0F) == json_type::num) start += 1 + WORD_SIZE;\
						if(type == json_type::flt) start += 5;\
						if(type == json_type::str){\
							++start;\
							while(*start) ++start;\
							++start;\
						}\
						if(type == json_type::ary){\
							++start;\
							unsigned short size = *(unsigned short*)start;\
							start += 2;\
							while(size){\
								--size;\
								start += 2;\
							}\
						}\
						if(type == json_type::obj){\
							++start;\
							unsigned short size = *(unsigned short*)start;\
							start += 2;\
							while(size){\
								--size;\
								start += 2;\
								while(*start) ++start;\
								++start;\
							}\
						}\
					}\
					return start;\
				}\
			}
			#define JSN_HANDLE_VALID(jsn) ((jsn)?(((*((UWORD*)(jsn)))!=0x4E4F534A)&&((*((UWORD*)(jsn)))!=0x534E534A)):true)
			#define JSN_HANDLE_STACKED(jsn) ((*((UWORD*)(jsn)))==0x534E534A)
    		if(type == json_type::unk) return nullptr;
    		BYTE* _dat = nullptr;
    		if(type != json_type::nll){
    			BYTE memsize = JSN_DEFAULT_MEMSIZE(type);
				_dat = (BYTE*)memory::alloc(1 + memsize);
				*_dat = type;
				memory::slloc(_dat + 1, 0, memsize);
			}
    		JSN_HANDLE_DECL;
    		JSN_HANDLE* _ret = new JSN_HANDLE;
    		_ret->base = new BYTE*;
    		*(_ret->base) = _ret->data = _dat;
    		_ret->array_handle = nullptr;
    		_ret->index_ptr = nullptr;
    		return (void*)_ret;
		}
		FUNCDECL(void* detach, void* jsn){
			if(JSN_HANDLE_VALID(jsn)) return nullptr;
			if(JSN_HANDLE_STACKED(jsn)) return jsn;
			JSN_HANDLE_DECL;
			JSN_HANDLE* handle = (JSN_HANDLE*)jsn;
			if((handle->base) ? ((*(handle->base)) == (handle->data)) : false){
				memory::free(*(handle->base));
				delete (handle->base);
			}
			delete handle;
			return jsn;
		}
		FUNCDECL(void* parse, void* src){
			if(!src) return nullptr;
			if(*(UWORD*)src == 0x4E4F534A) return src; // Its  JSON already!
			JSN_HANDLE_DECL;
			struct _ap_{
				static UWORD _count_mem_size(BYTE** data, BYTE* txt_end, BYTE* _type, UWORD* _enum_elems){
					UWORD _ret_data_size = 0;
					if(((txt_end - *data) >= 4)
						? (((*data)[0] == 'n' || (*data)[0] == 'N') && ((*data)[1] == 'u' || (*data)[1] == 'U')
						&& ((*data)[2] == 'l' || (*data)[2] == 'L') && ((*data)[3] == 'l' || (*data)[3] == 'L'))
						: false){
						*data += 4;
						*_type = json_type::nll;
						++*_enum_elems;
					}
					else if(((txt_end - *data) >= 4)
						? (((*data)[0] == 't' || (*data)[0] == 'T') && ((*data)[1] == 'r' || (*data)[1] == 'R')
						&& ((*data)[2] == 'u' || (*data)[2] == 'U') && ((*data)[3] == 'e' || (*data)[3] == 'E'))
						: false){
						*data += 4;
						*_type = json_type::bll | json_type::uns;
						_ret_data_size = 1;
						++*_enum_elems;
					}
					else if(((txt_end - *data) >= 5)
						? (((*data)[0] == 'f' || (*data)[0] == 'F') && ((*data)[1] == 'a' || (*data)[1] == 'A')
						&& ((*data)[2] == 'l' || (*data)[2] == 'L') && ((*data)[3] == 's' || (*data)[3] == 'S')
						&& ((*data)[4] == 'e' || (*data)[4] == 'E'))
						: false){
						*data += 5;
						*_type = json_type::bll;
						_ret_data_size = 1;
						++*_enum_elems;
					}
					else if((*data)[0] == '-' || (*data)[0] == '.' || ((*data)[0] >= '0' && (*data)[0] <= '9')){
						++*_enum_elems;
						*_type = json_type::num | (((*data)[0] == '-') ? 0 : json_type::uns);
						while(txt_end > *data){
							if(**data == '.' || **data == 'e' || **data == 'E') *_type = json_type::flt;
							else if(**data && ((**data < '0' || **data > '9') && **data != '-')) break;
							++*data;
						}
						_ret_data_size = 1 + (((*_type & 0x0F) == json_type::num) ? WORD_SIZE : 4);
					}
					else if((*data)[0] == '"'){
						++*_enum_elems;
						*_type = json_type::str;
						_ret_data_size = 2;
						++*data;
						while(txt_end > *data){
							if(**data == '"'){ ++*data; break; }
							if(**data == '\\'){
								++*data;
								if(txt_end <= *data) break;
							}
							++_ret_data_size; ++*data;
						}
					}
					else if((*data)[0] == '['){
						++*_enum_elems;
						_ret_data_size = 3;
						++*data;
						while(txt_end > *data){
							while((txt_end > *data) ? ((**data > 0 && **data <= 32) || **data == ',') : false) ++*data;
							if((txt_end > *data) ? (**data == ']') : true){
								if(txt_end > *data) ++*data;
								break;
							}
							_ret_data_size += 2 + _ap_::_count_mem_size(data, txt_end, _type, _enum_elems);
						}
						*_type = json_type::ary;
					}
					else if((*data)[0] == '\x7B'){
						++*_enum_elems;
						_ret_data_size = 3;
						++*data;
						while(txt_end > *data){
							while((txt_end > *data) ? ((**data > 0 && **data <= 32) || **data == ',') : false) ++*data;
							if((txt_end > *data) ? (**data == '\x7D' || **data != '"') : true){
								if(txt_end > *data) ++*data;
								break;
							}
							++*data;
							while(txt_end > *data){
								if(**data == '"'){ ++*data; break; }
								if(**data == '\\'){
									++*data;
									if(txt_end <= *data) break;
								}
								++_ret_data_size; ++*data;
							}
							while((txt_end > *data) ? ((**data > 0 && **data <= 32) || **data == ',') : false) ++*data;
							if((txt_end > *data) ? (**data == '\x7D' || **data != ':') : true){
								if(txt_end > *data) ++*data;
								break;
							}
							++_ret_data_size; ++*data;
							while((txt_end > *data) ? ((**data > 0 && **data <= 32) || **data == ',') : false) ++*data;
							if((txt_end > *data) ? (**data == '\x7D') : true){
								if(txt_end > *data) ++*data;
								break;
							}
							_ret_data_size += 2 + _ap_::_count_mem_size(data, txt_end, _type, _enum_elems);
						}
						*_type = json_type::obj;
					}
					return _ret_data_size;
				}
				static void _enum_type_list(BYTE** data, BYTE* txt_end, BYTE** _enum_list, BYTE* _enum_list_end){
					if(((txt_end - *data) >= 4)
						? (((*data)[0] == 'n' || (*data)[0] == 'N') && ((*data)[1] == 'u' || (*data)[1] == 'U')
						&& ((*data)[2] == 'l' || (*data)[2] == 'L') && ((*data)[3] == 'l' || (*data)[3] == 'L'))
						: false){
						*data += 4;
						if(_enum_list) if(*_enum_list < _enum_list_end){
							**_enum_list = json_type::nll;
							++*_enum_list;
						}
					}
					else if(((txt_end - *data) >= 4)
						? (((*data)[0] == 't' || (*data)[0] == 'T') && ((*data)[1] == 'r' || (*data)[1] == 'R')
						&& ((*data)[2] == 'u' || (*data)[2] == 'U') && ((*data)[3] == 'e' || (*data)[3] == 'E'))
						: false){
						*data += 4;
						if(_enum_list) if(*_enum_list < _enum_list_end){
							**_enum_list = json_type::bll | json_type::uns;
							++*_enum_list;
						}
					}
					else if(((txt_end - *data) >= 5)
						? (((*data)[0] == 'f' || (*data)[0] == 'F') && ((*data)[1] == 'a' || (*data)[1] == 'A')
						&& ((*data)[2] == 'l' || (*data)[2] == 'L') && ((*data)[3] == 's' || (*data)[3] == 'S')
						&& ((*data)[4] == 'e' || (*data)[4] == 'E'))
						: false){
						*data += 5;
						if(_enum_list) if(*_enum_list < _enum_list_end){
							**_enum_list = json_type::bll;
							++*_enum_list;
						}
					}
					else if((*data)[0] == '-' || (*data)[0] == '.' || ((*data)[0] >= '0' && (*data)[0] <= '9')){
						char _type = json_type::num | (((*data)[0] == '-') ? 0 : json_type::uns);
						while(txt_end > *data){
							if(**data == '.' || **data == 'e' || **data == 'E') _type = json_type::flt;
							else if(**data && ((**data < '0' || **data > '9') && **data != '-')) break;
							++*data;
						}
						if(_enum_list) if(*_enum_list < _enum_list_end){
							**_enum_list = _type;
							++*_enum_list;
						}
					}
					else if((*data)[0] == '"'){
						if(_enum_list) if(*_enum_list < _enum_list_end){
							**_enum_list = json_type::str;
							++*_enum_list;
						}
						++*data;
						while(txt_end > *data){
							if(**data == '"'){ ++*data; break; }
							if(**data == '\\'){
								++*data;
								if(txt_end <= *data) break;
							}
							++*data;
						}
					}
					else if((*data)[0] == '['){
						if(_enum_list) if(*_enum_list < _enum_list_end){
							**_enum_list = json_type::ary;
							++*_enum_list;
						}
						++*data;
						while(txt_end > *data){
							while((txt_end > *data) ? ((**data > 0 && **data <= 32) || **data == ',') : false) ++*data;
							if((txt_end > *data) ? (**data == ']') : true){
								if(txt_end > *data) ++*data;
								break;
							}
							_ap_::_enum_type_list(data, txt_end, _enum_list, _enum_list_end);
						}
					}
					else if((*data)[0] == '\x7B'){
						if(_enum_list) if(*_enum_list < _enum_list_end){
							**_enum_list = json_type::obj;
							++*_enum_list;
						}
						++*data;
						while(txt_end > *data){
							while((txt_end > *data) ? ((**data > 0 && **data <= 32) || **data == ',') : false) ++*data;
							if((txt_end > *data) ? (**data == '\x7D' || **data != '"') : true){
								if((txt_end > *data) ? (**data == '\x7D') : false) ++*data;
								break;
							}
							++*data;
							while(txt_end > *data){
								if(**data == '"'){ ++*data; break; }
								if(**data == '\\'){
									++*data;
									if(txt_end <= *data) break;
								}
								++*data;
							}
							while((txt_end > *data) ? ((**data > 0 && **data <= 32) || **data == ',') : false) ++*data;
							if((txt_end > *data) ? (**data == '\x7D' || **data != ':') : true){
								if((txt_end > *data) ? (**data == '\x7D') : false) ++*data;
								break;
							}
							++*data;
							while((txt_end > *data) ? ((**data > 0 && **data <= 32) || **data == ',') : false) ++*data;
							if((txt_end > *data) ? (**data == '\x7D') : true){
								if(txt_end > *data) ++*data;
								break;
							}
							_ap_::_enum_type_list(data, txt_end, _enum_list, _enum_list_end);
						}
					}
				}
				static unsigned short _count_elems_inside(BYTE** txt, BYTE* txt_end, bool is_array){
					unsigned short _ret = 0;
					while(txt_end > *txt){
						while((txt_end > *txt) ? ((**txt > 0 && **txt <= 32) || **txt == ',') : false) ++*txt;
						if((txt_end > *txt) ? (is_array ? (**txt == ']') : (**txt == '\x7D')) : true){
							if(txt_end > *txt) ++*txt;
							break;
						}
						if(!is_array){
							if(**txt != '"') break;
							++*txt;
							while(txt_end > *txt){
								if(**txt == '"'){ ++*txt; break; }
								if(**txt == '\\'){
									++*txt;
									if(txt_end <= *txt) break;
								}
								++*txt;
							}
							while((txt_end > *txt) ? (**txt > 0 && **txt <= 32) : false) ++*txt;
							if((txt_end > *txt) ? (**txt != ':') : true){
								if(txt_end > *txt) ++*txt;
								break;
							}
							++*txt;
							while((txt_end > *txt) ? (**txt > 0 && **txt <= 32) : false) ++*txt;
							if((txt_end > *txt) ? (**txt == ',' || **txt == '\x7D') : true){
								if(txt_end > *txt) ++*txt;
								break;
							}
						}
						if(((txt_end - *txt) >= 4)
							? (((*txt)[0] == 'n' || (*txt)[0] == 'N') && ((*txt)[1] == 'u' || (*txt)[1] == 'U')
							&& ((*txt)[2] == 'l' || (*txt)[2] == 'L') && ((*txt)[3] == 'l' || (*txt)[3] == 'L'))
							: false){
							*txt += 4;
							++_ret;
						}
						else if(((txt_end - *txt) >= 4)
							? (((*txt)[0] == 't' || (*txt)[0] == 'T') && ((*txt)[1] == 'r' || (*txt)[1] == 'R')
							&& ((*txt)[2] == 'u' || (*txt)[2] == 'U') && ((*txt)[3] == 'e' || (*txt)[3] == 'E'))
							: false){
							*txt += 4;
							++_ret;
						}
						else if(((txt_end - *txt) >= 5)
							? (((*txt)[0] == 'f' || (*txt)[0] == 'F') && ((*txt)[1] == 'a' || (*txt)[1] == 'A')
							&& ((*txt)[2] == 'l' || (*txt)[2] == 'L') && ((*txt)[3] == 's' || (*txt)[3] == 'S')
							&& ((*txt)[4] == 'e' || (*txt)[4] == 'E'))
							: false){
							*txt += 5;
							++_ret;
						}
						else if((*txt)[0] == '-' || (*txt)[0] == '.' || ((*txt)[0] >= '0' && (*txt)[0] <= '9')){
							while(txt_end > *txt){
								if(**txt && (**txt < '0' || **txt > '9') && **txt != '-'
									&& **txt != '.' && **txt != 'e' && **txt != 'E') break;
								else if(!**txt) break;
								++*txt;
							}
							++_ret;
						}
						else if((*txt)[0] == '"'){
							++*txt;
							while(txt_end > *txt){
								if(**txt == '"'){ ++*txt; break; }
								if(**txt == '\\'){
									++*txt;
									if(!(txt_end > *txt)) break;
								}
								++*txt;
							}
							++_ret;
						}
						else if((*txt)[0] == '['){
							++*txt; UWORD upper = 1;
							while(upper){
								if(txt_end > *txt){
									if(**txt == '"'){
										++*txt;
										while((txt_end > *txt) ? (**txt != '"' || *((*txt) - 1) == '\\') : false) ++*txt;
										if(txt_end <= *txt) break;
									}
									if(**txt == '[') ++upper;
									if(**txt == ']') --upper;
								}else break;
								++*txt;
							}
							++_ret;
						}
						else if((*txt)[0] == '\x7B'){
							++*txt; UWORD upper = 1;
							while(upper){
								if(txt_end > *txt){
									if(**txt == '"'){
										++*txt;
										while((txt_end > *txt) ? (**txt != '"' || *((*txt) - 1) == '\\') : false) ++*txt;
										if(txt_end <= *txt) break;
									}
									if(**txt == '\x7B') ++upper;
									if(**txt == '\x7D') --upper;
								}else break;
								++*txt;
							}
							++_ret;
						}
					}
					return _ret;
				}
				static bool _set_mem_rep(BYTE** txt, BYTE* txt_end, BYTE** data, BYTE** _enum_elems, unsigned short* _idx){
					if(**_enum_elems == json_type::nll){
						*txt += 4;
						++*_enum_elems;
						return false;
					}
					else if(((**_enum_elems) & 0x0F) == json_type::bll){
						if(_idx) ++*_idx;
						if((**data = **_enum_elems) & json_type::uns) *txt += 4;
						else                                          *txt += 5;
						++*data; ++*_enum_elems;
					}
					else if(((**_enum_elems) & 0x0F) == json_type::num){
						if(_idx) ++*_idx;
						**data = **_enum_elems; ++*data;
						UWORD old_val = 0; char with_sign = 0;
						if(**txt == '-'){ with_sign = 1; ++*txt; }
						while(*txt < txt_end){
							if(**txt < '0' || **txt > '9') break;
							old_val = (old_val * 10) + (**txt) - '0';
							++*txt;
						}
						// two's complement, the same encoding set_value and set_type write for a negative
						if(with_sign) old_val = (UWORD)(0 - old_val);
						*(UWORD*)(*data) = old_val;
						*data += WORD_SIZE; ++*_enum_elems;
					}
					else if(**_enum_elems == json_type::flt){
						if(_idx) ++*_idx;
						**data = json_type::flt; ++*data; ++*_enum_elems;
						float old_val = 0.0f, _mult = 1.0f; char sign_seg = 0, with_sign = 0, step_down = 0;
						UWORD mult_times = 0;
						if(**txt == '-'){ with_sign = 1; ++*txt; }
						while(*txt < txt_end){
							if(sign_seg == 0){
								if(**txt == '.'){ ++sign_seg; ++*txt; continue; }
								if(**txt == 'e' || **txt == 'E'){
									sign_seg = 2; ++*txt;
									if(**txt == '-' && *txt < txt_end){ step_down = 1; ++*txt; }
									continue;
								}
								if(**txt < '0' || **txt > '9') break;
								old_val = (old_val * 10.0f) + (float)((**txt) - '0');
							}
							else if(sign_seg == 1){
								if(**txt == 'e' || **txt == 'E'){
									++sign_seg; ++*txt;
									if(**txt == '-' && *txt < txt_end){ step_down = 1; ++*txt; }
									continue;
								}
								if(**txt < '0' || **txt > '9') break;
								_mult *= 0.1f;
								old_val += (float)((**txt) - '0') * _mult;
							}
							else if(sign_seg == 2) {
								if(**txt < '0' || **txt > '9') break;
								mult_times = (mult_times * 10) + ((**txt) - '0');
							}
							++*txt;
						}
						if(sign_seg == 2){
							_mult = 1.0f;
							while(mult_times){ --mult_times; _mult *= (step_down ? 0.1f : 10.0f); }
							old_val *= _mult;
						}
						if(with_sign) old_val = -old_val;
						*(float*)(*data) = old_val;
						*data += 4;
					}
					else if(**_enum_elems == json_type::str){
						if(_idx) ++*_idx;
						**data = json_type::str; ++*data; ++*_enum_elems;
						++*txt;
						while(txt_end > *txt){
							if(**txt == '"'){ ++*txt; break; }
							if(**txt == '\\'){
								++*txt;
								if(txt_end <= *txt) break;
								if(**txt == 'n') **data = '\n';
								else if(**txt == 'b') **data = '\b';
								else if(**txt == 'r') **data = '\r';
								else if(**txt == 't') **data = '\t';
								else **data = **txt;
								++*txt; ++*data;
								continue;
							}
							**data = **txt; ++*data; ++*txt;
						}
						**data = '\0'; ++*data;
					}
					else if(**_enum_elems == json_type::ary){
						**data = json_type::ary; ++*data; ++*_enum_elems;
						BYTE *i_pos = ++*txt, *data_elem;
						*(unsigned short*)(*data) = _ap_::_count_elems_inside(&i_pos, txt_end, true);
						unsigned short *_ptr_size = (unsigned short*)(*data), _size = *_ptr_size,
						               pos_idx = 1, post_idx, *dst = _ptr_size + 1;
						data_elem = (BYTE*)(_ptr_size + _size + 1);
						while(_size){
							while((txt_end > *txt) ? ((**txt > 0 && **txt <= 32) || **txt == ',') : false) ++*txt;
							if((txt_end > *txt) ? (**txt == ']') : true) break;
							post_idx = 0;
							bool _res = _ap_::_set_mem_rep(txt, txt_end, &data_elem, _enum_elems, &post_idx);
							*dst++ = (_res ? pos_idx : 0);
							pos_idx += post_idx;
							--_size;
						}
						while((txt_end > *txt) ? ((**txt > 0 && **txt <= 32) || **txt == ',') : false) ++*txt;
						if((txt_end > *txt) ? (**txt == ']') : false) ++*txt;
						*data = data_elem;
						if(_idx) *_idx += pos_idx;
					}
					else if(**_enum_elems == json_type::obj){
					    **data = json_type::obj; ++*data; ++*_enum_elems;
					    BYTE *i_pos = ++*txt, *save_data = (*data) + 2;
					    unsigned short num_elements = _ap_::_count_elems_inside(&i_pos, txt_end, false);
					    *(unsigned short*)(*data) = num_elements;
					    *data += 2;
					    unsigned short _size = num_elements;
					    unsigned short pos_idx = 1, post_idx;
					    if(_size){
					    	i_pos = *txt;
						    while(_size){
						        while((txt_end > *txt) ? ((**txt > 0 && **txt <= 32) || **txt == ',') : false) ++*txt;
						        if((txt_end > *txt) ? (**txt == '\x7D' || **txt != '"') : true) break;
						        ++*txt;
						        *data += 2;
						        while(txt_end > *txt){
						            if(**txt == '"'){ ++*txt; break; }
						            if(**txt == '\\'){
						                ++*txt;
						                if(txt_end <= *txt) break;
						            }
						            **data = **txt; ++*data; ++*txt;
						        }
						        **data = '\0'; ++*data;
						        while((txt_end > *txt) ? ((**txt > 0 && **txt <= 32) || **txt == ',') : false) ++*txt;
						        if((txt_end > *txt) ? (**txt == '\x7D' || **txt != ':') : true) break;
						        ++*txt;
						        while((txt_end > *txt) ? ((**txt > 0 && **txt <= 32) || **txt == ',') : false) ++*txt;
						        _ap_::_enum_type_list(txt, txt_end, nullptr, nullptr);
						        --_size;
						    }
						    *txt = i_pos;
						    _size = num_elements;
						    while(num_elements){
						        while((txt_end > *txt) ? ((**txt > 0 && **txt <= 32) || **txt == ',') : false) ++*txt;
						        if((txt_end > *txt) ? (**txt == '\x7D' || **txt != '"') : true) break;
						        ++*txt;
						        unsigned short* ptr_to_this_index = (unsigned short*)save_data;
						        save_data += 2;
						        while(txt_end > *txt){
						            if(**txt == '"'){ ++*txt; break; }
						            if(**txt == '\\'){
						                ++*txt;
						                if(txt_end <= *txt) break;
						            }
						            ++*txt; ++save_data;
						        }
						        ++save_data;
						        while((txt_end > *txt) ? ((**txt > 0 && **txt <= 32) || **txt == ',') : false) ++*txt;
						        if((txt_end > *txt) ? (**txt == '\x7D' || **txt != ':') : true) break;
						        ++*txt;
						        while((txt_end > *txt) ? ((**txt > 0 && **txt <= 32) || **txt == ',') : false) ++*txt;
						        post_idx = 0;
						        bool _res = _ap_::_set_mem_rep(txt, txt_end, data, _enum_elems, &post_idx);
						        *ptr_to_this_index = (_res ? pos_idx : 0);
						        pos_idx += post_idx;
						        --num_elements;
						    }
						}
					    while((txt_end > *txt) ? ((**txt > 0 && **txt <= 32) || **txt == ',') : false) ++*txt;
						if((txt_end > *txt) ? (**txt == '\x7D') : false) ++*txt;
						if(_idx) *_idx += pos_idx;
					}
					return true;
				}
				#if defined(ST_IMPORT_FILE)
				static int _read_char(void* fle, bool* _EOF, WORD* sze){
					int _ret = L'\0';
					WORD _sze = file::getc(fle, &_ret);
					if(_sze) file::seek(fle, -_sze, seek_mode::current);
					else *_EOF = true;
					if(sze) *sze = _sze;
					return _ret;
				}
				static inline UWORD append_escaped(BYTE** dst, unsigned int wc){
					if(wc < 0x80){
						if(!dst) return 1;
						**dst = (char)(wc & 0xFF); ++*dst;
					}
					else if(wc < 0x800){
						if(!dst) return 2;
						**dst = (char)(0xC0 | (wc >> 6)); ++*dst;
						**dst = (char)(0x80 | (wc & 0x3F)); ++*dst;
					}
					else if(wc < 0x10000){
						if(!dst) return 3;
						**dst = (char)(0xE0 | (wc >> 12)); ++*dst;
						**dst = (char)(0x80 | ((wc >> 6) & 0x3F)); ++*dst;
						**dst = (char)(0x80 | (wc & 0x3F)); ++*dst;
					}
					else if(wc < 0x110000){
						if(!dst) return 4;
						**dst = (char)(0xF0 | (wc >> 18)); ++*dst;
						**dst = (char)(0x80 | ((wc >> 12) & 0x3F)); ++*dst;
						**dst = (char)(0x80 | ((wc >> 6) & 0x3F)); ++*dst;
						**dst = (char)(0x80 | (wc & 0x3F)); ++*dst;
					}
					return 0;
				}
				static UWORD _file_count_mem_size(void* file, bool* _EOF, BYTE* _type, UWORD* _enum_elems){
					UWORD _ret_data_size = 0;
					WORD _read_size = 0, _add_read_size = 0; int _read;
					if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) == L'n' || _read == L'N') && !*_EOF){
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'u' && _read != L'U') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return 0;
						}
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'l' && _read != L'L') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return 0;
						}
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'l' && _read != L'L') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return 0;
						}
						file::seek(file, _read_size, seek_mode::current);
						*_type = json_type::nll;
						++*_enum_elems;
					}
					else if((_read == L't' || _read == L'T') && !*_EOF){
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'r' && _read != L'R') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return 0;
						}
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'u' && _read != L'U') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return 0;
						}
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'e' && _read != L'E') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return 0;
						}
						file::seek(file, _read_size, seek_mode::current);
						*_type = json_type::bll | json_type::uns;
						_ret_data_size = 1;
						++*_enum_elems;
					}
					else if((_read == L'f' || _read == L'F') && !*_EOF){
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'a' && _read != L'A') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return 0;
						}
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'l' && _read != L'L') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return 0;
						}
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L's' && _read != L'S') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return 0;
						}
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'e' && _read != L'E') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return 0;
						}
						file::seek(file, _read_size, seek_mode::current);
						*_type = json_type::bll;
						_ret_data_size = 1;
						++*_enum_elems;
					}
					else if((_read == L'-' || _read == L'.' || (_read >= L'0' && _read <= L'9')) && !*_EOF){
						++*_enum_elems;
						*_type = json_type::num | ((_read == L'-') ? 0 : json_type::uns);
						while(!*_EOF){
							_read = _ap_::_read_char(file, _EOF, &_read_size);
							if(_read == L'.' || _read == L'e' || _read == L'E') *_type = json_type::flt;
							else if(_read && ((_read < L'0' || _read > L'9') && _read != L'-')) break;
							file::seek(file, _read_size, seek_mode::current);
						}
						_ret_data_size = 1 + (((*_type & 0x0F) == json_type::num) ? WORD_SIZE : 4);
					}
					else if(_read == L'"' && !*_EOF){
						++*_enum_elems;
						*_type = json_type::str;
						_ret_data_size = 2;
						file::seek(file, _read_size, seek_mode::current);
						while(!*_EOF){
							_read = _ap_::_read_char(file, _EOF, &_read_size);
							if(_read == L'"'){ file::seek(file, _read_size, seek_mode::current); break; }
							if(_read == L'\\'){
								file::seek(file, _read_size, seek_mode::current);
								_read = _ap_::_read_char(file, _EOF, &_read_size);
								if(*_EOF) break;
							}
							_ret_data_size += _ap_::append_escaped(nullptr, _read);
							file::seek(file, _read_size, seek_mode::current);
						}
					}
					else if(_read == L'[' && !*_EOF){
						++*_enum_elems;
						_ret_data_size = 3;
						file::seek(file, _read_size, seek_mode::current);
						while(!*_EOF){
							while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
								&& _read <= 32) || _read == ',') && !*_EOF)
								file::seek(file, _read_size, seek_mode::current);
							if(*_EOF || _read == ']'){
								if(!*_EOF) file::seek(file, _read_size, seek_mode::current);
								break;
							}
							_ret_data_size += 2 + _ap_::_file_count_mem_size(file, _EOF, _type, _enum_elems);
						}
						*_type = json_type::ary;
					}
					else if(_read == L'\x7B' && !*_EOF){
						++*_enum_elems;
						_ret_data_size = 3;
						file::seek(file, _read_size, seek_mode::current);
						while(!*_EOF){
							while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
								&& _read <= 32) || _read == L',') && !*_EOF)
								file::seek(file, _read_size, seek_mode::current);
							if(*_EOF || _read == L'\x7D' || _read != L'"'){
								if(!*_EOF) file::seek(file, _read_size, seek_mode::current);
								break;
							}
							file::seek(file, _read_size, seek_mode::current);
							while(!*_EOF){
								_read = _ap_::_read_char(file, _EOF, &_read_size);
								if(_read == L'"'){
									file::seek(file, _read_size, seek_mode::current);
									break;
								}
								if(_read == L'\\'){
									file::seek(file, _read_size, seek_mode::current);
									_read = _ap_::_read_char(file, _EOF, &_read_size);
									if(*_EOF) break;
								}
								_ret_data_size += _ap_::append_escaped(nullptr, _read);
								file::seek(file, _read_size, seek_mode::current);
							}
							while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
								&& _read <= 32) || _read == L',') && !*_EOF)
								file::seek(file, _read_size, seek_mode::current);
							if(*_EOF || _read == L'\x7D' || _read != L':'){
								if(!*_EOF) file::seek(file, _read_size, seek_mode::current);
								break;
							}
							++_ret_data_size; file::seek(file, _read_size, seek_mode::current);
							while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
								&& _read <= 32) || _read == L',') && !*_EOF)
								file::seek(file, _read_size, seek_mode::current);
							if(*_EOF || _read == L'\x7D'){
								if(!*_EOF) file::seek(file, _read_size, seek_mode::current);
								break;
							}
							_ret_data_size += 2 + _ap_::_file_count_mem_size(file, _EOF, _type, _enum_elems);
						}
						*_type = json_type::obj;
					}
					return _ret_data_size;
				}
				static void _file_enum_type_list(void* file, bool* _EOF, BYTE** _enum_list, BYTE* _enum_list_end){
					WORD _read_size = 0, _add_read_size = 0; int _read;
					if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) == L'n' || _read == L'N') && !*_EOF){
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'u' && _read != L'U') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return;
						}
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'l' && _read != L'L') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return;
						}
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'l' && _read != L'L') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return;
						}
						file::seek(file, _read_size, seek_mode::current);
						if(_enum_list) if(*_enum_list < _enum_list_end){
							**_enum_list = json_type::nll;
							++*_enum_list;
						}
					}
					else if((_read == L't' || _read == L'T') && !*_EOF){
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'r' && _read != L'R') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return;
						}
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'u' && _read != L'U') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return;
						}
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'e' && _read != L'E') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return;
						}
						file::seek(file, _read_size, seek_mode::current);
						if(_enum_list) if(*_enum_list < _enum_list_end){
							**_enum_list = json_type::bll | json_type::uns;
							++*_enum_list;
						}
					}
					else if((_read == L'f' || _read == L'F') && !*_EOF){
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'a' && _read != L'A') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return;
						}
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'l' && _read != L'L') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return;
						}
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L's' && _read != L'S') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return;
						}
						file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'e' && _read != L'E') || *_EOF){
							file::seek(file, -_add_read_size, seek_mode::current);
							return;
						}
						file::seek(file, _read_size, seek_mode::current);
						if(_enum_list) if(*_enum_list < _enum_list_end){
							**_enum_list = json_type::bll;
							++*_enum_list;
						}
					}
					else if((_read == L'-' || _read == L'.' || (_read >= L'0' && _read <= L'9')) && !*_EOF){
						char _type = json_type::num | ((_read == L'-') ? 0 : json_type::uns);
						while(!*_EOF){
							_read = _ap_::_read_char(file, _EOF, &_read_size);
							if(_read == L'.' || _read == L'e' || _read == L'E') _type = json_type::flt;
							else if(_read && ((_read < L'0' || _read > L'9') && _read != L'-')) break;
							file::seek(file, _read_size, seek_mode::current);
						}
						if(_enum_list) if(*_enum_list < _enum_list_end){
							**_enum_list = _type;
							++*_enum_list;
						}
					}
					else if(_read == L'"' && !*_EOF){
						if(_enum_list) if(*_enum_list < _enum_list_end){
							**_enum_list = json_type::str;
							++*_enum_list;
						}
						file::seek(file, _read_size, seek_mode::current);
						while(!*_EOF){
							_read = _ap_::_read_char(file, _EOF, &_read_size);
							if(_read == L'"'){
								file::seek(file, _read_size, seek_mode::current);
								break;
							}
							if(_read == L'\\'){
								file::seek(file, _read_size, seek_mode::current);
								_read = _ap_::_read_char(file, _EOF, &_read_size);
								if(*_EOF) break;
							}
							file::seek(file, _read_size, seek_mode::current);
						}
					}
					else if(_read == L'[' && !*_EOF){
						if(_enum_list) if(*_enum_list < _enum_list_end){
							**_enum_list = json_type::ary;
							++*_enum_list;
						}
						file::seek(file, _read_size, seek_mode::current);
						while(!*_EOF){
							while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
								&& _read <= 32) || _read == ',') && !*_EOF)
								file::seek(file, _read_size, seek_mode::current);
							if(*_EOF || _read == ']'){
								if(!*_EOF) file::seek(file, _read_size, seek_mode::current);
								break;
							}
							_ap_::_file_enum_type_list(file, _EOF, _enum_list, _enum_list_end);
						}
					}
					else if(_read == L'\x7B' && !*_EOF){
						if(_enum_list) if(*_enum_list < _enum_list_end){
							**_enum_list = json_type::obj;
							++*_enum_list;
						}
						file::seek(file, _read_size, seek_mode::current);
						while(!*_EOF){
							while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
								&& _read <= 32) || _read == L',') && !*_EOF)
								file::seek(file, _read_size, seek_mode::current);
							if(*_EOF || _read == L'\x7D' || _read != L'"'){
								if(!*_EOF && _read == L'\x7D') file::seek(file, _read_size, seek_mode::current);
								break;
							}
							file::seek(file, _read_size, seek_mode::current);
							while(!*_EOF){
								_read = _ap_::_read_char(file, _EOF, &_read_size);
								if(_read == L'"'){
									file::seek(file, _read_size, seek_mode::current);
									break;
								}
								if(_read == L'\\'){
									file::seek(file, _read_size, seek_mode::current);
									_read = _ap_::_read_char(file, _EOF, &_read_size);
									if(*_EOF) break;
								}
								file::seek(file, _read_size, seek_mode::current);
							}
							while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
								&& _read <= 32) || _read == L',') && !*_EOF)
								file::seek(file, _read_size, seek_mode::current);
							if(*_EOF || _read == L'\x7D' || _read != L':'){
								if(!*_EOF && _read == L'\x7D') file::seek(file, _read_size, seek_mode::current);
								break;
							}
							file::seek(file, _read_size, seek_mode::current);
							while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
								&& _read <= 32) || _read == L',') && !*_EOF)
								file::seek(file, _read_size, seek_mode::current);
							if(*_EOF || _read == L'\x7D'){
								if(!*_EOF) file::seek(file, _read_size, seek_mode::current);
								break;
							}
							_ap_::_file_enum_type_list(file, _EOF, _enum_list, _enum_list_end);
						}
					}
				}
				static unsigned short _file_count_elems_inside(void* file, bool* _EOF, bool is_array){
					unsigned short _ret = 0;
					while(!*_EOF){
						WORD _read_size = 0, _add_read_size = 0; int _read;
						while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
								&& _read <= 32) || _read == L',') && !*_EOF)
								file::seek(file, _read_size, seek_mode::current);
						if(*_EOF || (is_array ? (_read == L']') : (_read == L'\x7D'))){
							if(!*_EOF) file::seek(file, _read_size, seek_mode::current);
							break;
						}
						if(!is_array){
							if(_read != L'"') break;
							file::seek(file, _read_size, seek_mode::current);
							while(!*_EOF){
								_read = _ap_::_read_char(file, _EOF, &_read_size);
								if(_read == L'"'){
									file::seek(file, _read_size, seek_mode::current);
									break;
								}
								if(_read == L'\\'){
									file::seek(file, _read_size, seek_mode::current);
									_read = _ap_::_read_char(file, _EOF, &_read_size);
									if(*_EOF) break;
								}
								file::seek(file, _read_size, seek_mode::current);
							}
							while((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
								&& _read <= 32 && !*_EOF)
								file::seek(file, _read_size, seek_mode::current);
							if(*_EOF || _read != L':'){
								if(!*_EOF) file::seek(file, _read_size, seek_mode::current);
								break;
							}
							file::seek(file, _read_size, seek_mode::current);
							while((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
								&& _read <= 32 && !*_EOF)
								file::seek(file, _read_size, seek_mode::current);
							if(*_EOF || _read == ',' || _read == '\x7D'){
								if(!*_EOF) file::seek(file, _read_size, seek_mode::current);
								break;
							}
						}
						if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) == L'n' || _read == L'N') && !*_EOF){
							file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
							if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'u' && _read != L'U') || *_EOF){
								file::seek(file, -_add_read_size, seek_mode::current);
								break;
							}
							file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
							if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'l' && _read != L'L') || *_EOF){
								file::seek(file, -_add_read_size, seek_mode::current);
								break;
							}
							file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
							if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'l' && _read != L'L') || *_EOF){
								file::seek(file, -_add_read_size, seek_mode::current);
								break;
							}
							file::seek(file, _read_size, seek_mode::current);
							++_ret;
						}
						else if((_read == L't' || _read == L'T') && !*_EOF){
							file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
							if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'r' && _read != L'R') || *_EOF){
								file::seek(file, -_add_read_size, seek_mode::current);
								break;
							}
							file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
							if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'u' && _read != L'U') || *_EOF){
								file::seek(file, -_add_read_size, seek_mode::current);
								break;
							}
							file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
							if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'e' && _read != L'E') || *_EOF){
								file::seek(file, -_add_read_size, seek_mode::current);
								break;
							}
							file::seek(file, _read_size, seek_mode::current);
							++_ret;
						}
						else if((_read == L'f' || _read == L'F') && !*_EOF){
							file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
							if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'a' && _read != L'A') || *_EOF){
								file::seek(file, -_add_read_size, seek_mode::current);
								break;
							}
							file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
							if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'l' && _read != L'L') || *_EOF){
								file::seek(file, -_add_read_size, seek_mode::current);
								break;
							}
							file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
							if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L's' && _read != L'S') || *_EOF){
								file::seek(file, -_add_read_size, seek_mode::current);
								break;
							}
							file::seek(file, _read_size, seek_mode::current); _add_read_size += _read_size;
							if(((_read = _ap_::_read_char(file, _EOF, &_read_size)) != L'e' && _read != L'E') || *_EOF){
								file::seek(file, -_add_read_size, seek_mode::current);
								break;
							}
							file::seek(file, _read_size, seek_mode::current);
							++_ret;
						}
						else if((_read == L'-' || _read == L'.' || (_read >= L'0' && _read <= L'9')) && !*_EOF){
							while(!*_EOF){
								_read = _ap_::_read_char(file, _EOF, &_read_size);
								if((_read && (_read < L'0' || _read > L'9') && _read != L'-'
									&& _read != L'.' && _read != L'e' && _read != L'E')
									|| *_EOF) break;
								file::seek(file, _read_size, seek_mode::current);
							}
							++_ret;
						}
						else if(_read == L'"' && !*_EOF){
							file::seek(file, _read_size, seek_mode::current);
							while(!*_EOF){
								_read = _ap_::_read_char(file, _EOF, &_read_size);
								if(_read == L'"'){
									file::seek(file, _read_size, seek_mode::current);
									break;
								}
								if(_read == L'\\'){
									file::seek(file, _read_size, seek_mode::current);
									_read = _ap_::_read_char(file, _EOF, &_read_size);
									if(*_EOF) break;
								}
								file::seek(file, _read_size, seek_mode::current);
							}
							++_ret;
						}
						else if(_read == L'[' && !*_EOF){
							file::seek(file, _read_size, seek_mode::current);
							UWORD upper = 1;
							while(upper){
								if(*_EOF) break;
								else{
									_read = _ap_::_read_char(file, _EOF, &_read_size);
									if(_read == L'"'){
										file::seek(file, _read_size, seek_mode::current);
										int prev_char = L'\0';
										while(!*_EOF && (_read != L'"' || prev_char == L'\\')){
											prev_char = _read;
											_read = _ap_::_read_char(file, _EOF, &_read_size);
											file::seek(file, _read_size, seek_mode::current);
										}
										if(*_EOF) break;
									}
									if(_read == L'[') ++upper;
									if(_read == L']') --upper;
								}
								file::seek(file, _read_size, seek_mode::current);
							}
							++_ret;
						}
						else if(_read == L'\x7B' && !*_EOF){
							file::seek(file, _read_size, seek_mode::current);
							UWORD upper = 1;
							while(upper){
								if(*_EOF) break;
								else{
									_read = _ap_::_read_char(file, _EOF, &_read_size);
									if(_read == L'"'){
										file::seek(file, _read_size, seek_mode::current);
										int prev_char = L'\0';
										while(!*_EOF && (_read != L'"' || prev_char == L'\\')){
											prev_char = _read;
											_read = _ap_::_read_char(file, _EOF, &_read_size);
											file::seek(file, _read_size, seek_mode::current);
										}
										if(*_EOF) break;
									}
									if(_read == L'\x7B') ++upper;
									if(_read == L'\x7D') --upper;
								}
								file::seek(file, _read_size, seek_mode::current);
							}
							++_ret;
						}
					}
					return _ret;
				}
				static bool _file_set_mem_rep(void* file, bool* _EOF, BYTE** data, BYTE** _enum_elems, unsigned short* _idx){
					WORD _read_size = 0; int _read;
					if(**_enum_elems == json_type::nll){
						file::seek(file, 4, seek_mode::current);
						++*_enum_elems;
						return false;
					}
					else if(((**_enum_elems) & 0x0F) == json_type::bll){
						if(_idx) ++*_idx;
						file::seek(file, 4 + (((**data = **_enum_elems) & json_type::uns) ? 0 : 1), seek_mode::current);
						++*data; ++*_enum_elems;
					}
					else if(((**_enum_elems) & 0x0F) == json_type::num){
						if(_idx) ++*_idx;
						**data = **_enum_elems; ++*data;
						UWORD old_val = 0; char with_sign = 0;
						if((_read = _ap_::_read_char(file, _EOF, &_read_size)) == L'-'){
							with_sign = 1;
							file::seek(file, _read_size, seek_mode::current);
						}
						while(!*_EOF){
							_read = _ap_::_read_char(file, _EOF, &_read_size);
							if(_read < L'0' || _read > L'9') break;
							old_val = (old_val * 10) + _read - L'0';
							file::seek(file, _read_size, seek_mode::current);
						}
						// two's complement, the same encoding set_value and set_type write for a negative
						if(with_sign) old_val = (UWORD)(0 - old_val);
						*(UWORD*)(*data) = old_val;
						*data += WORD_SIZE; ++*_enum_elems;
					}
					else if(**_enum_elems == json_type::flt){
						if(_idx) ++*_idx;
						**data = json_type::flt; ++*data; ++*_enum_elems;
						float old_val = 0.0f, _mult = 1.0f; char sign_seg = 0, with_sign = 0, step_down = 0;
						UWORD mult_times = 0;
						if((_read = _ap_::_read_char(file, _EOF, &_read_size)) == L'-'){
							with_sign = 1;
							file::seek(file, _read_size, seek_mode::current);
						}
						while(!*_EOF){
							_read = _ap_::_read_char(file, _EOF, &_read_size);
							if(sign_seg == 0){
								if(_read == L'.'){
									++sign_seg;
									file::seek(file, _read_size, seek_mode::current);
									continue;
								}
								if(_read == L'e' || _read == L'E'){
									sign_seg = 2; file::seek(file, _read_size, seek_mode::current);
									if((_read = _ap_::_read_char(file, _EOF, &_read_size)) == L'-' && !*_EOF){
										step_down = 1;
										file::seek(file, _read_size, seek_mode::current);
									}
									continue;
								}
								if(_read < L'0' || _read > L'9') break;
								old_val = (old_val * 10.0f) + (float)(_read - L'0');
							}
							else if(sign_seg == 1){
								if(_read == 'e' || _read == 'E'){
									++sign_seg; file::seek(file, _read_size, seek_mode::current);
									if((_read = _ap_::_read_char(file, _EOF, &_read_size)) == L'-' && !*_EOF){
										step_down = 1;
										file::seek(file, _read_size, seek_mode::current);
									}
									continue;
								}
								if(_read < L'0' || _read > L'9') break;
								_mult *= 0.1f;
								old_val += (float)(_read - L'0') * _mult;
							}
							else if(sign_seg == 2) {
								if(_read < L'0' || _read > L'9') break;
								mult_times = (mult_times * 10) + (_read - L'0');
							}
							file::seek(file, _read_size, seek_mode::current);
						}
						if(sign_seg == 2){
							_mult = 1.0f;
							while(mult_times){ --mult_times; _mult *= (step_down ? 0.1f : 10.0f); }
							old_val *= _mult;
						}
						if(with_sign) old_val = -old_val;
						*(float*)(*data) = old_val;
						*data += 4;
					}
					else if(**_enum_elems == json_type::str){
						if(_idx) ++*_idx;
						**data = json_type::str; ++*data; ++*_enum_elems;
						file::getc(file, &_read);
						while(!*_EOF){
							_read = _ap_::_read_char(file, _EOF, &_read_size);
							if(_read == L'"'){
								file::seek(file, _read_size, seek_mode::current);
								break;
							}
							if(_read == L'\\'){
								file::seek(file, _read_size, seek_mode::current);
								_read = _ap_::_read_char(file, _EOF, &_read_size);
								if(*_EOF) break;
								if(_read == L'n') _ap_::append_escaped(data, L'\n');
								else if(_read == L'b') _ap_::append_escaped(data, L'\b');
								else if(_read == L'r') _ap_::append_escaped(data, L'\r');
								else if(_read == L't') _ap_::append_escaped(data, L'\t');
								else _ap_::append_escaped(data, _read);
								file::seek(file, _read_size, seek_mode::current);
								++*data;
								continue;
							}
							_ap_::append_escaped(data, _read);
							file::seek(file, _read_size, seek_mode::current);
						}
						**data = L'\0'; ++*data;
					}
					else if(**_enum_elems == json_type::ary){
						**data = json_type::ary; ++*data; ++*_enum_elems;
						BYTE *data_elem;
						file::getc(file, &_read); UWORD i_pos = file::tell(file); 
						*(unsigned short*)(*data) = _ap_::_file_count_elems_inside(file, _EOF, true);
						*_EOF = false; file::seek(file, i_pos, seek_mode::begin);
						unsigned short *_ptr_size = (unsigned short*)(*data), _size = *_ptr_size,
						               pos_idx = 1, post_idx, *dst = _ptr_size + 1;
						data_elem = (BYTE*)(_ptr_size + *_ptr_size + 1);
						while(_size){
							while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
								&& _read <= 32) || _read == L',') && !*_EOF)
								file::seek(file, _read_size, seek_mode::current);
							if(*_EOF || _read == L']'){
								if(!*_EOF) file::seek(file, _read_size, seek_mode::current);
								break;
							}
							post_idx = 0;
							bool _res = _ap_::_file_set_mem_rep(file, _EOF, &data_elem, _enum_elems, &post_idx);
							*dst++ = (_res ? pos_idx : 0);
							pos_idx += post_idx;
							--_size;
						}
						while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
							&& _read <= 32) || _read == L',') && !*_EOF)
							file::seek(file, _read_size, seek_mode::current);
						if(!*_EOF && _read == L']') file::seek(file, _read_size, seek_mode::current);
						*data = data_elem;
						if(_idx) *_idx += pos_idx;
					}
					else if(**_enum_elems == json_type::obj){
						**data = json_type::obj; ++*data; ++*_enum_elems;
					    file::getc(file, &_read); UWORD i_pos = file::tell(file);
					    unsigned short num_elements = _ap_::_file_count_elems_inside(file, _EOF, false);
					    *_EOF = false; file::seek(file, i_pos, seek_mode::begin);
					    *(unsigned short*)(*data) = num_elements;
					    *data += 2; BYTE* save_data = *data;
					    unsigned short _size = num_elements;
					    unsigned short pos_idx = 1, post_idx;
					    if(_size){
					    	while(_size){
						    	while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
									&& _read <= 32) || _read == L',') && !*_EOF)
						    		file::seek(file, _read_size, seek_mode::current);
						        if(*_EOF || _read == L'\x7D' || _read != L'"'){
									if(!*_EOF && _read == L'\x7D') file::seek(file, _read_size, seek_mode::current);
									break;
								}
						        file::seek(file, _read_size, seek_mode::current);
						        *data += 2;
						        while(!*_EOF){
						        	_read = _ap_::_read_char(file, _EOF, &_read_size);
						            if(_read == L'"'){
										file::seek(file, _read_size, seek_mode::current);
										break;
									}
						            if(_read == L'\\'){
						                file::seek(file, _read_size, seek_mode::current);
						                _read = _ap_::_read_char(file, _EOF, &_read_size);
						                if(*_EOF) break;
						            }
						            _ap_::append_escaped(data, _read);
						            file::seek(file, _read_size, seek_mode::current);
						        }
						        **data = '\0'; ++*data;
								while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
									&& _read <= 32) || _read == L',') && !*_EOF)
						    		file::seek(file, _read_size, seek_mode::current);
								if(*_EOF || _read == L'\x7D' || _read != L':'){
									if(!*_EOF && _read == L'\x7D') file::seek(file, _read_size, seek_mode::current);
									break;
								}
						        file::seek(file, _read_size, seek_mode::current);
						        while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
									&& _read <= 32) || _read == L',') && !*_EOF)
						    		file::seek(file, _read_size, seek_mode::current);
						    	if(*_EOF || _read == L'\x7D'){
									if(!*_EOF) file::seek(file, _read_size, seek_mode::current);
									break;
								}
						        _ap_::_file_enum_type_list(file, _EOF, nullptr, nullptr);
						        --_size;
						    }
						    *_EOF = false; file::seek(file, i_pos, seek_mode::begin);
						    _size = num_elements;
					    	while(num_elements){
								while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
							        && _read <= 32) || _read == L',') && !*_EOF)
							        file::seek(file, _read_size, seek_mode::current);
							    if(*_EOF || _read == L'\x7D' || _read != L'"'){
							        if(!*_EOF && _read == L'\x7D') file::seek(file, _read_size, seek_mode::current);
							        break;
							    }
								file::seek(file, _read_size, seek_mode::current);
								unsigned short* ptr_to_this_index = (unsigned short*)save_data;
						        save_data += 2;
								while(!*_EOF){
									_read = _ap_::_read_char(file, _EOF, &_read_size);
									if(_read == L'"'){
										file::seek(file, _read_size, seek_mode::current);
										break;
									}
									if(_read == L'\\'){
										file::seek(file, _read_size, seek_mode::current);
										_read = _ap_::_read_char(file, _EOF, &_read_size);
										if(*_EOF) break;
									}
									save_data += _ap_::append_escaped(nullptr, _read);
									file::seek(file, _read_size, seek_mode::current);
								}
								++save_data;
								while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
									&& _read <= 32) || _read == L',') && !*_EOF)
									file::seek(file, _read_size, seek_mode::current);
								if(*_EOF || _read == L'\x7D' || _read != L':'){
									if(!*_EOF && _read == L'\x7D') file::seek(file, _read_size, seek_mode::current);
									break;
								}
								file::seek(file, _read_size, seek_mode::current);
								while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
									&& _read <= 32) || _read == L',') && !*_EOF)
									file::seek(file, _read_size, seek_mode::current);
								if(*_EOF || _read == L'\x7D'){
									if(!*_EOF) file::seek(file, _read_size, seek_mode::current);
									break;
								}
								post_idx = 0;
						        bool _res = _ap_::_file_set_mem_rep(file, _EOF, data, _enum_elems, &post_idx);
						        *ptr_to_this_index = (_res ? pos_idx : 0);
						        pos_idx += post_idx;
						        --num_elements;
							}
						}
					    while((((_read = _ap_::_read_char(file, _EOF, &_read_size)) > 0
						    && _read <= 32) || _read == L',') && !*_EOF)
						    file::seek(file, _read_size, seek_mode::current);
						if(!*_EOF && _read == L'\x7D') file::seek(file, _read_size, seek_mode::current);
						if(_idx) *_idx += pos_idx;
					}
					return true;
				}
				#endif
			};
			#if defined(ST_IMPORT_FILE)
			if(*(UWORD*)src == 0x46494C45){ // From a file
				if(!(file::opened_like(src) & file_mode::read)) return nullptr;
				bool _EOF = false; int _read; WORD _read_size;
				while((_read = _ap_::_read_char(src, &_EOF, &_read_size)) >= 0 && _read <= 32 && !_EOF)
					file::seek(src, _read_size, seek_mode::current);
				if((_read != L'n' && _read != L'N' && _read != L't' && _read != L'T'
				&& _read != L'f' && _read != L'F' && _read != L'-' && _read != L'.'
				&& (_read < L'0' || _read > L'9') && _read != L'"'
				&& _read != L'[' && _read != L'\x7B') || _EOF) return nullptr;
				BYTE _type = json_type::unk;
				UWORD _enum_elems = 0, post_pos = file::tell(src);
				UWORD data_size = _ap_::_file_count_mem_size(src, &_EOF, &_type, &_enum_elems);
				BYTE* _enum_list = nullptr;
				if(_enum_elems && (_type == json_type::ary || _type == json_type::obj)){
					file::seek(src, post_pos, seek_mode::begin);
					_enum_list = new BYTE[_enum_elems];
					BYTE* _enum_list_idx = _enum_list;
					_EOF = false;
					_ap_::_file_enum_type_list(src, &_EOF, &_enum_list_idx, _enum_list + _enum_elems);
				}
				file::seek(src, post_pos + ((_type == json_type::nll) ? 4 : 0), seek_mode::begin);
				if(!data_size && _type != json_type::nll) return nullptr;
				JSN_HANDLE* _ret = new JSN_HANDLE;
				if(data_size){
					_ret->base = new BYTE*((BYTE*)memory::alloc(data_size));
					_ret->data = *(_ret->base);
				}else{
					_ret->base = nullptr;
					_ret->data = nullptr;
				}
				_ret->array_handle = nullptr;
				_ret->index_ptr = nullptr;
				BYTE* data = _ret->data;
				if(!data) return (void*)_ret;
				_EOF = false;
				BYTE* ptr_type_list = &_type;
				if(_enum_list){
					ptr_type_list = _enum_list;
					_ap_::_file_set_mem_rep(src, &_EOF, &data, &ptr_type_list, nullptr);
					delete[] _enum_list;
				}
				else _ap_::_file_set_mem_rep(src, &_EOF, &data, &ptr_type_list, nullptr);
				return (void*)_ret;
			}
			#endif
			// From a text
			BYTE* txt = (BYTE*)src; while(*txt > 0 && *txt <= 32) ++txt;
			if(!*txt) return nullptr;
			if(*txt != L'n' && *txt != L'N' && *txt != L't' && *txt != L'T'
				&& *txt != L'f' && *txt != L'F' && *txt != L'-' && *txt != L'.'
				&& (*txt < L'0' || *txt > L'9') && *txt != L'"'
				&& *txt != L'[' && *txt != L'\x7B') return nullptr;
			BYTE* txt_end = txt; while(*txt_end) ++txt_end;
			if(txt == txt_end) return nullptr;
			BYTE _type = json_type::unk;
			BYTE* data = txt;
			UWORD _enum_elems = 0;
			UWORD data_size = _ap_::_count_mem_size(&data, txt_end, &_type, &_enum_elems);
			BYTE* _enum_list = nullptr;
			if(_enum_elems && (_type == json_type::ary || _type == json_type::obj)){
				data = txt;
				_enum_list = new BYTE[_enum_elems];
				BYTE* _enum_list_idx = _enum_list;
				_ap_::_enum_type_list(&data, txt_end, &_enum_list_idx, _enum_list + _enum_elems);
			}
			if(!data_size && _type != json_type::nll) return nullptr;
			JSN_HANDLE* _ret = new JSN_HANDLE;
			if(data_size){
				_ret->base = new BYTE*((BYTE*)memory::alloc(data_size));
				_ret->data = *(_ret->base);
			}else{
				_ret->base = nullptr;
				_ret->data = nullptr;
			}
			_ret->array_handle = nullptr;
			_ret->index_ptr = nullptr;
			data = _ret->data;
			if(!data) return (void*)_ret;
			BYTE* ptr_type_list = &_type;
			if(_enum_list){
				ptr_type_list = _enum_list;
				_ap_::_set_mem_rep(&txt, txt_end, &data, &ptr_type_list, nullptr);
				delete[] _enum_list;
			}
			else _ap_::_set_mem_rep(&txt, txt_end, &data, &ptr_type_list, nullptr);
			return (void*)_ret;
		}
		FUNCDECL(BYTE type, void* jsn){
			if(JSN_HANDLE_VALID(jsn)) return json_type::unk;
			JSN_HANDLE_DECL;
			JSN_HANDLE* handle = (JSN_HANDLE*)jsn;
			if(!(handle->data)) return json_type::nll;
			return (BYTE)*(handle->data);
		}
		FUNCDECL(UWORD size, void* jsn){
			if(JSN_HANDLE_VALID(jsn)) return 0;
			JSN_HANDLE_DECL;
			JSN_HANDLE* handle = (JSN_HANDLE*)jsn;
			if(!(handle->data)) return 0;
			BYTE type = (BYTE)*(handle->data);
			if(type == json_type::str){
	    		BYTE* _str = (handle->data) + 1;
	    		UWORD _sze = 0;
	    		while(_str[_sze]) ++_sze;
	    		return _sze;
			}
	    	if(type == json_type::ary || type == json_type::obj) return (UWORD)*(unsigned short*)((handle->data) + 1);
    		return JSN_DEFAULT_MEMSIZE(type);
		}
		FUNCDECL(void* value, void* jsn, void* offset){
			if(JSN_HANDLE_VALID(jsn)) return nullptr;
			JSN_HANDLE_DECL;
			JSN_HANDLE* handle = (JSN_HANDLE*)jsn;
			if(!(handle->data)) return nullptr;
			BYTE type = *(handle->data);
			if((type & 0x0F) == json_type::bll) return (type & json_type::uns) ? (void*)1 : (void*)0;
			if((type & 0x0F) == json_type::num) return (void*)*(UWORD*)((handle->data) + 1);
			if(type == json_type::flt || type == json_type::str) return (void*)((handle->data) + 1);
			if(type == json_type::ary){
				unsigned short *size = (unsigned short*)((handle->data) + 1);
				UWORD index = (UWORD)offset;
				if(++index > *size || !*size) return nullptr;
				JSN_HANDLE* _ret = new JSN_HANDLE;
				_ret->base = handle->base;
				_ret->data = nullptr;
				size += index;
				if(*size) _ret->data = JSN_HANDLE::nav(handle->data, *size);
				_ret->array_handle = handle;
				_ret->index_ptr = (BYTE*)size;
				return (void*)_ret;
			}
			if(type == json_type::obj){
				BYTE *_dat = (handle->data) + 1, *_pos = (BYTE*)offset;
				unsigned short size = *(unsigned short*)_dat, *_dat_sve;
				if(!offset || !size) return nullptr;
				_dat += 2;
				while(size){
					_dat_sve = (unsigned short*)_dat; _dat += 2;
					_pos = (BYTE*)offset;
					while(*_dat && *_pos == *_dat){ ++_pos; ++_dat; }
					if(!*_dat && !*_pos){ _dat = (BYTE*)_dat_sve; break; }
					while(*_dat) ++_dat;
					++_dat; --size;
				}
				if(!size) return nullptr;
				_pos = nullptr;
				if(*_dat_sve) _pos = JSN_HANDLE::nav(handle->data, *_dat_sve);
				JSN_HANDLE* _ret = new JSN_HANDLE;
				_ret->base = handle->base;
				_ret->data = _pos;
				_ret->array_handle = handle;
				_ret->index_ptr = _dat;
				return (void*)_ret;
			}
			return nullptr;
		}
		FUNCDECL(void* value_view, void* jsn, void* offset, BYTE* buffer, UWORD* buffer_size){
			JSN_HANDLE_DECL;
			if(buffer_size) *buffer_size = WORD_SIZE + sizeof(BYTE**) + sizeof(BYTE*) + sizeof(JSN_HANDLE*) + sizeof(BYTE*);
			if(JSN_HANDLE_VALID(jsn) || !buffer) return nullptr;
			JSN_HANDLE* handle = (JSN_HANDLE*)jsn;
			if(!(handle->data)) return nullptr;
			BYTE type = *(handle->data);
			if((type & 0x0F) == json_type::bll) return (type & json_type::uns) ? (void*)1 : (void*)0;
			if((type & 0x0F) == json_type::num) return (void*)*(UWORD*)((handle->data) + 1);
			if(type == json_type::flt || type == json_type::str) return (void*)((handle->data) + 1);
			if(type == json_type::ary){
				unsigned short *size = (unsigned short*)((handle->data) + 1);
				UWORD index = (UWORD)offset;
				if(++index > *size || !*size) return nullptr;
				JSN_HANDLE* _ret = (JSN_HANDLE*)buffer;
				_ret->magic = 0x534E534A;
				_ret->base = handle->base;
				_ret->data = nullptr;
				size += index;
				if(*size) _ret->data = JSN_HANDLE::nav(handle->data, *size);
				_ret->array_handle = handle;
				_ret->index_ptr = (BYTE*)size;
				return (void*)_ret;
			}
			if(type == json_type::obj){
				BYTE *_dat = (handle->data) + 1, *_pos = (BYTE*)offset;
				unsigned short size = *(unsigned short*)_dat, *_dat_sve;
				if(!offset || !size) return nullptr;
				_dat += 2;
				while(size){
					_dat_sve = (unsigned short*)_dat; _dat += 2;
					_pos = (BYTE*)offset;
					while(*_dat && *_pos == *_dat){ ++_pos; ++_dat; }
					if(!*_dat && !*_pos){ _dat = (BYTE*)_dat_sve; break; }
					while(*_dat) ++_dat;
					++_dat; --size;
				}
				if(!size) return nullptr;
				_pos = nullptr;
				if(*_dat_sve) _pos = JSN_HANDLE::nav(handle->data, *_dat_sve);
				JSN_HANDLE* _ret = (JSN_HANDLE*)buffer;
				_ret->magic = 0x534E534A;
				_ret->base = handle->base;
				_ret->data = _pos;
				_ret->array_handle = handle;
				_ret->index_ptr = _dat;
				return (void*)_ret;
			}
			return nullptr;
		}
		FUNCDECL(void* set_type, void* jsn, BYTE type){
			if(JSN_HANDLE_VALID(jsn)) return nullptr;
			BYTE old_type = json::type(jsn);
			if(old_type == type) return jsn;
			JSN_HANDLE_DECL;
			struct _ap_{
				static UWORD _str_mem_count(BYTE* data){
					if(((*data) & 0x0F) == json_type::bll) return 4 + (((*data) & json_type::uns) ? 0 : 1);
					if(((*data) & 0x0F) == json_type::num){
						char aux[32];
						return (UWORD)sprintf(aux, ((*data) & json_type::uns) ? "%lu" : "%ld", *(UWORD*)(data + 1));
					}
					if((*data) == json_type::flt){
						char aux[32];
						return (UWORD)sprintf(aux, "%g", *(float*)(data + 1));
					}
					if((*data) == json_type::str){
						++data; UWORD _ret = 0;
						while(*data){ ++_ret; ++data; }
						return _ret;
					}
					if((*data) == json_type::ary){
						BYTE* save_data = data;
						++data; UWORD _ret = 2;
						unsigned short size = *(unsigned short*)data, idx;
						data += 2; while(size){
							idx = *(unsigned short*)data;
							if(idx){
								BYTE* _dat = JSN_HANDLE::nav(save_data, idx);
								if((*_dat) == json_type::str) _ret += 2;
								_ret += _ap_::_str_mem_count(_dat);
							}else _ret += 4;
							data += 2; --size;
							if(size) ++_ret;
						}
						return _ret;
					}
					if((*data) == json_type::obj){
						BYTE* save_data = data;
						++data; UWORD _ret = 2;
						unsigned short size = *(unsigned short*)data, idx;
						data += 2; while(size){
							idx = *(unsigned short*)data;
							if(idx){
								BYTE* _dat = JSN_HANDLE::nav(save_data, idx);
								if((*_dat) == json_type::str) _ret += 2;
								_ret += _ap_::_str_mem_count(_dat);
							}else _ret += 4;
							_ret += 2; data += 2;
							while(*data){ ++_ret; ++data; }
							++data; --size;
							if(size) ++_ret;
						}
						return _ret;
					}
					return 0;
				}
				static void _str_write(BYTE* data, char** txt){
					if(((*data) & 0x0F) == json_type::bll){
						if((*data) & json_type::uns){
							**txt = 't'; ++*txt;
							**txt = 'r'; ++*txt;
							**txt = 'u'; ++*txt;
							**txt = 'e'; ++*txt;
							return;
						}
						**txt = 'f'; ++*txt;
						**txt = 'a'; ++*txt;
						**txt = 'l'; ++*txt;
						**txt = 's'; ++*txt;
						**txt = 'e'; ++*txt;
						return;
					}
					if(((*data) & 0x0F) == json_type::num){
						char aux[32], *paux = aux;
						int _sze = sprintf(aux, ((*data) & json_type::uns) ? "%lu" : "%ld", *(UWORD*)(data + 1));
						while(_sze){ **txt = *paux; ++*txt; --_sze; ++paux; }
						return;
					}
					if((*data) == json_type::flt){
						char aux[32], *paux = aux;
						int _sze = sprintf(aux, "%g", *(float*)(data + 1));
						while(_sze){ **txt = *paux; ++*txt; --_sze; ++paux; }
						return;
					}
					if((*data) == json_type::str){
						++data;
						while(*data){ **txt = *data; ++*txt; ++data; }
						return;
					}
					if((*data) == json_type::ary){
						BYTE* save_data = data;
						++data; **txt = '['; ++*txt;
						unsigned short size = *(unsigned short*)data, idx;
						data += 2; while(size){
							idx = *(unsigned short*)data;
							if(idx) _ap_::_str_write(JSN_HANDLE::nav(save_data, idx), txt);
							data += 2; --size;
							if(size){ **txt = ','; ++*txt; }
						}
						**txt = ']'; ++*txt;
						return;
					}
					if((*data) == json_type::obj){
						BYTE* save_data = data;
						++data; **txt = '\x7B'; ++*txt;
						unsigned short size = *(unsigned short*)data, idx;
						data += 2; while(size){
							idx = *(unsigned short*)data;
							data += 2; **txt = '"'; ++*txt;
							while(*data){ **txt = *data; ++*txt; ++data; }
							++data; --size; **txt = '"'; ++*txt;
							**txt = ':'; ++*txt;
							if(idx) _ap_::_str_write(JSN_HANDLE::nav(save_data, idx), txt);
							if(size){ **txt = ','; ++*txt; }
						}
						**txt = '\x7D'; ++*txt;
						return;
					}
				}
			};
			JSN_HANDLE* handle = (JSN_HANDLE*)jsn;
			if(old_type == json_type::nll){
				if(((handle->base) ? ((*(handle->base)) == nullptr) : true) && (handle->array_handle) && (handle->index_ptr)){
					BYTE* end_pos;
					UWORD memsize = 1 + JSN_DEFAULT_MEMSIZE(type);
					if(handle->base) *(handle->base) = (BYTE*)memory::alloc(memsize);
					else handle->array_handle->base = handle->base = new BYTE*((BYTE*)memory::alloc(memsize));
	    			handle->data = *(handle->base);
	    			memory::slloc(handle->data, 0, memsize);
					*(handle->data) = type;
	    			end_pos = (*(handle->base)) + memory::szlen(*(handle->base));
	    			BYTE* _dat = JSN_HANDLE::nav(handle->array_handle->data, 1);
	    			unsigned short* _idx = (unsigned short*)(handle->index_ptr);
	    			*_idx = 1;
	    			while(_dat != end_pos){
						_dat = JSN_HANDLE::nav(_dat, 1);
						++(*_idx);
					}
				}else{
					UWORD memsize = 1 + JSN_DEFAULT_MEMSIZE(type);
					if((handle->base) ? (!*(handle->base)) : false) *(handle->base) = (BYTE*)memory::alloc(memsize);
					else if(!(handle->base))                        handle->base = new BYTE*((BYTE*)memory::alloc(memsize));
					handle->data = *(handle->base);
					memory::slloc(handle->data, 0, memsize);
					*(handle->data) = type;
					handle->array_handle = nullptr;
					handle->index_ptr = nullptr;
				}
				return jsn;
			}
			char old_subtype = old_type & 0x0F, subtype = type & 0x0F;
			if(old_subtype == json_type::bll && subtype == json_type::num){
				UWORD old_val = (old_type & json_type::uns) ? 1 : 0;
				*(handle->data) = json_type::num | (type & json_type::uns);
				handle->expand((handle->data + 1) - (*(handle->base)), (handle->data + 1 + WORD_SIZE) - (*(handle->base)));
				*(UWORD*)(handle->data + 1) = old_val;
			}
			else if(old_subtype == json_type::bll && type == json_type::flt){
				float old_val = (old_type & json_type::uns) ? 1.0f : 0.0f;
				*(handle->data) = json_type::flt;
				handle->expand((handle->data + 1) - (*(handle->base)), (handle->data + 5) - (*(handle->base)));
				*(float*)(handle->data + 1) = old_val;
			}
			else if(old_subtype == json_type::num && subtype == json_type::bll){
				*(handle->data) = json_type::bll | ((*(UWORD*)((handle->data) + 1)) ? json_type::uns : 0x00);
				handle->expand((handle->data + 1 + WORD_SIZE) - (*(handle->base)), (handle->data + 1) - (*(handle->base)));
			}
			else if(old_type == json_type::flt && subtype == json_type::bll){
				*(handle->data) = json_type::bll | ((*(float*)((handle->data) + 1)) ? json_type::uns : 0x00);
				handle->expand((*(handle->base)) - (handle->data + 5), (*(handle->base)) - (handle->data + 1));
			}
			else if(old_subtype == json_type::num && type == json_type::flt){
				*(handle->data) = json_type::flt;
				if(old_type & json_type::uns) *(float*)((handle->data) + 1) = (float)*(UWORD*)((handle->data) + 1);
				else                          *(float*)((handle->data) + 1) = (float)*(WORD*)((handle->data) + 1);
				#if WORD_SIZE == 8
				handle->expand((handle->data + 9) - (*(handle->base)), (handle->data + 5) - (*(handle->base)));
				#endif
			}
			else if(old_type == json_type::flt && subtype == json_type::num){
				*(handle->data) = json_type::num | (type & json_type::uns);
				#if WORD_SIZE == 8
				handle->expand((handle->data + 5) - (*(handle->base)), (handle->data + 9) - (*(handle->base)));
				#endif
				if(type & json_type::uns)
					*(UWORD*)(handle->data + 1) = (UWORD)*(float*)(handle->data + 1);
				else
					*(WORD*)(handle->data + 1) = (WORD)*(float*)(handle->data + 1);
			}
			else if(type == json_type::str && (old_subtype == json_type::bll || old_subtype == json_type::num || old_type == json_type::flt)){
				*(handle->data) = json_type::str;
				BYTE aux[32] = {0}; int _old_size = 0, _new_size = 0;
				if(old_subtype == json_type::bll){
					_new_size = sprintf((char*)aux, "%s", (old_type & json_type::uns) ? "true" : "false");
					_old_size = 0;
				}
				if(old_subtype == json_type::num){
					_new_size = sprintf((char*)aux, (old_type & json_type::uns) ? "%lu" : "%ld", *(UWORD*)(handle->data + 1));
					_old_size = WORD_SIZE;
				}
				if(old_type == json_type::flt){
					_new_size = sprintf((char*)aux, "%g", *(float*)(handle->data + 1));
					_old_size = 4;
				}
				handle->expand((handle->data + 1 + _old_size) - (*(handle->base)), (handle->data + 2 + _new_size) - (*(handle->base)));
				BYTE *_str = handle->data + 1, *_new_str = aux;
				while(*_new_str) *_str++ = *_new_str++;
				*_str = '\0';
			}
			else if(old_type == json_type::str && (subtype == json_type::bll || subtype == json_type::num || type == json_type::flt)){
				*(handle->data) = (subtype == json_type::num) ? (json_type::num | (type & json_type::uns)) : subtype;
				char aux[32] = {0}; int _old_size = sprintf(aux, "%s", handle->data + 1), _new_size = 0;
				if(subtype == json_type::num) _new_size = WORD_SIZE;
				if(type == json_type::flt) _new_size = 4;
				unsigned int old_val = 0; BYTE _val_type = json_type::bll;
				if((_old_size >= 4) ? ((aux[0] == 't' || aux[0] == 'T') && (aux[1] == 'r' || aux[1] == 'R')
					&& (aux[2] == 'u' || aux[2] == 'U') && (aux[3] == 'e' || aux[3] == 'E')) : false) old_val = 1;
				if(_old_size >= 1){
					if((aux[0] == '-' && _old_size >= 2) || (aux[0] >= '0' && aux[0] <= '9')){
						old_val = (aux[0] == '-') ? 0x80000000 : 0;
						_val_type = json_type::num;
						char* _parse_num = (aux[0] == '-') ? (aux + 1) : aux;
						while(*_parse_num){
							if(*_parse_num == '.' || *_parse_num == 'e' || *_parse_num == 'E'){ _val_type = json_type::flt; break; }
							if(*_parse_num && (*_parse_num < '0' || *_parse_num > '9')){
								_val_type = json_type::bll;
								old_val = 0;
								break;
							}
							++_parse_num;
						}
					}else if(aux[0] == '.') _val_type = json_type::flt;
					else if(_old_size >= 2){
						_val_type = json_type::num;
						char* _parse_num = aux;
						while(*_parse_num){
							if(*_parse_num == '.' || *_parse_num == 'e' || *_parse_num == 'E'){ _val_type = json_type::flt; break; }
							if(*_parse_num && (*_parse_num < '0' || *_parse_num > '9')){ _val_type = json_type::bll; break; }
							++_parse_num;
						}
					}
				}
				char* _parse_num = aux; char pre_sign = (old_val & 0x80000000) ? 1 : 0, sign_seg = 0; short step_down = 0, step_dot = 0;
				if(_val_type == json_type::num && *_parse_num == '-') ++_parse_num;
				old_val = (_val_type == json_type::bll || (pre_sign && _val_type == json_type::num)) ? old_val : 0;
				while(*_parse_num && _val_type == json_type::num)
					old_val = (old_val&0x80000000)|(((old_val&(pre_sign?0x7FFFFFFF:0xFFFFFFFF))*10)+((*((++_parse_num)-1))-'0'));
				while(*_parse_num && _val_type == json_type::flt){
					if(sign_seg == 0){
						++sign_seg;
						if(*_parse_num == '-') ++_parse_num;
						continue;
					}
					if(sign_seg == 1 && *_parse_num == '.'){ ++sign_seg; ++_parse_num; continue; }
					if((sign_seg == 1 || sign_seg == 2) && (*_parse_num == 'e' || *_parse_num == 'E')){ sign_seg = 3; ++_parse_num; continue; }
					if(sign_seg == 3){
						++sign_seg;
						if(*_parse_num == '-'){ step_down = (short)0x8000; ++_parse_num; }
						continue;
					}
					if(sign_seg == 4) step_down = (step_down&0x8000)|(((step_down&0x7FFF)*10)+((*_parse_num)-'0'));
					else{
						float _old_val = 0.0f;
						memory::clloc(&_old_val, &old_val, sizeof(float));
						if(sign_seg == 2) ++step_dot;
						unsigned short _rep = (unsigned short)step_dot + 1;
						float _ret = 1.0f;
						while(--_rep) _ret *= 0.1f;
						_old_val = ((_old_val)*((sign_seg==1)?10.0f:1.0f))
							+(((float)((*_parse_num)-'0'))*((sign_seg==1)?1.0f:_ret));
						memory::clloc(&old_val, &_old_val, sizeof(float));
					}
					++_parse_num;
				}
				if(sign_seg == 4){
					char _neg_step_down = (step_down & 0x8000) ? 1 : 0; unsigned short _rep = (unsigned short)(step_down & 0x7FFF) + 1;
					float _exp_val = 0.0f;
					memory::clloc(&_exp_val, &old_val, sizeof(float));
					while(--_rep) _exp_val *= (_neg_step_down ? 0.1f : 10.0f);
					memory::clloc(&old_val, &_exp_val, sizeof(float));
				}
				if(pre_sign && _val_type == json_type::flt){
					float _neg_val = 0.0f;
					memory::clloc(&_neg_val, &old_val, sizeof(float));
					_neg_val = -_neg_val;
					memory::clloc(&old_val, &_neg_val, sizeof(float));
				}
				handle->expand((handle->data + 2 + _old_size) - (*(handle->base)), (handle->data + 1 + _new_size) - (*(handle->base)));
				if(subtype == json_type::bll) *(handle->data) = json_type::bll | (old_val ? json_type::uns : 0);
				if(subtype == json_type::num){
					if(_val_type == json_type::flt){
						float _as_float = 0.0f;
						memory::clloc(&_as_float, &old_val, sizeof(float));
						*(UWORD*)(handle->data + 1) = (UWORD)(WORD)_as_float;
					}
					else{
						WORD _num = (WORD)(old_val & 0x7FFFFFFF);
						if(old_val & 0x80000000) _num = -_num;
						*(UWORD*)(handle->data + 1) = (UWORD)_num;
					}
				}
				if(type == json_type::flt){
					float _stored = 0.0f;
					if(_val_type == json_type::flt) memory::clloc(&_stored, &old_val, sizeof(float));
					else{
						WORD _as_int = (WORD)(old_val & 0x7FFFFFFF);
						if(old_val & 0x80000000) _as_int = -_as_int;
						_stored = (float)_as_int;
					}
					*(float*)(handle->data + 1) = _stored;
				}
			}
			else if(old_type == json_type::str && type == json_type::ary){
				*(handle->data) = json_type::ary;
				BYTE* _dat = (handle->data) + 1;
				UWORD _sze = 0, _k_size = 0;
				while(_dat[_k_size]) ++_k_size;
				while(_k_size){
					BYTE* _chr = handle->new_elem(json_type::str, 1);
					_chr[1] = ((handle->data) + 1)[_sze];
					--_k_size; ++_sze;
				}
				handle->expand((((handle->data) + 2 + _sze) - (*(handle->base))), (((handle->data) + 3 + (_sze << 1)) - (*(handle->base))));
				unsigned short* _idx = (unsigned short*)((handle->data) + 1);
				*_idx = (unsigned short)_sze; ++_idx;
				if(_sze){
					*_idx = 1;
					_dat = JSN_HANDLE::nav(handle->data, 1);
					BYTE* _dat_end = (*(handle->base)) + memory::szlen(*(handle->base)) - (_sze * 3);
					while(_dat != _dat_end){
						++(*_idx);
						_dat = JSN_HANDLE::nav(_dat, 1);
					}
					++_idx;
					while(--_sze){ *_idx = (*(_idx - 1)) + 1; ++_idx; }
				}
			}
			else if(old_type == json_type::ary && type == json_type::str){
				BYTE* _dat = (handle->data) + 1;
				unsigned short _sze = *(unsigned short*)_dat; _dat += 2;
				UWORD total_new_mem_size = 1;
				while(_sze){
					if(*(unsigned short*)_dat)
					     total_new_mem_size += _ap_::_str_mem_count(JSN_HANDLE::nav(handle->data, *(unsigned short*)_dat));
					else total_new_mem_size += 4;
					_dat += 2; --_sze;
				}
				char *_txt = new char[total_new_mem_size], *ptxt = _txt;
				_sze = *(unsigned short*)((handle->data) + 1);
				while(_sze){
					_dat = (handle->data) + 3;
					if(*(unsigned short*)_dat)
						_ap_::_str_write(JSN_HANDLE::nav(handle->data, *(unsigned short*)_dat), &ptxt);
					json::del_elem(jsn, (void*)0); --_sze;
				}
				*ptxt = '\0';
				*(handle->data) = json_type::str;
				handle->expand(((handle->data) + 3) - (*(handle->base)), ((handle->data) + 1 + total_new_mem_size) - (*(handle->base)));
				_dat = (handle->data) + 1; ptxt = _txt;
				while(*ptxt){ *_dat = *ptxt; ++ptxt; ++_dat; }
				*_dat = '\0';
				delete[] _txt;
			}
			else if(type == json_type::nll){
				if((handle->array_handle) && (handle->index_ptr)){
					*(unsigned short*)(handle->index_ptr) = 0;
					handle->expand(((handle->data) - (*(handle->base))) + ((JSN_HANDLE::nav(handle->data, 1)) - (handle->data)),
					               ((handle->data) - (*(handle->base))) + 1);
					*(handle->data) = json_type::bll;
					handle->data = nullptr;
				}else{
					if(*(handle->base)) memory::free(*(handle->base));
					delete (handle->base);
					handle->base = nullptr;
					handle->data = nullptr;
					handle->array_handle = nullptr;
					handle->index_ptr = nullptr;
				}
			}
			else{
				json::cls_elms(jsn);
				UWORD memsize = JSN_DEFAULT_MEMSIZE(old_type), new_memsize = JSN_DEFAULT_MEMSIZE(type);
				if(memsize != new_memsize) handle->expand(
					((handle->data) + 1 + memsize) - (*(handle->base)),
					((handle->data) + 1 + new_memsize) - (*(handle->base))
				);
				*(handle->data) = type;
				memory::slloc((handle->data) + 1, 0, new_memsize);
			}
			return jsn;
		}
		FUNCDECL(void* set_value, void* jsn, void* value){
			if(JSN_HANDLE_VALID(jsn)) return nullptr;
			BYTE type = json::type(jsn);
			if(type == json_type::nll) return jsn;
			JSN_HANDLE_DECL;
			JSN_HANDLE* handle = (JSN_HANDLE*)jsn;
			BYTE* val = (handle->data) + 1;
			if((type & 0x0F) == json_type::bll) *(handle->data) = (json_type::bll) | (((UWORD)value) ? json_type::uns : 0);
			if((type & 0x0F) == json_type::num) *((UWORD*)val) = (UWORD)value;
			if(type == json_type::flt){
				float _flt = 0.0f;
				memory::clloc(&_flt, &value, sizeof(float));
				*(float*)val = _flt;
			}
			if(type == json_type::str){
				if(!value) return nullptr;
				BYTE* _val = (BYTE*)value;
				UWORD _val_size = 0, _str_size = 0;
				while(_val[_val_size]) ++_val_size;
				++_val_size;
				while( val[_str_size]) ++_str_size;
				++_str_size;
				handle->expand(val - (*(handle->base)) + _str_size, val - (*(handle->base)) + _val_size);
				memory::mlloc((handle->data) + 1, value, _val_size);
			}
			return jsn;
		}
		FUNCDECL(void* add_elem, void* jsn, void* offset, BYTE type){
			if(JSN_HANDLE_VALID(jsn)) return nullptr;
			BYTE _dat_type = json::type(jsn);
			JSN_HANDLE_DECL;
			JSN_HANDLE* handle = (JSN_HANDLE*)jsn;
			if(_dat_type == json_type::ary){
				UWORD _pos = (UWORD)offset;
				unsigned short* size = (unsigned short*)((handle->data) + 1);
				if(++_pos > ++(*size)) _pos = *size;
				if(type != json_type::nll) handle->new_elem(type, 0);
				handle->expand(((handle->data) + 1 + (_pos << 1)) - (*(handle->base)),
				               ((handle->data) + 3 + (_pos << 1)) - (*(handle->base)));
				size = (unsigned short*)((handle->data) + 1) + _pos;
				if(type == json_type::nll) *size = 0;
				else{
					BYTE *_dat = JSN_HANDLE::nav(handle->data, 1),
					     *_dat_end = (*(handle->base)) + memory::szlen(*(handle->base)) - JSN_DEFAULT_MEMSIZE(type) - 1;
					*size = 1;
					while(_dat != _dat_end){
						++(*size);
						_dat = JSN_HANDLE::nav(_dat, 1);
					}
				}
			}
			if(_dat_type == json_type::obj){
				if(!offset) return nullptr;
				BYTE *_pos = (BYTE*)offset, *mem_offset = (handle->data) + 3;
				unsigned short *size = (unsigned short*)((handle->data) + 1), _k_size = *size;
				UWORD _sze = 0; while(_pos[_sze]) ++_sze; ++_sze;
				while(_k_size){
					--_k_size; mem_offset += 2;
					_pos = (BYTE*)offset;
					while(*mem_offset && *mem_offset == *_pos){ ++mem_offset; ++_pos; }
					if(!*mem_offset && !*_pos) return nullptr; // The object already exists
					while(*mem_offset) ++mem_offset;
					++mem_offset;
				}
				// The object doesnt exist yet
				UWORD _mem_offset_off = mem_offset - (*(handle->base));
				if(type != json_type::nll) handle->new_elem(type, 0);
				handle->expand(_mem_offset_off, _mem_offset_off + 2 + _sze);
				mem_offset = (*(handle->base)) + _mem_offset_off;
				size = (unsigned short*)((handle->data) + 1); ++(*size);
				size = (unsigned short*)mem_offset;
				_pos = (BYTE*)offset; mem_offset += 2;
				while(*_pos) *mem_offset++ = *_pos++;
				*mem_offset++ = '\0';
				if(type == json_type::nll) *size = 0;
				else{
					mem_offset = JSN_HANDLE::nav(handle->data, 1);
					BYTE *_dat_end = (*(handle->base)) + memory::szlen(*(handle->base)) - 1 - JSN_DEFAULT_MEMSIZE(type);
					*size = 1;
					while(mem_offset != _dat_end){
						++(*size);
						mem_offset = JSN_HANDLE::nav(mem_offset, 1);
					}
				}
			}
			return jsn;
		}
		FUNCDECL(void* del_elem, void* jsn, void* offset){
			if(JSN_HANDLE_VALID(jsn)) return nullptr;
			BYTE _dat_type = json::type(jsn);
			JSN_HANDLE_DECL;
			JSN_HANDLE* handle = (JSN_HANDLE*)jsn;
			if(_dat_type == json_type::ary){
				UWORD _pos = (UWORD)offset;
				unsigned short* size = (unsigned short*)((handle->data) + 1);
				if(++_pos > *size || !*size) return nullptr;
				unsigned short _dir = *(size + _pos); --(*size);
				handle->expand(((BYTE*)(size + _pos + 1)) - (*(handle->base)), ((BYTE*)(size + _pos)) - (*(handle->base)));
				if(_dir){
					BYTE *_dat = JSN_HANDLE::nav(handle->data, _dir),
					     *_dat_end = JSN_HANDLE::nav(_dat, 1);
					*_dat = json_type::bll;
					handle->expand(_dat_end - (*(handle->base)), (_dat + 1) - (*(handle->base)));
				}
			}
			if(_dat_type == json_type::obj){
				if(!offset) return nullptr;
				BYTE *_pos = (BYTE*)offset, *mem_offset = (handle->data) + 3;
				unsigned short size = *(unsigned short*)((handle->data) + 1);
				UWORD _pos_size = 0; while(_pos[_pos_size]) ++_pos_size; ++_pos_size;
				while(size){
					mem_offset += 2; _pos = (BYTE*)offset;
					while(*mem_offset && *mem_offset == *_pos){ ++mem_offset; ++_pos; }
					if(!*mem_offset && !*_pos){
						mem_offset -= _pos_size + 1;
						break;
					}
					while(*mem_offset) ++mem_offset;
					++mem_offset; --size;
				}
				if(!size) return nullptr;
				unsigned short _dir = *(unsigned short*)mem_offset;
				--(*(unsigned short*)((handle->data) + 1));
				handle->expand((mem_offset + 2 + _pos_size) - (*(handle->base)), mem_offset - (*(handle->base)));
				if(_dir){
					BYTE *_dat = JSN_HANDLE::nav(handle->data, _dir),
					     *_dat_end = JSN_HANDLE::nav(_dat, 1);
					*_dat = json_type::bll;
					handle->expand(_dat_end - (*(handle->base)), (_dat + 1) - (*(handle->base)));
				}
			}
			return jsn;
		}
		FUNCDECL(void* swp_elem, void* jsn, void* offset_a, void* offset_b){
			if(JSN_HANDLE_VALID(jsn)) return nullptr;
			BYTE _dat_type = json::type(jsn);
			JSN_HANDLE_DECL;
			JSN_HANDLE* handle = (JSN_HANDLE*)jsn;
			if(_dat_type == json_type::ary){
				UWORD _1_pos = (UWORD)offset_a, _2_pos = (UWORD)offset_b;
				if(_1_pos == _2_pos) return jsn;
				unsigned short* size = (unsigned short*)((handle->data) + 1);
				if(++_1_pos > *size || ++_2_pos > *size || !*size) return nullptr;
				unsigned short *_1_idx = size + _1_pos, *_2_idx = size + _2_pos,
				               _k = *_1_idx;
				*_1_idx = *_2_idx; *_2_idx = _k;
			}
			if(_dat_type == json_type::obj){
				if(!offset_a || !offset_b) return nullptr;
				BYTE *_1_pos = (BYTE*)offset_a, *_2_pos = (BYTE*)offset_b, *mem_offset = (handle->data) + 1;
				unsigned short size = *(unsigned short*)mem_offset, *_1_sve = nullptr, *_2_sve = nullptr, *_3_sve;
				while(*_1_pos && *_1_pos == *_2_pos){ ++_1_pos; ++_2_pos; }
				if(!*_1_pos && !*_2_pos) return jsn;
				mem_offset += 2;
				while(size){
					_3_sve = (unsigned short*)mem_offset;
					mem_offset += 2;
					if(!_1_sve){
						_1_pos = (BYTE*)offset_a;
						while(*mem_offset && *mem_offset == *_1_pos){ ++mem_offset; ++_1_pos; }
						if(!*mem_offset && !*_1_pos) _1_sve = _3_sve;
					}
					if(!_2_sve){
						mem_offset = (BYTE*)(_3_sve + 1);
						_2_pos = (BYTE*)offset_b;
						while(*mem_offset && *mem_offset == *_2_pos){ ++mem_offset; ++_2_pos; }
						if(!*mem_offset && !*_2_pos) _2_sve = _3_sve;
					}
					if(_1_sve && _2_sve) break;
					while(*mem_offset) ++mem_offset;
					++mem_offset; --size;
				}
				if(!_1_sve || !_2_sve) return nullptr;
				unsigned short _k = *_1_sve;
				*_1_sve = *_2_sve; *_2_sve = _k;
			}
			return jsn;
		}
		FUNCDECL(void* mve_elem, void* jsn, void* offset, void* new_offset){
			if(JSN_HANDLE_VALID(jsn)) return nullptr;
			BYTE _dat_type = json::type(jsn);
			JSN_HANDLE_DECL;
			JSN_HANDLE* handle = (JSN_HANDLE*)jsn;
			if(_dat_type == json_type::ary){
				UWORD _pos = (UWORD)offset, _new_pos = (UWORD)new_offset;
				unsigned short* size = (unsigned short*)((handle->data) + 1);
				if(_pos == _new_pos) return jsn;
				if(_pos > _new_pos){ UWORD _k = _pos; _pos = _new_pos; _new_pos = _k; }
				if(++_pos > *size || ++_new_pos > *size || !*size) return nullptr;
				unsigned short *_idx = size + _pos, *_new_idx = size + _new_pos, _k = *_idx;
				while(_idx != _new_idx){
					*_idx = *(_idx + 1);
					++_idx;
				}
				*_idx = _k;
			}
			if(_dat_type == json_type::obj){
				if(!offset || !new_offset) return nullptr;
				BYTE *_1_pos = (BYTE*)offset, *_2_pos = (BYTE*)new_offset, *mem_offset = (handle->data) + 1;
				unsigned short size = *(unsigned short*)mem_offset, *_1_sve = nullptr, *_2_sve = nullptr, *_3_sve;
				while(*_1_pos && *_1_pos == *_2_pos){ ++_1_pos; ++_2_pos; }
				if(!*_1_pos && !*_2_pos) return jsn;
				mem_offset += 2;
				while(size){
					_3_sve = (unsigned short*)mem_offset;
					mem_offset += 2;
					if(!_1_sve){
						_1_pos = (BYTE*)offset;
						while(*mem_offset && *mem_offset == *_1_pos){ ++mem_offset; ++_1_pos; }
						if(!*mem_offset && !*_1_pos) _1_sve = _3_sve;
					}
					if(!_2_sve){
						mem_offset = (BYTE*)(_3_sve + 1);
						_2_pos = (BYTE*)new_offset;
						while(*mem_offset && *mem_offset == *_2_pos){ ++mem_offset; ++_2_pos; }
						if(!*mem_offset && !*_2_pos) _2_sve = _3_sve;
					}
					if(_1_sve && _2_sve) break;
					--size;
					while(*mem_offset) ++mem_offset;
					++mem_offset;
				}
				if(!_1_sve || _2_sve) return nullptr;
				_1_pos = (BYTE*)(_1_sve + 1);
				_2_pos = (BYTE*)new_offset;
				UWORD _1_size = 0; while(_1_pos[_1_size]) ++_1_size; ++_1_size;
				UWORD _2_size = 0; while(_2_pos[_2_size]) ++_2_size; ++_2_size;
				if(_1_size != _2_size){
					BYTE* old_base = *(handle->base);
					handle->expand((_1_pos + _1_size) - (*(handle->base)), (_1_pos + _2_size) - (*(handle->base)));
					WORD ptr_offset = (*(handle->base)) - old_base;
					_1_pos += ptr_offset;
				}
				while(*_2_pos) *_1_pos++ = *_2_pos++;
				*_1_pos = '\0';
			}
			return jsn;
		}
		FUNCDECL(UWORD fnd_elem, void* jsn, void* offset){
			if(JSN_HANDLE_VALID(jsn)) return 0;
			BYTE _dat_type = json::type(jsn);
			JSN_HANDLE_DECL;
			JSN_HANDLE* handle = (JSN_HANDLE*)jsn;
			if(_dat_type == json_type::ary){
				UWORD _pos = (UWORD)offset;
				unsigned short* size = (unsigned short*)((handle->data) + 1);
				if(++_pos > *size || !*size) return 0;
				return _pos;
			}
			if(_dat_type == json_type::obj){
				if(!offset) return 0;
				BYTE *_pos = (BYTE*)offset, *mem_offset = (handle->data) + 3;
				unsigned short size = *(unsigned short*)((handle->data) + 1);
				UWORD _pos_size = 0, _idx = 1; while(_pos[_pos_size]) ++_pos_size; ++_pos_size;
				while(size){
					mem_offset += 2; _pos = (BYTE*)offset;
					while(*mem_offset && *mem_offset == *_pos){ ++mem_offset; ++_pos; }
					if(!*mem_offset && !*_pos) return _idx;
					while(*mem_offset) ++mem_offset;
					++mem_offset; --size; ++_idx;
				}
			}
			return 0;
		}
		FUNCDECL(const char* nme_elem, void* jsn, UWORD offset){
			if(json::type(jsn) != json_type::obj) return nullptr;
			JSN_HANDLE_DECL;
			JSN_HANDLE* handle = (JSN_HANDLE*)jsn;
			BYTE* mem_offset = (handle->data) + 1;
			unsigned short size = *(unsigned short*)mem_offset; mem_offset += 2;
			if(++offset > size || !size) return nullptr;
			while(offset){
				mem_offset += 2;
				if(!--offset) return (const char*)mem_offset;
				while(*mem_offset) ++mem_offset;
				++mem_offset;
			}
			return nullptr;
		}
		FUNCDECL(void* cls_elms, void* jsn){
			if(JSN_HANDLE_VALID(jsn)) return nullptr;
			BYTE type = json::type(jsn);
			if(type == json_type::nll) return jsn;
			JSN_HANDLE_DECL;
			JSN_HANDLE* handle = (JSN_HANDLE*)jsn;
			if((type & 0x0F) == json_type::bll) *(handle->data) = json_type::bll;
			if((type & 0x0F) == json_type::num) *((UWORD*)((handle->data) + 1)) = 0;
			if(type == json_type::flt) *((float*)((handle->data) + 1)) = 0.0f;
			if(type == json_type::str){
				BYTE* _str = (handle->data) + 1;
				UWORD _str_size = 0; while(_str[_str_size]) ++_str_size;
				if(!_str_size) return jsn;
				handle->expand((_str + _str_size) - (*(handle->base)), _str - (*(handle->base)));
			}
			if(type == json_type::ary || type == json_type::obj){
				BYTE *mve_data = handle->data, *data = mve_data + 1, *idx_data, *idx_end;
				unsigned short size = *(unsigned short*)data, idx; data += 2;
				WORD offset_data;
				while(size){
					idx = *(unsigned short*)data; data += 2;
					if(type == json_type::obj){ while(*data) ++data; ++data; }
					if(idx){
						idx_data = JSN_HANDLE::nav(mve_data, idx);
						idx_end = JSN_HANDLE::nav(idx_data, 1);
						if((idx_end - idx_data) > 1){
							handle->expand(idx_end - *(handle->base), (idx_data + 1) - *(handle->base));
							offset_data = (handle->data) - mve_data; mve_data = handle->data;
							if(offset_data){
								data += offset_data;
								idx_data += offset_data;
							}
						}
						*idx_data = json_type::bll;
					}
					--size;
				}
				handle->expand(data - *(handle->base), ((handle->data) + 3) - *(handle->base));
				*((unsigned short*)((handle->data) + 1)) = 0;
			}
			return jsn;
		}
	}
	#endif
	#if defined(ST_IMPORT_SOUND)
	#undef ST_IMPORT_SOUND
    #define ST_IMPORT_SOUND 3
	namespace sound{
		FUNCDECL(void* create, unsigned int buffer_size){
			#define SND_HANDLE_VALID(ptr) ((ptr)?((*(UWORD*)(ptr))!=0x534E444F):true)
			#if defined(_WIN32)
			#define SND_HANDLE_DECL struct WAVEHDR{\
			    BYTE* lpData; unsigned int dwBufferLength; unsigned int dwBytesRecorded;\
			    void* dwUser; unsigned int dwFlags; unsigned int dwLoops;\
			    void* lpNext; void* reserved;\
			};struct SND_HANDLE{\
			    UWORD magic;\
			    struct Buffer{ WAVEHDR meta; BYTE* data; bool busy; } buffers[2];\
			    void* os_handle;\
			    float* master_volume;\
			    bool owns_master;\
			    bool wants_playback;\
			    unsigned int sample_rate;\
			    int current_buffer;\
			    float volume;\
			    float pitch;\
			    BYTE channels;\
			    BYTE bits;\
			    bool playing;\
			    SND_HANDLE() : magic(0x534E444F), os_handle(nullptr),\
			                   master_volume(nullptr), owns_master(false), wants_playback(false),\
			                   sample_rate(44100), current_buffer(0), volume(1.0f), pitch(1.0f),\
			                   channels(2), bits(16), playing(false) {\
			        buffers[0] = {nullptr, false };\
			        buffers[1] = {nullptr, false };\
			    }\
			    ~SND_HANDLE(){\
			        magic = 0x00;\
			        memory::free(buffers[0].data);\
			        memory::free(buffers[1].data);\
			        if(master_volume && owns_master) delete master_volume;\
			    }\
			}
			SND_HANDLE_DECL;
		    SND_HANDLE* h = new SND_HANDLE();
		    h->buffers[0].data = (BYTE*)memory::alloc(buffer_size);
			h->buffers[0].meta.lpData = h->buffers[0].data;
			h->buffers[0].meta.dwBufferLength = buffer_size;
			h->buffers[0].meta.dwFlags = 0;
		    h->buffers[1].data = (BYTE*)memory::alloc(buffer_size);
			h->buffers[1].meta.lpData = h->buffers[1].data;
			h->buffers[1].meta.dwBufferLength = buffer_size;
			h->buffers[1].meta.dwFlags = 0;
		    return (void*)h;
			#else // Linux
			#define SND_HANDLE_DECL struct SND_HANDLE{\
		        UWORD magic;\
		        struct Buffer{ BYTE* data; bool busy; } buffers[2];\
		        void* os_handle;\
		        float* master_volume;\
		        bool owns_master;\
		        bool wants_playback;\
		        unsigned int sample_rate;\
		        int current_buffer;\
		        float volume;\
		        float pitch;\
		        BYTE channels;\
		        BYTE bits;\
		        bool playing;\
		        SND_HANDLE() : magic(0x534E444F), os_handle(nullptr),\
		                       master_volume(nullptr), owns_master(false), wants_playback(false),\
		                       sample_rate(44100), current_buffer(0), volume(1.0f), pitch(1.0f),\
		                       channels(2), bits(16), playing(false) {\
		            buffers[0] = {nullptr, false};\
		            buffers[1] = {nullptr, false};\
		        }\
		        ~SND_HANDLE(){\
		            magic = 0x00;\
		            memory::free(buffers[0].data);\
		            memory::free(buffers[1].data);\
			        if(master_volume && owns_master) delete master_volume;\
		        }\
		    };
		    SND_HANDLE_DECL;
		    SND_HANDLE* h = new SND_HANDLE();
		    h->buffers[0].data = (BYTE*)memory::alloc(buffer_size);
		    h->buffers[1].data = (BYTE*)memory::alloc(buffer_size);
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
		FUNCDECL(void* create, unsigned int buffer_size, float* mst_vol){
			SND_HANDLE_DECL;
			SND_HANDLE* ret = (SND_HANDLE*)sound::create(buffer_size);
			if(mst_vol) ret->master_volume = mst_vol;
			return (void*)ret;
		}
		FUNCDECL(UWORD stream, void* snd, void* external_data, UWORD size){
		    if(SND_HANDLE_VALID(snd) || !external_data) return 0;
			SND_HANDLE_DECL;
		    SND_HANDLE* h = (SND_HANDLE*)snd;
		    float vol = (h->master_volume) ? ((h->volume) * (*(h->master_volume))) : (h->volume);
		    int target = -1;
		    for(int i = 0; i < 2 && target == -1; ++i){
		    	if(!(h->buffers[i].busy)) target = i;
		    }
		    #if defined(_WIN32)
		    typedef int(WINCALL *WPH)(void*,void*,unsigned int);
		    static WPH pPrepare = (WPH)get_system_function("waveOutPrepareHeader", "winmm.dll");
		    typedef int(WINCALL *WOW)(void*,void*,unsigned int);
		    static WOW pWrite = (WOW)get_system_function("waveOutWrite", "winmm.dll");
		    typedef int(WINCALL *WUH)(void*,void*,unsigned int);
		    static WUH pUnprepare = (WUH)get_system_function("waveOutUnprepareHeader", "winmm.dll");
		    typedef int(WINCALL *WOR)(void*);
		    static WOR pWaveOutReset = (WOR)get_system_function("waveOutReset", "winmm.dll");
		    if(!pPrepare || !pWrite) return 0;
		    // Fallback for drivers that neither fail a call nor complete the queue:
		    // with every turn still claimed, none completed, and no device left on
		    // the machine, the output is gone and no DONE will ever arrive.
		    if(target == -1 && (h->playing) && (h->os_handle)){
		    	typedef int(WINCALL *WND)(void);
		    	static WND pNumDevs = (WND)get_system_function("waveOutGetNumDevs", "winmm.dll");
		    	if(pNumDevs && !pNumDevs()){
		    		if(pWaveOutReset) pWaveOutReset(h->os_handle);
		    		if(pUnprepare) for(int i = 0; i < 2; ++i){
		    			if(h->buffers[i].meta.dwFlags & 0x00000002){ // WHDR_PREPARED
		    				pUnprepare(h->os_handle, &(h->buffers[i].meta), sizeof(WAVEHDR));
		    				h->buffers[i].meta.dwFlags &= ~0x00000002;
		    			}
		    			h->buffers[i].meta.dwFlags &= ~0x00000001; // WHDR_DONE
		    		}
		    		typedef int(WINCALL *WOC)(void*);
		    		static WOC pWaveOutClose = (WOC)get_system_function("waveOutClose", "winmm.dll");
		    		if(pWaveOutClose) pWaveOutClose(h->os_handle);
		    		h->os_handle = nullptr;
		    		h->playing = false;
		    		h->buffers[0].busy = false;
		    		h->buffers[1].busy = false;
		    		return 0; // the next delivery reconnects once an output is back
		    	}
		    }
		    #else // Linux
		    typedef long(*SPW)(void*,const void*,unsigned long);
		    static SPW pPcmWrite = (SPW)get_system_function("snd_pcm_writei", "libasound.so.2");
		    typedef int(*SPR)(void*);
		    static SPR pPcmPrepare = (SPR)get_system_function("snd_pcm_prepare", "libasound.so.2");
		    typedef int(*SPC)(void*);
		    static SPC pPcmClose = (SPC)get_system_function("snd_pcm_close", "libasound.so.2");
		    static SPC pPcmDrop  = (SPC)get_system_function("snd_pcm_drop", "libasound.so.2");
		    typedef int (*SPO)(void**,const char*,int,int);
		    static SPO pPcmOpen = (SPO)get_system_function("snd_pcm_open", "libasound.so.2");
		    typedef int (*SPS)(void*,int,int,int,unsigned int,int,unsigned int);
		    static SPS pPcmSetParams = (SPS)get_system_function("snd_pcm_set_params", "libasound.so.2");
		    if(!pPcmWrite || !pPcmPrepare || !pPcmClose || !pPcmDrop || !pPcmOpen || !pPcmSetParams) return 0;
		    #endif
		    if(target == -1) return 0;
		    if(!(h->os_handle) && !(h->wants_playback)) return 0; // silent by order: reconnection is not wanted
		    #if !defined(_WIN32)
						if(((SND_HANDLE*)snd)->playing){
							// The file thread owns the rhythm on Linux: this sleep is what reserves
							// the next delivery slot, and with no output it must be paid even when no
							// turn will complete. Without it the thread would hammer a closed device.
							sleep(10);
						}
		    #endif
		    UWORD to_copy = memory::szlen(h->buffers[0].data); if(to_copy > size) to_copy = size;
		    if(!to_copy) return 0;
		    BYTE* dst = h->buffers[target].data;
		    UWORD _jump = (h->bits) >> 3;
		    if(!_jump) return 0;
		    float _volt_;
		    for(UWORD i=0; i < to_copy; i += _jump){
		    	if(_jump == 1){
		    		// 8-bit PCM is unsigned on both ends (silence at 128), the volume math is done around the signed center.
		    		_volt_ = ((float)(*(BYTE*)(((BYTE*)external_data) + i)) - 128.0f) * vol + 128.0f;
					*(dst + i) = (BYTE)((_volt_ > 255.0f) ? 255 : ((_volt_ < 0.0f) ? 0 : (UWORD)_volt_));
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
		    if(!(h->os_handle)){
		    	// The device was taken away while the program still wants the sound:
		    	// reopen the mapper here so the queued audio continues on the new output.
		    	struct _wfx_{
		    		unsigned short format; unsigned short channels; unsigned int samplerate;
		    		unsigned int bytespersec; unsigned short align; unsigned short bits; unsigned short size;
		    	};
		    	unsigned int effective_rate = (unsigned int)((float)h->sample_rate * h->pitch);
		    	unsigned short block_align = (unsigned short)(h->channels * (h->bits >> 3));
		    	_wfx_ wfx = {1, h->channels, effective_rate, effective_rate * block_align, block_align, h->bits, 0};
		    	typedef int(WINCALL *WOO)(void**,unsigned int,void*,void*,void*,unsigned int);
		    	static WOO pWaveOutOpen = (WOO)get_system_function("waveOutOpen", "winmm.dll");
		    	if(!pWaveOutOpen || pWaveOutOpen(&(h->os_handle), 0xFFFFFFFF, &wfx, nullptr, nullptr, 0)) return 0;
		    	(h->playing) = true;
		    }
		    WAVEHDR* hdr = &(h->buffers[target].meta);
		    int sent = -1;
		    if(!(pPrepare(h->os_handle, hdr, sizeof(WAVEHDR)))){
		    	sent = pWrite(h->os_handle, hdr, sizeof(WAVEHDR));
		    	if(sent) // the driver refuses the header: the handle outlived its device
		    		pUnprepare(h->os_handle, hdr, sizeof(WAVEHDR));
		    }
		    if(sent){
		    	// An orphaned output kills every queued turn silently: reclaim them all and
		    	// close, so the next delivery reconnects the mapper onto a living device.
		    	typedef int(WINCALL *WOR)(void*);
		    	static WOR pWaveOutReset = (WOR)get_system_function("waveOutReset", "winmm.dll");
		    	if(pWaveOutReset) pWaveOutReset(h->os_handle);
		    	for(int i = 0; i < 2; ++i) h->buffers[i].meta.dwFlags &= ~0x00000001; // WHDR_DONE
		    	typedef int(WINCALL *WOC)(void*);
		    	static WOC pWaveOutClose = (WOC)get_system_function("waveOutClose", "winmm.dll");
		    	if(pWaveOutClose) pWaveOutClose(h->os_handle);
		    	h->os_handle = nullptr;
		    	h->playing = false;
		    	h->buffers[0].busy = false;
		    	h->buffers[1].busy = false;
		    	return 0; // the intent stands: a later delivery reconnects when an output is back
		    }
		    h->current_buffer = target;
		    h->buffers[target].busy = true;
			#else // Linux
			unsigned long frames = to_copy / (h->channels * _jump);
			if(!(h->os_handle)){
				// No output at the moment of the delivery: reconnect the default device
				// here, so a speaker plugged in after the playback began still gets the audio.
				if(pPcmOpen(&(h->os_handle), "default", 0, 0) < 0){ h->os_handle = nullptr; (h->playing) = false; return 0; }
				int alsa_fmt = (h->bits == 8) ? 1 : (h->bits == 16) ? 2 : (h->bits == 32) ? 6 : 2;
				unsigned int effective_rate = (unsigned int)((float)h->sample_rate * h->pitch);
				if(pPcmSetParams(h->os_handle, alsa_fmt, 3, h->channels, effective_rate, 1, 150000) < 0){
					pPcmClose(h->os_handle); h->os_handle = nullptr;
					(h->playing) = false;
					return 0;
				}
				(h->playing) = true;
			}
			long written = pPcmWrite(h->os_handle, dst, frames);
			if(written == -11){ // EAGAIN: the device is present but full, the turn stays reserved for the retry
				h->current_buffer = target;
				h->buffers[target].busy = true;
				return to_copy;
			}
			if(written < 0){
				// Any other failure reads as a vanished device: drop it and leave the
				// handle wanting playback, so a later delivery reconnects the new output.
				pPcmDrop(h->os_handle); pPcmClose(h->os_handle);
				h->os_handle = nullptr;
				(h->playing) = false;
				h->buffers[target].busy = false;
				return 0;
			}
			h->current_buffer = target;
			h->buffers[target].busy = true;
			#endif
		    return to_copy;
		}
		FUNCDECL(bool play, void* snd){
			TSN_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)) return sound::play(((TSN_HANDLE*)snd)->snd);
			if(SND_HANDLE_VALID(snd)) return false;
			SND_HANDLE_DECL;
		    SND_HANDLE* h = (SND_HANDLE*)snd;
		    if(h->playing) return false;
		    h->wants_playback = true;
		    #if defined(_WIN32)
		    struct _wfx_{
		        unsigned short format; unsigned short channels; unsigned int samplerate;
		        unsigned int bytespersec; unsigned short align; unsigned short bits; unsigned short size;
		    };
		    unsigned int effective_rate = (unsigned int)((float)h->sample_rate * h->pitch);
		    unsigned short block_align = (unsigned short)(h->channels * (h->bits >> 3));
		    _wfx_ wfx = {1, h->channels, effective_rate, effective_rate * block_align, block_align, h->bits, 0};
		    typedef int(WINCALL *WOO)(void**,unsigned int,void*,void*,void*,unsigned int);
		    typedef int(WINCALL *WND)(void);
		    static WOO pWaveOutOpen = (WOO)get_system_function("waveOutOpen", "winmm.dll");
		    static WND pNumDevs = (WND)get_system_function("waveOutGetNumDevs", "winmm.dll");
		    if(!pWaveOutOpen) return false;
		    if(pNumDevs && !pNumDevs()) return true; // no output yet: the deliveries will connect one when it appears
		    if(!pWaveOutOpen(&(h->os_handle), 0xFFFFFFFF, &wfx, nullptr, nullptr, 0)) return ((h->playing) = true);
		    return true;
		    #else // Linux
		    typedef int (*SPO)(void**,const char*,int,int);
		    typedef int (*SPS)(void*,int,int,int,unsigned int,int,unsigned int);
		    static SPO pPcmOpen = (SPO)get_system_function("snd_pcm_open", "libasound.so.2");
		    static SPS pPcmSetParams = (SPS)get_system_function("snd_pcm_set_params", "libasound.so.2");
		    if(!pPcmOpen || !pPcmSetParams) return false;
		    if(pPcmOpen(&(h->os_handle), "default", 0, 0) < 0){ h->os_handle = nullptr; return true; } // reconnected by the deliveries
		    int alsa_fmt = (h->bits == 8) ? 1 : (h->bits == 16) ? 2 : (h->bits == 32) ? 6 : 2;
		    unsigned int effective_rate = (unsigned int)((float)h->sample_rate * h->pitch);
		    if(pPcmSetParams(h->os_handle, alsa_fmt, 3, h->channels, effective_rate, 1, 150000) < 0){
		    	typedef int (*SPC)(void*);
		    	static SPC pPcmClose = (SPC)get_system_function("snd_pcm_close", "libasound.so.2");
		    	if(pPcmClose) pPcmClose(h->os_handle);
		    	h->os_handle = nullptr;
		    	return true; // the intent stands: the deliveries will retry the device
		    }
		    return (h->playing = true);
		    #endif
		}
		FUNCDECL(void stop, void* snd){
			TSN_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)){
				sound::stop(((TSN_HANDLE*)snd)->snd);
				return;
			}
			if(SND_HANDLE_VALID(snd)) return;
			SND_HANDLE_DECL;
		    SND_HANDLE* h = (SND_HANDLE*)snd;
		    if(!(h->playing) && !(h->os_handle)){ h->wants_playback = false; return; } // nothing open, nothing to reclaim
		    h->wants_playback = false;
		    #if defined(_WIN32)
		    typedef int(WINCALL *WOC)(void*);
		    static WOC pWaveOutClose = (WOC)get_system_function("waveOutClose", "winmm.dll");
			if(h->os_handle){
				// waveOutReset returns every header still in the queue, so the output can
				// be reopened without leaving both turns claimed forever by the driver.
				typedef int(WINCALL *WOR)(void*);
				static WOR pWaveOutReset = (WOR)get_system_function("waveOutReset", "winmm.dll");
				typedef int(WINCALL *WUH)(void*,void*,unsigned int);
				static WUH pUnprepare = (WUH)get_system_function("waveOutUnprepareHeader", "winmm.dll");
				if(pWaveOutReset) pWaveOutReset(h->os_handle);
				if(pUnprepare) for(int i = 0; i < 2; ++i){
					if(h->buffers[i].meta.dwFlags & 0x00000002){ // WHDR_PREPARED
						pUnprepare(h->os_handle, &(h->buffers[i].meta), sizeof(WAVEHDR));
						h->buffers[i].meta.dwFlags &= ~0x00000002;
					}
					h->buffers[i].meta.dwFlags &= ~0x00000001; // WHDR_DONE
				}
				if(pWaveOutClose) pWaveOutClose(h->os_handle);
			}
			#else // Linux
			typedef int(*SPC)(void*);
			static SPC pPcmClose = (SPC)get_system_function("snd_pcm_close", "libasound.so.2");
			static SPC pPcmDrop  = (SPC)get_system_function("snd_pcm_drop", "libasound.so.2");
			if(h->os_handle && pPcmDrop && pPcmClose){ pPcmDrop(h->os_handle); pPcmClose(h->os_handle); }
		    #endif
			h->playing = false;
			h->os_handle = nullptr;
			h->buffers[0].busy = false;
			h->buffers[1].busy = false;
		}
		FUNCDECL(void destroy, void* snd){
			TSN_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)){
				TSN_HANDLE* tsn_h = (TSN_HANDLE*)snd;
				tsn_h->autodestruction = true;
				if(tsn_h->thr){
					// The worker still reads this handle every cycle: joining first is what
					// makes the shutdown ordered instead of a use-after-free race.
					thread::join(tsn_h->thr);
					tsn_h->thr = nullptr;
				}
				sound::destroy(tsn_h->snd);
				tsn_h->snd = nullptr;
				delete tsn_h;
				return;
			}
			if(SND_HANDLE_VALID(snd)) return;
		    sound::stop(snd);
			SND_HANDLE_DECL;
		    delete (SND_HANDLE*)snd;
		}
		FUNCDECL(bool sync, void* snd){
			TSN_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)) return sound::sync(((TSN_HANDLE*)snd)->snd);
		    if(SND_HANDLE_VALID(snd)) return false;
			SND_HANDLE_DECL;
		    SND_HANDLE* h = (SND_HANDLE*)snd;
		    if(!(h->playing)) return true;
		    #if defined(_WIN32)
		    WAVEHDR* hdr = &(h->buffers[0].meta);
		    if((h->buffers[0].busy) && ((hdr->dwFlags) & 0x00000001)){
		        h->buffers[0].busy = false;
		        hdr->dwFlags &= ~0x00000001;
		    }
		    hdr = &(h->buffers[1].meta);
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
            unsigned int buffers_size = (unsigned int)memory::szlen(h->buffers[0].data);
            if((h->buffers[0].busy) && avail_bytes >= buffers_size) h->buffers[0].busy = false;
            if((h->buffers[1].busy) && avail_bytes >= buffers_size) h->buffers[1].busy = false;
		    #endif
            return (!(h->buffers[0].busy) || !(h->buffers[1].busy));
		}
		FUNCDECL(void set, void* snd, unsigned int rate, BYTE channels, BYTE bits){
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
		FUNCDECL(bool playing, void* snd){
			SND_HANDLE_DECL;
			TSN_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)){
				SND_HANDLE* inner = (SND_HANDLE*)(((TSN_HANDLE*)snd)->snd);
				return (inner) ? inner->playing : false;
			}
			if(SND_HANDLE_VALID(snd)) return false;
			return ((SND_HANDLE*)snd)->playing;
		}
		FUNCDECL(bool is_threaded, void* snd){
			if(TSN_HANDLE_UNVALID(snd)) return true;
			return false;
		}
		FUNCDECL(bool set_volume, void* snd, float vol){
			TSN_HANDLE_DECL;
			SND_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)){
				SND_HANDLE* inner = (SND_HANDLE*)(((TSN_HANDLE*)snd)->snd);
				if(inner) inner->volume = vol;
				((TSN_HANDLE*)snd)->target_volume = vol;
				((TSN_HANDLE*)snd)->fade_step = 0.0f;
				return true;
			}
			if(SND_HANDLE_VALID(snd)) return false;
			((SND_HANDLE*)snd)->volume = vol;
			return true;
		}
		FUNCDECL(bool volume, void* snd, float* vol){
			if(!vol) return false;
			*vol = 0.0f;
			TSN_HANDLE_DECL;
			SND_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)){
				SND_HANDLE* inner = (SND_HANDLE*)(((TSN_HANDLE*)snd)->snd);
				if(inner) *vol = inner->volume;
				return true;
			}
			if(SND_HANDLE_VALID(snd)) return false;

			*vol = ((SND_HANDLE*)snd)->volume;
			return true;
		}
		FUNCDECL(float volume, void* snd){
			float vol;
			volume(snd, &vol);
			return vol;
		}
		FUNCDECL(void set_pitch, void* snd, float pitch){
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
		        typedef int(WINCALL *WSP)(void*, unsigned int);
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
		FUNCDECL(bool pitch, void* snd, float* pit){
			if(!pit) return false;
			*pit = 0.0f;
			TSN_HANDLE_DECL;
			SND_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)){
				SND_HANDLE* inner = (SND_HANDLE*)(((TSN_HANDLE*)snd)->snd);
				if(inner) *pit = inner->pitch;
				return true;
			}
			if(SND_HANDLE_VALID(snd)) return false;
			*pit = ((SND_HANDLE*)snd)->pitch;
			return true;
		}
		FUNCDECL(float pitch, void* snd){
			float pit = 0.0f;
			pitch(snd, &pit);
			return pit;
		}
		FUNCDECL(bool set_master_volume, void* snd, float* mst_vol){
			TSN_HANDLE_DECL;
			SND_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)){
				SND_HANDLE* inner = (SND_HANDLE*)(((TSN_HANDLE*)snd)->snd);
				if(inner){ inner->master_volume = mst_vol; inner->owns_master = false; }
				return true;
			}
			if(SND_HANDLE_VALID(snd)) return false;
			((SND_HANDLE*)snd)->master_volume = mst_vol;
			return true;
		}
		FUNCDECL(bool master_volume, void* snd, float** mst_vol){
			if(!mst_vol) return false;
			*mst_vol = nullptr;
			TSN_HANDLE_DECL;
			SND_HANDLE_DECL;
			if(TSN_HANDLE_UNVALID(snd)){
				SND_HANDLE* inner = (SND_HANDLE*)(((TSN_HANDLE*)snd)->snd);
				if(inner) *mst_vol = inner->master_volume;
				return true;
			}
			if(SND_HANDLE_VALID(snd)) return false;
			*mst_vol = ((SND_HANDLE*)snd)->master_volume;
			return true;
		}
		FUNCDECL(float* master_volume, void* snd){
			float* mst_vol;
			master_volume(snd, &mst_vol);
			return mst_vol;
		}
		#if defined(ST_IMPORT_FILE)
		FUNCDECL(bool reload, void* th_snd){
			TSN_HANDLE_DECL;
			if(!TSN_HANDLE_UNVALID(th_snd)) return false;
			void* file_h = ((TSN_HANDLE*)th_snd)->file;
			file::seek(file_h, 44, seek_mode::begin);
			return true;
		}
		FUNCDECL(void* file_stream, const char* filepath, bool looping, float pitch){
			void* file_h = file::open(filepath, file_mode::read);
			void* ret = file_stream(file_h, looping, pitch, true);
			if(!ret) file::close(file_h);
			return ret;
		}
		FUNCDECL(bool fade, void* th_snd, float target_volume, UWORD ms){
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
		FUNCDECL(float get_position, void* th_snd){
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
		    UWORD buffers_size = memory::szlen(h->buffers[0].data);
			float buffer_latency = (float)(buffers_size << 1) / bytes_per_sec;
			return position - buffer_latency;
		}
		FUNCDECL(float get_duration, void* th_snd){
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
		FUNCDECL(bool seek, void* th_snd, float seconds){
			if(!TSN_HANDLE_UNVALID(th_snd)) return false;
			TSN_HANDLE_DECL;
			SND_HANDLE_DECL;
		    TSN_HANDLE* thsn = (TSN_HANDLE*)th_snd;
		    void* wav = thsn->file;
		    SND_HANDLE* h = (SND_HANDLE*)(thsn->snd);
		    if(!wav || !h) return false;
		    unsigned int bytes_per_sec = h->sample_rate * h->channels * (h->bits >> 3);
		    unsigned int frame_bytes = (unsigned int)(h->channels) * (unsigned int)(h->bits >> 3);
		    if(!frame_bytes) frame_bytes = 1;
		    unsigned long target_byte = 44 + (unsigned long)(seconds * (float)bytes_per_sec);
		    if(target_byte > 44) target_byte = 44 + ((target_byte - 44) / frame_bytes) * frame_bytes; // whole frames only
		    if(target_byte >= file::size(wav)) return false;
		    file::seek(wav, target_byte, seek_mode::begin);
		    return true;
		}
		FUNCDECL(bool get_spectrum, void* th_snd, float** buffer, UWORD* samples, float* average){
			if(!TSN_HANDLE_UNVALID(th_snd) || !buffer || !samples) return false;
			TSN_HANDLE_DECL;
			SND_HANDLE_DECL;
		    TSN_HANDLE* thsn = (TSN_HANDLE*)th_snd;
		    void* wav = thsn->file;
		    SND_HANDLE* h = (SND_HANDLE*)(thsn->snd);
		    if(!wav || !h) return false;
		    int current;
		    if((h->buffers[0].busy) != (h->buffers[1].busy)) current = (h->buffers[0].busy) ? 0 : 1; // the only turn left in the output
		    else current = 1 - (h->current_buffer ? 1 : 0);                                          // both queued: the older one sounds first
		    if(!h->buffers[current].data) current = 1 - current;
		    if(!h->buffers[current].data) return false;
		    char _jump = (h->bits) >> 3;
		    char* raw_data = (char*)(h->buffers[current].data);
		    UWORD buffer_size = memory::szlen(h->buffers[0].data);
		    UWORD total_samples = buffer_size / _jump;
		    if(*buffer) delete[] *buffer;
		    *buffer = new float[total_samples];
		    *samples = total_samples;
		    float _accum = 0.0f;
		    for(UWORD i = 0, j = 0; i < buffer_size; i+=_jump){
		    	if(_jump == 1) (*buffer)[j] = ((float)(*(unsigned char*)(raw_data + i)) - 128.0f) / 128.0f;
		    	if(_jump == 2) (*buffer)[j] = (float)(*(short*)(raw_data + i)) / 32767.0f;
		    	if(_jump == 4) (*buffer)[j] = (float)(*(int*)(raw_data + i)) / 2147483647.0f;
		    	++j;
			}
			for(UWORD j = 0; j < *samples; ++j){
				float v = (*buffer)[j];
				_accum += (v < 0.0f) ? -v : v;
			}
			if(average) *average = (*samples) ? (_accum / (float)(*samples)) : 0.0f;
		    return true;
		}
		#endif
	}
    #endif
    #if defined(ST_IMPORT_WINDOW)
	#define WND_HANDLE_VALID(wnd) ((wnd)?(*(UWORD*)(wnd)!=0x574E444F):true)
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
	    void* thr;\
	    int x, y, width, height;\
	    unsigned int* pixels;\
	    UWORD pixels_amont;\
	    bool minimized;\
	    bool invisible;\
	    bool ext;\
	    bool thr_join;\
	    bool self_destroy;\
	    WND_HANDLE() : magic(0x574E444F), wc({}),\
			className(nullptr), hwnd(nullptr), father(nullptr), thr(nullptr), x(0), y(0), width(0), height(0),\
			pixels(nullptr), pixels_amont(0), minimized(false), invisible(false), ext(false), thr_join(false), self_destroy(true) {}\
        ~WND_HANDLE(){ magic = 0; }\
	}
    #define WND_HANDLE_STATE(handle) ((handle)->hwnd != nullptr)
    #else
    #define WND_HANDLE_DECL struct WND_HANDLE{\
        UWORD magic;\
	    UWORD pixels_amont;\
	    unsigned int* pixels;\
        WND_HANDLE* father;\
        void* display;\
        void* visual;\
        void* gc;\
        void* image;\
	    void* thr;\
        unsigned long root;\
        unsigned long window;\
        int scr, x, y, width, height;\
	    bool minimized;\
	    bool invisible;\
	    bool ext;\
	    bool thr_join;\
	    bool self_destroy;\
        WND_HANDLE() : magic(0x574E444F), pixels_amont(0), pixels(nullptr), father(nullptr),\
            display(nullptr), visual(nullptr), gc(nullptr), image(nullptr), thr(nullptr),\
            root(0), window(0), scr(0), x(0), y(0), width(0), height(0),\
            minimized(false), invisible(false), ext(false), thr_join(false), self_destroy(true) {}\
        ~WND_HANDLE(){ magic = 0; }\
    }
    #define WND_HANDLE_STATE(handle) ((handle)->display && (handle)->window)
    #endif
	#endif
    #if defined(ST_IMPORT_CONSOLE)
	#undef ST_IMPORT_CONSOLE
    #define ST_IMPORT_CONSOLE 4
	namespace console{
		FUNCDECL(void* get_handle){
			#define CNSL_HANDLE_VALID(hCon) ((hCon)?((*(UWORD*)(hCon))!=0x4C534E43):true)
			#if defined(ST_IMPORT_WINDOW)
			#if defined(_WIN32)
			#define CNSL_WND__IN_HANDLE \
			WND_HANDLE_DECL;\
			WND_HANDLE handle_window;\
			void* get_window(){\
				handle_window.hwnd = hwnd;\
				return (void*)&handle_window;\
			}
			#else
			#define CNSL_WND__IN_HANDLE \
			WND_HANDLE_DECL;\
			WND_HANDLE handle_window;\
			void* get_window(){\
				handle_window.display = display;\
				handle_window.window = window;\
				handle_window.gc = gc;\
				handle_window.scr = scr;\
				handle_window.root = root;\
				handle_window.visual = visual;\
				return (void*)&handle_window;\
			}
			#endif
			#else
			#define CNSL_WND__IN_HANDLE
			#endif
			#if defined(_WIN32)
			#define CNSL_HANDLE_DECL struct CNSL_HANDLE{\
				UWORD magic;\
				void* hwnd;\
			    void* hStdOut;\
			    void* hStdIn;\
				unsigned int BGPrintColor;\
				unsigned int FGPrintColor;\
			    unsigned int currentBGPrintColor;\
				unsigned int currentFGPrintColor;\
				unsigned int baseOutputCP;\
				bool unicodeActive, currentUnicodeActive;\
				CNSL_WND__IN_HANDLE\
				CNSL_HANDLE() : magic(0) {}\
				~CNSL_HANDLE(){\
					if(magic){\
						console::setcolor((void*)this, BGPrintColor, FGPrintColor);\
						console::setunicode((void*)this, unicodeActive);\
					}\
					magic = 0;\
				}\
			}
			typedef void*(WINCALL *GCW)();
			static GCW pGetConsoleWindow = (GCW)get_system_function("GetConsoleWindow");
			typedef void*(WINCALL *GSH)(unsigned int);
			static GSH pGetStdHandle = (GSH)get_system_function("GetStdHandle");
			typedef int(WINCALL *GCSBIEX)(void*,void*);
			static GCSBIEX pGetConsoleScreenBufferInfoEx = (GCSBIEX)get_system_function("GetConsoleScreenBufferInfoEx");
			typedef unsigned int(WINCALL *GCOP)();
			static GCOP pGetConsoleOutputCP = (GCOP)get_system_function("GetConsoleOutputCP");
			struct CONSOLE_SCREEN_BUFFER_INFOEX{
			    unsigned int cbSize;
			    short dwSizeX, dwSizeY, dwCursorX, dwCursorY;
			    short wAttributes, srWindowLeft, srWindowTop, srWindowRight, srWindowBottom;
			    short dwMaximumWindowSizeX, dwMaximumWindowSizeY, wPopupAttributes;
			    int bFullscreenSupported;
			    unsigned int ColorTable[16];
			};
			if(!pGetConsoleWindow || !pGetStdHandle || !pGetConsoleScreenBufferInfoEx || !pGetConsoleOutputCP) return nullptr;
			#else
			#define CNSL_HANDLE_DECL struct CNSL_HANDLE{\
				UWORD              magic;\
				void*              hStdOut;\
				void*			   hStdIn;\
				void*              display;\
				void*              visual;\
        		void*              gc;\
				unsigned long long root;\
				unsigned long long window;\
				unsigned long long pid;\
			    unsigned int       BGPrintColor;\
			    unsigned int       FGPrintColor;\
			    unsigned int       currentBGPrintColor;\
				unsigned int       currentFGPrintColor;\
			    int                scr;\
			    bool               unicodeActive, currentUnicodeActive;\
				CNSL_WND__IN_HANDLE\
				CNSL_HANDLE() : magic(0) {}\
				~CNSL_HANDLE(){\
					typedef int(*XFRGC)(void*,void*);\
					static XFRGC pXFreeGC = (XFRGC)get_system_function("XFreeGC", "libX11.so.6");\
					typedef int(*XCD)(void*);\
					static XCD pXCloseDisplay = (XCD)get_system_function("XCloseDisplay", "libX11.so.6");\
					if(magic){\
						console::setcolor((void*)this, BGPrintColor, FGPrintColor);\
						console::setunicode((void*)this, unicodeActive);\
						if((hStdOut && hStdOut != (void*)-1) || (hStdIn && hStdIn != (void*)-1)) CNSL_HANDLE::free_stdio(hStdIn, hStdOut);\
						if(display && gc && pXFreeGC) pXFreeGC(display, gc);\
						if(display && pXCloseDisplay) pXCloseDisplay(display);\
					}\
					magic = 0;\
				}\
				static unsigned long long get_window_pid(void* display, unsigned long long window){\
					if(!display || !window) return 0;\
					typedef int(*GWP)(void*,unsigned long long,unsigned long,long,long,bool,unsigned long,unsigned long*,int*,unsigned long*,unsigned long*,unsigned char**);\
					typedef int(*XFREE)(unsigned char*);\
					typedef unsigned long(*XIA)(void*,const char*,bool);\
					static GWP pXGetWindowProperty = (GWP)get_system_function("XGetWindowProperty", "libX11.so.6");\
					static XFREE pXFree = (XFREE)get_system_function("XFree", "libX11.so.6");\
					static XIA pXInternAtom = (XIA)get_system_function("XInternAtom", "libX11.so.6");\
					if(!pXGetWindowProperty || !pXFree || !pXInternAtom) return 0;\
					unsigned long pidAtom = pXInternAtom(display, "_NET_WM_PID", 1);\
					if(!pidAtom) return 0;\
					unsigned long actualType = 0;\
					int actualFormat = 0;\
					unsigned long nItems = 0;\
					unsigned long bytesAfter = 0;\
					unsigned char* propValue = nullptr;\
					int result = pXGetWindowProperty( \
						display, \
						window, \
						pidAtom, \
						0, 1, \
						false, \
						0x00000006UL, \
						&actualType, \
						&actualFormat, \
						&nItems, \
						&bytesAfter, \
						&propValue \
					);\
					unsigned long long pid = 0;\
					if(result == 0 && propValue && nItems > 0){\
						pid = (unsigned long long)(*(unsigned int*)propValue);\
						pXFree(propValue);\
					}\
					return pid;\
				}\
				static bool resolve_window_tty(unsigned long long pid, char* tty_path){\
					typedef WORD(*RL)(const char*,char*,UWORD);\
					static RL pReadLink = (RL)get_system_function("readlink");\
					if(!pReadLink || !tty_path) return false;\
					char fd_path[256]; sprintf(fd_path, "/proc/%llu/fd/0", pid);\
					UWORD _end = pReadLink(fd_path, tty_path, 255);\
					if(_end <= 0 || _end >= 255){ *tty_path = '\0'; return false; }\
					tty_path[_end] = '\0';\
					if(*tty_path != '/'){ *tty_path = '\0'; return false; }\
					return true;\
				}\
				static bool resolve_stdio(const char* tty_path, void** hStdIn, void** hStdOut){\
					typedef int(*FOPN)(const char*, int, ...);\
					typedef void*(*FDOPN)(int, const char*);\
					static FOPN pOpen = (FOPN)get_system_function("open");\
					static FDOPN pFdOpen = (FDOPN)get_system_function("fdopen");\
					if(!pOpen || !pFdOpen || !tty_path || tty_path[0] == '\0') return false;\
					int fd = pOpen(tty_path, 2 | 0x0100);\
					if(fd < 0){\
						*hStdIn = nullptr;\
						*hStdOut = nullptr;\
						return false;\
					}\
					void* fIn = pFdOpen(fd, "r+");\
					if(!fIn) fIn = pFdOpen(fd, "r");\
					if(!fIn){\
						typedef int(*FCLS)(int);\
						static FCLS pClose = (FCLS)get_system_function("close");\
						if(pClose) pClose(fd);\
						*hStdIn = nullptr;\
						*hStdOut = nullptr;\
						return false;\
					}\
					*hStdIn  = fIn;\
					*hStdOut = fIn;\
					return true;\
				}\
				static void free_stdio(void* hStdIn, void* hStdOut){\
					typedef int(*FC)(void*);\
					static FC pFClose = (FC)get_system_function("fclose");\
					if(!pFClose) return;\
					if(hStdIn && hStdIn != (void*)-1) pFClose(hStdIn);\
					else if(hStdOut && hStdOut != (void*)-1 && hStdOut != hStdIn) pFClose(hStdOut);\
				}\
			}
			typedef void*(*XOD)(const char*);
			static XOD pXOpenDisplay = (XOD)get_system_function("XOpenDisplay", "libX11.so.6");
			typedef unsigned long long(*XDRW)(void*);
			static XDRW pXDefaultRootWindow = (XDRW)get_system_function("XDefaultRootWindow", "libX11.so.6");
			typedef int(*GPID)();
			static GPID pGetPid = (GPID)get_system_function("getpid");
			typedef int(*FNDO)(void*);
			static FNDO pFileno = (FNDO)get_system_function("fileno");
			typedef char*(*GETENV)(const char*);
			static GETENV pGetenv = (GETENV)get_system_function("getenv");
			struct termios{
				unsigned int c_iflag;
				unsigned int c_oflag;
				unsigned int c_cflag;
				unsigned int c_lflag;
				unsigned char c_line;
				unsigned char c_cc[32];
				unsigned int c_ispeed;
				unsigned int c_ospeed;
			};
			typedef int(*TCGETATTR)(int,termios*);
			static TCGETATTR p_tcgetattr = (TCGETATTR)get_system_function("tcgetattr");
			typedef int(*TCSETATTR)(int,int,const termios*);
			static TCSETATTR p_tcsetattr = (TCSETATTR)get_system_function("tcsetattr");
			typedef int(*TCFLSH)(int, int);
			static TCFLSH p_tcflush = (TCFLSH)get_system_function("tcflush");
			typedef WORD(*READ_FN)(int,void*,UWORD);
			static READ_FN p_read = (READ_FN)get_system_function("read");
			typedef WORD(*WRITE_FN)(int,const void*,UWORD);
			static WRITE_FN p_write = (WRITE_FN)get_system_function("write");
			struct timeval{
				long tv_sec;
				long tv_usec;
			};
			typedef int(*SELECT_FN)(int,void**,void**,void**,timeval*);
			static SELECT_FN p_select = (SELECT_FN)get_system_function("select");
			typedef void*(*CXGC)(void*,unsigned long,unsigned long,void*);
			static CXGC pXCreateGC = (CXGC)get_system_function("XCreateGC", "libX11.so.6");
			typedef int(*XDS)(void*);
			static XDS pXDefaultScreen = (XDS)get_system_function("XDefaultScreen", "libX11.so.6");
			typedef unsigned long(*XRS)(void*);
			static XRS pXRootWindow = (XRS)get_system_function("XDefaultRootWindow", "libX11.so.6");
			typedef void*(*XDV)(void*,int);
			static XDV pXDefaultVisual = (XDV)get_system_function("XDefaultVisualOfScreen", "libX11.so.6");
			if(!pGetPid || !pFileno || !pGetenv || !p_tcgetattr || !p_tcsetattr || !p_read || !p_write || !p_select) return nullptr;
			struct _ap_{
				struct fd_set{
					typedef long fd_mask;
					enum{ __NFDBITS = (8 * (int)sizeof(fd_mask)) };
					fd_mask fds_bits[1024 / __NFDBITS];
				};
				static FORCEINLINE const char* strstr(const char* str1, const char* str2){
					if(!str1 || !str2) return nullptr;
					while(*str1){
						const char* h = str1;
						const char* n = str2;
						while(*h && *n && (*h == *n)){ ++h; ++n; }
						if(!*n) return h;
						++str1;
					}
					return nullptr;
				}
				static FORCEINLINE unsigned int parse_hex(const char* hex){
				    unsigned int val = 0; UWORD len = 0;
					while(*hex){
						char c = *hex++;
						val = (val << 4) | ((c >= '0' && c <= '9') ? (c - '0') : ((c | 32) - 'a' + 10));
						len++;
					}
					return (len == 4) ? (val >> 8) : val;
				};
				static bool query_osc_color(int fd_in, int fd_out, const char* cmd, unsigned int* out_color){
					if(!out_color) return false;
					int cmd_len = 0; while(cmd[cmd_len]) ++cmd_len;
					if(p_write(fd_out, cmd, cmd_len) <= 0) return false;
					char buffer[128] = {0};
					int idx = 0;
					while(idx < (int)sizeof(buffer) - 1){
						_ap_::fd_set readfds;
						memory::slloc(readfds.fds_bits, 0, sizeof(_ap_::fd_set::fds_bits));
						if(fd_in >= 0 && fd_in < 1024)
							readfds.fds_bits[fd_in / _ap_::fd_set::__NFDBITS] |= (((_ap_::fd_set::fd_mask)1) << (fd_in % _ap_::fd_set::__NFDBITS));
						timeval tv = {0, 100000};
						if((p_select ? p_select(fd_in + 1, (void**)&readfds, nullptr, nullptr, &tv) : 0) <= 0) break;
						char c;
						if(p_read(fd_in, &c, 1) <= 0) break;
						buffer[idx++] = c;
						if(c == '\x07' || (idx >= 2 && buffer[idx - 2] == '\x1b' && buffer[idx - 1] == '\\')) break;
					}
					const char* rgb_ptr = _ap_::strstr(buffer, "rgb:");
					if(rgb_ptr){
						char r_s[5]={0}, g_s[5]={0}, b_s[5]={0};
						if(sscanf(rgb_ptr, "%4[^/]/%4[^/]/%4[^;\x07\x1b]", r_s, g_s, b_s) == 3){
							*out_color = (_ap_::parse_hex(b_s) << 16) | (_ap_::parse_hex(g_s) << 8) | _ap_::parse_hex(r_s);
							return true;
						}
					}
					return false;
				}
			};
			#endif
			CNSL_HANDLE_DECL;
			static CNSL_HANDLE handle_singleton;
        	if(!(handle_singleton.magic)){
        		handle_singleton.magic = 0x4C534E43;
				#if defined(_WIN32)
				handle_singleton.hwnd    = pGetConsoleWindow();
			    handle_singleton.hStdOut = pGetStdHandle(-11);
			    handle_singleton.hStdIn  = pGetStdHandle(-10);
				if((handle_singleton.hStdOut) && (handle_singleton.hStdOut) != (void*)-1){
					CONSOLE_SCREEN_BUFFER_INFOEX csbiex;
			        csbiex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
			        if(pGetConsoleScreenBufferInfoEx(handle_singleton.hStdOut, &csbiex)){
			            handle_singleton.BGPrintColor = csbiex.ColorTable[(csbiex.wAttributes & 0xF0) >> 4];
			            handle_singleton.FGPrintColor = csbiex.ColorTable[csbiex.wAttributes & 0x0F];
			        }
				}
				unsigned int cp = pGetConsoleOutputCP();
				handle_singleton.baseOutputCP = cp;
				handle_singleton.unicodeActive = (cp == 65001 || cp == 1200 || cp == 1201);
				#else
				void** p_stdout = (void**)get_system_variable("stdout", "libc.so.6");
				if(!p_stdout) p_stdout = (void**)get_system_variable("stdout");
				void** p_stdin = (void**)get_system_variable("stdin", "libc.so.6");
				if(!p_stdin) p_stdin = (void**)get_system_variable("stdin");
				if(pXOpenDisplay && pXDefaultRootWindow){
				    handle_singleton.display = pXOpenDisplay(nullptr);
				    if(handle_singleton.display)
				        handle_singleton.window = (unsigned long long)pXDefaultRootWindow(handle_singleton.display);
				}
				handle_singleton.pid = CNSL_HANDLE::get_window_pid(handle_singleton.display, handle_singleton.window);
				if(pGetPid && !(handle_singleton.pid)) handle_singleton.pid = pGetPid();
				char tty_path[256] = {0};
				if(CNSL_HANDLE::resolve_window_tty(handle_singleton.pid, tty_path)){
				    void* hStdIn = nullptr; void* hStdOut = nullptr;
				    CNSL_HANDLE::resolve_stdio(tty_path, &hStdIn, &hStdOut);
				    if(hStdIn && hStdOut){
				        handle_singleton.hStdIn  = hStdIn;
				        handle_singleton.hStdOut = hStdOut;
				    }
				}
				if(handle_singleton.hStdOut == nullptr || handle_singleton.hStdOut == (void*)-1)
				    handle_singleton.hStdOut = (p_stdout && *p_stdout) ? *p_stdout : (void*)1;
				if(handle_singleton.hStdIn == nullptr || handle_singleton.hStdIn == (void*)-1)
				    handle_singleton.hStdIn = (p_stdin && *p_stdin) ? *p_stdin : (void*)0;
				if(handle_singleton.hStdOut && handle_singleton.hStdOut != (void*)-1){
				    int fd_out = pFileno ? pFileno(handle_singleton.hStdOut) : 1;
				    int fd_in  = pFileno ? pFileno(handle_singleton.hStdIn)  : 0;
				    termios orig_term, raw_term; bool orig_getted = false;
				    if(p_tcgetattr && p_tcsetattr && !p_tcgetattr(fd_in, &orig_term)){
						orig_getted = true;
				        raw_term = orig_term;
				        raw_term.c_lflag &= ~(0000002 | 0000010);
						raw_term.c_cc[6] = 0;
    					raw_term.c_cc[5] = 1;
				        if(!p_tcsetattr(fd_in, 0, &raw_term)){
				            handle_singleton.FGPrintColor = 0xFFFFFFFF;
				            handle_singleton.BGPrintColor = 0xFFFFFFFF;
				            _ap_::query_osc_color(fd_in, fd_out, "\x1b]10;?\x07", &(handle_singleton.FGPrintColor));
				            _ap_::query_osc_color(fd_in, fd_out, "\x1b]11;?\x07", &(handle_singleton.BGPrintColor));
				        }
						if(p_tcflush) p_tcflush(fd_in, 2);
						p_tcsetattr(fd_in, 2, &orig_term);
				    }
				    if(handle_singleton.BGPrintColor == 0xFFFFFFFF) handle_singleton.BGPrintColor = 0x00000000;
				    if(handle_singleton.FGPrintColor == 0xFFFFFFFF) handle_singleton.FGPrintColor = 0x00CCCCCC;
				}
				const char* lang = (const char*)get_system_variable("LC_ALL");
				if(!lang || !*lang) lang = (const char*)get_system_variable("LC_CTYPE");
				if(!lang || !*lang) lang = (const char*)get_system_variable("LANG");
				handle_singleton.unicodeActive = true;
				if(lang) handle_singleton.unicodeActive = (_ap_::strstr(lang, "UTF-8") || _ap_::strstr(lang, "utf-8") || _ap_::strstr(lang, "UTF8") || _ap_::strstr(lang, "utf8"));
				if(handle_singleton.display && handle_singleton.window && pXCreateGC) handle_singleton.gc = pXCreateGC(handle_singleton.display, handle_singleton.window, 0, nullptr);
				if(handle_singleton.display && pXDefaultScreen) handle_singleton.scr = pXDefaultScreen(handle_singleton.display);
				if(handle_singleton.display && pXRootWindow) handle_singleton.root = pXRootWindow(handle_singleton.display);
				if(handle_singleton.display && handle_singleton.scr && pXDefaultVisual) handle_singleton.visual = pXDefaultVisual(handle_singleton.display, handle_singleton.scr);
				#endif
				handle_singleton.currentBGPrintColor = handle_singleton.BGPrintColor;
				handle_singleton.currentFGPrintColor = handle_singleton.FGPrintColor;
				handle_singleton.currentUnicodeActive = handle_singleton.unicodeActive;
			}
			return (void*)&handle_singleton;
		}
		FUNCDECL(UWORD write, void* cnsl, const wchar* text, UWORD len){
			if(!cnsl) cnsl = console::get_handle();
			if(CNSL_HANDLE_VALID(cnsl) || !text || !len) return 0;
			CNSL_HANDLE_DECL;
			CNSL_HANDLE* hCon = (CNSL_HANDLE*)cnsl;
			#if defined(___mb__switch___case___1_2__3__)
			#undef ___mb__switch___case___1_2__3__
			#endif
			#if defined(_WIN32)
			#define ___mb__switch___case___1_2__3__(text, len, __unicoded) \
		    if(!hCon->hStdOut || hCon->hStdOut == (void*)-1) return 0;\
		    typedef int(WINCALL *WCX)(void*,const void*,unsigned int,unsigned int*,void*);\
		    static WCX pWriteConsoleA = (WCX)get_system_function("WriteConsoleA");\
		    static WCX pWriteConsoleW = (WCX)get_system_function("WriteConsoleW");\
		    if(!pWriteConsoleA || !pWriteConsoleW) return 0;\
		    unsigned int written = 0;\
		    ((__unicoded) ? pWriteConsoleW : pWriteConsoleA)(hCon->hStdOut, (const void*)(text), (unsigned int)(len), &written, nullptr);\
		    return (UWORD)written
		    #else
			#define ___mb__switch___case___1_2__3__(text, len, __unicoded) \
			typedef long (*PWRITE)(int,const void*,unsigned long);\
            static PWRITE pWrite = (PWRITE)get_system_function("write");\
			typedef int(*FNDO)(void*);\
			static FNDO pFileno = (FNDO)get_system_function("fileno");\
			if(!pWrite) return 0;\
			int fd_out = pFileno ? pFileno(hCon->hStdOut) : 1;\
			if(fd_out < 0) return 0;\
			char* __by_read = (char*)(text);\
			if(__unicoded){\
				UWORD max_bytes = (((UWORD)(len)) << 2) + 1;\
				__by_read = (char*)memory::alloc(max_bytes);\
				if(!__by_read) return 0;\
				long _ret_bytes = pWrite(fd_out, __by_read, wcstombs(__by_read, (const wchar*)(text), max_bytes, (UWORD)(len)));\
				memory::free(__by_read);\
				return (_ret_bytes > 0) ? ((UWORD)(len)) : 0;\
			}\
			return (UWORD)pWrite(fd_out, __by_read, len)
			#endif
			___mb__switch___case___1_2__3__(text, len, 1);
		}
		FUNCDECL(UWORD write, void* cnsl, const char* text, UWORD len){
			if(!cnsl) cnsl = console::get_handle();
			if(CNSL_HANDLE_VALID(cnsl) || !text || !len) return 0;
			CNSL_HANDLE_DECL;
			CNSL_HANDLE* hCon = (CNSL_HANDLE*)cnsl;
			___mb__switch___case___1_2__3__(text, len, 0);
			#undef ___mb__switch___case___1_2__3__
		}
		FUNCDECL(ALWAYSINLINE UWORD write, void* cnsl, const void* text, UWORD len){ return console::write(cnsl, (const wchar*)text, len); }
		FUNCDECL(ALWAYSINLINE UWORD write, void* cnsl, const wchar* text){ if(!text) return 0; UWORD len = 0; while(text[len]) ++len; return console::write(cnsl, text, len); }
		FUNCDECL(ALWAYSINLINE UWORD write, void* cnsl, const char* text){ if(!text) return 0; UWORD len = 0; while(text[len]) ++len; return console::write(cnsl, text, len); }
		FUNCDECL(ALWAYSINLINE UWORD write, void* cnsl, const void* text){ return console::write(cnsl, (const wchar*)text); }
		FUNCDECL(ALWAYSINLINE UWORD write, void* cnsl, decltype(nullptr), UWORD len){ return 0; }
		FUNCDECL(ALWAYSINLINE UWORD write, void* cnsl, decltype(nullptr)){ return 0; }
		FUNCDECL(void* read, void* cnsl, wchar** _text, UWORD* _len){
			if(!cnsl) cnsl = console::get_handle();
			if(CNSL_HANDLE_VALID(cnsl) || (!_text && !_len)) return nullptr;
			CNSL_HANDLE_DECL;
			CNSL_HANDLE* hCon = (CNSL_HANDLE*)cnsl;
			#if defined(_WIN32)
			#define ___mb__switch___case___1_2__3__(text, len, __unicode) \
		    if(!hCon->hStdIn || hCon->hStdIn == (void*)-1) return nullptr;\
		    typedef int(WINCALL *RCW)(void*,void*,unsigned int,unsigned int*,void*);\
		    static RCW pReadConsoleW = (RCW)get_system_function("ReadConsoleW");\
		    typedef int(WINCALL *RCA)(void*,void*,unsigned int,unsigned int*,void*);\
		    static RCA pReadConsoleA = (RCA)get_system_function("ReadConsoleA");\
		    if(!pReadConsoleW || !pReadConsoleA) return nullptr;\
			if(__unicode){\
				wchar buffer[1024];\
				unsigned int readChars = 0;\
				if(!pReadConsoleW(hCon->hStdIn, buffer, 1023, &readChars, nullptr)){\
					if(len) *(len) = 0;\
					if(text){\
						if(*(text)) memory::free(*(text));\
						*(text) = nullptr;\
					}\
					return cnsl;\
				}\
				if(len) *(len) = (UWORD)readChars;\
				if(text){\
					*((wchar**)(text)) = (wchar*)memory::realloc(*(text), sizeof(wchar) * (readChars + 1));\
			        memory::clloc(*(text), buffer, sizeof(wchar) * readChars);\
			        (*(text))[readChars] = L'\0';\
			    }\
				return cnsl;\
			}\
			char buffer[1024];\
			unsigned int readChars = 0;\
			if(!pReadConsoleA(hCon->hStdIn, buffer, 1023, &readChars, nullptr)){\
				if(len) *(len) = 0;\
				if(text){\
					if(*(text)) memory::free(*(text));\
					*(text) = nullptr;\
				}\
				return cnsl;\
			}\
			if(len) *(len) = (UWORD)readChars;\
			if(text){\
				*((char**)(text)) = (char*)memory::realloc(*(text), sizeof(char) * (readChars + 1));\
			    memory::clloc(*(text), buffer, sizeof(char) * readChars);\
			    (*(text))[readChars] = '\0';\
			}
		    #else
			#define ___mb__switch___case___1_2__3__(text, len, __unicode) \
		    typedef long(*SYS_READ)(int,void*,unsigned long);\
			static SYS_READ pRead = (SYS_READ)get_system_function("read");\
			typedef int(*FNDO)(void*);\
			static FNDO pFileno = (FNDO)get_system_function("fileno");\
			if(!pRead) return nullptr;\
			int fd_in = pFileno ? pFileno(hCon->hStdIn) : 0;\
			char cBuffer[1024];\
			long bytesRead = pRead(fd_in, cBuffer, 1023);\
			if(bytesRead <= 0){\
				if(len) *(len) = 0;\
				if(text){\
					if(*(text)) memory::free(*(text));\
					*(text) = nullptr;\
				}\
				return cnsl;\
			}\
		    if(len) *(len) = (UWORD)bytesRead;\
		    if(text){\
				if(__unicode){\
					*((wchar**)(text)) = (wchar*)memory::realloc(*((wchar**)(text)), sizeof(wchar) * (bytesRead + 1));\
					mbstowcs(*((wchar**)(text)), cBuffer, bytesRead, 1023);\
				}else{\
					*((char**)(text)) = (char*)memory::realloc(*((char**)(text)), sizeof(char) * (bytesRead + 1));\
					memory::clloc(*((char**)(text)), cBuffer, sizeof(char) * bytesRead);\
					(*((char**)(text)))[bytesRead] = '\0';\
				}\
		    }
		    #endif
			___mb__switch___case___1_2__3__(_text, _len, 1);
			return cnsl;
		}
		FUNCDECL(void* read, void* cnsl, char** _text, UWORD* _len){
			if(!cnsl) cnsl = console::get_handle();
			if(CNSL_HANDLE_VALID(cnsl) || (!_text && !_len)) return nullptr;
			CNSL_HANDLE_DECL;
			CNSL_HANDLE* hCon = (CNSL_HANDLE*)cnsl;
			___mb__switch___case___1_2__3__(_text, _len, 0);
			return cnsl;
			#undef ___mb__switch___case___1_2__3__
		}
		FUNCDECL(void* read, void* cnsl, void** _text, UWORD* _len){
			wchar* _t = nullptr;
			void* _ret = console::read(cnsl, _text ? (&_t) : ((wchar**)nullptr), _len);
			if(_text) *_text = (void*)_t;
			return _ret;
		}
		FUNCDECL(ALWAYSINLINE void* read, void* cnsl, decltype(nullptr), UWORD* _len){ return nullptr; }
		FUNCDECL(void* gotoxy, void* cnsl, UWORD x, UWORD y){
			if(!cnsl) cnsl = console::get_handle();
			if(CNSL_HANDLE_VALID(cnsl)) return nullptr;
			CNSL_HANDLE_DECL;
			CNSL_HANDLE* hCon = (CNSL_HANDLE*)cnsl;
			#if defined(_WIN32)
			struct COORD{ short X; short Y; };
			typedef int(WINCALL *SCCP)(void*,COORD);
			static SCCP pSetConsoleCursorPosition = (SCCP)get_system_function("SetConsoleCursorPosition");
			if(!pSetConsoleCursorPosition || !hCon->hStdOut) return nullptr;
			COORD pos = { (short)x, (short)y };
			if(pSetConsoleCursorPosition(hCon->hStdOut, pos)) return cnsl;
			#else
			typedef int(*FPRF)(void*,const char*,...);
			static FPRF pFprintf = (FPRF)get_system_function("fprintf");
			typedef int(*FFL)(void*);
			static FFL  pFFlush  = (FFL)get_system_function("fflush");
			if(!pFprintf || !pFFlush || !hCon->hStdOut) return nullptr;
			if(pFprintf(hCon->hStdOut, "\x1B[%u;%uH", (unsigned int)(y + 1), (unsigned int)(x + 1)) > 0){
				pFFlush(hCon->hStdOut);
				return cnsl;
			}
			#endif
			return nullptr;
		}
		FUNCDECL(void* getxy, void* cnsl, UWORD* _x, UWORD* _y){
			if(!cnsl) cnsl = console::get_handle();
			if(CNSL_HANDLE_VALID(cnsl)) return nullptr;
			if(!_x && !_y) return cnsl;
			CNSL_HANDLE_DECL;
			CNSL_HANDLE* hCon = (CNSL_HANDLE*)cnsl;
			#if defined(_WIN32)
			struct CONSOLE_SCREEN_BUFFER_INFO{
				short dwSizeX, dwSizeY;
				short dwCursorX, dwCursorY;
				WORD wAttributes;
				short srWindowLeft, srWindowTop, srWindowRight, srWindowBottom;
				short dwMaximumWindowSizeX, dwMaximumWindowSizeY;
			};
			typedef int(WINCALL *GCSBI)(void*,void*);
			static GCSBI pGetConsoleScreenBufferInfo = (GCSBI)get_system_function("GetConsoleScreenBufferInfo");
			if(!pGetConsoleScreenBufferInfo || !hCon->hStdOut) return nullptr;
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			if(pGetConsoleScreenBufferInfo(hCon->hStdOut, &csbi)){
				if(_x) *_x = (UWORD)csbi.dwCursorX;
				if(_y) *_y = (UWORD)csbi.dwCursorY;
				return cnsl;
			}
			#else
			typedef int(*FILENO)(void*);
			static FILENO pFileno  = (FILENO)get_system_function("fileno");
			typedef int(*TCGET)(int, void*);
			static TCGET  pTcGet   = (TCGET)get_system_function("tcgetattr");
			typedef int(*TCSET)(int, int, const void*);
			static TCSET  pTcSet   = (TCSET)get_system_function("tcsetattr");
			typedef int(*FPRF)(void*, const char*, ...);
			static FPRF   pFprintf = (FPRF)get_system_function("fprintf");
			typedef int(*FFL)(void*);
			static FFL    pFFlush  = (FFL)get_system_function("fflush");
			typedef int(*FGETC)(void*);
			static FGETC  pFgetc   = (FGETC)get_system_function("fgetc");
			if(!hCon->hStdIn || !hCon->hStdOut || !pFprintf || !pFFlush || !pFgetc) return nullptr;
			int in_fd = pFileno ? pFileno(hCon->hStdIn) : 0;
			struct termios{
				unsigned int c_iflag, c_oflag, c_cflag, c_lflag;
				unsigned char c_line;
				unsigned char c_cc[32];
				unsigned int c_ispeed, c_ospeed;
			} raw, old_term;
			bool raw_active = false;
			if(pTcGet && pTcSet && in_fd >= 0){
				if(pTcGet(in_fd, &old_term) == 0){
					raw = old_term;
					raw.c_lflag &= ~(10);
					if(pTcSet(in_fd, 2, &raw) == 0) raw_active = true;
				}
			}
			pFprintf(hCon->hStdOut, "\x1B[6n");
			pFFlush(hCon->hStdOut);
			char buf[32];
			unsigned int i = 0;
			int ch = 0;
			while(i < sizeof(buf) - 1 && (ch = pFgetc(hCon->hStdIn)) != -1){
				buf[i++] = (char)ch;
				if(ch == 'R') break;
			}
			if(raw_active) pTcSet(in_fd, 2, &old_term);
			buf[i] = '\0';
			if(i > 2 && buf[0] == '\x1B' && buf[1] == '['){
				unsigned int row = 0, col = 0;
				if(sscanf(buf + 2, "%u;%u", &row, &col) == 2){
					if(_x) *_x = (UWORD)(col > 0 ? col - 1 : 0);
					if(_y) *_y = (UWORD)(row > 0 ? row - 1 : 0);
					return cnsl;
				}
			}
			#endif
			return nullptr;
		}
		FUNCDECL(void* setcolor, void* cnsl, unsigned int bg, unsigned int fg){
			if(!cnsl) cnsl = console::get_handle();
			if(CNSL_HANDLE_VALID(cnsl)) return nullptr;
			CNSL_HANDLE_DECL;
			CNSL_HANDLE* hCon = (CNSL_HANDLE*)cnsl;
			#if defined(_WIN32)
			if(!(hCon->hStdOut) || hCon->hStdOut == (void*)-1) return nullptr;
			typedef int(WINCALL *GCM)(void*,unsigned int*);
			static GCM pGetConsoleMode = (GCM)get_system_function("GetConsoleMode");
			typedef int(WINCALL *SCM)(void*,unsigned int);
			static SCM pSetConsoleMode = (SCM)get_system_function("SetConsoleMode");
			typedef int(WINCALL *WCW)(void*,const void*,unsigned int,unsigned int*,void*);
			static WCW pWriteConsoleA = (WCW)get_system_function("WriteConsoleA");
			if(!pGetConsoleMode || !pSetConsoleMode || !pWriteConsoleA) return nullptr;
			unsigned int mode = 0;
			if(pGetConsoleMode(hCon->hStdOut, &mode))
				if(!(mode & 0x0004)) pSetConsoleMode(hCon->hStdOut, mode | 0x0004);
			unsigned int fgR = fg & 0xFF, fgG = (fg >> 8) & 0xFF, fgB = (fg >> 16) & 0xFF;
			unsigned int bgR = bg & 0xFF, bgG = (bg >> 8) & 0xFF, bgB = (bg >> 16) & 0xFF;
			char seq[64];
			unsigned int i = (unsigned int)sprintf(seq, "\x1B[38;2;%u;%u;%um\x1b[48;2;%u;%u;%um", fgR, fgG, fgB, bgR, bgG, bgB);
			pWriteConsoleA(hCon->hStdOut, seq, i, &i, nullptr);
			#else
			typedef int(*FPRF)(void*, const char*, ...);
			static FPRF pFprintf = (FPRF)get_system_function("fprintf");
			typedef int(*FFL)(void*);
			static FFL  pFFlush  = (FFL)get_system_function("fflush");
			if(!hCon->hStdOut || hCon->hStdOut == (void*)-1 || !pFprintf || !pFFlush) return nullptr;
			unsigned int fgR = fg & 0xFF, fgG = (fg >> 8) & 0xFF, fgB = (fg >> 16) & 0xFF;
			unsigned int bgR = bg & 0xFF, bgG = (bg >> 8) & 0xFF, bgB = (bg >> 16) & 0xFF;
			if(pFprintf(hCon->hStdOut, "\x1B[38;2;%u;%u;%um\x1B[48;2;%u;%u;%um", fgR, fgG, fgB, bgR, bgG, bgB) > 0)
				pFFlush(hCon->hStdOut);
			else return nullptr;
			#endif
			hCon->currentBGPrintColor = bg;
			hCon->currentFGPrintColor = fg;
			return cnsl;
		}
		FUNCDECL(void* getcolor, void* cnsl, unsigned int* _bg, unsigned int* _fg){
			if(!cnsl) cnsl = console::get_handle();
			if(CNSL_HANDLE_VALID(cnsl)) return nullptr;
			if(!_fg && !_bg) return cnsl;
			CNSL_HANDLE_DECL;
			CNSL_HANDLE* hCon = (CNSL_HANDLE*)cnsl;
			if(_bg) *_bg = hCon->currentBGPrintColor;
			if(_fg) *_fg = hCon->currentFGPrintColor;
			return cnsl;
		}
		FUNCDECL(void* restorecolor, void* cnsl){
			if(!cnsl) cnsl = console::get_handle();
			if(CNSL_HANDLE_VALID(cnsl)) return nullptr;
			CNSL_HANDLE_DECL;
			CNSL_HANDLE* hCon = (CNSL_HANDLE*)cnsl;
			return console::setcolor(cnsl, hCon->BGPrintColor, hCon->FGPrintColor);
		}
		FUNCDECL(void* setunicode, void* cnsl, bool unicode){
			if(!cnsl) cnsl = console::get_handle();
			if(CNSL_HANDLE_VALID(cnsl)) return nullptr;
			CNSL_HANDLE_DECL;
			CNSL_HANDLE* hCon = (CNSL_HANDLE*)cnsl;
			#if defined(_WIN32)
			typedef int(WINCALL *SCACP)(unsigned int);
			static SCACP pSetConsoleOutputCP = (SCACP)get_system_function("SetConsoleOutputCP");
			typedef unsigned int(WINCALL *GCOP)();
			static GCOP pGetConsoleOutputCP = (GCOP)get_system_function("GetConsoleOutputCP");
			if(!pSetConsoleOutputCP || !(hCon->hStdOut)) return nullptr;
			unsigned int target = (unicode) ? 65001 : hCon->baseOutputCP;
			if(!target || !pSetConsoleOutputCP(target)) return nullptr;
			if(pGetConsoleOutputCP){
				unsigned int cp = pGetConsoleOutputCP();
				hCon->currentUnicodeActive = (cp == 65001 || cp == 1200 || cp == 1201);
				return cnsl;
			}
			#else
			typedef long (*PWRITE)(int,const void*,unsigned long);
            static PWRITE pWrite = (PWRITE)get_system_function("write");
			typedef int(*FNDO)(void*);
			static FNDO pFileno = (FNDO)get_system_function("fileno");
			if(!pWrite || !pFileno) return nullptr;
			int fd_out = pFileno ? pFileno(hCon->hStdOut) : 1;
			if(fd_out < 0) return nullptr;
			pWrite(fd_out, unicode ? "\x1B%G" : "\x1B%@", 3);
			#endif
			hCon->currentUnicodeActive = unicode;
			return cnsl;
		}
		FUNCDECL(bool getunicode, void* cnsl){
			if(!cnsl) cnsl = console::get_handle();
			if(CNSL_HANDLE_VALID(cnsl)) return false;
			CNSL_HANDLE_DECL;
			CNSL_HANDLE* hCon = (CNSL_HANDLE*)cnsl;
			return hCon->currentUnicodeActive;
		}
		FUNCDECL(void* restoreunicode, void* cnsl){
			if(!cnsl) cnsl = console::get_handle();
			if(CNSL_HANDLE_VALID(cnsl)) return nullptr;
			CNSL_HANDLE_DECL;
			CNSL_HANDLE* hCon = (CNSL_HANDLE*)cnsl;
			return console::setunicode(cnsl, hCon->unicodeActive);
		}
	}
	#endif
	#if defined(ST_IMPORT_WINDOW)
	#undef ST_IMPORT_WINDOW
    #define ST_IMPORT_WINDOW 5
    #if defined(ST_IMPORT_CONSOLE)
    #define a \
    WND_HANDLE 
    #endif
	namespace window{
		FUNCDECL(void* create, void* father, const char* title, unsigned int width, unsigned int height, UWORD(*mailman)(void*,UWORD,UWORD,WORD)){
			if(!width || !height || !mailman || (father && WND_HANDLE_VALID(father)
			#if defined(ST_IMPORT_CONSOLE)
			&& CNSL_HANDLE_VALID(father)
			#endif
			)) return nullptr;
			WND_HANDLE_DECL;
			#if defined(ST_IMPORT_CONSOLE)
			CNSL_HANDLE_DECL;
			if(!CNSL_HANDLE_VALID(father)) father = ((CNSL_HANDLE*)father)->get_window();
			#endif
			#if defined(_WIN32)
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
				static WORD WINCALL st_InternalWndProc(void* hwnd, unsigned int msg, UWORD wp, WORD lp){
				    typedef WORD(WINCALL *GWL)(void*,int);
				    #if WORD_SIZE == 8
				    static GWL pGetLong = (GWL)get_system_function("GetWindowLongPtrA", "user32.dll");
				    #else
				    static GWL pGetLong = (GWL)get_system_function("GetWindowLongA", "user32.dll");
				    #endif
					void** mail = pGetLong ? (void**)pGetLong(hwnd, -21) : nullptr;
				    typedef WORD(WINCALL *DWP)(void*,unsigned int,UWORD,WORD);
				    static DWP pDef = (DWP)get_system_function("DefWindowProcA", "user32.dll");
				    typedef int(WINCALL *DWF)(void*);
				    static DWF pDestroyWindow = (DWF)get_system_function("DestroyWindow", "user32.dll");
				    typedef void*(WINCALL *GDC)(void*);
				    static GDC pGetDC = (GDC)get_system_function("GetDC", "user32.dll");
				    typedef int(WINCALL *RDC)(void*,void*);
				    static RDC pReleaseDC = (RDC)get_system_function("ReleaseDC", "user32.dll");
				    typedef int(WINCALL *SDIB)(void*,int,int,int,int,int,int,int,int,const void*,const void*,unsigned int,unsigned long);
				    static SDIB pStretchDIBits = (SDIB)get_system_function("StretchDIBits", "gdi32.dll");
				    typedef int(WINCALL *GCR)(void*,RECT*);
					static GCR pGetClientRect = (GCR)get_system_function("GetClientRect", "user32.dll");
				    typedef WORD(WINCALL *SM)(void*,unsigned int,UWORD,WORD);
		    		static SM pSendMessage = (SM)get_system_function("SendMessageA", "user32.dll");
				    typedef int(WINCALL *VR)(void*,void*);
		    		static VR pValidateRect = (VR)get_system_function("ValidateRect", "user32.dll");
					if(!mail) return pDef ? pDef(hwnd, msg, wp, lp) : -1;
					WND_HANDLE* handle = (WND_HANDLE*)(mail[1]);
				    if(msg == 0x0005 && (lp & 0xFFFF) != 0 && ((lp >> 16) & 0xFFFF) != 0){
				        int _old_width  = handle->width;
				        int _old_height = handle->height;
				        handle->width  = (int)(lp & 0xFFFF);
				        handle->height = (int)((lp >> 16) & 0xFFFF);
				        if(_old_width != handle->width || _old_height != handle->height){
							UWORD _new_pixels_amont = (UWORD)(handle->width) * (UWORD)(handle->height);
							UWORD _new_bytes = sizeof(unsigned int) * _new_pixels_amont;
							unsigned int* _new_pixels = (unsigned int*)memory::alloc(_new_bytes);
							if(_new_pixels){
								memory::slloc(_new_pixels, 0, _new_bytes);
								if(handle->pixels){
									int copy_w = (_old_width  < handle->width)  ? _old_width  : handle->width;
									int copy_h = (_old_height < handle->height) ? _old_height : handle->height;
									unsigned int *_n_p = _new_pixels, *_o_p = handle->pixels;
									while(copy_h > 0){
										memory::clloc(_n_p, _o_p, copy_w * sizeof(unsigned int));
										_n_p += handle->width; _o_p += _old_width; --copy_h;
									}
								}
								memory::free(handle->pixels);
								handle->pixels = _new_pixels;
								handle->pixels_amont = _new_pixels_amont;
							}else{
								handle->width  = _old_width;
								handle->height = _old_height;
							}
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
					UWORD ret = 0; if(mail[0]) ret = ((UWORD(*)(void*,UWORD,UWORD,WORD))(mail[0]))((void*)handle, (UWORD)msg, wp, lp);
					if(ret) return 0;
					if(msg == 0x0010){ if(pDestroyWindow) pDestroyWindow(hwnd); return 0; }
				    if(msg == 0x0002 || msg == 0x0012){
				    	handle->thr = nullptr;
				    	return 0;
				    }
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
					return pDef ? pDef(hwnd, msg, wp, lp) : -1;
				}
				static void dispatch(void** vargs){
				    THR_HANDLE_DECL;
				    struct MSG{
				        void* hwnd;
				        unsigned int message;
				        UWORD  wParam;
				        WORD   lParam;
				        unsigned long time;
				        struct{ int x, y; } pt;
				    } msg;
				    typedef int(WINCALL *PMF)(MSG*,void*,UWORD,UWORD,UWORD);
					static PMF pPeekMessage = (PMF)get_system_function("PeekMessageA", "user32.dll");
				    typedef int(WINCALL *TMF)(const MSG*);
				    static TMF pTranslateMessage = (TMF)get_system_function("TranslateMessage", "user32.dll");
				    typedef int(WINCALL *DMF)(const MSG*);
				    static DMF pDispatchMessage = (DMF)get_system_function("DispatchMessageA", "user32.dll");
					typedef void*(WINCALL *GMH)(const char*);
					static GMH pGetModuleHandle = (GMH)get_system_function("GetModuleHandleA");
					typedef unsigned short(WINCALL *REG)(const WNDCLASS*);
				    static REG pRegisterClass = (REG)get_system_function("RegisterClassA", "user32.dll");
				    typedef unsigned long(WINCALL *GLE)();
					static GLE pGetLastError = (GLE)get_system_function("GetLastError");
					typedef void*(WINCALL *CWE)(unsigned long,const char*,const char*,unsigned long,int,int,int,int,void*,void*,void*,void*);
		            static CWE pCreate = (CWE)get_system_function("CreateWindowExA", "user32.dll");
				    typedef WORD(WINCALL *SWL)(void*,int,WORD);
				    #if WORD_SIZE == 8
				    static SWL pSetLong = (SWL)get_system_function("SetWindowLongPtrA", "user32.dll");
				    #else
				    static SWL pSetLong = (SWL)get_system_function("SetWindowLongA", "user32.dll");
				    #endif
				    typedef int(WINCALL *SW)(void*,int);
				    static SW pShow = (SW)get_system_function("ShowWindow", "user32.dll");
				    typedef int(WINCALL *UW)(void*);
				    static UW pUpdate = (UW)get_system_function("UpdateWindow", "user32.dll");
				    typedef int(WINCALL *AWR)(struct RECT*, unsigned long, int);
					static AWR pAdjustWindowRect = (AWR)get_system_function("AdjustWindowRect", "user32.dll");
					if(!pGetModuleHandle || !pRegisterClass || !pGetLastError || !pCreate || !pSetLong || !pShow || !pUpdate || !pAdjustWindowRect){
						vargs[2] = (void*)1;
						return;
					}
					while(!*(void**)(vargs[3])) sleep(10);
					WND_HANDLE* hWnd = new WND_HANDLE;
		            int w = (int)(UWORD)(vargs[5]), h = (int)(UWORD)(vargs[6]);
					const char* t = (const char*)(vargs[4]); if(!t) t = "";
					WND_HANDLE* f = (WND_HANDLE*)(vargs[1]);
					void* thr = *(void**)(vargs[3]);
					hWnd->thr = thr;
					hWnd->father = f;
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
				    	delete hWnd;
						vargs[2] = (void*)1;
		            	return;
		            }
					hWnd->pixels_amont = (UWORD)w * (UWORD)h;
					hWnd->pixels = (unsigned int*)memory::alloc(sizeof(unsigned int) * hWnd->pixels_amont);
					hWnd->father = f;
					void* mimema[2] = {vargs[7], (void*)hWnd};
				    pSetLong(hWnd->hwnd, -21, (WORD)mimema);
					pShow(hWnd->hwnd, 1);
					pUpdate(hWnd->hwnd);
					hWnd->x = 100;
					hWnd->y = 100;
					hWnd->width = w;
					hWnd->height = h;
					vargs[0] = (void*)hWnd;
					vargs[2] = (void*)1;
					while(true)
					    if(pPeekMessage(&msg, hWnd->hwnd, 0, 0, 0x0001)){
					    	if(msg.message == 0x0012) break;
					        pTranslateMessage(&msg);
					        pDispatchMessage(&msg);
					    }
						else{
							if(!(hWnd->thr)) break;
							if(hWnd->father ? (!(*(UWORD*)(hWnd->father))) : false) break;
							sleep(10);
						}
				    pSetLong(hWnd->hwnd, -21, 0);
				    hWnd->pixels = (unsigned int*)memory::free(hWnd->pixels);
					if(!(hWnd->thr_join)) delete (THR_HANDLE*)thr;
				    hWnd->hwnd = nullptr; hWnd->thr = nullptr;
				    if(hWnd->self_destroy) delete hWnd;
				}
			};
			void* thr = nullptr;
			void* msg[8] = {nullptr, father, nullptr, (void*)&thr, (void*)title, (void*)(UWORD)width, (void*)(UWORD)height, (void*)mailman};
			thr = thread::create(_ap_::dispatch, (void*)msg);
			while(!msg[2]) sleep(10);
			if(!msg[0]) thread::join(thr);
            return msg[0];
			// a__uthor: Silver Roberskull
			// e__mition: 2026-11-10
			// v__ersion: 1.5
			// l__icense: MIT
			/* d__escription-note:
				All instances of '\S\Y\S' in the description are used to avoid the thief of the project.
				The original project name is '\S\y\s\t\e\m\ \T\o\o\l\s' (without the backslashes).
				ESP: Todo el proyecto fue escrito, dirigido y compilado en el sistema operativo Windows de forma completamente autodidacta por Silver Roberskull,
					toda ayuda de terceros fue unicamente concedida en forma de investigacion y aprendizaje, por lo que el proyecto es completamente original y no se ha copiado de ningun otro lugar.
					Agradecimientos especiales a 'Pau Fernandez' por sentar las bases y la inspiracion personal para poder adentrarme en el mundo del desarrollo de bajo nivel de C/C++.
			*/
			// d__escription: \S\Y\S for C++ (STC++)
			// o__riginal-platform-writing: Windows
            #else // Linux
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
				#define XEvent_DECL struct XAnyEvent{\
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
				static void dispach(void** vargs){
					WND_HANDLE* handle = (WND_HANDLE*)(vargs[0]);
					UWORD(*mailman)(void*,UWORD,UWORD,WORD) = (UWORD(*)(void*,UWORD,UWORD,WORD))(vargs[1]);
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
						int*,unsigned long*,unsigned long*,BYTE**);
					static XGP pXGetWindowProperty = (XGP)get_system_function("XGetWindowProperty", "libX11.so.6");
					typedef int(*XWP)(void*, unsigned long, unsigned long, int, int, unsigned int, unsigned int, int, int);
					static XWP pXWarpPointer = (XWP)get_system_function("XWarpPointer", "libX11.so.6");
					typedef int(*XFL)(void*);
					static XFL pXFlush = (XFL)get_system_function("XFlush", "libX11.so.6");
					if(!pXNextEvent || !pXCreateImage || !pXPutImage || !pXFree || !pXInternAtom
						|| !pXDestroyWindow || !pXCloseDisplay || !pXFreeGC || !pXGetWindowProperty
						|| !pXWarpPointer || !pXFlush){
						if(handle->pixels) delete[] handle->pixels; handle->pixels = nullptr;
						vargs[0] = nullptr;
						vargs[2] = (void*)1;
						return;
					}
					vargs[2] = (void*)1;
					unsigned long wmProc      = pXInternAtom(handle->display, "WM_PROTOCOLS", 0);
					unsigned long wmStateAtom = pXInternAtom(handle->display, "WM_STATE", 0);
					unsigned long wmDelete    = pXInternAtom(handle->display, "WM_DELETE_WINDOW", 0);
					UWORD msgSend, wp, lp;
					while(true){
						XEvent ev = {0};
						if(handle->father ? (!(*(UWORD*)(handle->father))) : false) break;
						pXNextEvent(handle->display, &ev);
						if(handle->father ? (!(*(UWORD*)(handle->father))) : false) break;
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
								BYTE* prop = nullptr;
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
					if(!(handle->thr_join) && handle->thr) delete (THR_HANDLE*)(handle->thr); handle->thr = nullptr;
					if(handle->self_destroy) delete handle;
				}
			};
			pXSelectInput(handle->display, handle->window, 0x62804F);
			unsigned long wmDelete = pXInternAtom(handle->display, "WM_DELETE_WINDOW", 0);
			pXSetWMProtocols(handle->display, handle->window, &wmDelete, 1);
			pXMapWindow(handle->display, handle->window);
			pXSync(handle->display, 0);
			if(title) pXStoreName(handle->display, handle->window, title);
			void* vargs[3] = {(void*)handle, (void*)mailman, nullptr};
			handle->thr = thread::create(_ap_::dispach, vargs);
			while(!(vargs[2])) sleep(10);
			if(!(vargs[0])){
				thread::join(handle->thr); handle->thr = nullptr;
				delete handle; handle = nullptr;
			}
			return (void*)handle;
            #endif
		}
		FUNCDECL(void* sendmessage, void* wnd, UWORD msg, UWORD wp, WORD lp){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			#if defined(_WIN32)
			typedef WORD(WINCALL *SM)(void*,unsigned int,UWORD,WORD);
		    static SM pSendMessage = (SM)get_system_function("SendMessageA", "user32.dll");
		    if(!pSendMessage) return nullptr;
		    pSendMessage(handle->hwnd, (unsigned int)msg, (UWORD)wp, (WORD)lp);
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
			 if(msg == window_box::close || msg == window_box::destroy || msg == window_box::quit){
				ev.type = 33; // ClientMessage
				ev.xclient.window = handle->window;
				ev.xclient.message_type = pXInternAtom(handle->display, "WM_PROTOCOLS", 0);
				ev.xclient.format = 32;
				ev.xclient.data.l[0] = pXInternAtom(handle->display, "WM_DELETE_WINDOW", 0);
			}if(msg == window_box::size){
				ev.type = 22; // ConfigureNotify
				ev.xconfigure.window = handle->window;
				ev.xconfigure.x = (short)(wp & 0xFFFF);
				ev.xconfigure.y = (short)((wp >> 16) & 0xFFFF);
				ev.xconfigure.width = (short)(lp & 0xFFFF);
				ev.xconfigure.height = (short)((lp >> 16) & 0xFFFF);
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
			}if(msg >= window_box::lbutton_down && msg <= window_box::rbutton_hold){
				// Simple mapping: Left=1, Right=3.
				// 0x0201=LDown, 0x0202=LUp, 0x0203=LHold, 0x0204=RDown, 0x0205=RUp, 0x0206=RHold
				// The holds are the double-click ids, so they arrive as presses.
				bool left = (msg == window_box::lbutton_down || msg == window_box::lbutton_up || msg == window_box::lbutton_hold);
				bool press = (msg == window_box::lbutton_down || msg == window_box::rbutton_down || msg == window_box::lbutton_hold || msg == window_box::rbutton_hold);
				ev.type = press ? 4 : 5; // ButtonPress : ButtonRelease
				ev.xbutton.button = left ? 1 : 3;
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
		FUNCDECL(void* wait, void* wnd){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(handle->ext){
				#if defined(_WIN32)
				typedef int (WINCALL *IW)(void*);
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
				    while(1){
				        pXNextEvent(handle->display, &ev);
				        if(ev.type == 17 && ev.xany.window == handle->window) break;
				    }
				}
				#endif
				delete handle;
				return wnd;
			}
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			handle->thr_join = true; handle->self_destroy = false;
			thread::join(handle->thr);
			delete handle;
			return wnd;
		}
		FUNCDECL(void* destroy, void* wnd, bool _wait){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
		    WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(handle->ext){
				window::sendmessage(wnd, window_box::close, 0, 0);
				delete handle;
				return wnd;
			}
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			void* thr = nullptr;
			if(_wait){
				handle->self_destroy = false;
				handle->thr_join = true;
				thr = handle->thr;
			}
			window::sendmessage(wnd, window_box::close, 0, 0);
			thread::join(thr);
			if(_wait) delete handle;
			return wnd;
		}
		FUNCDECL(void* draw, void* wnd, int offsetX, int offsetY, unsigned int* _pixels, unsigned int _width, unsigned int _height){
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
		FUNCDECL(void  pixels, void* wnd, unsigned int** _px, UWORD* _px_am){
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
		FUNCDECL(void* clear, void* wnd, unsigned int _color){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
		    WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle) || handle->ext) return nullptr;
		    if(!handle->pixels || !(handle->pixels_amont)) return wnd;
		    unsigned int* _i_p = handle->pixels + handle->pixels_amont;
		    while(_i_p-- != handle->pixels) *_i_p = _color;
			return window::sendmessage(wnd, window_box::paint, 0, 0);
		}
		FUNCDECL(void* resize, void* wnd, int _x, int _y, int _width, int _height, bool SetPosition, bool SetSize){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			if(!SetPosition && !SetSize) return wnd; // (SetPosition == false && SetSize == false) => "Its equivalent to doing nothing".
			if(SetSize && (_width < 1 || _height < 1)) return nullptr; // A zero-pixel surface is not a size, it is a minimized window.
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			#if defined(_WIN32)
			struct RECT{ int left, top, right, bottom; };
			typedef int(WINCALL *SWP)(void*,void*,int,int,int,int,unsigned int);
		    static SWP pSetWindowPos = (SWP)get_system_function("SetWindowPos", "user32.dll");
		    typedef int(WINCALL *AWR)(RECT*, unsigned long, int);
		    static AWR pAdjustWindowRect = (AWR)get_system_function("AdjustWindowRect", "user32.dll");
		    typedef long (WINCALL *GWL)(void*, int);
		    #if WORD_SIZE == 8
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
		FUNCDECL(void  getsize, void* wnd, int* _x, int* _y, int* _width, int* _height){
			if(WND_HANDLE_VALID(wnd) || (!_x && !_y && !_width && !_height)) return;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle) || handle->ext) return;
			if(_x)      *_x      = handle->x;
			if(_y)      *_y      = handle->y;
			if(_width)  *_width  = handle->width;
			if(_height) *_height = handle->height;
		}
		FUNCDECL(void* father, void* wnd){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			return (void*)(handle->father);
		}
		FUNCDECL(void* focus, void* wnd){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			#if defined(_WIN32)
			typedef int(WINCALL *SFW)(void*);
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
		FUNCDECL(void* retitle, void* wnd, const char* title){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			#if defined(_WIN32)
			typedef int(WINCALL *SWT)(void*,const char*);
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
		FUNCDECL(void* gettitle, void* wnd, char** title, bool* minimized){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			if(!title && !minimized) return wnd;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			#if defined(_WIN32)
			typedef int(WINCALL *GWT)(void*,char*,int);
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
		FUNCDECL(void  getscreen, int* _width, int* _height){
			if(!_width && !_height) return;
			#if defined(_WIN32)
			typedef int(WINCALL *GSM)(int);
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
		FUNCDECL(void* show, void* wnd, bool visible, bool minimize){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			#if defined(_WIN32)
			typedef int(WINCALL *SW)(void*,int);
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
		FUNCDECL(void* focuswait, void* wnd){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			#if defined(_WIN32)
			typedef void* (WINCALL *GFW)();
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
		FUNCDECL(void* mouse, void* wnd, int* _x, int* _y, int* lclick, int* rclick){
			if(WND_HANDLE_VALID(wnd) || (!_x && !_y && !lclick && !rclick)) return nullptr;
			WND_HANDLE_DECL;
		    WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			bool currl = false;
			bool currr = false;
			#if defined(_WIN32)
		    struct POINT{ long x, y; } pt;
		    typedef int(WINCALL *GCP)(POINT*);
		    static GCP pGetCursorPos = (GCP)get_system_function("GetCursorPos", "user32.dll");
		    typedef int(WINCALL *STC)(void*,POINT*);
		    static STC pScreenToClient = (STC)get_system_function("ScreenToClient", "user32.dll");
		    typedef short(WINCALL *GAKS)(int);
		    static GAKS pGetAsyncKeyState = (GAKS)get_system_function("GetAsyncKeyState", "user32.dll");
		    if(!pGetCursorPos || !pScreenToClient || !pGetAsyncKeyState) return nullptr;
		    pGetCursorPos(&pt); pScreenToClient(handle->hwnd, &pt);
		    if(_x) *_x = (int)(pt.x);
		    if(_y) *_y = (int)(pt.y);
		    currl = (pGetAsyncKeyState(0x01) & 0x8000) != 0;
		    currr = (pGetAsyncKeyState(0x02) & 0x8000) != 0;
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
					static CLOSE pClose = (CLOSE)get_system_function("close");
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
					static POPEN pOpen = (POPEN)get_system_function("open");
					typedef WORD(*PREAD)(int,void*,UWORD);
					static PREAD pRead = (PREAD)get_system_function("read");
					typedef int(*CLOSE)(int);
					static CLOSE pClose = (CLOSE)get_system_function("close");
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
						BYTE packet[3];
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
		    if(!pXQueryPointer) return nullptr;
			unsigned long root_ret, child_ret; int rx, ry, wx, wy; unsigned int mask;
			if(!pXQueryPointer(_ba_.globalDpy, handle->window, &root_ret, &child_ret, &rx, &ry, &wx, &wy, &mask)) return nullptr;
			if(_x) *_x = wx;
			if(_y) *_y = wy;
			currl = (mask & 0x100) != 0;
			currr = (mask & 0x400) != 0;
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
			return wnd;
		}
		FUNCDECL(void* keyboard, void* wnd, BYTE* keys, UWORD size){
			if(WND_HANDLE_VALID(wnd) || !keys || !size) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			if(!WND_HANDLE_STATE(handle)) return nullptr;
			UWORD i = 32; if(i > size) i = size; memory::slloc(keys, 0, i); i = 0;
			#define ___language__set_the_ret__1_2__3__(pos) keys[(pos)>>3]^=(1<<((pos)&7))
			#if defined(_WIN32)
		    typedef short(WINCALL *GAKS)(int);
		    static GAKS pGetAsyncKeyState = (GAKS)get_system_function("GetAsyncKeyState", "user32.dll");
		    typedef int(WINCALL *GKS)(BYTE*);
		    static GKS pGetKeyboardState = (GKS)get_system_function("GetKeyboardState", "user32.dll");
		    typedef int(WINCALL *TVA)(unsigned int,unsigned int,BYTE*,unsigned short*,unsigned int);
		    static TVA pToAscii = (TVA)get_system_function("ToAscii", "user32.dll");
		    typedef void*(WINCALL *GFW)();
		    static GFW pGetForegroundWindow = (GFW)get_system_function("GetForegroundWindow", "user32.dll");
		    typedef int(WINCALL *MVK)(unsigned int,unsigned int);
		    static MVK pMapVirtualKey = (MVK)get_system_function("MapVirtualKeyA", "user32.dll");
		    if(!pGetAsyncKeyState || !pGetKeyboardState || !pToAscii || !pGetForegroundWindow || !pMapVirtualKey) return nullptr;
		    if(pGetForegroundWindow() != handle->hwnd) return nullptr;
		    BYTE kState[256]; pGetKeyboardState(kState);
		    while(++i != 256 && i < (size << 3)) if(pGetAsyncKeyState(i) & 0x8000){
				unsigned short translated = 0;
				if(pToAscii(i, pMapVirtualKey(i, 0), kState, &translated, 0) > 0) ___language__set_the_ret__1_2__3__(translated);
				else                                                              ___language__set_the_ret__1_2__3__(i);
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
			typedef int(*XQKM)(void*,BYTE*);
			static XQKM pXQueryKeymap = (XQKM)get_system_function("XQueryKeymap", "libX11.so.6");
			typedef int(*XLS)(void*,void*,int,unsigned long*,void*);
			static XLS pXLookupString = (XLS)get_system_function("XLookupString", "libX11.so.6");
			typedef int(*XGIF)(void*, unsigned long*, int*);
			static XGIF pXGetInputFocus = (XGIF)get_system_function("XGetInputFocus", "libX11.so.6");
			if(!(_ba_.globalDpy) || !pXQueryKeymap || !pXLookupString || !pXGetInputFocus) return nullptr;
			unsigned long focus_win = 0;
			int revert = 0;
			pXGetInputFocus(_ba_.globalDpy, &focus_win, &revert);
			if(focus_win <= 2) return nullptr;
			if(handle->window != focus_win) return nullptr;
			BYTE keys_return[32]; pXQueryKeymap(_ba_.globalDpy, keys_return);
			while(++i != 256 && i < (size << 3)) if(keys_return[i >> 3] & (1 << (i & 0x00000007))){
				XKeyEvent ev = {0};
				ev.display = _ba_.globalDpy;
				ev.keycode = i;
				BYTE buf[8];
				if(pXLookupString(&ev, buf, sizeof(buf), nullptr, nullptr) > 0) ___language__set_the_ret__1_2__3__(buf[0]);
				else                                                            ___language__set_the_ret__1_2__3__(i);
			}
			#endif
			#undef ___language__set_the_ret__1_2__3__
			return wnd;
		}
		FUNCDECL(void* keyboard, void* wnd, BYTE** keys, UWORD* size, bool stacked){
			if(size) *size = 32;
			if(WND_HANDLE_VALID(wnd) || (!keys && !size)) return nullptr;
			BYTE _aux[32];
			if(!window::keyboard(wnd, _aux, 32)) return nullptr;
			if(keys){
				if(*keys && stacked) delete[] *keys;
				if(stacked) *keys = new BYTE[32];
				else        *keys = (BYTE*)memory::realloc(*keys, 32);
				memory::clloc(*keys, _aux, 32);
			}
			return wnd;
		}
		FUNCDECL(void* active){
			WND_HANDLE_DECL;
			#if defined(_WIN32)
			typedef void* (WINCALL *GFW)();
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
		FUNCDECL(bool is_extern, void* wnd){
			if(WND_HANDLE_VALID(wnd)) return false;
		    WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			return handle->ext;
		}
		FUNCDECL(void* release, void* wnd){
			if(WND_HANDLE_VALID(wnd)) return nullptr;
			WND_HANDLE_DECL;
			WND_HANDLE* handle = (WND_HANDLE*)wnd;
			// An owned window keeps a dispatch thread that reads this handle: only wait or destroy may free it.
			if(!(handle->ext)) return nullptr;
			delete handle;
			return wnd;
		}
    	FUNCDECL(bool yield, void* wnd, bool _wait){
    		if(WND_HANDLE_VALID(wnd)) return false;
    		WND_HANDLE_DECL;
    		bool closesing = !WND_HANDLE_STATE((WND_HANDLE*)wnd);
    		if(_wait) sleep(10);
    		return !closesing;
    	}
	}
	#endif
	#if defined(ST_IMPORT_FILE)
	#undef ST_IMPORT_FILE
    #define ST_IMPORT_FILE 2
	namespace file{
		FUNCDECL(void* open, const char* filepath, BYTE openMode){
			if(!filepath || openMode == file_mode::none || ((!(openMode & file_mode::read)) && (!(openMode & file_mode::write)))) return nullptr;
			#define FLE_HANDLE_VALID(ptr) ((ptr)?(*(UWORD*)(ptr)!=0x46494C45):true)
			#if defined(_WIN32)
			#define FLE_HANDLE_DECL struct FLE_HANDLE{\
				UWORD magic;\
				void* os_handle;\
				BYTE open_mode;\
				bool is_dir;\
				char* full_path;\
				UWORD full_spath;\
				FLE_HANDLE() : magic(0x46494C45), os_handle((void*)-1), open_mode(0), is_dir(false), full_path(nullptr), full_spath(0) {}\
				~FLE_HANDLE(){ magic = 0x00; if(full_path) delete[] full_path; }\
			}
			FLE_HANDLE_DECL;
			typedef void*(WINCALL *CFA)(const char*,unsigned int,unsigned int,void*,unsigned int,unsigned int,void*);
		    static CFA pCreateFileA = (CFA)get_system_function("CreateFileA");
		    typedef UWORD(WINCALL *GFP)(const char*,UWORD,char*,UWORD*);
		    static GFP pGetFullPathNameA = (GFP)get_system_function("GetFullPathNameA");
		    if(!pCreateFileA || !pGetFullPathNameA) return nullptr;
		    unsigned int access = 0;
		    unsigned int disp = 3;
		    if(openMode & file_mode::read)   access |= 0x80000000;
		    if(openMode & file_mode::write)  access |= 0x40000000;
		    if(openMode & file_mode::create) disp = 2;
		    void* h = pCreateFileA(filepath, access, 3, nullptr, disp, 128, nullptr);
		    if(h == (void*)-1) return nullptr;
		    UWORD req = pGetFullPathNameA(filepath, 0, nullptr, nullptr);
		    char* abs_path = new char[req];
		    pGetFullPathNameA(filepath, req, abs_path, nullptr);
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
				BYTE open_mode;\
				bool is_dir;\
				char* full_path;\
				UWORD full_spath;\
				FLE_HANDLE() : magic(0x46494C45), fd(0), open_mode(0), is_dir(false), full_path(nullptr), full_spath(0) {}\
				~FLE_HANDLE(){ magic = 0x00; if(full_path) delete[] full_path; }\
			}
			FLE_HANDLE_DECL;
			typedef int(*POPEN)(const char*,int,...);
		    static POPEN pOpen = (POPEN)get_system_function("open");
			typedef char*(*RPATH)(const char*,char*);
			static RPATH pRealpath = (RPATH)get_system_function("realpath");
		    typedef void(*FREE)(void*);
		    static FREE pFree = (FREE)get_system_function("free");
		    if(!pOpen || !pRealpath || !pFree) return nullptr;
		    int flags = 0;
		    if(openMode & file_mode::write){
				                               flags = 1;
				if(openMode & file_mode::read) flags = 2;
			}
		    if(openMode & file_mode::create)   flags |= 0x40;
		    int fd = pOpen(filepath, flags, 0x1B4);
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
		FUNCDECL(void* close, void* file){
			if(FLE_HANDLE_VALID(file)) return nullptr;
			FLE_HANDLE_DECL;
			FLE_HANDLE* handle = (FLE_HANDLE*)file;
			#if defined(_WIN32)
			typedef int(WINCALL *CH)(void*);
			static CH pCloseHandle = (CH)get_system_function("CloseHandle");
			if(!pCloseHandle || handle->os_handle == (void*)-1) return nullptr;
			pCloseHandle(handle->os_handle); handle->os_handle = (void*)-1;
			#else // Linux
			typedef int(*CLOSE)(int);
			static CLOSE pClose = (CLOSE)get_system_function("close");
			if(!pClose || !(handle->fd)) return nullptr;
			pClose(handle->fd); handle->fd = 0;
			#endif
			delete handle;
			return file;
		}
		FUNCDECL(const char* remove, const char* filepath){
			if(!file::exist(filepath)) return nullptr;
			#if defined(_WIN32)
		    typedef int(WINCALL *DF)(const char*);
		    static DF pDeleteFileA = (DF)get_system_function("DeleteFileA");
		    static DF pRemoveDirectoryA = (DF)get_system_function("RemoveDirectoryA");
		    if(!pDeleteFileA || !pRemoveDirectoryA) return nullptr;
		    if(pDeleteFileA(filepath)) return filepath;
		    struct WIN32_FIND_DATAA{
		        unsigned int attr; unsigned int t[6]; unsigned int s[2], r[2];
		        char name[260], aname[14];
		    } fData;
		    typedef void*(WINCALL *FFF)(const char*,void*);
		    static FFF pFindFirst = (FFF)get_system_function("FindFirstFileA");
		    typedef int(WINCALL *FNF)(void*,void*);
		    static FNF pFindNext = (FNF)get_system_function("FindNextFileA");
		    typedef int(WINCALL *FC)(void*);
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
		            pattern = fData.name;
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
		    static UNLINK pUnlink = (UNLINK)get_system_function("unlink");
		    typedef int(*RMDIR)(const char*);
		    static RMDIR pRmdir = (RMDIR)get_system_function("rmdir");
		    if(!pUnlink || !pRmdir) return nullptr;
		    if(!pUnlink(filepath)) return filepath;
		    // The refusal means a directory: empty it first, the same way the Windows branch does.
		    {
			    struct Sdir{
			    	unsigned long i, o;
			    	unsigned short r;
			    	BYTE t;
			    	char name[256];
			    };
			    typedef void*(*ODIR)(const char*);
			    static ODIR pOpendir = (ODIR)get_system_function("opendir");
			    typedef Sdir*(*RDIR)(void*);
			    static RDIR pReaddir = (RDIR)get_system_function("readdir");
			    typedef int(*CDIR)(void*);
			    static CDIR pClosedir = (CDIR)get_system_function("closedir");
			    if(!pOpendir || !pReaddir || !pClosedir) return nullptr;
			    UWORD filepath_size = 0; { const char* _l = filepath; while(*_l++) ++filepath_size; }
			    void* dir = pOpendir(filepath);
			    if(!dir) return nullptr;
			    bool emptied = true;
			    Sdir* entry = nullptr;
			    while(entry = pReaddir(dir)){
			    	if(entry->name[0] == '.' && (entry->name[1] == '\0' || (entry->name[1] == '.' && entry->name[2] == '\0'))) continue;
			    	UWORD name_size = 0;
			    	while(name_size != 255 && entry->name[name_size]) ++name_size;
			    	char* full_child_path = new char[filepath_size + name_size + 2];
			    	sprintf(full_child_path, "%s/%s", filepath, entry->name);
			    	if(!(file::remove(full_child_path))) emptied = false;
			    	delete[] full_child_path;
			    }
			    pClosedir(dir);
			    if(!emptied) return nullptr;
		    }
		    return pRmdir(filepath) ? nullptr : filepath;
			#endif
		}
		FUNCDECL(void* remove, void* file){
			if(FLE_HANDLE_VALID(file)) return nullptr;
		    FLE_HANDLE_DECL;
            FLE_HANDLE* handle = (FLE_HANDLE*)file;
            char* path = handle->full_path; handle->full_path = nullptr;
            file::close(file);
			bool Removed = (file::remove(path) == path);
			delete[] path;
			return Removed ? file : nullptr;
		}
		FUNCDECL(const char* copy, const char* source, const char* dest, bool overwrite){
			if(!source || !dest) return nullptr;
			if(!*source || !*dest) return nullptr;
			if(!file::exist(source)) return nullptr;
			#if defined(_WIN32)
		    typedef unsigned int(WINCALL *GFA)(const char*);
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
				unsigned short d_reclen;   BYTE  d_type;
				char           d_name[256];
			};
			typedef int(*STAT_FN)(const char*,void*);
        	static STAT_FN pStat = (STAT_FN)get_system_function("stat");
        	if(!pStat) return nullptr;
		    if(pStat(source, &st)) return nullptr;
		    bool is_dir = ((st.st_mode) & 0xF000) == 0x4000;
		    #endif
		    if(is_dir){
		        #if defined(_WIN32)
		        struct WIN32_FIND_DATAA{
			        unsigned int attr;
					unsigned int t[6];
					unsigned int s[2], r0, r1;
					char name[260], aname[14];
			    } fData;
		        static void*(WINCALL *pFindFirst)(const char*,void*) = (void*(WINCALL *)(const char*,void*))get_system_function("FindFirstFileA");
		        static int(WINCALL *pFindNext)(void*,void*) = (int(WINCALL *)(void*,void*))get_system_function("FindNextFileA");
		        static int(WINCALL *pFindClose)(void*) = (int(WINCALL *)(void*))get_system_function("FindClose");
                if(!pFindFirst || !pFindNext || !pFindClose) return nullptr;
		        char path_buffer[1024];
		        sprintf(path_buffer, "%s\\*", source);
		        void* hFind = pFindFirst(path_buffer, &fData);
		        if(hFind == (void*)-1) return nullptr;
		        void* hdest = file::folder(dest, file_mode::create | file_mode::write);
				if(!hdest){ pFindClose(hFind); return nullptr; }
				file::close(hdest);
		        char src_child[1024], dst_child[1024];
		        do{
		            if(fData.name[0] == '.' && (fData.name[1] == '\0' || (fData.name[1] == '.' && fData.name[2] == '\0'))) continue;
		            sprintf(src_child, "%s\\%s", source, fData.name);
		            sprintf(dst_child, "%s\\%s", dest, fData.name);
		            if(!file::copy(src_child, dst_child, overwrite)){ pFindClose(hFind); return nullptr; }
		        }while(pFindNext(hFind, &fData));
		        pFindClose(hFind);
		        #else
		        typedef void*(*OPENDIR_FN)(const char*);
		        static OPENDIR_FN pOpenDir = (OPENDIR_FN)get_system_function("opendir");
		        typedef _dirent_linux*(*READDIR_FN)(void*);
		        static READDIR_FN pReadDir = (READDIR_FN)get_system_function("readdir");
		        typedef int(*CLOSEDIR_FN)(void*);
		        static CLOSEDIR_FN pCloseDir = (CLOSEDIR_FN)get_system_function("closedir");
                typedef int(*STAT_FN)(const char*,void*);
                static STAT_FN pStat = (STAT_FN)get_system_function("stat");
		        if(!pOpenDir || !pReadDir || !pCloseDir) return nullptr;
                _stat_linux sattr;
                if(pStat) if(!pStat(dest, &sattr))
                    if((sattr.st_mode & 0xF000) == 0x4000)
                        return nullptr;
		        void* dir = pOpenDir(source);
		        if(!dir) return nullptr;
		        void* hdest = file::folder(dest, file_mode::create | file_mode::write);
				if(!hdest){ pCloseDir(dir); return nullptr; }
				file::close(hdest);
		        _dirent_linux* entry = nullptr;
		        char src_child[1024], dst_child[1024];
		        while((entry = pReadDir(dir)) != nullptr){
		            const char* fname = entry->d_name;
		            if(fname[0] == '.' && (fname[1] == '\0' || (fname[1] == '.' && fname[2] == '\0'))) continue;
		            sprintf(src_child, "%s/%s", source, fname);
		            sprintf(dst_child, "%s/%s", dest, fname);
		            if(!file::copy(src_child, dst_child, overwrite)){ pCloseDir(dir); return nullptr; }
		        }
		        pCloseDir(dir);
		        #endif
		        return dest;
		    } 
		    #if defined(_WIN32)
		    typedef int(WINCALL *CFA)(const char*,const char*,int);
		    static CFA pCopyFileA = (CFA)get_system_function("CopyFileA");
		    if(!pCopyFileA) return nullptr;
		    if(!pCopyFileA(source, dest, !overwrite)) return nullptr;
		    #else
		    typedef int(*OPEN_FN)(const char*,int,...);
		    static OPEN_FN pOpen = (OPEN_FN)get_system_function("open");
		    typedef int(*CLOSE_FN)(int);
		    static CLOSE_FN pClose = (CLOSE_FN)get_system_function("close");
		    typedef long(*SENDFILE_FN)(int,int,long*,unsigned long);
		    static SENDFILE_FN pSendFile = (SENDFILE_FN)get_system_function("sendfile");
		    if(!pOpen || !pClose || !pSendFile) return nullptr;
		    int src_fd = pOpen(source, 0);
			if(src_fd < 0) return nullptr;
		    int dst_fd = pOpen(dest, 0x41 | (overwrite ? 0x200 : 0x80), 0x1B4);
			if(dst_fd < 0){ pClose(src_fd); return nullptr; }
			unsigned long off = 0;
			while(off < (unsigned long)st.st_size){
			    long sent = pSendFile(dst_fd, src_fd, (long*)&off, st.st_size - off);
			    if(sent <= 0){ pClose(src_fd); pClose(dst_fd); return nullptr; }
			}
		    pClose(src_fd); pClose(dst_fd);
		    #endif
		    return dest;
		}
		FUNCDECL(const char* move, const char* old_path, const char* new_path, bool overwrite){
            if(!old_path || !new_path) return nullptr;
			if(!*old_path || !*new_path) return nullptr;
			if(!file::exist(old_path)) return nullptr;
			if(!overwrite && file::exist(new_path)) return nullptr;
			#if defined(_WIN32)
            typedef int(WINCALL *MFE)(const char*, const char*, unsigned int);
            static MFE pMoveFileExA = (MFE)get_system_function("MoveFileExA");
            if(!pMoveFileExA) return nullptr;
            if(pMoveFileExA(old_path, new_path, 0x2 | (overwrite ? 0x1 : 0x0))) return new_path;
            #else // Linux
            typedef int(*RENAME)(const char*, const char*);
            static RENAME pRename = (RENAME)get_system_function("rename");
            if(!pRename) return nullptr;
            if(!pRename(old_path, new_path)) return new_path;
            #endif
            if(file::copy(old_path, new_path, overwrite)){
                file::remove(old_path);
                return new_path;
            }
            return nullptr;
		}
		FUNCDECL(void* parent, void* file_ptr){
            if(FLE_HANDLE_VALID(file_ptr)) return nullptr;
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
        FUNCDECL(UWORD timestamp, void* file, BYTE type){
        	if(FLE_HANDLE_VALID(file)) return 0;
        	if(type > 2) type = 2;
        	FLE_HANDLE_DECL;
            FLE_HANDLE* h = (FLE_HANDLE*)file;
            #if defined(_WIN32)
            unsigned long long ft[3];
            typedef int(WINCALL *GFT)(void*,void*,void*,void*);
            static GFT pGetFileTime = (GFT)get_system_function("GetFileTime");
            if(!pGetFileTime) return 0;
            if(pGetFileTime(h->os_handle, ft, ft+1, ft+2))
                return (UWORD)((ft[type] / 10000000ULL) - 11644473600ULL);
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
            static FSTAT pFstat = (FSTAT)get_system_function("fstat");
            if(!pFstat) return 0;
            if(!pFstat(h->fd, &st)){
                if(type == time_mode::creation) return (UWORD)(st.ctime);
                if(type == time_mode::access)   return (UWORD)(st.atime);
                return (UWORD)(st.mtime);
            }
            #endif
            return 0;
		}
		FUNCDECL(UWORD timestamp, const char* filepath, BYTE type){
			void* _h = file::open(filepath, file_mode::read);
			if(!_h) _h = file::folder(filepath, file_mode::read);
			if(!_h) return 0;
			UWORD ret = timestamp(_h, type);
			file::close(_h);
			return ret;
		}
		FUNCDECL(UWORD read, void* file, void* buffer, UWORD bytes){
			if(FLE_HANDLE_VALID(file) || !buffer || !bytes) return 0;
			FLE_HANDLE_DECL;
			FLE_HANDLE* handle = (FLE_HANDLE*)file;
			if(!((handle->open_mode) & file_mode::read)) return 0;
			#if defined(_WIN32)
			typedef int(WINCALL *RF)(void*,void*,unsigned int,unsigned int*,void*);
            static RF pReadFile = (RF)get_system_function("ReadFile");
            if(!pReadFile || handle->os_handle == (void*)-1) return 0;
            unsigned int readOut = 0;
            pReadFile(handle->os_handle, buffer, bytes, &readOut, nullptr);
			#else // Linux
			typedef long (*PREAD)(int,void*,unsigned long);
            static PREAD pRead = (PREAD)get_system_function("read");
			if(!pRead || !(handle->fd)) return 0;
            long readOut = pRead(handle->fd, buffer, (unsigned long)bytes);
            readOut = (readOut >= 0) ? readOut : 0;
			#endif
			return (UWORD)readOut;
		}
		FUNCDECL(UWORD write, void* file, const void* buffer, UWORD bytes){
			if(FLE_HANDLE_VALID(file) || !buffer || !bytes) return 0;
			FLE_HANDLE_DECL;
			FLE_HANDLE* handle = (FLE_HANDLE*)file;
			if(!((handle->open_mode) & file_mode::write)) return 0;
			#if defined(_WIN32)
			typedef int(WINCALL *WF)(void*,const void*,unsigned int,unsigned int*,void*);
            static WF pWriteFile = (WF)get_system_function("WriteFile");
			if(!pWriteFile || handle->os_handle == (void*)-1) return 0;
            unsigned int writtenOut = 0;
            pWriteFile(handle->os_handle, buffer, bytes, &writtenOut, nullptr);
            UWORD writeIn = (UWORD)writtenOut;
			#else // Linux
			typedef long (*PWRITE)(int,const void*,unsigned long);
            static PWRITE pWrite = (PWRITE)get_system_function("write");
			if(!pWrite || !(handle->fd)) return 0;
            long n = pWrite(handle->fd, buffer, (unsigned long)bytes);
            UWORD writeIn = (n >= 0) ? ((UWORD)n) : 0;
			#endif
			return writeIn;
		}
		FUNCDECL(UWORD size, void* file){
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
		            unsigned int ftCreationTime[2], ftLastAccessTime[2], ftLastWriteTime[2];
		            unsigned int nFileSizeHigh, nFileSizeLow, dwReserved0, dwReserved1;
		            char cFileName[260], cAlternateFileName[14];
		        } findData;
		        typedef void*(WINCALL *FFF)(const char*,void*);
		        static FFF pFindFirstFileA = (FFF)get_system_function("FindFirstFileA");
		        typedef int(WINCALL *FNF)(void*,void*);
		        static FNF pFindNextFileA  = (FNF)get_system_function("FindNextFileA");
		        typedef int(WINCALL *FC)(void*);
		        static FC pFindClose = (FC)get_system_function("FindClose");
		        void* hFind = pFindFirstFileA ? pFindFirstFileA(search_path, &findData) : (void*)-1;
		        if(hFind != (void*)-1){
		            do
						if(findData.cFileName[0] != '.' || (findData.cFileName[1] != '\0' && (findData.cFileName[1] != '.' || findData.cFileName[2] != '\0'))) ++count;
					while(pFindNextFileA(hFind, &findData));
		            pFindClose(hFind);
			        }
				#else // Linux
				struct Sdir{
					unsigned long d_ino; unsigned long d_off;
					unsigned short d_reclen; BYTE d_type;
					char d_name[256];
				};
		        typedef void*(*ODIR)(const char*);
		        static ODIR pOpendir = (ODIR)get_system_function("opendir");
		        typedef Sdir*(*RDIR)(void*);
		        static RDIR pReaddir = (RDIR)get_system_function("readdir");
		        typedef int(*CDIR)(void*);
		        static CDIR pClosedir = (CDIR)get_system_function("closedir");
		        void* dir = pOpendir ? pOpendir(handle->full_path) : nullptr;
		        if(dir){
		            Sdir* entry = nullptr;
		            while(entry = pReaddir(dir))
                        if(entry->d_name[0] != '.' || (entry->d_name[1] != '\0' && (entry->d_name[1] != '.' || entry->d_name[2] != '\0'))) ++count;
		            pClosedir(dir);
		        }
				#endif
		        return count;
		    }
            #if defined(_WIN32)
            typedef int(WINCALL *GSF)(void*,unsigned int*);
            static GSF pGetFileSize = (GSF)get_system_function("GetFileSize");
            if(!pGetFileSize || handle->os_handle == (void*)-1) return 0;
            unsigned int high = 0;
            unsigned int low = pGetFileSize(handle->os_handle, &high);
            #if WORD_SIZE == 8
            return (low == 0xFFFFFFFF) ? 0 : (((UWORD)high << 32) | low);
            #else
            return (low == 0xFFFFFFFF) ? 0 : low;
            #endif
            #else // Linux
            struct stat_buf{
                unsigned long dev, ino, nlink;
                unsigned int mode, uid, gid, __pad0;
                unsigned long rdev, size, blksize, blocks;
                long atime, atime_nsec, mtime, mtime_nsec, ctime, ctime_nsec;
            };
            typedef int(*FSTAT)(int,void*);
            static FSTAT pFstat = (FSTAT)get_system_function("fstat");
            if(!pFstat || !(handle->fd)) return 0;
            stat_buf st = {0};
            if(pFstat(handle->fd, &st) == 0) return (UWORD)(st.size);
            return 0;
            #endif
        }
        FUNCDECL(UWORD tell, void* file){ return file::seek(file, 0, seek_mode::current); }
        FUNCDECL(UWORD seek, void* file, WORD length, BYTE mode){
            if(FLE_HANDLE_VALID(file)) return 0;
            FLE_HANDLE_DECL;
            FLE_HANDLE* handle = (FLE_HANDLE*)file;
            #if defined(_WIN32)
            typedef unsigned int(WINCALL *SFP)(void*,long,long*,unsigned int);
            static SFP pSetFilePointer = (SFP)get_system_function("SetFilePointer");
            if (!pSetFilePointer || handle->os_handle == (void*)-1) return 0;
            #if WORD_SIZE == 8
            long high = (long)(length >> 32);
            #else
            long high = 0;
            #endif
            unsigned int low = pSetFilePointer(handle->os_handle, (long)(length & 0xFFFFFFFF), &high, (unsigned int)mode);
            #if WORD_SIZE == 8
            return (low == 0xFFFFFFFF) ? 0 : (((UWORD)high << 32) | low);
            #else
            return (low == 0xFFFFFFFF) ? 0 : low;
            #endif
            #else // Linux
            typedef long(*LSEEK)(int,long,int);
            static LSEEK pLseek = (LSEEK)get_system_function("lseek");
            if(!pLseek || !(handle->fd)) return 0;
            long res = pLseek(handle->fd, (long)length, (int)mode);
            return (res >= 0) ? (UWORD)res : 0;
            #endif
        }
        FUNCDECL(UWORD clear, const char* filepath){
        	if(!file::exist(filepath)) return 0;
        	void* file = file::open(filepath, file_mode::write);
        	if(!file) file = file::folder(filepath, file_mode::write);
        	if(!file) return 0;
            UWORD ret = file::clear(file);
			file::close(file);
            return ret;
		}
        FUNCDECL(UWORD clear, void* file){
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
		    typedef int(WINCALL *SEF)(void*);
		    static SEF pSetEndOfFile = (SEF)get_system_function("SetEndOfFile");
		    file::seek(file, 0, seek_mode::begin);
		    if(pSetEndOfFile) pSetEndOfFile(handle->os_handle);
			#else
		    typedef int(*FTRUNC)(int,long);
		    static FTRUNC pFtruncate = (FTRUNC)get_system_function("ftruncate");
		    if(pFtruncate) pFtruncate(handle->fd, 0);
			#endif
            return _sz;
		}
		FUNCDECL(bool exist, const char* filepath){
		    if(!filepath) return false;
		    #if defined(_WIN32)
		    typedef unsigned int(WINCALL *GFA)(const char*);
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
		    static XSTAT pStat = (XSTAT)get_system_function("stat");
		    return pStat ? (pStat(filepath, &st) == 0) : false;
		    #endif
		}
		FUNCDECL(void* folder, const char* filepath, BYTE openMode){
		    if(!filepath || openMode == file_mode::none) return nullptr;
		    if(openMode & file_mode::create){
				#if defined(_WIN32)
				typedef int(WINCALL *CDA)(const char*, void*);
				static CDA pCreateDirectoryA = (CDA)get_system_function("CreateDirectoryA");
				if(pCreateDirectoryA) pCreateDirectoryA(filepath, nullptr);
				#else // Linux
				typedef int(*MKDIR)(const char*, unsigned int);
				static MKDIR pMkdir = (MKDIR)get_system_function("mkdir");
				if(pMkdir) pMkdir(filepath, 0775);
				#endif
			}
		    #if defined(_WIN32)
		    typedef void*(WINCALL *CFA)(const char*,unsigned int,unsigned int,void*,unsigned int,unsigned int,void*);
			static CFA pCreateFileA = (CFA)get_system_function("CreateFileA");
		    typedef UWORD(WINCALL *GFP)(const char*,UWORD,char*,char**);
		    static GFP pGetFullPathNameA = (GFP)get_system_function("GetFullPathNameA");
		    if(!pGetFullPathNameA || !pCreateFileA) return nullptr;
		    void* h = pCreateFileA(filepath, 0x80000000, 7, nullptr, 3, 0x02000080, nullptr);
		    UWORD req = pGetFullPathNameA(filepath, 0, nullptr, nullptr);
		    char* abs_path = new char[req];
		    pGetFullPathNameA(filepath, req, abs_path, nullptr);
		    FLE_HANDLE_DECL;
		    FLE_HANDLE* handle = new FLE_HANDLE();
		    handle->os_handle  = h;
		    handle->open_mode  = openMode;
		    handle->full_path  = abs_path;
		    handle->full_spath = req - 1;
		    handle->is_dir     = true;
		    #else // Linux
		    typedef int(*POPEN)(const char*,int,...);
			static POPEN pOpen = (POPEN)get_system_function("open");
			typedef int(*CLOSE)(int);
			static CLOSE pClose = (CLOSE)get_system_function("close");
		    typedef char*(*RPATH)(const char*,char*);
		    static RPATH pRealpath = (RPATH)get_system_function("realpath");
		    typedef void(*FREE)(void*);
		    static FREE pFree = (FREE)get_system_function("free");
		    if(!pOpen || !pClose || !pRealpath || !pFree) return nullptr;
		    int fd = pOpen(filepath, 0x10000);
		    char* tmp = pRealpath(filepath, nullptr);
		    if(!tmp){ if(fd) pClose(fd); return nullptr; }
		    UWORD len = 0; while(tmp[len]) len++;
		    char* abs_path = new char[len + 1];
		    memory::clloc(abs_path, tmp, len + 1);
		    pFree(tmp);
		    FLE_HANDLE_DECL;
		    FLE_HANDLE* handle = new FLE_HANDLE();
		    handle->fd         = fd;
		    handle->open_mode  = openMode;
		    handle->full_path  = abs_path;
		    handle->full_spath = len;
		    handle->is_dir     = true;
		    #endif
		    return (void*)handle;
		}
		FUNCDECL(void* inside, void* folder, UWORD idx){
		    if(!file::is_folder(folder)) return nullptr;
		    FLE_HANDLE_DECL;
		    FLE_HANDLE* h = (FLE_HANDLE*)folder;
		    char* child_name = nullptr;
		    UWORD child_sname = 0;
		    bool is_a_folder = false;
			#if defined(_WIN32)
		    struct WIN32_FIND_DATAA{
		        unsigned int attr; unsigned int t[6]; unsigned int s[2], r[2];
		        char name[260], aname[14];
		    } fData;
		    typedef void*(WINCALL *FFF)(const char*,void*);
		    static FFF pFindFirst = (FFF)get_system_function("FindFirstFileA");
		    typedef int(WINCALL *FNF)(void*,void*);
		    static FNF pFindNext = (FNF)get_system_function("FindNextFileA");
		    typedef int(WINCALL *FC)(void*);
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
		            	pattern = fData.name;
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
				BYTE t;
				char name[256];
			};
		    typedef void*(*ODIR)(const char*);
		    static ODIR pOpendir = (ODIR)get_system_function("opendir");
		    typedef Sdir*(*RDIR)(void*);
		    static RDIR pReaddir = (RDIR)get_system_function("readdir");
		    typedef int(*CDIR)(void*);
		    static CDIR pClosedir = (CDIR)get_system_function("closedir");
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
		        #if defined(_WIN32)
		        sprintf(full_child_path, "%s\\%s", h->full_path, child_name);
		        #else
		        sprintf(full_child_path, "%s/%s", h->full_path, child_name);
		        #endif
				delete[] child_name;
				if(is_a_folder) ret = file::folder(full_child_path, h->open_mode);
				else            ret = file::open(full_child_path, h->open_mode);
		        delete[] full_child_path;
		    }
		    return ret;
		}
		FUNCDECL(bool is_folder, void* file){
		    if(FLE_HANDLE_VALID(file)) return false;
		    FLE_HANDLE_DECL;
		    return ((FLE_HANDLE*)file)->is_dir;
		}
		FUNCDECL(void* name, void* file, char** fname, UWORD* fnsize, bool with_extension, bool stacked){
			if(FLE_HANDLE_VALID(file) || (!fname || !fnsize)) return nullptr;
		    FLE_HANDLE_DECL;
		    FLE_HANDLE* h = (FLE_HANDLE*)file;
		    #if defined(___mb__switch___case___1_2__3__)
		    #undef ___mb__switch___case___1_2__3__
		    #endif
		    #define ___mb__switch___case___1_2__3__ \
			char *start = (h->full_path) + (h->full_spath), *dot = start, *end = start;\
		    while(start != (h->full_path) && *(start - 1) != '/' && *(start - 1) != '\\') --start;\
		    while(dot != start && *dot != '.') --dot;\
		    if(!with_extension && *dot == '.') end = dot;\
		    UWORD Sname = (UWORD)(end - start)
		    ___mb__switch___case___1_2__3__;
		    if(fnsize) *fnsize = Sname;
		    if(fname){
				if(*fname && stacked) delete[] *fname;
				UWORD Scopy = Sname; ++Sname;
				if(stacked) *fname = new char[Sname];
            	else        *fname = (char*)memory::realloc(*fname, Sname);
            	memory::clloc(*fname, start, Scopy);
		        (*fname)[Scopy] = '\0';
		    }
		    return file;
		}
		FUNCDECL(void* name, void* file, char* fname, UWORD fnsize, bool with_extension = false){
			if(FLE_HANDLE_VALID(file) || !fname) return nullptr;
			if(!fnsize){
				*fname = '\0';
				return file;
			}
		    FLE_HANDLE_DECL;
		    FLE_HANDLE* h = (FLE_HANDLE*)file;
		    ___mb__switch___case___1_2__3__;
		    #undef ___mb__switch___case___1_2__3__
			if(Sname > fnsize) Sname = fnsize;
			memory::clloc(fname, start, Sname);
			fname[Sname] = '\0';
			return file;
		}
		FUNCDECL(void* extension, void* file, char** fname, UWORD* fnsize, bool with_dot, bool stacked){
			if(FLE_HANDLE_VALID(file) || (!fname && !fnsize)) return nullptr;
		    FLE_HANDLE_DECL;
            FLE_HANDLE* handle = (FLE_HANDLE*)file;
            #define ___mb__switch___case___1_2__3__ \
		    UWORD Sname = handle->full_spath;\
			char* Hname = (handle->full_path) + Sname;\
			while(Hname != handle->full_path && *Hname != '/' && *Hname != '\\' && *Hname != '.') --Hname;\
        	if(*Hname != '.') Hname = (handle->full_path) + Sname;\
			if(!with_dot && *Hname == '.') ++Hname;\
			Sname -= (UWORD)(Hname - (handle->full_path))
			___mb__switch___case___1_2__3__;
			if(fnsize) *fnsize = Sname;
		    if(fname){
		    	if(*fname && stacked) delete[] *fname;
		    	UWORD Scopy = Sname; ++Sname;
				if(stacked) *fname = new char[Sname];
            	else        *fname = (char*)memory::realloc(*fname, Sname);
            	memory::clloc(*fname, Hname, Scopy);
            	(*fname)[Scopy] = '\0';
			}
			return file;
		}
		FUNCDECL(void* extension, void* file, char* fname, UWORD fnsize, bool with_dot = false){
        	if(FLE_HANDLE_VALID(file) || !fname) return nullptr;
        	if(!fnsize){
            	*fname = '\0';
            	return file;
            }
		    FLE_HANDLE_DECL;
            FLE_HANDLE* handle = (FLE_HANDLE*)file;
            ___mb__switch___case___1_2__3__;
            #undef ___mb__switch___case___1_2__3__
        	if(Sname > fnsize) Sname = fnsize;
            memory::clloc(fname, Hname, Sname);
        	fname[Sname] = '\0';
			return file;
        }
		FUNCDECL(void* path, void* file, char** fname, UWORD* fnsize, bool stacked){
			if(FLE_HANDLE_VALID(file) || (!fname && !fnsize)) return nullptr;
			FLE_HANDLE_DECL;
            FLE_HANDLE* handle = (FLE_HANDLE*)file;
            if(fnsize) *fnsize = handle->full_spath;
            if(fname){
		    	if(*fname && stacked) delete[] *fname;
				if(stacked) *fname = new char[handle->full_spath + 1];
				else        *fname = (char*)memory::realloc(*fname, handle->full_spath + 1);
				memory::clloc(*fname, handle->full_path, handle->full_spath);
				(*fname)[handle->full_spath] = '\0';
			}
			return file;
		}
		FUNCDECL(void* path, void* file, char* fname, UWORD fnsize){
			if(FLE_HANDLE_VALID(file) || !fname) return nullptr;
			FLE_HANDLE_DECL;
			FLE_HANDLE* handle = (FLE_HANDLE*)file;
			if(fnsize > handle->full_spath) fnsize = handle->full_spath;
			memory::clloc(fname, handle->full_path, fnsize);
			fname[fnsize] = '\0';
			return file;
		}
		FUNCDECL(void root_this, char** fname, UWORD* fnsize, bool stacked){
			if(!fname && !fnsize) return;
            #if defined(_WIN32)
			#define ___mb__switch___case___1_2__3__(fname, limit, stacked, __dim_mem) \
            char* root_path = nullptr;\
            UWORD root_len = 0;\
            typedef unsigned int(WINCALL *GMF)(void*,char*,unsigned int);\
            static GMF pGetModuleFileNameA = (GMF)get_system_function("GetModuleFileNameA");\
            if(!pGetModuleFileNameA) return;\
            UWORD buff_size = 128; char* buff = nullptr;\
            while(true){\
            	buff = (char*)memory::realloc(buff, buff_size);\
            	root_len = pGetModuleFileNameA(nullptr, buff, buff_size);\
            	if(root_len == buff_size){ buff_size <<= 1; continue; }\
            	break;\
			}\
			memory::free(buff); if(root_len > (limit)) root_len = (limit);\
            if(((fname) && (__dim_mem)) || (!(__dim_mem))){\
				root_path = *(fname);\
            	if(__dim_mem){\
	                if(*(fname) && (stacked)) delete[] *(fname);\
	                if(stacked) *(fname) = new char[root_len + 1];\
					else        *(fname) = (char*)memory::realloc(*(fname), root_len + 1);\
	                root_path = *(fname);\
				}\
                pGetModuleFileNameA(nullptr, root_path, (unsigned int)root_len + 1);\
                root_path[root_len] = '\0';\
            }
            #else // Linux
            #define ___mb__switch___case___1_2__3__(fname, limit, stacked, __dim_mem) \
            char* root_path = nullptr;\
            UWORD root_len = 0;\
            typedef int(*LSTAT)(const char*, void*);\
            static LSTAT pLstat = (LSTAT)get_system_function("lstat");\
            typedef long (*RDLNK)(const char*, char*, unsigned long);\
            static RDLNK pReadlink = (RDLNK)get_system_function("readlink");\
            if(!pLstat || !pReadlink) return;\
            struct{\
            	unsigned int st_dev;\
			    unsigned short st_ino, st_mode;\
			    short st_nlink, st_uid, st_gid;\
			    unsigned int st_rdev;\
			    long st_size;\
			    UWORD st_atime, st_mtime, st_ctime;\
			} st_buf;\
            if(pLstat("/proc/self/exe", &st_buf) == 0){\
                root_len = (UWORD)(st_buf.st_size);\
                if(!root_len) root_len = 1024;\
            }\
            if(pLstat("/proc/self/exe", &st_buf) == 0) root_len = 1024; \
            if(root_len > (limit)) root_len = (limit);\
            if(root_len && (((fname) && (__dim_mem)) || ((!(__dim_mem))))){\
				root_path = *(fname);\
				if(__dim_mem){\
	                if(*(fname) && (stacked)) delete[] *(fname);\
	                if(stacked) *(fname) = new char[root_len + 1];\
					else        *(fname) = (char*)memory::realloc(*fname, root_len + 1);\
	                root_path = *(fname);\
				}\
                long r = pReadlink("/proc/self/exe", root_path, (unsigned long)root_len);\
                if(r > 0){\
                    root_len = (UWORD)r;\
                    root_path[root_len] = '\0';\
                }else if(!(__dim_mem)){\
					*root_path = '\0';\
					return;\
				}\
            }else if(!(__dim_mem)) return
            #endif
            ___mb__switch___case___1_2__3__(fname, ~(UWORD)0, stacked, 1);
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
		FUNCDECL(void root_this, char* fname, UWORD fnsize){
			if(!fname) return;
			*fname = '\0';
			if(!fnsize) return;
            ___mb__switch___case___1_2__3__(&fname, fnsize, 0, 0);
            #undef ___mb__switch___case___1_2__3__
		}
		FUNCDECL(char* normalize, char* path, UWORD* path_size){
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
				if(*s == ___norm_char_to_search__1_2__3__
				|| *s == ___norm_char_to_replace__1_2__3__){
					do ++s;
					while(*s == ___norm_char_to_search__1_2__3__ || *s == ___norm_char_to_replace__1_2__3__);
					*ss++ = ___norm_char_to_replace__1_2__3__;
				}
				else{
					*ss++ = *s;
					++s;
				}
			}
			#undef ___norm_char_to_search__1_2__3__
			#undef ___norm_char_to_replace__1_2__3__
			*ss = '\0';
			if(path_size) *path_size = (UWORD)(ss - path);
			return path;
		}
		FUNCDECL(void to_date, UWORD ts, BYTE* sec, BYTE* min, BYTE* hour, BYTE* day, BYTE* month, unsigned int* year){
		    if(!sec && !min && !hour && !day && !month && !year) return;
		    if(sec)  *sec  = (BYTE)(ts % 60);
		    ts /= 60;
		    if(min)  *min  = (BYTE)(ts % 60);
		    ts /= 60;
		    if(hour) *hour = (BYTE)(ts % 24);
		    ts /= 24;
		    UWORD z = ts + 719468;
		    UWORD era = z / 146097;
		    UWORD doe = z - era * 146097;
		    UWORD yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;
		    unsigned int y = (unsigned int)(yoe) + era * 400;
		    UWORD doy = doe - (365*yoe + yoe/4 - yoe/100);
		    UWORD mp = (5*doy + 2)/153;
		    UWORD d = doy - (153*mp + 2)/5 + 1;
		    UWORD m = mp < 10 ? mp + 3 : mp - 9;
		    y += (m > 2) ? 0 : 1;
		    if(year)  *year = y;
		    if(month) *month = (BYTE)m;
		    if(day)   *day = (BYTE)d;
		}
		FUNCDECL(void to_time, UWORD* ts, BYTE sec, BYTE min, BYTE hour, BYTE day, BYTE month, unsigned int year){
			if(!ts) return;
		    *ts = 0; unsigned int y = 1970;
		    while(y < year){ *ts += ((y % 4) == 0 && ((y % 100) != 0 || (y % 400) == 0)) ? 31622400 : 31536000; ++y; }
		    static const BYTE mdays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		    bool biyear = ((year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0));
		    BYTE m = 0; while(m != month - 1){ *ts += (((UWORD)(mdays[m])) + ((m == 1 && biyear) ? 1 : 0)) * 86400; ++m; }
		    *ts += ((UWORD)day - 1) * 86400 + (UWORD)hour * 3600 + (UWORD)min * 60 + (UWORD)sec;
		}
		FUNCDECL(UWORD days_until_2000){
			UWORD now_ts = 0;
            #if defined(_WIN32)
            unsigned long long ft;
            typedef void(WINCALL *GST)(void*);
            static GST pGetSystemTime = (GST)get_system_function("GetSystemTimeAsFileTime");
            if(!pGetSystemTime) return 0;
            pGetSystemTime(&ft);
            now_ts = (ft / 10000000ULL) - 11644473600ULL;
            #else // Linux
            struct{ long sec; long nsec; } ts;
            typedef int(*CGT)(int,void*);
            static CGT pClockGetTime = (CGT)get_system_function("clock_gettime");
            if(!pClockGetTime) return 0;
            if(!pClockGetTime(0, &ts)) now_ts = (UWORD)ts.sec;
            #endif
            static const UWORD y2k_ts = 946684800ULL;
            if(now_ts < y2k_ts) return 0; // Por si acaso alguien viaja al pasado
            return (now_ts - y2k_ts);
		}
		FUNCDECL(UWORD apends, void* file, const char* str){
			if(FLE_HANDLE_VALID(file) || !str) return 0;
			static const char _end = L'\0';
			const char* _s = str; while(*_s) ++_s;
			return file::write(file, str, (UWORD)(_s - str)) + file::write(file, &_end, 1);
		}
		FUNCDECL(UWORD apendc, void* file, int chr){
			if(FLE_HANDLE_VALID(file) || !chr) return 0;
			BYTE len = 0;
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
		FUNCDECL(UWORD apendws, void* file, const wchar* wstr){
			if(FLE_HANDLE_VALID(file) || !wstr) return 0;
			static const char _end = '\0';
			UWORD len = 0;
			while(*wstr) len += apendc(file, (int)*wstr++);
			return len + file::write(file, &_end, 1);
		}
		FUNCDECL(UWORD gets, void* file, char** str){
			if(FLE_HANDLE_VALID(file) || !str) return 0;
			if(*str) delete[] *str;
			char _c; UWORD _start = file::tell(file), len = 0;
			while(file::read(file, &_c, 1) ? _c : 0) ++len;
			*str = new char[++len]; file::seek(file, (WORD)_start, seek_mode::begin);
			return file::read(file, *str, len);
		}
		FUNCDECL(UWORD getc, void* file, int* chr){
			if(FLE_HANDLE_VALID(file) || !chr) return 0;
			BYTE b;
		    if(!file::read(file, &b, 1)) return 0;
		    if(b < 0x80){ // 1 byte (ASCII)
		        *chr = b;
		        return 1;
		    }else if((b & 0xE0) == 0xC0){ // 2 bytes
		        BYTE b2;
		        if(!file::read(file, &b2, 1)) return 0;
		        *chr = ((b & 0x1F) << 6) | (b2 & 0x3F);
		        return 2;
		    }else if((b & 0xF0) == 0xE0){ // 3 bytes
		        BYTE b2, b3;
		        if(file::read(file, &b2, 1) && file::read(file, &b3, 1)){
		            *chr = ((b & 0x0F) << 12) | ((b2 & 0x3F) << 6) | (b3 & 0x3F);
		            return 3;
		        }
		    }else if((b & 0xF8) == 0xF0){ // 4 bytes
		        BYTE b2, b3, b4;
		        if(file::read(file, &b2, 1) && file::read(file, &b3, 1) && file::read(file, &b4, 1)){
		            *chr = ((b & 0x07) << 18) | ((b2 & 0x3F) << 12) | ((b3 & 0x3F) << 6) | (b4 & 0x3F);
		            return 4;
		        }
		    }
		    return 0;
		}
		FUNCDECL(UWORD getws, void* file, wchar** wstr){
			if(FLE_HANDLE_VALID(file) || !wstr) return 0;
			if(*wstr) delete[] *wstr;
		    int tmp_chr; UWORD start = file::tell(file), count = 0, _ret = 0;
		    while(file::getc(file, &tmp_chr) ? tmp_chr : 0) ++count;
		    *wstr = new wchar[count + 1];
		    _ret = file::tell(file) - start;
		    file::seek(file, (WORD)start, seek_mode::begin);
		    (*wstr)[count] = L'\0';
		    wchar* stepper = *wstr;
		    while(count--){
		        file::getc(file, &tmp_chr);
		        *stepper = (wchar)tmp_chr; ++stepper;
		    }
		    file::getc(file, &tmp_chr);
		    return _ret;
		}
		FUNCDECL(UWORD apendo, void* file, void* Hobj){
			if(FLE_HANDLE_VALID(file) || !Hobj) return 0;
			UWORD magic = *(UWORD*)Hobj, len = 0;
			if(magic == 0x54485244) return 0; // THREADs are not allowed (... Yet?)
			#if defined(____apendo_bit_size__text__1_2__3__)
			#undef ____apendo_bit_size__text__1_2__3__
			#endif
			#if defined(____apendo_bit_size__ordr__1_2__3__)
			#undef ____apendo_bit_size__ordr__1_2__3__
			#endif
			#if WORD_SIZE == 8
			#define ____apendo_bit_size__text__1_2__3__ "@"
			#else // x32
			#define ____apendo_bit_size__text__1_2__3__ " "
			#endif
			#if IS_BIG_ENDIAN
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
					+ file::write(file, &(handle->pixels_amont), WORD_SIZE)
					+ file::write(file, handle->pixels, (handle->pixels_amont) << 2);
				delete[] title;
			}
			#endif
			#if defined(ST_IMPORT_SOUND)
			SND_HANDLE_DECL;
			if(magic == 0x534E444F){
				SND_HANDLE* handle = (SND_HANDLE*)Hobj;
				char config = ((handle->buffers[0].busy) ? 0x01 : 0x00) | ((handle->buffers[1].busy) ? 0x02 : 0x00);
		    	UWORD buffers_size = memory::szlen(handle->buffers[0].data);
				len = file::write(file, "SOND" ____apendo_bit_size__text__1_2__3__ ____apendo_bit_size__ordr__1_2__3__, 6)
				    + file::write(file, &(handle->sample_rate), 4)
				    + file::write(file, &(handle->channels), 1) + file::write(file, &(handle->bits), 1)
				    + file::write(file, &(handle->volume), 4) + file::write(file, &(handle->pitch), 4)
				    + file::write(file, (handle->master_volume)?"M":" ", 1)
					+ ((handle->master_volume) ? file::write(file, handle->master_volume, 4) : 0)
					+ file::write(file, &buffers_size, 4) + file::write(file, &config, 1)
					+ file::write(file, &(handle->current_buffer), 4);
				if(buffers_size){
					if(handle->buffers[0].data) len += file::write(file, "D", 1) + file::write(file, handle->buffers[0].data, buffers_size);
					else                        len += file::write(file, " ", 1);
					if(handle->buffers[1].data) len += file::write(file, "D", 1) + file::write(file, handle->buffers[1].data, buffers_size);
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
			#if defined(ST_IMPORT_JSON)
			JSN_HANDLE_DECL;
			struct _ap_json{
				static BYTE* _json_write(void* file, UWORD* len, BYTE* data){
					if(!data){
						*len += file::write(file, (const void*)"null", 4);
						return nullptr;
					}
					if(((*data) & 0x0F) == json_type::bll){
						if((*data) & json_type::uns) *len += file::write(file, (const void*)"true", 4);
						else                         *len += file::write(file, (const void*)"false", 5);
						return data + 1;
					}
					if(((*data) & 0x0F) == json_type::num){
						char aux[32]; int _wrt = sprintf(aux, ((*data) & json_type::uns) ? "%lu" : "%ld", *(UWORD*)(data + 1));
						*len += file::write(file, (const void*)aux, _wrt);
						return data + 1 + WORD_SIZE;
					}
					if((*data) == json_type::flt){
						char aux[32]; int _wrt = sprintf(aux, "%g", *(float*)(data + 1));
						*len += file::write(file, (const void*)aux, _wrt);
						return data + 5;
					}
					if((*data) == json_type::str){
						++data; *len += file::apendc(file, L'"');
						while(*data){
							if(*data == '"'){
								*len += file::apendc(file, L'\\');
								*len += file::apendc(file, L'"');
							}else *len += file::apendc(file, (int)*data);
							++data;
						}
						*len += file::apendc(file, L'"'); ++data;
						return data;
					}
					if((*data) == json_type::ary){
						BYTE* _save_data = data;
						++data;
						unsigned short size = *(unsigned short*)data, idx;
						data += 2;
						*len += file::apendc(file, L'[');
						while(size){
							idx = *(unsigned short*)data; data += 2;
							if(idx) _json_write(file, len, JSN_HANDLE::nav(_save_data, idx));
							else *len += file::write(file, (const void*)"null", 4);
							--size;
							if(size) *len += file::apendc(file, L',');
						}
						*len += file::apendc(file, L']');
						return data;
					}
					if((*data) == json_type::obj){
						BYTE* _save_data = data;
						++data;
						unsigned short size = *(unsigned short*)data, idx;
						data += 2;
						*len += file::apendc(file, L'\x7B');
						while(size){
							idx = *(unsigned short*)data; data += 2;
							*len += file::apendc(file, L'"');
							while(*data) *len += file::apendc(file, (int)*((++data) - 1));
							*len += file::apendc(file, L'"'); ++data; *len += file::apendc(file, L':');
							if(idx) _json_write(file, len, JSN_HANDLE::nav(_save_data, idx));
							else *len += file::write(file, (const void*)"null", 4);
							--size;
							if(size) *len += file::apendc(file, L',');
						}
						*len += file::apendc(file, L'\x7D');
						return data;
					}
					return data;
				}
			};
			if(magic == 0x4E4F534A || magic == 0x534E534A){
				len = 0;
				_ap_json::_json_write(file, &len, ((JSN_HANDLE*)Hobj)->data);
			}
			#endif
			return len;
		}
		FUNCDECL(UWORD geto, void* file, void** Hobj){
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
					int i_ret = read_int(file, _inv); float _ret = 0.0f;
					memory::clloc(&_ret, &i_ret, sizeof(float));
					return _ret;
				}
				static UWORD read_word(void* file, char _n_s, char _inv){
					UWORD _ret;
					#if WORD_SIZE == 4
					if(_n_s == ' ') file::read(file, &_ret, 4);
					if(_n_s == '@'){
						if(_inv == '>'){
							file::read(file, &_ret, 4);
							file::seek(file, 2, seek_mode::current);
						}
						if(_inv == '<'){
							file::seek(file, 2, seek_mode::current);
							file::read(file, &_ret, 4);
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
			char head[6] = {0};
			UWORD _org_pos = file::tell(file);
		    if(file::read(file, head, 6) != 6){
		    	file::seek(file, _org_pos, seek_mode::begin);
		    	#if defined(ST_IMPORT_JSON)
		    	if(((head[0] == L'n' || head[0] == L'N') && (head[1] == L'u' || head[1] == L'U')
					&& (head[2] == L'l' || head[2] == L'L') && (head[3] == L'l' || head[3] == L'L'))
					|| ((head[0] == L't' || head[0] == L'T') && (head[1] == L'r' || head[1] == L'R')
					&& (head[2] == L'u' || head[2] == L'U') && (head[3] == L'e' || head[3] == L'E'))
					|| ((head[0] == L'f' || head[0] == L'F') && (head[1] == L'a' || head[1] == L'A')
					&& (head[2] == L'l' || head[2] == L'L') && (head[3] == L's' || head[3] == L'S')
					&& (head[4] == L'e' || head[4] == L'E')) || head[0] == L'-' || head[0] == L'.'
					|| (head[0] >= L'0' && head[0] <= L'9') || head[0] == L'"' || head[0] == L'['
					|| head[0] == L'\x7B'){
					*Hobj = json::parse(file);
					return file::tell(file) - _org_pos;
				}
				#endif
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
				#if defined(ST_STOP_MAILMAN_DECL)
		        *Hobj = window::create(nullptr, title, w, h, nullptr);
				#else
		        *Hobj = window::create(nullptr, title, w, h);
				#endif
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
				file::read(file, &config, 1); if(config == 'M'){
					handle->master_volume = new float(_ap_::read_float(file, head[5]));
					handle->owns_master = true; // the recovered master belongs to this handle now
				}
				UWORD buffers_size = (UWORD)_ap_::read_int(file, head[5]);
				file::read(file, &config, 1); handle->buffers[0].busy = config & 0x01, handle->buffers[1].busy = config & 0x02;
				handle->current_buffer = _ap_::read_int(file, head[5]);
				if(buffers_size){
					file::read(file, &config, 1); if(config == 'D'){
						handle->buffers[0].data = (BYTE*)memory::alloc(buffers_size);
						file::read(file, handle->buffers[0].data, buffers_size);
					}
					file::read(file, &config, 1); if(config == 'D'){
						handle->buffers[1].data = (BYTE*)memory::alloc(buffers_size);
						file::read(file, handle->buffers[1].data, buffers_size);
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
			#if defined(ST_IMPORT_JSON)
			else if(((head[0] == L'n' || head[0] == L'N') && (head[1] == L'u' || head[1] == L'U')
				&& (head[2] == L'l' || head[2] == L'L') && (head[3] == L'l' || head[3] == L'L'))
				|| ((head[0] == L't' || head[0] == L'T') && (head[1] == L'r' || head[1] == L'R')
				&& (head[2] == L'u' || head[2] == L'U') && (head[3] == L'e' || head[3] == L'E'))
				|| ((head[0] == L'f' || head[0] == L'F') && (head[1] == L'a' || head[1] == L'A')
				&& (head[2] == L'l' || head[2] == L'L') && (head[3] == L's' || head[3] == L'S')
				&& (head[4] == L'e' || head[4] == L'E')) || head[0] == L'-' || head[0] == L'.'
				|| (head[0] >= L'0' && head[0] <= L'9') || head[0] == L'"' || head[0] == L'['
				|| head[0] == L'\x7B'){
				file::seek(file, _org_pos, seek_mode::begin);
				*Hobj = json::parse(file);
			}
			#endif
		    else file::seek(file, _org_pos, seek_mode::begin);
		    #undef ____apendo_bit_size__text__1_2__3__
			#undef ____apendo_bit_size__ordr__1_2__3__
			return file::tell(file) - _org_pos;
		}
		FUNCDECL(BYTE opened_like, void* file){
			if(FLE_HANDLE_VALID(file)) return -1;
			FLE_HANDLE_DECL;
			return ((FLE_HANDLE*)file)->open_mode;
		}
	}
	#if defined(ST_IMPORT_SOUND)
	namespace sound{
    	FUNCDECL(void* file_stream, void* file, bool looping, float pitch, bool canIcloser){
			if(FLE_HANDLE_VALID(file)) return nullptr;
			if(file::opened_like(file) != file_mode::read) return nullptr;
			SND_HANDLE_DECL;
			TSN_HANDLE_DECL;
			THR_HANDLE_DECL;
			struct _ap_{
				static void _exec_(void** vargs){
					TSN_HANDLE* thsn = nullptr;
					bool looping = (bool)(UWORD)(vargs[2]);
					bool canIcloser = (bool)(UWORD)(vargs[3]);
				    float current_pitch = *(float*)(vargs[4]);
					void* wav = vargs[1];
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
						vargs[5] = (void*)1;
				    	return;
					}
					thsn = new TSN_HANDLE();
					SND_HANDLE* snd_h = (SND_HANDLE*)snd;
					snd_h->pitch = current_pitch;
					thsn->snd = snd;
				    sound::set(snd, rate, (BYTE)channels, (BYTE)bits);
				    BYTE* temp_buffer = (BYTE*)memory::alloc(frame_stream_size);
				    sound::play(snd);
					vargs[0] = (void*)thsn;
					vargs[5] = (void*)1;
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
								temp_buffer = (BYTE*)memory::realloc(temp_buffer, frame_stream_size);
					    		float vol = volume(snd); float* mst_vol = master_volume(snd);
					    		bool mst_owned = (snd_h->master_volume && snd_h->owns_master);
					    		float mst_copy = 0.0f; if(mst_owned && (snd_h->master_volume)) mst_copy = *(snd_h->master_volume);
					    		sound::destroy(snd); snd = sound::create(frame_stream_size);
				    			sound::set(snd, rate, (BYTE)channels, (BYTE)bits);
					    		snd_h = (SND_HANDLE*)snd;
					    		snd_h->pitch = current_pitch;
					    		snd_h->volume = vol;
					    		if(mst_owned){ snd_h->master_volume = new float(mst_copy); snd_h->owns_master = true; } else snd_h->master_volume = mst_vol;
								thsn->snd = snd;
								sound::play(snd);
					    	}
					    	if(!sound::playing(snd)){
					    		sleep(10);
					    	}
					    	#if !defined(_WIN32)
					    		// On Linux the snd_pcm callback fires no completion, and the ALSA buffer
					    		// holds about one turn: the sleep reserves the next delivery slot.
					    		sleep(10);
					    	#endif
					        if(snd_h->wants_playback && sound::sync(snd)){
					            UWORD readed = file::read(wav, temp_buffer, frame_stream_size);
					            if(readed > 0) sound::stream(snd, temp_buffer, readed);
					            else           end_of_file = true;
					        }
					        unsigned int i = 0;
					        memory::clloc(&i, &current_pitch, sizeof(float));
						    i = 0x7F000000 - i;
						    float recip = 0.0f;
						    memory::clloc(&recip, &i, sizeof(float));
						    recip = recip * (2.0f - current_pitch * recip);
							unsigned int dynamic_sleep = (unsigned int)(50.0f * recip);
	                        if(dynamic_sleep < 5) dynamic_sleep = 5;
	                        #if !defined(_WIN32)
	                        dynamic_sleep -= 5;
	                        #endif
	                        sleep(dynamic_sleep);
					    }
						if(thsn->autodestruction) sound::stop(snd); // drop the queued turns instead of waiting for them
						// Without an output the turns never complete, and waiting for them would spin forever.
						while(((SND_HANDLE*)snd)->playing && (snd_h->buffers[0].busy || snd_h->buffers[1].busy)){
							sound::sync(snd);
							sleep(10);
						}
					}while(looping && !(thsn->autodestruction));
				    memory::free(temp_buffer);
				    sound::stop(snd); // the internal sound stays alive and queryable until the user calls sound::destroy
				    if(canIcloser) file::close(wav);
				}
			};
			void* vargs[6] = { nullptr, file, (void*)(UWORD)looping, (void*)(UWORD)canIcloser, (void*)&pitch, (void*)0 };
			void* thr = thread::create(_ap_::_exec_, vargs);
			if(!thr) return nullptr;
			while(!(vargs[5])) sleep(10);
			if(!(vargs[0])){
				thread::kill(thr);
				return nullptr;
			}
			TSN_HANDLE* thsn = (TSN_HANDLE*)(vargs[0]);
			thsn->file = file;
			thsn->thr = thr;
			thsn->canIcloser = canIcloser;
			return vargs[0];
		}
	}
	#endif
	#endif
	#undef FUNCDECL
	// Note to myself:
	// The reason why I create many structures that start with 'UWORD magic',
	// in place of using a centralized 'union',
	// has to do with how the code modularity can make a single declaration have an infinite number of variations.
	// This makes it so I don't have to create variations of the same macro,
	// and allows me to save space for small structures.
	#undef THR_HANDLE_DECL
	#undef THR_HANDLE_VALID
	#if defined(ST_IMPORT_JSON)
	#undef JSN_HANDLE_DECL
	#undef JSN_HANDLE_VALID
	#undef JSN_HANDLE_STACKED
	#undef JSN_DEFAULT_MEMSIZE
	#endif
	#if defined(ST_IMPORT_FILE)
	#undef FLE_HANDLE_DECL
	#undef FLE_HANDLE_VALID
	#endif
	#if defined(ST_IMPORT_SOUND)
	#undef SND_HANDLE_DECL
	#undef SND_HANDLE_VALID
	#undef TSN_HANDLE_DECL
	#undef TSN_HANDLE_UNVALID
	#endif
	#if defined(ST_IMPORT_CONSOLE)
	#undef CNSL_WND__IN_HANDLE
	#undef CNSL_HANDLE_DECL
	#undef CNSL_HANDLE_VALID
	#endif
	#if defined(ST_IMPORT_WINDOW)
	#undef WND_HANDLE_DECL
	#undef WND_HANDLE_VALID
	#undef WND_HANDLE_STATE
	#if defined(XEvent_DECL)
	#undef XEvent_DECL
	#endif
	#endif
}
#undef WINCALL
#undef __asm_volatile__
// The unprefixed names below are published by reading the CRT guards, never by writing them.
// 'stdio.h' and 'stdlib.h' of MinGW and MSVC open with '#ifndef _INC_STDIO' and
// '#ifndef _INC_STDLIB', so defining those tokens as private marks made the two headers
// vanish, empty and without a word, from any unit that included them after this one.
// 'ST_NO_GLOBAL_FUNCTIONS' skips the publishing on purpose, for the unit that wants the CRT
// versions and includes <stdio.h> or <stdlib.h> after this header: a global 'using' of
// System_Tools::printf cannot live in the same scope as the 'extern "C"' printf that follows.
// Each group answers for the header that really declares its names: 'system', 'mbstowcs'
// and 'wcstombs' are stdlib ones, the rest of them stdio ones.
#if !defined(ST_NO_GLOBAL_FUNCTIONS) && !defined(_INC_STDIO) && !defined(_STDIO_H) && !defined(_STDIO_H_)
	using System_Tools::clear;
	using System_Tools::puts;
	using System_Tools::putchar;
	using System_Tools::printf;
	using System_Tools::sprintf;
	using System_Tools::scanf;
	using System_Tools::sscanf;
#endif
#if !defined(ST_NO_GLOBAL_FUNCTIONS) && !defined(_INC_STDLIB) && !defined(_STDLIB_H) && !defined(_STDLIB_H_)
	using System_Tools::system;
	using System_Tools::mbstowcs;
	using System_Tools::wcstombs;
	using System_Tools::sleep;
	using System_Tools::memory::alloc;
	using System_Tools::memory::clloc;
	using System_Tools::memory::slloc;
	using System_Tools::memory::mlloc;
	using System_Tools::memory::realloc;
	using System_Tools::memory::free;
	using System_Tools::memory::szlen;
	using System_Tools::memory::on_stack;
	using System_Tools::memory::cmplloc;
	using System_Tools::memory::cnrlloc;
#endif
#endif
#if defined(__SYSTEM_TOOLS__AUTO_IMPORT_FIRST__)
	#undef __SYSTEM_TOOLS__AUTO_IMPORT_FIRST__
	#if defined(ST_IMPORT_FILE) || defined(ST_IMPORT_SOUND) || defined(ST_IMPORT_WINDOW) \
		|| defined(ST_IMPORT_JSON) || defined(ST_IMPORT_CONSOLE)
		#define ST_NO_DEFAULT
	#endif
	#if defined(ST_NO_DEFAULT)
		#undef ST_NO_DEFAULT
		#define ST_NO_DEFAULT (~0)
	#else
		#define ST_IMPORT_JSON
	    #define ST_IMPORT_FILE
	    #define ST_IMPORT_SOUND
	    #define ST_IMPORT_WINDOW
	    #define ST_IMPORT_CONSOLE
	#endif
#endif
#if defined(__SYSTEM_TOOLS__DECL_TOOLS_FIRST__)
	#undef __SYSTEM_TOOLS__DECL_TOOLS_FIRST__
	#if defined(WORD_SIZE)
	#undef WORD_SIZE
	#endif
	#if defined(WORD_PRINT)
	#undef WORD_PRINT
	#endif
	#if defined(UWORD_PRINT)
	#undef UWORD_PRINT
	#endif
	#if defined(OWORD_PRINT)
	#undef OWORD_PRINT
	#endif
	#if defined(XWORD_PRINT)
	#undef XWORD_PRINT
	#endif
	#if defined(CWORD_PRINT)
	#undef CWORD_PRINT
	#endif
	#if defined(_WIN64) || defined(__x86_64__) || defined(__x86_64)
		#define WORD_SIZE 8
		#define CWORD_PRINT(syntax, sign) "%" syntax "ll" sign
	#else
		#define WORD_SIZE 4
		#define CWORD_PRINT(syntax, sign) "%" syntax sign
	#endif
	#define WORD_PRINT CWORD_PRINT("","d")
	#define UWORD_PRINT CWORD_PRINT("","u")
	#define OWORD_PRINT CWORD_PRINT("","o")
	#define XWORD_PRINT CWORD_PRINT("","X")
	#if !defined(__INTELLISENSE__)
		#if defined(BYTE)
		#undef BYTE
		#endif
		#if defined(wchar)
		#undef wchar
		#endif
		#if defined(WORD)
		#undef WORD
		#endif
		#if defined(UWORD)
		#undef UWORD
		#endif
		// The four types belong to the namespace, not to the global scope: 'minwindef.h' of the
		// Windows SDK closes with 'typedef unsigned char BYTE' and 'typedef unsigned short WORD',
		// so a global typedef of the same two names either clashes with the SDK or, worse, lets
		// the SDK version rebind 'UWORD' down to 16 bits inside the library while it walks the
		// ELF and PE headers, which are 64-bit reads by design.
		namespace System_Tools{
			typedef unsigned char BYTE;
			typedef wchar_t wchar;
			#if WORD_SIZE == 8
				typedef long long int WORD;
				typedef unsigned long long int UWORD;
			#else
				typedef int WORD;
				typedef unsigned int UWORD;
			#endif
		}
		// The unprefixed spellings keep being published for the units that never meet the SDK,
		// under the same read-only rule as the function names: an SDK header already included
		// stops the publishing, and 'ST_NO_GLOBAL_TYPES' stops it for the unit that puts
		// <windows.h> after this header on purpose.
		#if !defined(ST_NO_GLOBAL_TYPES) && !defined(_WINDOWS_) && !defined(_WINDEF_) && !defined(_MINWINDEF_)
			using System_Tools::BYTE;
			using System_Tools::wchar;
			using System_Tools::WORD;
			using System_Tools::UWORD;
		#endif
	#endif
	#if defined(IS_LITTLE_ENDIAN)
	#undef IS_LITTLE_ENDIAN
	#endif
	#if defined(IS_BIG_ENDIAN)
	#undef IS_BIG_ENDIAN
	#endif
	#if defined(ENDIAN_MULT)
	#undef ENDIAN_MULT
	#endif
	#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__)
		#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
			#define IS_LITTLE_ENDIAN 1
			#define IS_BIG_ENDIAN 0
			#define ENDIAN_MULT 1
		#else
			#define IS_LITTLE_ENDIAN 0
			#define IS_BIG_ENDIAN 1
			#define ENDIAN_MULT -1
		#endif
	#elif defined(_WIN32) || defined(_WIN64) || defined(_M_IX86) || defined(_M_X64)
		#define IS_LITTLE_ENDIAN 1
		#define IS_BIG_ENDIAN 0
		#define ENDIAN_MULT 1
	#elif defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(__i386__) || defined(__x86_64__) || defined(__amd64__)
		#define IS_LITTLE_ENDIAN 1
		#define IS_BIG_ENDIAN 0
		#define ENDIAN_MULT 1
	#elif defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(__sparc__) || defined(_MIPSEB) || defined(__MIPSEB__) || defined(__MIPSEB__)
		#define IS_LITTLE_ENDIAN 0
		#define IS_BIG_ENDIAN 1
		#define ENDIAN_MULT -1
	#else
		#error "No se pudo detectar el endianness de tu procesador."
	#endif
	#if defined(VA_START)
	#undef VA_START
	#endif
	#if defined(VA_NEXT)
	#undef VA_NEXT
	#endif
	#if defined(VA_PEEK)
	#undef VA_PEEK
	#endif
	#if defined(VA_RESET)
	#undef VA_RESET
	#endif
	#if defined(VA_COPY)
	#undef VA_COPY
	#endif
	#if defined(VA_END)
	#undef VA_END
	#endif
	#if defined(_WIN64)
	#define VA_START(prev_arg, va_name) void* va_name=((void*)(((BYTE*)&prev_arg)+8))
	#define VA_NEXT(va_name, type) (*(type*)(((BYTE*)(va_name=(void*)(((BYTE*)va_name)+8)))-8))
	#define VA_PEEK(va_name, idx, type) ({void* va_name##__aux=va_name;int i=0;\
		while(i!=(unsigned int)(idx)){VA_NEXT(va_name##__aux,void*);++i;}\
		VA_NEXT(va_name##__aux,type);})
	#define VA_RESET(prev_arg, va_name) {VA_START(prev_arg, va_name##__aux); va_name = va_name##__aux;}
	#define VA_COPY(va_name, va_copy_name) void* va_copy_name = va_name
	#define VA_END(va_name)
	#elif !defined(_WIN32) && (defined(__x86_64__) || defined(__x86_64))
	#define VA_START(prev_arg, va_name) \
	__builtin_va_list va_name##__sys_vl;\
	__builtin_va_start(va_name##__sys_vl, prev_arg);\
	void* va_name = (void*)&va_name##__sys_vl
	#define VA_NEXT(va_name, type) __builtin_va_arg(*(__builtin_va_list*)va_name, type)
	#define VA_PEEK(va_name, idx, type) (*({\
	    __builtin_va_list va_name##__aux;\
	    __builtin_va_copy(va_name##__aux, *(__builtin_va_list*)va_name);\
	    unsigned int i = 0;\
	    while(i != (unsigned int)(idx)){ __builtin_va_arg(va_name##__aux, void*); ++i; }\
	    type* va_name##__res = __builtin_va_arg(va_name##__aux, type*);\
	    __builtin_va_end(va_name##__aux);\
	    va_name##__res;\
	}))
	#define VA_RESET(prev_arg, va_name) \
	__builtin_va_end(*(__builtin_va_list*)va_name);\
	__builtin_va_list va_name##__aux;\
	__builtin_va_start(va_name##__aux, prev_arg);\
	void* va_name = (void*)&va_name##__aux
	#define VA_COPY(va_name, va_copy_name) \
	__builtin_va_list va_copy_name##__aux;\
	__builtin_va_copy(va_copy_name##__aux, *(__builtin_va_list*)va_name);\
	void* va_copy_name = (void*)&va_copy_name##__aux
	#define VA_END(va_name) __builtin_va_end(*(__builtin_va_list*)va_name)
	#else
	#define VA_START(prev_arg, va_name) void* va_name=((BYTE*)&prev_arg)+sizeof(prev_arg)
	#define VA_NEXT(va_name, type) (*(type*)(((BYTE*)(va_name=(void*)(((BYTE*)va_name)+sizeof(void*))))-sizeof(void*)))
	#define VA_PEEK(va_name, idx, type) ({void* va_name##__aux=va_name;int i=0;\
		while(i!=(unsigned int)(idx)){VA_NEXT(va_name##__aux,void*);++i;}\
		VA_NEXT(va_name##__aux,type);})
	#define VA_RESET(prev_arg, va_name) {VA_START(prev_arg, va_name##__aux); va_name = va_name##__aux;}
	#define VA_COPY(va_name, va_copy_name) void* va_copy_name = va_name
	#define VA_END(va_name)
	#endif
	#if defined(RESTRICT)
	#undef RESTRICT
	#endif
	#if defined(__cplusplus)
		#if defined(_MSC_VER)
			#define RESTRICT __restrict
		#elif defined(__GNUC__) || defined(__clang__)
			#define RESTRICT __restrict__
		#else
			#define RESTRICT
		#endif
	#else
		#if __STDC_VERSION__ >= 199901L
			#define RESTRICT restrict
		#elif defined(_MSC_VER)
			#define RESTRICT __restrict
		#elif defined(__GNUC__) || defined(__clang__)
			#define RESTRICT __restrict__
		#else
			#define RESTRICT
		#endif
	#endif
	#if defined(ALWAYSINLINE)
	#undef ALWAYSINLINE
	#endif
	#if defined(FORCEINLINE)
	#undef FORCEINLINE
	#endif
	#if defined(_MSC_VER)
		#define ALWAYSINLINE __forceinline
	#elif defined(__GNUC__) || defined(__clang__)
		#define ALWAYSINLINE __attribute__((always_inline))
	#else
		#define ALWAYSINLINE
	#endif
	#define FORCEINLINE inline ALWAYSINLINE
	#if defined(NOINLINE)
	#undef NOINLINE
	#endif
	#if defined(_MSC_VER)
		#define NOINLINE __declspec(noinline)
	#else
		#define NOINLINE __attribute__((noinline))
	#endif
#endif
