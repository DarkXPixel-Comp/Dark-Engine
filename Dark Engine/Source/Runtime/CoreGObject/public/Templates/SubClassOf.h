#pragma once
#include "Class.h"




template <class T>
class TSubClassOf
{
public:
	TSubClassOf() { StaticClassPrivate = T::StaticClass(); CurrentClass = StaticClassPrivate; }
	TSubClassOf(GClass* Class) { CurrentClass = T::StaticClass()->IsChildOf(Class) ? Class : nullptr; StaticClassPrivate = T::StaticClass(); } //<- Temp

	GClass* GetCurrentClass() const { return CurrentClass; }
	void SetCurrentClass(GClass* InClass) { CurrentClass = StaticClassPrivate->IsChildOf(InClass) ? InClass : nullptr; } //<- Temp
	void ForceSetClass(GClass* InClass) { CurrentClass = InClass; }


	operator GClass* () const { return CurrentClass; }


	GClass* StaticClass() const { return StaticClassPrivate; }

private:
	GClass* CurrentClass = nullptr;
	GClass* StaticClassPrivate;


};