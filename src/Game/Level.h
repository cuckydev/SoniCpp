/*
Project: SoniC++

File: src/Game/Level.h
Purpose: Declare the level class

Authors: Regan "cuckydev" Green
*/

//Game mode class
#include "GameMode.h"

//Error class
#include "Error.h"

//Level classes
#include "Object.h"

//SoniC++ namespace
namespace SCPP
{
	//Game namespace
	namespace Game
	{
		//Level class
		class Level
		{
			friend class ObjectBase;
			
			protected:
				//Error
				Error error;
				
				//Parent game mode
				GameMode *parent_game = nullptr;
				
				//Loaded players and objects
				//std::vector<Player*> players;
				std::vector<Object*> objects;
				
			public:
				//Constructor and destructor
				Level(GameMode *_parent_game);
				~Level();
				
				//Get error
				const SCPP::Error &GetError() const { return error; }
		};
	}
}
