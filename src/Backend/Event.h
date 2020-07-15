#pragma once

/*
Project: SoniC++

File: src/Backend/Event.h
Purpose: Declare the backend event base class

Authors: Regan "cuckydev" Green
*/

//Int sizes
#include <stddef.h>

//Error class
#include "../Error.h"

//SoniC++ namespace
namespace SCPP
{
	//Backend namespace
	namespace Backend
	{
		//Event namespace
		namespace Event
		{
			//Keyboard mappings - https://github.com/SDL-mirror/SDL/blob/master/include/SDL_scancode.h
			enum KeyboardKey
			{
				KEY_UNKNOWN,
				
				//Letters
				KEY_A,
				KEY_B,
				KEY_C,
				KEY_D,
				KEY_E,
				KEY_F,
				KEY_G,
				KEY_H,
				KEY_I,
				KEY_J,
				KEY_K,
				KEY_L,
				KEY_M,
				KEY_N,
				KEY_O,
				KEY_P,
				KEY_Q,
				KEY_R,
				KEY_S,
				KEY_T,
				KEY_U,
				KEY_V,
				KEY_W,
				KEY_X,
				KEY_Y,
				KEY_Z,
				
				//Numbers
				KEY_1,
				KEY_2,
				KEY_3,
				KEY_4,
				KEY_5,
				KEY_6,
				KEY_7,
				KEY_8,
				KEY_9,
				KEY_0,
				
				//Whitespace and escape
				KEY_RETURN,
				KEY_ESCAPE,
				KEY_BACKSPACE,
				KEY_TAB,
				KEY_SPACE,
				
				//Symbols
				KEY_MINUS,
				KEY_EQUALS,
				KEY_LEFTBRACKET,
				KEY_RIGHTBRACKET,
				KEY_BACKSLASH,
				KEY_NONUSHASH,
				KEY_SEMICOLON,
				KEY_APOSTROPHE,
				KEY_GRAVE,
				KEY_COMMA,
				KEY_PERIOD,
				KEY_SLASH,
				
				//Caps lock
				KEY_CAPSLOCK,
				
				//Function keys
				KEY_F1,
				KEY_F2,
				KEY_F3,
				KEY_F4,
				KEY_F5,
				KEY_F6,
				KEY_F7,
				KEY_F8,
				KEY_F9,
				KEY_F10,
				KEY_F11,
				KEY_F12,
				
				//Control keys
				KEY_PRINTSCREEN,
				KEY_SCROLLLOCK,
				KEY_PAUSE,
				KEY_INSERT,
				KEY_HOME,
				KEY_PAGEUP,
				KEY_DELETE,
				KEY_END,
				KEY_PAGEDOWN,
				
				//Arrow keys
				KEY_RIGHT,
				KEY_LEFT,
				KEY_DOWN,
				KEY_UP,
				
				//Keypad
				KEY_NUMLOCKCLEAR,
				KEY_KP_DIVIDE,
				KEY_KP_MULTIPLY,
				KEY_KP_MINUS,
				KEY_KP_PLUS,
				KEY_KP_ENTER,
				KEY_KP_1,
				KEY_KP_2,
				KEY_KP_3,
				KEY_KP_4,
				KEY_KP_5,
				KEY_KP_6,
				KEY_KP_7,
				KEY_KP_8,
				KEY_KP_9,
				KEY_KP_0,
				KEY_KP_PERIOD,
				
				//Some USB standard stuff
				KEY_NONUSBACKSLASH,
				KEY_APPLICATION,
				KEY_POWER,
				KEY_KP_EQUALS,
				KEY_F13,
				KEY_F14,
				KEY_F15,
				KEY_F16,
				KEY_F17,
				KEY_F18,
				KEY_F19,
				KEY_F20,
				KEY_F21,
				KEY_F22,
				KEY_F23,
				KEY_F24,
				KEY_EXECUTE,
				KEY_HELP,
				KEY_MENU,
				KEY_SELECT,
				KEY_STOP,
				KEY_AGAIN,
				KEY_UNDO,
				KEY_CUT,
				KEY_COPY,
				KEY_PASTE,
				KEY_FIND,
				KEY_MUTE,
				KEY_VOLUMEUP,
				KEY_VOLUMEDOWN,
				KEY_KP_COMMA,
				KEY_KP_EQUALSAS400,
				
				KEY_INTERNATIONAL1,
				KEY_INTERNATIONAL2,
				KEY_INTERNATIONAL3,
				KEY_INTERNATIONAL4,
				KEY_INTERNATIONAL5,
				KEY_INTERNATIONAL6,
				KEY_INTERNATIONAL7,
				KEY_INTERNATIONAL8,
				KEY_INTERNATIONAL9,
				KEY_LANG1,
				KEY_LANG2,
				KEY_LANG3,
				KEY_LANG4,
				KEY_LANG5,
				KEY_LANG6,
				KEY_LANG7,
				KEY_LANG8,
				KEY_LANG9,
				
				KEY_ALTERASE,
				KEY_SYSREQ,
				KEY_CANCEL,
				KEY_CLEAR,
				KEY_PRIOR,
				KEY_RETURN2,
				KEY_SEPARATOR,
				KEY_OUT,
				KEY_OPER,
				KEY_CLEARAGAIN,
				KEY_CRSEL,
				KEY_EXSEL,
				
