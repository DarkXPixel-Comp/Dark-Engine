#pragma once
#include <type_traits>
#include "CoreTypes.h"




#ifdef _DEVELOPMENT
#define DEVELOP_CLASS(name) class name ## Develop
#else
#define DEVELOP_CLASS(name) class name ## _ENCRYPTED
#endif 



