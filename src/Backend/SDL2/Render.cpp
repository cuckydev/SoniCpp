/*
Project: SoniC++

File: src/Backend/SDL2/Render.cpp
Purpose: Define the SDL2 backend render class

Authors: Regan "cuckydev" Green
*/

//Timer
#include "SDL_timer.h"

//Declaration
#include "Render.h"

//SoniC++ namespace
namespace SCPP
{
	//Backend namespace
	namespace Backend
	{
		//Render namespace
		namespace Render
		{
			//Destructor
			SDL2::~SDL2()
			{
				//Delete texture, renderer, and window
				if (texture != nullptr)
					SDL_DestroyTexture(texture);
				if (renderer != nullptr)
					SDL_DestroyRenderer(renderer);
				if (window != nullptr)
					SDL_DestroyWindow(window);
			}
			
			//Render interface
			bool SDL2::SetConfig(const Config &_config)
			{
				//Get width and height to use
				unsigned int use_width = _config.width, use_height = _config.height;
				if (use_width == 0)
					if ((use_width = config.width) == 0)
						return error.Push("Width of 0 can't be used");
				if (use_height == 0)
					if ((use_height = config.height) == 0)
						return error.Push("Height of 0 can't be used");
				
				//Create window, renderer, and texture
				if (window == nullptr || renderer == nullptr || texture == nullptr)
				{
					//Delete old instances if they were somehow made
					if (texture != nullptr)
						SDL_DestroyTexture(texture);
					if (renderer != nullptr)
						SDL_DestroyRenderer(renderer);
					if (window != nullptr)
						SDL_DestroyWindow(window);
					
					SDL_DisplayMode display_mode;
					if (/* Create window */		(window = SDL_CreateWindow(_config.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, use_width * _config.scale, use_height * _config.scale, _config.fullscreen ? SDL_WINDOW_FULLSCREEN : 0)) == nullptr ||
						/* Determine VSync */	(GetDisplayMode(&display_mode, _config.framerate) == 0) ||
						/* Create renderer */	(renderer = SDL_CreateRenderer(window, -1, vsync_multiple ? SDL_RENDERER_PRESENTVSYNC : 0)) == nullptr ||
						/* Get window's display mode for texture */ SDL_GetWindowDisplayMode(window, &display_mode) < 0 ||
						/* Create texture */	(texture = SDL_CreateTexture(renderer, pixel_format_enum = (SDL_PixelFormatEnum)display_mode.format, SDL_TEXTUREACCESS_STREAMING, _config.width, _config.height)) == nullptr)
						return error.Push(SDL_GetError());
					
					//Get our pixel format (display_mode was filled in earlier)
					SDL_PixelFormat *sdl_pixel_format;
					if ((sdl_pixel_format = SDL_AllocFormat(pixel_format_enum)) == nullptr)
						return error.Push(SDL_GetError());
					
					pixel_format.bits_per_pixel = sdl_pixel_format->BitsPerPixel;
					pixel_format.bytes_per_pixel = sdl_pixel_format->BytesPerPixel;
					
					pixel_format.r_loss = sdl_pixel_format->Rloss;
					pixel_format.g_loss = sdl_pixel_format->Gloss;
					pixel_format.b_loss = sdl_pixel_format->Bloss;
					pixel_format.a_loss = sdl_pixel_format->Aloss;
					
					pixel_format.r_shift = sdl_pixel_format->Rshift;
					pixel_format.g_shift = sdl_pixel_format->Gshift;
					pixel_format.b_shift = sdl_pixel_format->Bshift;
					pixel_format.a_shift = sdl_pixel_format->Ashift;
					
					SDL_FreeFormat(sdl_pixel_format);
				}
				else
				{
					//Resize window if size changed
					if (use_width != config.width || use_height != config.height || _config.scale != config.scale)
						SDL_SetWindowSize(window, use_width * _config.scale, use_height * _config.scale);
					
					//Recreate texture if width or height changed
					if (_config.framerate != config.framerate || use_width != config.width || use_height != config.height)
					{
						//Delete old texture
						if (texture != nullptr)
							SDL_DestroyTexture(texture);
						
						//Create a new texture with the new dimensions
						if ((texture = SDL_CreateTexture(renderer, pixel_format_enum, SDL_TEXTUREACCESS_STREAMING, _config.width, _config.height)) == nullptr)
							return error.Push(SDL_GetError());
					}
				}
				
				//Use given configuration
				config = _config;
				config.width = use_width;
				config.height = use_height;
				return false;
			}
			
			void *SDL2::StartFrame(int *pitch)
			{
				//Lock texture
				void *data;
				if (SDL_LockTexture(texture, nullptr, &data, pitch) < 0)
				{
					error.Push(SDL_GetError());
					return nullptr;
				}
				return data;
			}
			
			bool SDL2::EndFrame()
			{
				//If not using VSync, wait for the next frame manually
				if (vsync_multiple == 0)
				{
					//Get our current time
					double curr_time = (double)SDL_GetTicks();
					
					if (curr_time >= next_time + timeout_time || curr_time < next_time - frame_time)
					{
						next_time = curr_time;
					}
					else
					{
						if (curr_time < next_time)
							SDL_Delay(next_time - curr_time);
						next_time += frame_time;
					}
				}
				else
				{
					//Wait vsync_multiple - 1 frames before drawing and doing final vsync
					for (unsigned int i = 1; i < vsync_multiple; i++)
					{
						if (SDL_RenderCopy(renderer, texture, nullptr, nullptr) < 0)
							return error.Push(SDL_GetError());
						SDL_RenderPresent(renderer);
					}
				}
				
				//Unlock texture and draw to the screen
				SDL_UnlockTexture(texture);
				if (SDL_RenderCopy(renderer, texture, nullptr, nullptr) < 0)
					return error.Push(SDL_GetError());
				SDL_RenderPresent(renderer);
				return false;
			}
			
			//Internal things
			unsigned int SDL2::GetDisplayMode(SDL_DisplayMode *mode, unsigned int framerate)
			{
				//Get the display mode containing our window
				if (SDL_GetWindowDisplayMode(window, mode) < 0)
					return 0;
				
				//Use our the display mode's refresh rate as our framerate if we wanted VSync
				if (framerate == 0)
					framerate = mode->refresh_rate;
				
				//Use manual framerate limiter if it was specified or VSync failed
				if (framerate != 0)
				{
					//Check if VSync can be used between the refresh rate and wanted framerate
					double refresh_integral;
					double refresh_fractional = std::modf((double)mode->refresh_rate / (double)framerate, &refresh_integral);
					unsigned long _vsync_multiple = (unsigned long)refresh_integral;
					
					if (_vsync_multiple > 0 && refresh_fractional == 0.0)
					{
						//Use VSync
						vsync_multiple = _vsync_multiple;
					}
					else
					{
						//Don't use VSync
						vsync_multiple = 0;
						
						//Get our frame time and timeout time (how long we can be late for a frame before resync)
						frame_time = 1000.0 / (double)framerate;
						timeout_time = frame_time * 10.0f;
						
						//Initialize limiter time
						next_time = (double)SDL_GetTicks() + frame_time;
					}
				}
			}
		}
	}
}
