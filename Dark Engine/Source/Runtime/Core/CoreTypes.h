#pragma once

#pragma once


typedef unsigned int uint32_t;
typedef long long unsigned int uint64_t;
typedef short unsigned int uint16_t;
typedef short int int16_t;
typedef int int32_t;
typedef long long int int64_t;
typedef signed char int8_t;
typedef uint32_t BOOL32;

typedef long long intmax_t;
typedef unsigned long long uintmax_t;




#define MAX_NAME_STRING 255
//#define HInstance() GetModuleHandle(NULL)


#define INT8_MIN         (-127i8 - 1)
#define INT16_MIN        (-32767i16 - 1)
#define INT32_MIN        (-2147483647i32 - 1)
#define INT64_MIN        (-9223372036854775807i64 - 1)
#define INT8_MAX         127i8
#define INT16_MAX        32767i16
#define INT32_MAX        2147483647i32
#define INT64_MAX        9223372036854775807i64
#define UINT8_MAX        0xffui8
#define UINT16_MAX       0xffffui16
#define UINT32_MAX       0xffffffffui32
#define UINT64_MAX       0xffffffffffffffffui64



#ifdef BUILD_DLL
#define GameEngine_API __declspec(dllexport)

#else
#define GameEngine_API __declspec(dllimport)

#endif





