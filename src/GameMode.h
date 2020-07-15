#pragma once

/*
Project: SoniC++

File: src/GameMode.h
Purpose: Provide the base game mode class

Authors: Regan "cuckydev" Green
*/

//Error class
#include "Error.h"

//SoniC++ namespace
namespace SCPP
{
	class GameMode
	{
		protected:
			//Error
			SCPP::Error error;
			
		public:
			//Virtual destructor
			virtual ~GameMode() {}
			
			//Game mode interface
			virtual GameMode *Iterate() = 0;
			
			//Get error
			const SCPP::Error &GetError() const { return error; }
	};
}
