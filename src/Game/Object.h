#pragma once

/*
Project: SoniC++

File: src/Game/Object.h
Purpose: Declare the base class for level objects

Authors: Regan "cuckydev" Green
*/

//Base class
#include "ObjectBase.h"

//SoniC++ namespace
namespace SCPP
{
	//Game namespace
	namespace Game
	{
		//Status bitfield
		#pragma pack(push)
		#pragma pack(1)
		
		struct ObjectStatus
		{
			bool anything : 1;
			bool player2_pushing : 1; //UPGRADE: 2 players
			bool player1_pushing : 1;
			bool player2_standing : 1; //UPGRADE: 2 players
			bool player1_standing : 1;
			bool unused_2 : 1;
			bool y_flip : 1;
			bool x_flip : 1;
		};
		
		#pragma pack(pop)
		
		//Object class
		class Object : public ObjectBase
		{
			protected:
				//Status
				union
				{
					ObjectStatus status;
					uint8_t status_field = 0;
				};
				
				//Respawn index
				uint8_t respawn_index = 0;
				
			public:
				//Virtual destructor
				virtual ~Object() {}
				
				//Object interface
				virtual void Update() = 0;
		};
	}
}
