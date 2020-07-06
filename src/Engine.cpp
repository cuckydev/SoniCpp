/*
Project: SoniC++

File: src/Engine.cpp
Purpose: Define the SoniC++ underlaying engine

Authors: Regan "cuckydev" Green
*/

//Declaration
#include "Engine.h"

//Backends
#ifdef SCPP_COMPILE_SDL2
	#include "Backend/SDL2/Core.h"
	#include "Backend/SDL2/Render.h"
#endif

#include <iostream>

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
			SCPP::Backend::Render::PixelFormat pixel_format = render->GetPixelFormat();
			for (int i = 0; ; i += 426)
			{
				unsigned int *data;
				int pitch;
				if ((data = (unsigned int*)render->StartFrame(&pitch)) == nullptr)
					return error.Push(render->GetError());
				for (int v = 0; v <= i; v++)
					*data++ = pixel_format.MapRGBA(0x00, 0x80, 0x80, 0xFF);
				if (render->EndFrame())
					return error.Push(render->GetError());
			}
			return false;
		}
	}
}
