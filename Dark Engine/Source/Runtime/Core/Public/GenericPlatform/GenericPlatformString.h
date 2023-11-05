#pragma once


#include "CoreTypes.h"
#include "Templates/EnableIf.h"


#include <type_traits>




namespace DE::Core::Private
{
	DENGINE_API int32 GetConvertedLength(const UTF8CHAR* Dest, const WIDECHAR* Src);


}


struct FGenericPlatformString/* : public FGenericPlatformStricmp*/
{

};