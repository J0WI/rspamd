#ifndef REPLXX_IO_HXX_INCLUDED
#define REPLXX_IO_HXX_INCLUDED 1

#include <deque>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#endif

namespace replxx {

class Terminal {
public:
	enum class EVENT_TYPE {
		KEY_PRESS,
		MESSAGE,
		TIMEOUT
	};
private:
#ifdef _WIN32
	HANDLE _consoleOut;
	HANDLE _consoleIn;
	DWORD _oldMode;
	WORD _oldDisplayAttribute;
	UINT const _inputCodePage;
	UINT const _outputCodePage;
	HANDLE _interrupt;
	typedef std::deque<EVENT_TYPE> events_t;
	events_t _events;
#else
	struct termios _origTermios; /* in order to restore at exit */
	int _interrupt[2];
#endif
	bool _rawMode; /* for destructor to check if restore is needed */
public:
	enum class CLEAR_SCREEN {
		WHOLE,
		TO_END
	};
public:
	Terminal( void );
	~Terminal( void );
	void write32( char32_t const*, int );
	void write8( char const*, int );
	int get_screen_columns(void);
	int get_screen_rows(void);
	int enable_raw_mode(void);
	void disable_raw_mode(void);
	char32_t read_char(void);
	void clear_screen( CLEAR_SCREEN );
	EVENT_TYPE wait_for_input( int long = 0 );
	void notify_event( EVENT_TYPE );
	void jump_cursor( int, int );
#ifndef _WIN32
	int read_verbatim( char32_t*, int );
#endif
private:
	Terminal( Terminal const& ) = delete;
	Terminal& operator = ( Terminal const& ) = delete;
	Terminal( Terminal&& ) = delete;
	Terminal& operator = ( Terminal&& ) = delete;
};

void beep();
char32_t read_unicode_character(void);

namespace tty {

extern bool in;
extern bool out;

}

}

#endif

