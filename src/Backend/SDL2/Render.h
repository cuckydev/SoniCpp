#pragma once

/*
Project: SoniC++

File: src/Backend/SDL2/Render.h
Purpose: Declare the SDL2 backend render class

Authors: Regan "cuckydev" Green
*/

//SDL2
#include "SDL_video.h"
#include "SDL_render.h"

//Base class declaration
#include "../Render.h"

//SoniC++ namespace
namespace SCPP
{
	//Backend namespace
	namespace Backend
	{
		//Render namespace
		namespace Render
		{
			//Render class
			class SDL2 : public Base
			{
				private:
					//SDL window, renderer, and output texture
					SDL_Window *window = nullptr;
					SDL_Renderer *renderer = nullptr;
					SDL_Texture *texture = nullptr;
					SDL_PixelFormatEnum pixel_format_enum;
					
					//Framerate limiter state
					unsigned int vsync_multiple;
					double next_time, frame_time, timeout_time;
					
				public:
					//Constructors and destructor
					SDL2() {}
					SDL2(const Config &_config) { SetConfig(_config); }
					~SDL2();
					
					//Render interface
					bool SetConfig(const Config &_config);
					
					void *StartFrame(unsigned int *pitch);
					bool EndFrame();
					
				private:
					//Internal things
					unsigned int GetDisplayMode(SDL_DisplayMode *mode, unsigned int framerate);
			};
		}
	}
}