				KEY_KP_00,
				KEY_KP_000,
				KEY_THOUSANDSSEPARATOR,
				KEY_DECIMALSEPARATOR,
				KEY_CURRENCYUNIT,
				KEY_CURRENCYSUBUNIT,
				KEY_KP_LEFTPAREN,
				KEY_KP_RIGHTPAREN,
				KEY_KP_LEFTBRACE,
				KEY_KP_RIGHTBRACE,
				KEY_KP_TAB,
				KEY_KP_BACKSPACE,
				KEY_KP_A,
				KEY_KP_B,
				KEY_KP_C,
				KEY_KP_D,
				KEY_KP_E,
				KEY_KP_F,
				KEY_KP_XOR,
				KEY_KP_POWER,
				KEY_KP_PERCENT,
				KEY_KP_LESS,
				KEY_KP_GREATER,
				KEY_KP_AMPERSAND,
				KEY_KP_DBLAMPERSAND,
				KEY_KP_VERTICALBAR,
				KEY_KP_DBLVERTICALBAR,
				KEY_KP_COLON,
				KEY_KP_HASH,
				KEY_KP_SPACE,
				KEY_KP_AT,
				KEY_KP_EXCLAM,
				KEY_KP_MEMSTORE,
				KEY_KP_MEMRECALL,
				KEY_KP_MEMCLEAR,
				KEY_KP_MEMADD,
				KEY_KP_MEMSUBTRACT,
				KEY_KP_MEMMULTIPLY,
				KEY_KP_MEMDIVIDE,
				KEY_KP_PLUSMINUS,
				KEY_KP_CLEAR,
				KEY_KP_CLEARENTRY,
				KEY_KP_BINARY,
				KEY_KP_OCTAL,
				KEY_KP_DECIMAL,
				KEY_KP_HEXADECIMAL,
				
				KEY_LCTRL,
				KEY_LSHIFT,
				KEY_LALT,
				KEY_LGUI,
				KEY_RCTRL,
				KEY_RSHIFT,
				KEY_RALT,
				KEY_RGUI,
				
				KEY_MODE,
				
				KEY_AUDIONEXT,
				KEY_AUDIOPREV,
				KEY_AUDIOSTOP,
				KEY_AUDIOPLAY,
				KEY_AUDIOMUTE,
				KEY_MEDIASELECT,
				KEY_WWW,
				KEY_MAIL,
				KEY_CALCULATOR,
				KEY_COMPUTER,
				KEY_AC_SEARCH,
				KEY_AC_HOME,
				KEY_AC_BACK,
				KEY_AC_FORWARD,
				KEY_AC_STOP,
				KEY_AC_REFRESH,
				KEY_AC_BOOKMARKS,
				
				KEY_BRIGHTNESSDOWN,
				KEY_BRIGHTNESSUP,
				KEY_DISPLAYSWITCH,
				
				KEY_KBDILLUMTOGGLE,
				KEY_KBDILLUMDOWN,
				KEY_KBDILLUMUP,
				KEY_EJECT,
				KEY_SLEEP,
				
				KEY_APP1,
				KEY_APP2,
				KEY_AUDIOREWIND,
				KEY_AUDIOFASTFORWARD,
				
				KEY_MAX,
			};

			//Game controller button enum - https://github.com/SDL-mirror/SDL/blob/master/include/SDL_gamecontroller.h
			enum GameControllerButton
			{
				GCB_UNKNOWN,
				
				//Face buttons
				GCB_A,
				GCB_B,
				GCB_X,
				GCB_Y,
				
				//Middle buttons
				GCB_BACK,
				GCB_GUIDE,
				GCB_START,
				
				//Left and right analogue sticks
				GCB_LEFTSTICK,
				GCB_RIGHTSTICK,
				
				//Shoulder buttons
				GCB_LEFTSHOULDER,
				GCB_RIGHTSHOULDER,
				
				//DPad
				GCB_DPAD_UP,
				GCB_DPAD_DOWN,
				GCB_DPAD_LEFT,
				GCB_DPAD_RIGHT,
				
				GCB_MAX,
			};
			
			//Game controller axis enum
			typedef enum
			{
				GCA_INVALID,
				GCA_LEFTX,
				GCA_LEFTY,
				GCA_RIGHTX,
				GCA_RIGHTY,
				GCA_TRIGGERLEFT,
				GCA_TRIGGERRIGHT,
				GCA_MAX
			} GameControllerAxis;
			
			//Event base class
			class Base
			{
				protected:
					//Error
					SCPP::Error error;
					
				public:
					//Virtual destructor
					virtual ~Base() {}
					
					//Event interface
					virtual bool HandleEvents() = 0;
					
					virtual bool GetButtonState(size_t index, const GameControllerButton button) = 0;
					virtual float GetAxisState(size_t index, const GameControllerAxis axis) = 0;
					virtual bool GetKeyState(const KeyboardKey key) = 0;
					
					//Get error
					const SCPP::Error &GetError() const { return error; }
			};
		}
	}
}
