/*
Project: SoniC++

File: src/VDP.cpp
Purpose: Define VDP (Video Display Processor) classes

Authors: Regan "cuckydev" Green

Note: This code isn't very good. I designed it in hopes that it'll run as
      quickly as possible. However, because of the use of templates and macros,
      speed is put forward at the expense of size. To be fair, it's trying to
      replicate the VDP, but simplified so it only uses what Sonic needs, so I
      really don't care. I don't believe there's much I can do to make
      it any cleaner.
*/

//Memcpy
#include <string.h>

//Declaration
#include "VDP.h"

//SoniC++ namespace
namespace SCPP
{
	//VDP namespace
	namespace VDP
	{
		//VDP instance class
		//Destructor
		Instance::~Instance()
		{
			//Unload data
			if (pattern != nullptr)
				delete[] pattern;
			if (palette != nullptr)
				delete[] palette;
			DeleteSprites();
		}
		
		//VDP interface
		void Instance::SetOutput(const Output &_output)
		{
			//Map all palettes to renderable colours
			for (size_t i = 0; i < palettes; i++)
				palette[i].MapValue(_output.pixel_format);
			
			//Use given output
			output = _output;
		}
		
		bool Instance::Allocate(size_t _patterns, size_t _palettes)
		{
			if (pattern != nullptr)
				delete[] pattern;
			if ((pattern = new uint8_t[(patterns = _patterns) * 4 * 8]{}) == nullptr)
				return error.Push("Failed to allocate patterns");
				
			if (palette != nullptr)
				delete[] palette;
			if ((palette = new Palette[palettes = _palettes]{}) == nullptr)
				return error.Push("Failed to allocate palettes");
			return false;
		}
		
