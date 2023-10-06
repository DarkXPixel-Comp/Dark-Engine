#pragma once

#define SET_TYPEDEF(type, def) typedef type def

#ifdef _WIN64
struct FGenericPlatformTypes
{
	SET_TYPEDEF(signed char, int8);
	SET_TYPEDEF(unsigned char, uint8);
	SET_TYPEDEF(short, int16);
	SET_TYPEDEF(unsigned short, uint16);
	SET_TYPEDEF(signed int, int32);
	SET_TYPEDEF(unsigned int, uint32);
	SET_TYPEDEF(signed long  long, int64);
	SET_TYPEDEF(unsigned long long, uint64);


	typedef char ANSICHAR;

	typedef wchar_t WIDECHAR;

	// An 8-bit character type. In-memory only. 8-bit representation. Should really be char8_t but making this the generic option is easier for compilers which don't fully support C++20 yet.
	enum UTF8CHAR : unsigned char {};

	
	typedef uint8 CHAR8;
	typedef uint16 CHAR16;
	typedef uint32 CHAR32;
	typedef WIDECHAR TCHAR;

	typedef int32 TYPE_OF_NULL;
	typedef decltype(nullptr) TYPE_OF_NULLPTR;



};
#endif // _WIN64

#undef SET_TYPEDEF
