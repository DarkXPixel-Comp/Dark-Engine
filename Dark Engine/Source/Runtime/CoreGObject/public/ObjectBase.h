#pragma once
#include "Containers/DarkString.h"
#include "Memory/TUniquePtr.h"
#include "ObjectPtr.h"

class GObject;
class GClass;


template <typename T>
struct TRegistrationInfo
{
	using TType = T;

	TType* InnerSingleton = nullptr;
	TType* OuterSingleton = nullptr;
};


using FClassRegistrationInfo = TRegistrationInfo<GClass>;

class GObjectBase
{
public:
	GObjectBase() = default;

	GObjectBase(GClass* InClass, GObject* InOuter, FString InName);


	virtual void RegisterDependencies() {}

	void Register(const TCHAR* InName);

	GObject* GetOuter() const
	{
		return OuterPrivate.Get();
	}

	GClass* GetClass() const
	{
		return ClassPrivate.Get();
	}

	FString GetName() const
	{
		return NamePrivate;
	}



private:



private:
	TObjectPtr<GObject> OuterPrivate;
	TObjectPtr<GClass>	ClassPrivate;
	FString NamePrivate;



};