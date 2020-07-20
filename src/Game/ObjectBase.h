#pragma once

/*
Project: SoniC++

File: src/Game/ObjectBase.h
Purpose: Declare the object base class

Authors: Regan "cuckydev" Green
*/

//Int sizes
#include <stdint.h>

//Fixed point
#include "../Fixed.h"

//VDP classes
#include "../VDP.h"

//Mappings classes
#include "Mappings.h"

//SoniC++ namespace
namespace SCPP
{
	//Game namespace
	namespace Game
	{
		//Prototypes
		class Level;
		
		//Object bit-fields
		#pragma pack(push) //Make sure all bit-fields are packed
		#pragma pack(1)
		
		struct ObjectRenderFlags
		{
			bool on_screen : 1;
			bool behind_loop : 1; //Used by Sonic
			bool raw_mappings : 1; //Unused
			bool assume_height : 1;
			bool align_background : 1; //I can tell this is gonna be interesting
			bool align_level : 1;
			bool y_flip : 1;
			bool x_flip : 1;
		};
		
		struct ObjectArtTile
		{
			bool priority : 1;
			uint8_t palette : 2;
			bool y_flip : 1;
			bool x_flip : 1;
			uint16_t pattern : 10;
		};
		
		struct ObjectCollisionResponse
		{
			enum
			{
				ObjColResponse_Enemy,
				ObjColResponse_IncRoutine,
				ObjColResponse_Hurt,
				ObjColResponse_Special,
			} type : 2;
			uint8_t size : 6;
		};
		
		#pragma pack(pop)
		
		//Object base class
		class ObjectBase
		{
			protected:
				//Parent level
				Level *parent_level = nullptr;
				
				//Render information
				union
				{
					ObjectRenderFlags render_flags;
					uint8_t render_field = 0;
				};
				
				union
				{
					ObjectArtTile art_tile;
					uint16_t art_field = 0;
				};
				
				Mapping::Base *mappings = nullptr;
				
				//Position
				union
				{
					struct
					{
						Fixed_S16_16 x, y;
					} level;
					struct
					{
						uint16_t x = 0, y = 0;
					} screen;
				} pos;
				
				//Speeds
				Fixed_S8_8 x_vel, y_vel, inertia;
				
				//Radii
				uint8_t y_radius = 0, x_radius = 0;
				
				//Other visual stuff (including animation)
				uint8_t priority = 0;
				uint8_t width_pixels = 0;
				
				uint8_t frame = 0, anim_frame = 0;
				uint8_t anim = 0, prev_anim = 0;
				uint8_t frame_time = 0;
				
				//Collision
				union
				{
					ObjectCollisionResponse collision_response;
					uint8_t collision_response_field = 0;
				};
				uint8_t collision_property = 0;
				
				//Routine
				uint8_t routine = 0, routine_secondary = 0;
				
				//Angle
				uint8_t angle = 0;
				
			public:
				//Virtual destructor
				virtual ~ObjectBase() {}
				
				//Object interface
				virtual void Update() = 0;
				bool BuildSprites(SCPP::VDP::Sprite **prev);
				
			private:
				//Internal object interface
				bool DrawSprite(SCPP::VDP::Sprite **prev, uint16_t x, uint16_t y);
		};
	}
}
