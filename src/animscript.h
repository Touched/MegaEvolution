#ifndef ANIM_SCRIPT_H_
#define ANIM_SCRIPT_H_

#include "types.h"

// Basic types
#define ANIM_LONG(x) (u8) (((u32) (x)) & 0xFF), \
    (u8) ((((u32) (x)) & 0xFF00) >> 8),		\
    (u8) ((((u32) (x)) & 0xFF0000) >> 16),	\
    (u8) ((((u32) (x)) & 0xFF000000) >> 24)
	
#define ANIM_SHORT(x) (u8) (((u32) (x)) & 0xFF),	\
    (u8) ((((u32) (x)) & 0xFF00) >> 8)
	
// Script commands
// Table at 083ADF5C
#define ANIM_LOAD_GRAPHICS(a, b) 0, ANIM_SHORT(a), ANIM_SHORT(b)
// 1
// 2

// word:			funcptr
// byte:			priority (of task)
// byte:			length
// short[lenth]:	data
#define ANIM_CALLASM(a, b, c) ANIM_LONG(a), b, c // 17 bytes
	
#endif
