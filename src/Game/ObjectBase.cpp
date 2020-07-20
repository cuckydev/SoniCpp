/*
Project: SoniC++

File: src/Game/ObjectBase.h
Purpose: Define the object base class

Authors: Regan "cuckydev" Green
*/

//Level
#include "Level.h"

//Declaration
#include "ObjectBase.h"

//SoniC++ namespace
namespace SCPP
{
	//Game namespace
	namespace Game
	{
		//Build sprites
		bool ObjectBase::BuildSprites(SCPP::VDP::Sprite **prev)
		{
			//Quit if sprite list is null
			if (prev == nullptr)
				return false;
			
			//Clear on-screen flag
			render_flags.on_screen = false;
			
			//Get object position (also do on-screen checks)
			uint8_t align = (render_flags.align_background << 1) | render_flags.align_level;
			int16_t x, y;
			
			if (align != 0)
			{
				//TODO: Get screen coordinate to scroll from
				int16_t x_scroll = 0, y_scroll = 0;
				
				//Get X coordinate and check if it's on-screen
				x = pos.level.x.value - x_scroll;
				if ((x + width_pixels) < 0 || (x - width_pixels) >= parent_level->parent_game->GetWidth())
					return false;
				x += 0x80; //Shift so it's in VDP coordinates
				
				//Get Y coordinate and check if it's on-screen
				uint8_t height = render_flags.assume_height ? 0x20 : y_radius;
				y = pos.level.y.value - y_scroll;
				if ((y + height) < 0 || (y - height) >= parent_level->parent_game->GetHeight())
					return false;
				y += 0x80; //Shift so it's in VDP coordinates
			}
			else
			{
				//Use screen space position
				x = pos.screen.x;
				y = pos.screen.y;
			}
			
			//Draw sprite and set as visible
			if (DrawSprite(prev, x, y))
				return true;
			render_flags.on_screen = true;
			return false;
		}
		
		bool ObjectBase::DrawSprite(SCPP::VDP::Sprite **prev, uint16_t x, uint16_t y)
		{
			//Get frames to use
			if (mappings == nullptr)
				return false;
			const Mapping::Frame &map_frame = mappings->GetFrame(frame);
			
			//Render differently depending on x and y flip
			switch ((render_flags.y_flip << 1) | render_flags.x_flip)
			{
				case 0x0: //00
				{
					for (auto &i : map_frame)
					{
						//Construct a new sprite
						SCPP::VDP::Sprite *spr;
						if ((spr = new SCPP::VDP::Sprite()) == nullptr)
							return true;
						
						//Fill with data from the mapping
						spr->y = y + i.y;
						spr->width = i.width;
						spr->height = i.height;
						spr->pattern = art_tile.pattern + i.pattern;
						spr->x_flip = i.x_flip;
						spr->y_flip = i.y_flip;
						spr->palette = art_tile.palette + i.palette;
						spr->priority = art_tile.priority ^ i.priority;
						spr->x = x + i.x;
						
						//Link us to the sprite list
						if (*prev != nullptr)
							(*prev)->next = spr;
						*prev = spr;
					}
					break;
				}
				case 0x1: //01
				{
					for (auto &i : map_frame)
					{
						//Construct a new sprite
						SCPP::VDP::Sprite *spr;
						if ((spr = new SCPP::VDP::Sprite()) == nullptr)
							return true;
						
						//Fill with data from the mapping
						spr->y = y + i.y;
						spr->width = i.width;
						spr->height = i.height;
						spr->pattern = art_tile.pattern + i.pattern;
						spr->x_flip = !i.x_flip;
						spr->y_flip = i.y_flip;
						spr->palette = art_tile.palette + i.palette;
						spr->priority = art_tile.priority ^ i.priority;
						spr->x = x - i.x - ((i.width + 1) * 8);
						
						//Link us to the sprite list
						if (*prev != nullptr)
							(*prev)->next = spr;
						*prev = spr;
					}
					break;
				}
				case 0x2: //10
				{
					for (auto &i : map_frame)
					{
						//Construct a new sprite
						SCPP::VDP::Sprite *spr;
						if ((spr = new SCPP::VDP::Sprite()) == nullptr)
							return true;
						
						//Fill with data from the mapping
						spr->y = y - i.y - ((i.height + 1) * 8);
						spr->width = i.width;
						spr->height = i.height;
						spr->pattern = art_tile.pattern + i.pattern;
						spr->x_flip = i.x_flip;
						spr->y_flip = !i.y_flip;
						spr->palette = art_tile.palette + i.palette;
						spr->priority = art_tile.priority ^ i.priority;
						spr->x = x + i.x;
						
						//Link us to the sprite list
						if (*prev != nullptr)
							(*prev)->next = spr;
						*prev = spr;
					}
					break;
				}
				case 0x3: //11
				{
					for (auto &i : map_frame)
					{
						//Construct a new sprite
						SCPP::VDP::Sprite *spr;
						if ((spr = new SCPP::VDP::Sprite()) == nullptr)
							return true;
						
						//Fill with data from the mapping
						spr->y = y - i.y - ((i.height + 1) * 8);
						spr->width = i.width;
						spr->height = i.height;
						spr->pattern = art_tile.pattern + i.pattern;
						spr->x_flip = !i.x_flip;
						spr->y_flip = !i.y_flip;
						spr->palette = art_tile.palette + i.palette;
						spr->priority = art_tile.priority ^ i.priority;
						spr->x = x - i.x - ((i.width + 1) * 8);
						
						//Link us to the sprite list
						if (*prev != nullptr)
							(*prev)->next = spr;
						*prev = spr;
					}
					break;
				}
			}
			
		}
	}
}
