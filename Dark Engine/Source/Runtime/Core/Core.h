#pragma once



#ifdef _WINDLL
#define DENGINE_API __declspec(dllexport)
#elif _TTEE
#define DENGINE_API
#else
#define DENGINE_API __declspec(dllimport)
#endif // _WINDLL



#ifdef _DEVELOPMENT
#define DEVELOP_CLASS(name) class name ## Develop
#else
#define DEVELOP_CLASS(name) class name ## _ENCRYPTED
#endif 



