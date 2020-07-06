#pragma once

/*
Project: SoniC++

File: src/Backend/Render.h
Purpose: Declare the backend render base class

Authors: Regan "cuckydev" Green
*/

//String and int sizes
#include <string>
#include <stdint.h>

//Error class
#include "../Error.h"

//SoniC++ namespace
namespace SCPP
{
	//Backend namespace
	namespace Backend
	{
		//Render namespace
		namespace Render
		{
			//Pixel format structure
			struct PixelFormat
			{
				uint8_t bits_per_pixel, bytes_per_pixel;	//Sizes of a pixel
				uint8_t r_loss, g_loss, b_loss, a_loss;		//Bitshift to mask size
				uint8_t r_shift, g_shift, b_shift, a_shift;	//Bitshift into mask position
					
				//Map / get colour
				uint32_t MapRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const
				{
					return (r >> r_loss) << r_shift
						 | (g >> g_loss) << g_shift
						 | (b >> b_loss) << b_shift
						 | (a >> a_loss) << a_shift;
				}
				
				void GetRGBA(uint32_t rgb, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a) const
				{
					if (r != nullptr)
						*r = (rgb >> r_shift) << r_loss;
					if (g != nullptr)
						*g = (rgb >> g_shift) << g_loss;
					if (b != nullptr)
						*b = (rgb >> b_shift) << b_loss;
					if (a != nullptr)
						*a = (rgb >> a_shift) << a_loss;
				}
				
				//No alpha channel (0xFF, opaque)
				uint32_t MapRGB(uint8_t r, uint8_t g, uint8_t b) const { return MapRGBA(r, g, b, 0xFF); }
				void GetRGB(uint32_t rgb, uint8_t *r, uint8_t *g, uint8_t *b) const { GetRGBA(rgb, r, g, b, nullptr); }
			};
			//Render configuration
			struct Config
			{
				std::string title;
				unsigned int width, height, scale;
				unsigned int framerate;
				bool fullscreen;
			};
			
			//Render base class
			class Base
			{
				protected:
					//Error
					SCPP::Error error;
					
					//Current configuration
					Config config;
					
					//Render pixel format
					PixelFormat pixel_format;
					
				public:
					//Virtual destructor
					virtual ~Base() {}
					
					//Render interface
					virtual bool SetConfig(const Config &_config) = 0;
					const Config &GetConfig() { return config; }
					
					const PixelFormat &GetPixelFormat() const { return pixel_format; }
					
					virtual void *StartFrame(int *pitch) = 0;
					virtual bool EndFrame() = 0;
					
					//Get error
					const SCPP::Error &GetError() const { return error; }
			};
		}
	}
}
