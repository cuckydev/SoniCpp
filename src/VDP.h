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
			void *buffer;
			SCPP::Backend::Render::PixelFormat pixel_format;
			unsigned int width, pitch, height;
		};
		
		//VDP types
		static const uint8_t colour_lut[8] = {0, 52, 87, 116, 144, 172, 206, 255};
		
		struct Colour
		{
			uint8_t r : 4;
			uint8_t g : 4;
			uint8_t b : 4;
			uint32_t value;
			
			void MapValue(const SCPP::Backend::Render::PixelFormat &pixel_format)
			{ value = pixel_format.MapRGB(colour_lut[r >> 1], colour_lut[g >> 1], colour_lut[b >> 1]); }
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
			uint16_t x : 9;
			uint16_t y : 10;
			uint8_t width : 2;
			uint8_t height : 2;
			
			//Next sprite (linked list)
			Sprite *next;
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
				
				template <typename T>
				void WriteScanlines(const unsigned from, const unsigned to);
				
				//Get error
				const SCPP::Error &GetError() const { return error; }
		};
	}
}
