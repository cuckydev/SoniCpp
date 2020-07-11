/*
Project: SoniC++

File: src/VDP.cpp
Purpose: Define VDP (Video Display Processor) classes

Authors: Regan "cuckydev" Green
*/

//Declaration
#include "VDP.h"

//SoniC++ namespace
namespace SCPP
{
	//VDP namespace
	namespace VDP
	{
		//VDP instance class
		//Destructor
		Instance::~Instance()
		{
			//Unload data
			if (pattern != nullptr)
				delete[] pattern;
			if (palette != nullptr)
				delete[] palette;
			DeleteSprites();
		}
		
		//VDP interface
		void Instance::SetOutput(const Output &_output)
		{
			//Use given output
			output = _output;
		}
		
		bool Instance::Allocate(size_t _patterns, size_t _palettes)
		{
			if (pattern != nullptr)
				delete[] pattern;
			if ((pattern = new uint8_t[(patterns = _patterns) * 4 * 3]) == nullptr)
				return error.Push("Failed to allocate patterns");
				
			if (palette != nullptr)
				delete[] palette;
			if ((palette = new Palette[palettes = _palettes]) == nullptr)
				return error.Push("Failed to allocate palettes");
			return false;
		}
		
		template <typename T>
		void Instance::WriteScanlines(const unsigned from, const unsigned to)
		{
			
		}
	}
}
