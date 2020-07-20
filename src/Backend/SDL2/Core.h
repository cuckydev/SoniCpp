#pragma once

/*
Project: SoniC++

File: src/Backend/SDL2/Core.h
Purpose: Declare the SDL2 backend core class

Authors: Regan "cuckydev" Green
*/

//Base class declaration
#include "../Core.h"

//SoniC++ namespace
namespace SCPP
{
	//Backend namespace
	namespace Backend
	{
		//Core namespace
		namespace Core
		{
			//Core class
			class SDL2 : public Base
			{
				public:
					//Constructor and destructor
					SDL2();
					~SDL2() override;
			};
		}
	}
}
