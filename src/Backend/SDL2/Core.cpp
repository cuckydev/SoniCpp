/*
Project: SoniC++

File: src/Backend/SDL2/Core.cpp
Purpose: Define the SDL2 backend core class

Authors: Regan "cuckydev" Green
*/

//SDL2
#include "SDL.h"

//Declaration
#include "Core.h"

//SoniC++ namespace
namespace SCPP
{
	//Backend namespace
	namespace Backend
	{
		//Core namespace
		namespace Core
		{
			//Constructor and destructor
			SDL2::SDL2()
			{
				//Initialize SDL2
				if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0)
					error.Push(SDL_GetError());
			}
			
			SDL2::~SDL2()
			{
				//Quit SDL2
				SDL_Quit();
			}
		}
	}
}
