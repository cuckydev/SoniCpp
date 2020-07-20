#pragma once

/*
Project: SoniC++

File: src/Game/Mappings.h
Purpose: Declare the object mappings class

Authors: Regan "cuckydev" Green
*/

//Int sizes
#include <stdint.h>

//Streams and strings
#include <istream>
#include <string>

//Vectors
#include <vector>

//Error class
#include "../Error.h"

//SoniC++ namespace
namespace SCPP
{
	//Game namespace
	namespace Game
	{
		//Mapping namespace
		namespace Mapping
		{
			//Mappings frames and pieces
			struct Piece
			{
				int8_t x, y;
				uint8_t width : 2;
				uint8_t height : 2;
				bool priority : 1;
				uint8_t palette : 2;
				bool x_flip : 1;
				bool y_flip : 1;
				uint16_t pattern : 10;
			};
			
			typedef std::vector<Piece> Frame;
			
			//Base mapping class
			class Base
			{
				protected:
					//Error
					SCPP::Error error;
					
				public:
					//Virtual destructor
					virtual ~Base() {}
					
					//Mapping interface
					virtual const std::vector<Frame> GetFrames() const = 0;
					virtual const Frame &GetFrame(size_t i) const = 0;
					
					//Get error
					const SCPP::Error &GetError() const { return error; }
			};
			
			//Mappings class
			class Mappings : public Base
			{
				private:
					//Loaded frames
					std::vector<Frame> frames;
					
				public:
					//Constructor and destructor
					Mappings(const std::vector<Frame> &_frames) : frames(_frames) {}
					Mappings(std::istream &stream) { Read(stream); }
					~Mappings() override;
					
					//Mapping interface
					const std::vector<Frame> GetFrames() const override { return frames; }
					const Frame &GetFrame(size_t i) const override { return frames[i]; }
					
				private:
					//Internal mapping interface
					bool Read(std::istream &stream);
			};
			
			//Raw mapping class
			class Mapping : public Base
			{
				private:
					//Mapped frame
					Frame frame;
					
				public:
					//Constructor and destructor
					Mapping(const Frame &_frame) : frame(_frame) {}
					~Mapping() override;
					
					//Mapping interface
					const std::vector<Frame> GetFrames() const override { return std::vector<Frame>{frame}; }
					const Frame &GetFrame(size_t i) const override { return frame; }
			};
		}
	}
}
