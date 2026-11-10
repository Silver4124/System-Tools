#if !defined(__SYSTEM_TOOLS__H_FIRST__)
// /////////////// Checking your platform...  ///////////////// //
#if !defined(_WIN32) && !defined(__linux__)
#error "Your platform is incompatible with System_Tools..."
#endif
// ///////////////// Checking your IMPORTs... ///////////////// //
#define __SYSTEM_TOOLS__AUTO_IMPORT_FIRST__
#include "System_Tools.cpp"
// //////////// Checking your processor word size... ////////// //
#define BYTE
#define wchar
#define WORD
#define UWORD
// Printing WORD size and format specifiers
#define WORD_SIZE
#define WORD_PRINT
#define UWORD_PRINT
#define OWORD_PRINT
#define XWORD_PRINT
#define CWORD_PRINT(syntax, sign) // Commanded WORD PRINT
// ///////////////// Checking your endianess... /////////////// //
#define IS_LITTLE_ENDIAN
#define IS_BIG_ENDIAN
#define ENDIAN_MULT
// ///////////// Making the Argumentation tools... //////////// //
#define VA_START(prev_arg, va_name)
#define VA_NEXT(va_name, type)
#define VA_PEEK(va_name, idx, type)
#define VA_RESET(prev_arg, va_name)
#define VA_COPY(va_name, va_copy_name)
#define VA_END(va_name)
#define RESTRICT
#define FORCEINLINE
#define ALWAYSINLINE
#define NOINLINE
// ////////////////// Making this tokens... /////////////////// //
#define __SYSTEM_TOOLS__DECL_TOOLS_FIRST__
#include "System_Tools.cpp"
#if defined(__INTELLISENSE__) // Avoid Intellisense errors, types are normally declared in the System_Tools.cpp file.
	#undef BYTE
	#undef wchar
	#undef WORD
	#undef UWORD
	namespace System_Tools{
		typedef unsigned char BYTE;
		typedef wchar_t wchar;
		#if defined(__x86_64__) || defined(__x86_64) || defined(_WIN64)
			typedef long long int WORD;
			typedef unsigned long long int UWORD;
		#else
			typedef int WORD;
			typedef unsigned int UWORD;
		#endif
	}
	#if !defined(ST_NO_GLOBAL_TYPES) && !defined(_WINDOWS_) && !defined(_WINDEF_) && !defined(_MINWINDEF_)
		using System_Tools::BYTE;
		using System_Tools::wchar;
		using System_Tools::WORD;
		using System_Tools::UWORD;
	#endif
