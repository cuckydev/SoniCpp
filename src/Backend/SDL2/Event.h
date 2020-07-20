#pragma once

/*
Project: SoniC++

File: src/Backend/SDL2/Event.h
Purpose: Declare the SDL2 backend event class

Authors: Regan "cuckydev" Green
*/

//SDL2 keyboard and game controllers
#include "SDL_keyboard.h"
#include "SDL_gamecontroller.h"

//Base class declaration
#include "../Event.h"

//SoniC++ namespace
namespace SCPP
{
	//Backend namespace
	namespace Backend
	{
		//Event namespace
		namespace Event
		{
			//Event class
			class SDL2 : public Base
			{
				private:
					//Keyboard and game controller states
					int keyboard_states;
					const uint8_t *keyboard_state = nullptr;
					
				public:
					//Constructor and destructor
					SDL2();
					~SDL2() override;
					
					//Event interface
					bool HandleEvents() override;
					
					bool GetButtonState(size_t index, const GameControllerButton button) override;
					float GetAxisState(size_t index, const GameControllerAxis axis) override;
					bool GetKeyState(const KeyboardKey key) override;
			};
		}
	}
}
