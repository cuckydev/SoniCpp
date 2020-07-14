#pragma once

/*
Project: SoniC++

File: src/Attributes.h
Purpose: Provide common attributes

Authors: Regan "cuckydev" Green
*/

#ifdef __GNUC__

#define ATTRIBUTE_HOT __attribute__((hot))
#define LIKELY(condition) __builtin_expect((condition), 1)
#define UNLIKELY(condition) __builtin_expect((condition), 0)
#define PREFETCH(address, isWrite, locality) __builtin_prefetch((address), (isWrite), (locality))

#else

#define ATTRIBUTE_HOT
#define LIKELY(condition) condition
#define UNLIKELY(condition) condition
#define PREFETCH(address, isWrite, locality)

#endif
