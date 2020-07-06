#pragma once

/*
Project: SoniC++

File: src/Engine.h
Purpose: Declare the SoniC++ underlaying engine

Authors: Regan "cuckydev" Green
*/

//Error class
#include "Error.h"

//Backend classes
#include "Backend/Core.h"
#include "Backend/Render.h"

//SoniC++ namespace
namespace SCPP
{
	//Engine namespace
	namespace Engine
	{
		//Engine configuration
		enum Config_Backend
		{
			Null,
			SDL2,
		};
		
		struct Config
		{
			Config_Backend backend = Config_Backend::Null;
			SCPP::Backend::Render::Config render_config;
		};
		
		//Engine instance class
		class Instance
		{
			private:
				//Error
				Error error;
				
				//Current configuration
				Config config;
				
				//Backend sub-systems
				SCPP::Backend::Core::Base *core = nullptr;
				SCPP::Backend::Render::Base *render = nullptr;
				
			public:
				//Constructors and destructor
				Instance() {}
				Instance(const Config &_config) { SetConfig(_config); }
				~Instance();
				
				//Engine interface
				bool SetConfig(const Config &_config);
				bool Start();
				
				//Get error
				const SCPP::Error &GetError() const { return error; }
		};
	}
}
