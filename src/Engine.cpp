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
	#include "Backend/SDL2/Event.h"
#endif

//Game classes
#include "Game/GameMode.h"

//SoniC++ namespace
namespace SCPP
{
	//Engine namespace
	namespace Engine
	{
		//Destructor
		Instance::~Instance()
		{
			//Delete current game mode in case it wasn't already
			delete game_mode;
			
			//Delete backend sub-systems
			delete event;
			delete render;
			delete core;
		}
		
		//Engine interface
		bool Instance::SetConfig(const Config &_config)
		{
			//Handle new backends
			if ((core == nullptr || render == nullptr) || _config.backend != config.backend)
			{
				//Delete old backend sub-systems
				delete event;
				delete render;
				delete core;
				
				//Create new backend sub-systems
				switch (_config.backend)
				{
				#ifdef SCPP_COMPILE_SDL2
					case Config_Backend::SDL2:
						core = new SCPP::Backend::Core::SDL2();
						render = new SCPP::Backend::Render::SDL2();
						event = new SCPP::Backend::Event::SDL2();
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
			
			if (event == nullptr)
				return error.Push("Failed to create backend event sub-system instance");
			else if (event->GetError())
				return error.Push(event->GetError());
			
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
			if (vdp.Allocate(2048, 4))
				return error.Push(vdp.GetError());
			
			//Initialize and run game (start with level game mode)
			if ((game_mode = new SCPP::Game::GameMode(this)) == nullptr)
				return error.Push("Failed to create level game mode instance");
			
			while (game_mode != nullptr)
			{
				SCPP::GameMode *next = game_mode->Iterate();
				if (game_mode->GetError())
				{
					delete next;
					return error.Push(game_mode->GetError());
				}
				if (next != game_mode)
				{
					delete game_mode;
					game_mode = next;
				}
			}
			return false;
		}
	}
}
