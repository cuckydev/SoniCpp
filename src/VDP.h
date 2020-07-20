#pragma once

/*
Project: SoniC++

File: src/VDP.h
Purpose: Declare VDP (Video Display Processor) classes and structures

Authors: Regan "cuckydev" Green
*/

//Standard library
#include <stdint.h>
#include <array>

//Compiler attributes
#include "Attributes.h"

//Backend classes
#include "Backend/Render.h"

//SoniC++ namespace
namespace SCPP
{
	//VDP namespace
	namespace VDP
	{
		//VDP output
		struct Output
		{
			void *buffer = nullptr;
			SCPP::Backend::Render::PixelFormat pixel_format;
			unsigned int width, height, pitch;
		};
		
		//VDP types
		static const uint8_t colour_lut[8] = {0, 52, 87, 116, 144, 172, 206, 255};
		
		struct ColourValue
		{
			union
			{
				uint8_t value : 4;
				uint8_t _ = 0;
			};
			
			uint8_t GetLevel() { return colour_lut[value >> 1]; }
		};
		
		struct Colour
		{
			ColourValue r;
			ColourValue g;
			ColourValue b;
			uint32_t value = 0;
			
			void MapValue(const SCPP::Backend::Render::PixelFormat &pixel_format)
			{ value = pixel_format.MapRGB(r.GetLevel(), g.GetLevel(), b.GetLevel()); }
		};
		
		struct Palette
		{
			std::array<Colour, 16> colour;
			
			void MapValue(const SCPP::Backend::Render::PixelFormat &pixel_format)
			{
				for (auto &i : colour)
					i.MapValue(pixel_format);
			}
		};
		
		struct Sprite
		{
			//Pattern, palette, priority, and flipping
			size_t pattern;
			size_t palette;
			bool priority : 1;
			bool x_flip : 1;
			bool y_flip : 1;
			
			//Position and size
			uint16_t x;
			uint16_t y;
			uint8_t width : 2;
			uint8_t height : 2;
			
			//Next sprite (linked list)
			Sprite *next = nullptr;
		};
		
		//VDP instance class
		class Instance
		{
			private:
				//Error
				SCPP::Error error;
				
				//VDP state and data
				Output output;
				
				uint8_t *pattern = nullptr;
				size_t patterns;
				
				Palette *palette = nullptr;
				size_t palettes;
				
				Sprite *sprite = nullptr;
				
			public:
				//Constructors and destructor
				Instance() {}
				Instance(const Output &_output) { SetOutput(_output); }
				~Instance();
				
				//VDP interface
				void SetOutput(const Output &_output);
				const Output &GetOutput() const { return output; }
				
				bool Allocate(size_t _patterns, size_t _palettes);
				uint8_t *GetPattern(size_t i) { return &pattern[i * (4 * 8)]; }
				Palette *GetPalette(size_t i) { return &palette[i]; }
				
				void SetHeadSprite(Sprite *_sprite) { sprite = _sprite; }
				Sprite *GetHeadSprite() { return sprite; }
				void DeleteSprites()
				{
					for (Sprite *i = sprite; i != nullptr;)
					{
						Sprite *next = i->next;
						delete i;
						i = next;
					}
					sprite = nullptr;
				}
				
				bool WriteScanlines(const unsigned int from, const unsigned int to);
				
				//Get error
				const SCPP::Error &GetError() const { return error; }
				
			private:
				//Internal VDP interface
				template <typename T>
				ATTRIBUTE_HOT bool WriteScanlines_Internal(const unsigned int from, const unsigned int to);
		};
	}
}
