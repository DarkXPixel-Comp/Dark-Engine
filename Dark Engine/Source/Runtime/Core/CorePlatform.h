#pragma once

#ifdef _WINDLL
#define DENGINE_API __declspec(dllexport)
#elif _TTEE
#define DENGINE_API
#else
#define DENGINE_API __declspec(dllimport)
#endif // _WINDLL

