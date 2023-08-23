#pragma once
#include <Engine/public/DEngine.h>



template<typename T>
class TGlobalPtr
{
public:
	TGlobalPtr();
	TGlobalPtr(T* ptr);


public:
	T* Reset(T* obj);
	T* Get() { return pointer; }
	void Release();



private:
	T* pointer = nullptr;
};


