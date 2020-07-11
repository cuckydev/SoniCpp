/*
Project: SoniC++

File: src/Main.cpp
Purpose: Provide the entry point

Authors: Regan "cuckydev" Green
*/

#include "Engine.h"

#include <iostream>

int main(int argc, char *argv[])
{
	//Engine configuration
	SCPP::Engine::Config engine_config = {
		//Backend to use
		SCPP::Engine::Config_Backend::SDL2,
		
		//Render configuration
		{
			"SoniC++", //Title
			320, 240, 2, //Dimensions and scale
			60, //Framerate
			false, //Fullscreen
		}
	};
	
	//Initialize and run engine
	SCPP::Engine::Instance engine;
	if (engine.SetConfig(engine_config) ||
		engine.Start())
	{
		//Error output
		std::cout << "An error occured: " << engine.GetError() << std::endl;
		return -1;
	}
	
	//No errors
	return 0;
}