		template <typename T>
		ATTRIBUTE_HOT bool Instance::WriteScanlines_Internal(const unsigned int from, const unsigned int to)
		{
			//Allocate output buffer with clip padding
			if (output.buffer == nullptr)
				return error.Push("No output buffer was given");
			
			#define PAD_R	0x20
			#define PAD_D	(PAD_R*2)
			
			unsigned int width = output.width + PAD_D;
			unsigned int height = to - from + PAD_D;
			
			T *buffer;
			if ((buffer = new T[width * height]{}) == nullptr)
				return error.Push("Failed to allocate padded buffer");
			
			//Allocate composite mask buffer
			uint8_t *composite;
			if ((composite = new uint8_t[width * height]{}) == nullptr)
			{
				delete[] buffer;
				return error.Push("Failed to allocate composite buffer");
			}
			
			#define COMPOSITE_SPRITE_MASK	0x01	//00000001
			#define COMPOSITE_SPRITE_HI		0x02	//00000010
			#define COMPOSITE_PLANEA_HI		0x04	//00000100
			#define COMPOSITE_PLANEB_HI		0x08	//00001000
			
			#define WRITE_NIBBLE(dst, cmp, src, msk, shf, pal, tst, set)	\
				if ((*src & msk) && !(*cmp & tst))							\
				{															\
					*dst = palette[pal].colour[(*src >> shf) & 0xF].value;	\
					*cmp |= set;											\
				}															\
				dst++;														\
				cmp++;
			
			#define WRITE_BYTE(dst, cmp, src, mskl, shfl, mskr, shfr, pal, tst, set)	\
				WRITE_NIBBLE(dst, cmp, src, mskl, shfl, pal, tst, set)					\
				WRITE_NIBBLE(dst, cmp, src, mskr, shfr, pal, tst, set)					\
			
			//Draw sprites
			for (Sprite *s = sprite; s != nullptr; s = s->next)
			{
				//Special case: If x == 0, mask the entire sprite's containing scanlines
				if (s->x == 0)
				{
					int sprite_top = (int)s->y - (0x80 + to);
					if (sprite_top < 0)
						sprite_top = 0;
					int sprite_bottom = sprite_top + (s->width + 1) * 8;
					if (sprite_bottom > height)
						sprite_bottom = height;
					for (int y = sprite_top; y < sprite_bottom; y++)
						composite[y * output.width] |= COMPOSITE_SPRITE_MASK;
					continue;
				}
				
				//Get sprite dimensions
				unsigned int sprite_width = (s->width + 1) * 8;
				unsigned int sprite_height = (s->height + 1) * 8;
				
				//Get sprite coordinates
				int sprite_top = (int)s->y - (0x80 - PAD_R + from);
				int sprite_bottom = sprite_top + sprite_height;
				int sprite_left = (int)s->x - (0x80 - PAD_R);
				int sprite_right = sprite_left + sprite_width;
				
				//Reject if off-screen
				if (sprite_top < 0 || sprite_bottom > height || sprite_left < 0 || sprite_right > width)
					continue;
				
				//Remember some other information
				uint8_t cmp_set = s->priority ? COMPOSITE_SPRITE_HI : 0;
				
				//Write sprite to buffer
				switch ((s->y_flip << 1) | s->x_flip)
				{
					case 0x0: //00
					{
						uint8_t *src = pattern + (s->pattern * (4 * 8));
						T *dst = buffer + (sprite_top * width + sprite_left);
						uint8_t *cmp = composite + (sprite_top * width + sprite_left);
						for (unsigned int x = 0; x <= s->width; x++)
						{
							for (int y = sprite_top; y < sprite_bottom; y++)
							{
								if (composite[y * width] & COMPOSITE_SPRITE_MASK)
									continue;
								WRITE_BYTE(dst, cmp, src, 0xF0, 4, 0x0F, 0, s->palette, 0, cmp_set) src++;
								WRITE_BYTE(dst, cmp, src, 0xF0, 4, 0x0F, 0, s->palette, 0, cmp_set) src++;
								WRITE_BYTE(dst, cmp, src, 0xF0, 4, 0x0F, 0, s->palette, 0, cmp_set) src++;
								WRITE_BYTE(dst, cmp, src, 0xF0, 4, 0x0F, 0, s->palette, 0, cmp_set) src++;
								dst += width - 8;
							}
							dst -= width * sprite_height - 8;
						}
						break;
					}
					case 0x1: //01
					{
						uint8_t *src = pattern + (s->pattern * (4 * 8) + 3);
						T *dst = buffer + (sprite_top * width + sprite_right - 8);
						uint8_t *cmp = composite + (sprite_top * width + sprite_right - 8);
						for (unsigned int x = 0; x <= s->width; x++)
						{
							for (int y = sprite_top; y < sprite_bottom; y++)
							{
								if (composite[y * width] & COMPOSITE_SPRITE_MASK)
									continue;
								WRITE_BYTE(dst, cmp, src, 0x0F, 0, 0xF0, 4, s->palette, 0, cmp_set) src--;
								WRITE_BYTE(dst, cmp, src, 0x0F, 0, 0xF0, 4, s->palette, 0, cmp_set) src--;
								WRITE_BYTE(dst, cmp, src, 0x0F, 0, 0xF0, 4, s->palette, 0, cmp_set) src--;
								WRITE_BYTE(dst, cmp, src, 0x0F, 0, 0xF0, 4, s->palette, 0, cmp_set) src--;
								src += 8;
								dst += width - 8;
							}
							dst -= width * sprite_height + 8;
						}
						break;
					}
					case 0x2: //10
					{
						uint8_t *src = pattern + (s->pattern * (4 * 8));
						T *dst = buffer + ((sprite_bottom - 1) * width + sprite_left);
						uint8_t *cmp = composite + ((sprite_bottom - 1) * width + sprite_left);
						for (unsigned int x = 0; x <= s->width; x++)
						{
							for (int y = sprite_top; y < sprite_bottom; y++)
							{
								if (composite[y * width] & COMPOSITE_SPRITE_MASK)
									continue;
								WRITE_BYTE(dst, cmp, src, 0xF0, 4, 0x0F, 0, s->palette, 0, cmp_set) src++;
								WRITE_BYTE(dst, cmp, src, 0xF0, 4, 0x0F, 0, s->palette, 0, cmp_set) src++;
								WRITE_BYTE(dst, cmp, src, 0xF0, 4, 0x0F, 0, s->palette, 0, cmp_set) src++;
								WRITE_BYTE(dst, cmp, src, 0xF0, 4, 0x0F, 0, s->palette, 0, cmp_set) src++;
								dst -= width + 8;
							}
							dst += width * sprite_height + 8;
						}
						break;
					}
					case 0x3: //11
					{
						uint8_t *src = pattern + (s->pattern * (4 * 8) + 3);
						T *dst = buffer + ((sprite_bottom - 1) * width + sprite_right - 8);
						uint8_t *cmp = composite + ((sprite_bottom - 1) * width + sprite_right - 8);
						for (unsigned int x = 0; x <= s->width; x++)
						{
							for (int y = sprite_top; y < sprite_bottom; y++)
							{
								if (composite[y * width] & COMPOSITE_SPRITE_MASK)
									continue;
								WRITE_BYTE(dst, cmp, src, 0x0F, 0, 0xF0, 4, s->palette, 0, cmp_set) src--;
								WRITE_BYTE(dst, cmp, src, 0x0F, 0, 0xF0, 4, s->palette, 0, cmp_set) src--;
								WRITE_BYTE(dst, cmp, src, 0x0F, 0, 0xF0, 4, s->palette, 0, cmp_set) src--;
								WRITE_BYTE(dst, cmp, src, 0x0F, 0, 0xF0, 4, s->palette, 0, cmp_set) src--;
								src += 8;
								dst -= width + 8;
							}
							dst += width * sprite_height - 8;
						}
						break;
					}
				}
			}
			
			//Copy padded buffer to output buffer
			for (unsigned int y = from; y < to; y++)
				memcpy((T*)output.buffer + (y * output.pitch), buffer + ((y + PAD_R) * width + PAD_R), output.width * sizeof(T));
			
			//Delete composite buffer
			delete[] composite;
			return false;
		}
		
		bool Instance::WriteScanlines(const unsigned int from, const unsigned int to)
		{
			switch (output.pixel_format.bytes_per_pixel)
			{
				case 1:
					return WriteScanlines_Internal<uint8_t>(from, to);
				case 2:
					return WriteScanlines_Internal<uint16_t>(from, to);
				case 4:
					return WriteScanlines_Internal<uint32_t>(from, to);
				default:
					return error.Push("Invalid bit-depth for VDP output");
			}
		}
	}
}
