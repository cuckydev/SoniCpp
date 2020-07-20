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
				
				//Screen dimensions
				unsigned int width, height;
				
			public:
				//Constructor and destructor
				GameMode(SCPP::Engine::Instance *_engine);
				~GameMode();
				
				//Game mode interface
				SCPP::GameMode *Iterate();
				
				unsigned int GetWidth() const { return width; }
				unsigned int GetHeight() const { return height; }
				void GetScreenDimensions(unsigned int *_width, unsigned int *_height) const
				{
					if (_width != nullptr)
						*_width = width;
					if (_height != nullptr)
						*_height = height;
				}
		};
	}
}
