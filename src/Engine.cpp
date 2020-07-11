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
			return false;
		}
	}
}
