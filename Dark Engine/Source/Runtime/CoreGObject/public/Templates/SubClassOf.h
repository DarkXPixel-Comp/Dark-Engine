#pragma once
#include "Class.h"




template <class T>
class TSubClassOf
{
public:
	TSubClassOf() = default;
	TSubClassOf(GClass* Class) { CurrentClass = Class; } //<- Temp

	GClass* GetCurrentClass() const { return CurrentClass; }
	void SetCurrentClass(GClass* InClass) { CurrentClass = InClass; } //<- Temp




private:
	GClass* CurrentClass = nullptr;


};