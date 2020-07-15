#pragma once

/*
Project: SoniC++

File: src/Game/GameMode.h
Purpose: Declare the game game mode class

Authors: Regan "cuckydev" Green
*/

//Base class
#include "../GameMode.h"

//Engine class
#include "../Engine.h"

//SoniC++ namespace
namespace SCPP
{
	//Game namespace
	namespace Game
	{
		//Game mode class
		class GameMode : public SCPP::GameMode
		{
			private:
				//Parent engine instance
				SCPP::Engine::Instance *engine;
				
			public:
				//Constructor and destructor
				GameMode(SCPP::Engine::Instance *_engine);
				~GameMode();
				
				//Game mode interface
				SCPP::GameMode *Iterate();
		};
	}
}
