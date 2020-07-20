#pragma once

/*
Project: SoniC++

File: src/Fixed.h
Purpose: Provide a struct for fixed point arithmatic

Authors: Regan "cuckydev" Green
*/

//SoniC++ namespace
namespace SCPP
{
	//Fixed point struct
	template <typename V, typename U, typename L>
	struct Fixed
	{
		union
		{
			struct
			{
				#ifdef SCPP_BIG_ENDIAN
					U upper;
					L lower;
				#else
					L lower;
					U upper;
				#endif
			} fixed;
			V value = 0;
		};
	};
	
	//Typical fixed types
	typedef Fixed<int32_t,int16_t,uint16_t> Fixed_S16_16;
	typedef Fixed<uint32_t,uint16_t,uint16_t> Fixed_16_16;
	typedef Fixed<int16_t,int8_t,uint8_t> Fixed_S8_8;
	typedef Fixed<uint16_t,uint8_t,uint8_t> Fixed_8_8;
}
