#pragma once

/*
Project: SoniC++

File: src/Backend/Core.h
Purpose: Declare the backend core base class

Authors: Regan "cuckydev" Green
*/

//Error class
#include "../Error.h"

//SoniC++ namespace
namespace SCPP
{
	//Backend namespace
	namespace Backend
	{
		//Core namespace
		namespace Core
		{
			//Core base class
			class Base
			{
				protected:
					//Error
					SCPP::Error error;
					
				public:
					//Virtual destructor
					virtual ~Base() {}
					
					//Get error
					const SCPP::Error &GetError() const { return error; }
			};
		}
	}
}
