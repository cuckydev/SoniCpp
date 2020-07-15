/*
Project: SoniC++

File: src/Game/GameMode.h
Purpose: Define the level game mode class

Authors: Regan "cuckydev" Green
*/

//Declaration
#include "GameMode.h"

//SoniC++ namespace
namespace SCPP
{
	//Game namespace
	namespace Game
	{
		//Constructor
		GameMode::GameMode(SCPP::Engine::Instance *_engine) : engine(_engine)
		{
			
		}
		
		//Destructor
		GameMode::~GameMode()
		{
			
		}
		
		//Game mode interface
		SCPP::GameMode *GameMode::Iterate()
		{
			//Get backend sub-systems
			SCPP::Backend::Render::Base *render = engine->GetRender();
			SCPP::Backend::Event::Base *event = engine->GetEvent();
			
			//Get system modules
			SCPP::VDP::Instance &vdp = engine->GetVDP();
			
			while (!event->HandleEvents())
			{
				
			}
			return nullptr;
		}
	}
}