#endif
// ////////////////////// System Tools //////////////////////// //
namespace System_Tools{
    void* get_system_function(const char* RESTRICT func_name, const char* RESTRICT lib_target = nullptr); // 'lib_target = nullptr' => "kernel32.dll"/"libc.so"/"libc.so.X"
    void* get_system_variable(const char* RESTRICT var_name, const char* RESTRICT lib_target = nullptr);  // 'lib_target = nullptr' => System Variable
    int system(const char* cmd);
    FORCEINLINE void clear();
    int puts(const char* str);
    int putchar(int chr);
    int printf(const char* RESTRICT format, ...);
    int sprintf(char* RESTRICT str, const char* RESTRICT format, ...);
    int scanf(const char* RESTRICT format, ...);
    int sscanf(const char* RESTRICT str, const char* RESTRICT format, ...);
	UWORD mbstowcs(wchar** dst, const char* src, UWORD size, bool stacked = false);
	UWORD mbstowcs(wchar* dst, const char* src, UWORD dst_size, UWORD src_size);
	UWORD wcstombs(char** dst, const wchar* src, UWORD size, bool stacked = false);
	UWORD wcstombs(char* dst, const wchar* src, UWORD dst_size, UWORD src_size);
	UWORD language();
    void sleep(unsigned int ms);
	enum message_box : int{
    	ok               = 0x000,
    	okcancel         = 0x001,
    	abortretryignore = 0x002,
    	yesnocancel      = 0x003,
    	yesno            = 0x004,
    	retrycancel      = 0x005,
    	info             = 0x000,
    	error            = 0x010,
    	question         = 0x020,
    	warning          = 0x030,
    	defbutton1       = 0x000,
    	defbutton2       = 0x100,
    	defbutton3       = 0x200
    };
	int message(const char* RESTRICT txt, const char* RESTRICT ttl, int type = message_box::ok);
    namespace memory{
		void* alloc(UWORD size);
		void* clloc(void* RESTRICT dest, const void* RESTRICT src, UWORD size);
		void* slloc(void* dest, BYTE value, UWORD size);
		void* mlloc(void* dest, const void* src, UWORD size);
		void* realloc(void* ptr, UWORD size);
		void* free(void* ptr);
		void* cmplloc(const void* p1, const void* p2, UWORD size);
		void* cnrlloc(const void* p1, const void* p2, UWORD size);
		UWORD szlen(void* ptr);
		bool on_stack(void* ptr);
	}
    namespace thread{
    	void* create(void*(*func)(void*), void* arg = nullptr);
    	void* join(void* thr);
    	void* kill(void* thr);
    	bool yield(void* thr, bool _wait = true);
    }
	#if defined(ST_IMPORT_JSON)
	enum json_type : BYTE{
		nll = 0x00,
		bll = 0x01,
		num = 0x02,
		flt = 0x03,
		str = 0x04,
		ary = 0x05,
		obj = 0x06,
		uns = 0x10, // unsigned flag
		unk = 0xFF
	};
	namespace json{
		void* init(BYTE type);
		void* detach(void* jsn);
		void* parse(void* src);
		BYTE type(void* jsn);
		UWORD size(void* jsn);
		void* value(void* jsn, void* offset);
		void* value_view(void* jsn, void* offset, BYTE* buffer, UWORD* buffer_size = nullptr);
		void* set_type(void* jsn, BYTE type);
		void* set_value(void* jsn, void* value);
		void* add_elem(void* jsn, void* offset, BYTE type);
		void* del_elem(void* jsn, void* offset);
		void* swp_elem(void* jsn, void* offset_a, void* offset_b);
		void* mve_elem(void* jsn, void* offset, void* new_offset);
		UWORD fnd_elem(void* jsn, void* offset);
		const char* nme_elem(void* jsn, UWORD offset);
		void* cls_elms(void* jsn);
	}
	#endif
	#if defined(ST_IMPORT_FILE)
	enum file_mode : BYTE{
		none   = 0x00,
        read   = 0x01,
        write  = 0x02,
        create = 0x08
	};
	enum seek_mode : BYTE{
        begin   = 0,
        current = 1,
        end     = 2
    };
    enum time_mode : BYTE{
    	creation = 0,
    	access   = 1,
    	writed   = 2
	};
	namespace file{
		bool  exist(const char* filepath);
		BYTE  opened_like(void* file);
		void* open(const char* filepath, BYTE openMode = file_mode::read);
		const char* copy(const char* source, const char* dest, bool overwrite = true);
		const char* move(const char* old_path, const char* new_path, bool overwrite = true);
		void* parent(void* file);
		void* close(void* file);
		const char* remove(const char* filepath);
		UWORD timestamp(void* file, BYTE type = time_mode::creation);
		UWORD timestamp(const char* filepath, BYTE type = time_mode::creation);
		UWORD read(void* file, void* buffer, UWORD bytes);
		UWORD write(void* file, const void* buffer, UWORD bytes);
		UWORD size(void* file);
		UWORD tell(void* file);
		UWORD seek(void* file, WORD lenght, BYTE mode);
		UWORD clear(const char* filepath);
		UWORD clear(void* file);
		void* folder(const char* filepath, BYTE openMode = file_mode::read);
		void* inside(void* folder, UWORD idx);
		bool  is_folder(void* file);
		void* name(void* file, char** fname, UWORD* fnsize, bool with_extension = false, bool stacked = false);
		void* extension(void* file, char** fname, UWORD* fnsize, bool with_dot = false, bool stacked = false);
		void* path(void* file, char** fname, UWORD* fnsize, bool stacked = false);
		void  root_this(char** fname, UWORD* fnsize, bool stacked = false);
		char* normalize(char* path, UWORD* path_size = nullptr);
		UWORD apends(void* file, const char* str);
		UWORD apendws(void* file, const wchar* wstr);
		UWORD apendc(void* file, int chr);
		UWORD apendo(void* file, void* Hobj);
		UWORD gets(void* file, char** str);
		UWORD getws(void* file, wchar** wstr);
		UWORD getc(void* file, int* chr);
		UWORD geto(void* file, void** Hobj);
		void to_date(UWORD ts, BYTE* sec, BYTE* min, BYTE* hour, BYTE* day, BYTE* month, unsigned int* year);
		void to_time(UWORD* ts, BYTE sec, BYTE min, BYTE hour, BYTE day, BYTE month, unsigned int year);
		UWORD days_until_2000();
	}
	#endif
	#if defined(ST_IMPORT_SOUND)
	namespace sound{
		void* create(unsigned int buffer_size);
		UWORD stream(void* snd, void* external_data, UWORD size);
		bool play(void* snd);
		void stop(void* snd);
		void destroy(void* snd);
		bool sync(void* snd);
		void set(void* snd, unsigned int rate, BYTE channels, BYTE bits);
		bool playing(void* snd);
		bool is_threaded(void* snd);
		bool set_volume(void* snd, float vol);
		bool volume(void* snd, float* vol);
		float volume(void* snd);
		void set_pitch(void* snd, float pit);
		bool pitch(void* snd, float* pit);
		float pitch(void* snd);
		bool set_master_volume(void* snd, float* mst_vol);
		bool master_volume(void* snd, float** mst_vol);
		float* master_volume(void* snd);
		#if defined(ST_IMPORT_FILE)
		bool reload(void* th_snd);
		void* file_stream(void* file, bool looping, float pitch, bool canIcloser = false); // Only Wav
		void* file_stream(const char* filepath, bool looping, float pitch);
		float get_position(void* th_snd);
		float get_duration(void* th_snd);
		bool seek(void* th_snd, float seconds);
		bool get_spectrum(void* th_snd, float** buffer, UWORD* samples, float* average);
		bool fade(void* th_snd, float target_volume, UWORD ms);
		#endif
	}
	#endif
	#if defined(ST_IMPORT_CONSOLE)
	namespace console{
		void* get_handle();
		UWORD write(void* cnsl, const wchar* text, UWORD len);
		void* read(void* cnsl, wchar** _text, UWORD* _len);
		void* gotoxy(void* cnsl, UWORD x, UWORD y);
		void* getxy(void* cnsl, UWORD* _x, UWORD* _y);
		void* setcolor(void* cnsl, unsigned int bg, unsigned int fg);
		void* getcolor(void* cnsl, unsigned int* _bg, unsigned int* _fg);
		void* restorecolor(void* cnsl);
		void* setunicode(void* cnsl, bool unicode);
		bool  getunicode(void* cnsl);
		void* restoreunicode(void* cnsl);
	}
	#endif
    #if defined(ST_IMPORT_WINDOW)
	enum window_box : UWORD{
		def      = 0x0000,
		accepted = 0x0001,
		// life cycle
	    destroy      = 0x0002,
	    size         = 0x0005,
	    activate     = 0x0006,
	    paint        = 0x000F,
	    close        = 0x0010,
	    quit         = 0x0012,
	    // keyboard
	    key_down     = 0x0100,
	    key_up       = 0x0101,
	    // mouse
	    mouse_move   = 0x0200,
	    lbutton_down = 0x0201,
	    lbutton_up   = 0x0202,
	    lbutton_hold = 0x0203,
	    rbutton_down = 0x0204,
	    rbutton_up   = 0x0205,
	    rbutton_hold = 0x0206
	};
	namespace window{
		#if defined(ST_STOP_MAILMAN_DECL)
		void* create(void* father, const char* title, unsigned int width, unsigned int height, UWORD(*mailman)(void*,UWORD,UWORD,WORD));
		#else
		UWORD defMailMan(void* wnd, UWORD msg, UWORD wp, WORD lp);
		void* create(void* father, const char* title, unsigned int width, unsigned int height, UWORD(*mailman)(void*,UWORD,UWORD,WORD) = defMailMan);
		#endif
		void* wait(void* wnd);
		void* destroy(void* wnd, bool _wait = true);
		void* sendmessage(void* wnd, UWORD msg, UWORD wp, WORD lp);
		void* draw(void* wnd, int offsetX, int offsetY, unsigned int* _pixels, unsigned int _width, unsigned int _height);
		void  pixels(void* wnd, unsigned int** _px, UWORD* _px_am);
		void* clear(void* wnd, unsigned int _color = 0x000000);
		void* resize(void* wnd, int _x, int _y, int _width, int _height, bool SetPosition = false, bool SetSize = true);
		void  getsize(void* wnd, int* _x, int* _y, int* _width, int* _height);
		void* retitle(void* wnd, const char* title);
		void* gettitle(void* wnd, char** title, bool* minimized);
		void* father(void* wnd);
		void  getscreen(int* _width, int* _height);
		void* show(void* wnd, bool visible, bool minimize = false);
		void* focus(void* wnd);
		void* focuswait(void* wnd);
		void* mouse(void* wnd, int* _x, int* _y, int* lclick, int* rclick);
		void* keyboard(void* wnd, BYTE** keys, UWORD* size, bool stacked = false);
		void* keyboard(void* wnd, BYTE* keys, UWORD size);
		void* active();
		bool is_extern(void* wnd);
		void* release(void* wnd);
		bool yield(void* wnd, bool _wait = true);
		#if !defined(ST_STOP_MAILMAN_DECL)
		inline UWORD defMailMan(void* wnd, UWORD msg, UWORD wp, WORD lp){
		    switch(msg){
		        case window_box::destroy: case window_box::close:
		        	return window_box::def;
		    }
            return window_box::def;
		}
		#endif
	}
	#endif
}
// ///////////////// Circular include... ////////////////////// //
#define __SYSTEM_TOOLS__H_FIRST__ 1
#include "System_Tools.cpp"
// //////////////////////////////////////////////////////////// //
// @author: Silver Roberskull
// @emition: 2026-11-10
// @version: 1.5
// @description: System Tools for C++ (STC++)
// @todo:
//   Add more features and improve cross-platform compatibility without library dependencies.
//   Are recommended erase any including of standard C libraries for the most complete cross-platform experience.
//   The project is still in development, so any suggestions or contributions are welcome.
//   The modules system (the macros like 'ST_IMPORT_*') are open to be expanded with new features.
// //////////////////////////////////////////////////////////// //
#endif