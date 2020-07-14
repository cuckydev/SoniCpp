/*
Project: SoniC++

File: src/Engine.cpp
Purpose: Define the SoniC++ underlaying engine

Authors: Regan "cuckydev" Green
*/

//Declaration
#include "Engine.h"

//Hardware classes
#include "VDP.h"

//Backends
#ifdef SCPP_COMPILE_SDL2
	#include "Backend/SDL2/Core.h"
	#include "Backend/SDL2/Render.h"
#endif

//SoniC++ namespace
namespace SCPP
{
	//Engine namespace
	namespace Engine
	{
		//Destructor
		Instance::~Instance()
		{
			//Delete backend sub-systems
			if (render != nullptr)
				delete render;
			if (core != nullptr)
				delete core;
		}
		
		//Engine interface
		bool Instance::SetConfig(const Config &_config)
		{
			//Handle new backends
			if ((core == nullptr || render == nullptr) || _config.backend != config.backend)
			{
				//Delete old backend sub-systems
				if (render != nullptr)
					delete render;
				if (core != nullptr)
					delete core;
				
				//Create new backend sub-systems
				switch (_config.backend)
				{
				#ifdef SCPP_COMPILE_SDL2
					case Config_Backend::SDL2:
						core = new SCPP::Backend::Core::SDL2();
						render = new SCPP::Backend::Render::SDL2();
						break;
				#endif
					default:
						return error.Push("Invalid backend (" + std::to_string((unsigned)_config.backend) + ") selected");
				}
			}
			
			//Ensure backends were successfully created
			if (core == nullptr)
				return error.Push("Failed to create backend core sub-system instance");
			else if (core->GetError())
				return error.Push(core->GetError());
			
			if (render == nullptr)
				return error.Push("Failed to create backend render sub-system instance");
			else if (render->GetError())
				return error.Push(render->GetError());
			
			//Set backend configurations
			if (render->SetConfig(_config.render_config))
				return error.Push(render->GetError());
			
			//Use given configuration
			config = _config;
			return false;
		}
		
		bool Instance::Start()
		{
			//Initialize VDP
			SCPP::VDP::Instance vdp;
			if (vdp.Allocate(2048, 4))
				return error.Push(vdp.GetError());
			
			//Test graphic
			uint8_t *graphic = vdp.GetPattern(0);
			for (size_t i = 0; i < 2048; i++)
			{
				*graphic++ = 0x11+i; *graphic++ = 0x11; *graphic++ = 0x11; *graphic++ = 0x11;
				*graphic++ = 0x12; *graphic++ = 0x22; *graphic++ = 0x22; *graphic++ = 0x21;
				*graphic++ = 0x12; *graphic++ = 0x30; *graphic++ = 0x00; *graphic++ = 0x21;
				*graphic++ = 0x12; *graphic++ = 0x03; *graphic++ = 0x00; *graphic++ = 0x21;
				*graphic++ = 0x12; *graphic++ = 0x00; *graphic++ = 0x30; *graphic++ = 0x21;
				*graphic++ = 0x12; *graphic++ = 0x00; *graphic++ = 0x03; *graphic++ = 0x21;
				*graphic++ = 0x12; *graphic++ = 0x22; *graphic++ = 0x22; *graphic++ = 0x21;
				*graphic++ = 0x11; *graphic++ = 0x11; *graphic++ = 0x11; *graphic++ = 0x11;
			}
			
			//Test palette
			SCPP::VDP::Palette *palette = vdp.GetPalette(0);
			//palette->colour[0].r.value = 0xE;
			//palette->colour[0].g.value = 0xE;
			//palette->colour[0].b.value = 0xE;
			palette->colour[1].r.value = 0xE;
			palette->colour[2].g.value = 0xE;
			palette->colour[3].b.value = 0xE;
			palette->colour[4].r.value = 0xC;
			palette->colour[5].g.value = 0xC;
			palette->colour[6].b.value = 0xC;
			palette->colour[7].r.value = 0xA;
			palette->colour[8].g.value = 0xA;
			palette->colour[9].b.value = 0xA;
			palette->colour[10].r.value = 0x8;
			palette->colour[11].g.value = 0x8;
			palette->colour[12].b.value = 0x8;
			palette->colour[13].r.value = 0x6;
			palette->colour[14].g.value = 0x6;
			palette->colour[15].b.value = 0x6;
			
			//Test sprite
			SCPP::VDP::Sprite *test_sprite = new SCPP::VDP::Sprite{
				0,
				0,
				false,
				true,
				true,
				0x80,
				0x80,
				3,
				3,
				nullptr
			};
			
			//VDP test loop
			const SCPP::Backend::Render::Config &render_config = render->GetConfig();
			const SCPP::Backend::Render::PixelFormat &render_pixel_format = render->GetPixelFormat();
			int timer = 30;
			do
			{
				//Start frame and send output information to VDP
				unsigned int pitch;
				void *buffer = render->StartFrame(&pitch);
				
				SCPP::VDP::Output output = {buffer, render_pixel_format, render_config.width, render_config.height, pitch};
				vdp.SetOutput(output);
				
				//Setup frame
				if (--timer <= 0)
				{
					timer = 30;
					switch ((test_sprite->y_flip << 1) | test_sprite->x_flip)
					{
						case 0:
							test_sprite->x_flip = true;
							test_sprite->y_flip = false;
							break;
						case 1:
							test_sprite->x_flip = false;
							test_sprite->y_flip = true;
							break;
						case 2:
							test_sprite->x_flip = true;
							test_sprite->y_flip = true;
							break;
						case 3:
							test_sprite->x_flip = false;
							test_sprite->y_flip = false;
							if (test_sprite->width)
								test_sprite->width--;
							else if (test_sprite->height)
								test_sprite->height--;
							break;
					}
				}
				vdp.SetHeadSprite(test_sprite);
				
				//Draw entire screen
				vdp.WriteScanlines(0, render_config.height);
			} while (!render->EndFrame());
			
			return false;
		}
	}
}
