#define ST_VERSION 0x79614442
#define ST_VERDATE 0x000B4921 // = DD + (MM x 31) + (YYYY x 365)
#if defined(ST_IMPORT_FILE) || defined(ST_IMPORT_SOUND) || defined(ST_IMPORT_INPUT) || defined(ST_IMPORT_WINDOW)
	#define ST_NO_DEFAULT
#endif
#if !defined(ST_NO_DEFAULT)
    #define ST_IMPORT_FILE
    #define ST_IMPORT_SOUND
    #define ST_IMPORT_INPUT
    #define ST_IMPORT_WINDOW
#endif
namespace System_Tools{
	// Check your platform... 
	#if !defined(_WIN32) && !defined(__linux__)
	#error "Tu plataforma es incompatible con System_Tools..."
	#endif
	// Check your processor word size...
	#if defined(_WIN64) || defined(__x86_64__) || defined(__x86_64)
	typedef long long WORD;
	typedef unsigned long long UWORD;
	#else
	typedef int WORD;
	typedef unsigned int UWORD;
	#endif
	// Check your endianess...
	#define IS_LITTLE_ENDIAN
	#define IS_BIG_ENDIAN
    void* get_system_function(const char* func_name, const char* lib_target = nullptr); // 'lib_target = nullptr' => kernel
    int system(const char* cmd);
    void clear();
    int puts(const char* str);
    int putchar(int chr);
    int printf(const char* format, ...);
    int sprintf(char* str, const char* format, ...);
    int scanf(const char* format, ...);
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
    	defbutton2       = 0x100
    };
	int message(const char* txt, const char* ttl, int type = message_box::ok);
	int message(const wchar_t* txt, const wchar_t* ttl, int type = message_box::ok);
    namespace memory{
		void* alloc(unsigned int size);
		void* clloc(void* dest, const void* src, unsigned int size);
		void* slloc(void* dest, unsigned char value, unsigned int size);
		void* mlloc(void* dest, const void* src, unsigned int size);
		void* realloc(void* ptr, unsigned int size);
		void* free(void* ptr);
		void* cmplloc(const void* p1, const void* p2, unsigned int size);
		void* cnrlloc(const void* p1, const void* p2, unsigned int size);
	}
    namespace thread{
    	void* create(void*(*func)(void*), void* arg = nullptr);
    	template<typename R, typename... Args> void* create(R(*func)(Args...), void* arg = nullptr);
    	void* join(void* thr);
    	void* kill(void* thr);
    	inline bool yiel(void* thr, bool _wait = true);
    }
	#if defined(ST_IMPORT_FILE)
	enum file_mode : UWORD{
		none   = 0x00,
        read   = 0x01,
        write  = 0x02,
        create = 0x08
	};
	enum seek_mode : UWORD{
        begin   = 0,
        current = 1,
        end     = 2
    };
    enum time_mode : UWORD{
    	creation = 0,
    	access   = 1,
    	writed   = 2
	};
	namespace file{
		bool  exist(const char* filepath);
		UWORD opened_like(void* file);
		void* open(const char* filepath, UWORD openMode = file_mode::read);
		void* open(void* folder, const char* subFilepath);
		const char* copy(const char* source, const char* dest, bool overwrite = true);
		const char* move(const char* old_path, const char* new_path, bool overwrite = true);
		void* parent(void* file);
		void* close(void* file);
		const char* remove(const char* filepath);
		void* remove(void* file);
		UWORD timestamp(void* file, UWORD type = time_mode::creation);
		UWORD timestamp(const char* filepath, UWORD type = time_mode::creation);
		UWORD read(void* file, void* buffer, UWORD bytes);
		UWORD write(void* file, const void* buffer, UWORD bytes);
		UWORD size(void* file);
		UWORD tell(void* file);
		UWORD seek(void* file, WORD lenght, UWORD mode);
		UWORD clear(const char* filepath);
		UWORD clear(void* file);
		void* folder(const char* filepath, UWORD openMode = file_mode::read);
		void* inside(void* folder, UWORD idx);
		bool  is_folder(void* file);
		void* name(void* file, char** fname, UWORD* fnsize, bool with_extension = false);
		void* extension(void* file, char** fname, UWORD* fnsize, bool with_dot = false);
		void* path(void* file, char** fname, UWORD* fnsize);
		void  root_this(char** fname, UWORD* fnsize);
		char* normalize(char* path, UWORD* path_size = nullptr);
		UWORD apends(void* file, const char* str);
		UWORD apendws(void* file, const wchar_t* wstr);
		UWORD apendc(void* file, int chr);
		UWORD apendo(void* file, void* Hobj);
		UWORD gets(void* file, char** str);
		UWORD getws(void* file, wchar_t** wstr);
		UWORD getc(void* file, int* chr);
		UWORD geto(void* file, void** Hobj);
		void to_date(UWORD ts, char* sec, char* min, char* hour, char* day, char* month, int* year);
		void to_time(UWORD* ts, char sec, char min, char hour, char day, char month, int year);
		UWORD days_until_2000();
	}
	#endif
	#if defined(ST_IMPORT_SOUND)
	namespace sound{
		void* create(unsigned int buffer_size);
		void* create(unsigned int buffer_size, float* mst_vol);
		UWORD stream(void* snd, void* external_data, UWORD size);
		bool play(void* snd);
		void stop(void* snd);
		void destroy(void* snd);
		bool sync(void* snd);
		void set(void* snd, unsigned int rate, unsigned char channels, unsigned char bits);
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
	#if defined(ST_IMPORT_INPUT)
	namespace input{
		void mouse(void* wnd, int* _x, int* _y, int* lclick, int* rclick);
		void keyboard(void* wnd, char* key, char* ascii_key, int* outstanding_amount);
		void mousewait(void* wnd, bool buttonDirection, bool unPress = false); // buttonDirection = false => buttonDirection = Left
		char keywait(void* wnd, char key = '\0', bool dontPress = false);
		UWORD language();
		bool set_console_layout(bool UTF);
		bool gotoxy(UWORD _x, UWORD _y);
    	#if defined(ST_IMPORT_WINDOW)
		void* mouse_show(void* wnd, bool visible);
		void* mouse_capture(void* wnd, bool lock);
		#endif
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
		void* active();
		bool is_extern(void* wnd);
		inline bool yiel(void* wnd, bool _wait = true);
		#if !defined(ST_STOP_MAILMAN_DECL)
		UWORD defMailMan(void* wnd, UWORD msg, UWORD wp, WORD lp){
		    switch(msg){
		        case window_box::destroy: case window_box::close:
		        	window::destroy(wnd, false);
		            return window_box::accepted;
		    }
            return window_box::def;
		}
		#endif
	}
	#endif
}
#if !defined(__SYSTEM_TOOLS__CPP_FIRST__)
#define __SYSTEM_TOOLS__H_FIRST__
#include "System_Tools.cpp"
#undef __SYSTEM_TOOLS__H_FIRST__
#endif
