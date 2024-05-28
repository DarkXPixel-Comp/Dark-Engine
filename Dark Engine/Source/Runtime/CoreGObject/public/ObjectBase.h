#pragma once
#include "Containers/DarkString.h"
#include "ObjectPtr.h"
#include "GObjectArray.h"



class GClass;
class GObject;


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
protected:
	GObjectBase() = default;
public:
	GObjectBase(GClass* InClass, GObject* InOuter, FString InName, int32 InIndex, int32 InSerialNumber);


	void DeferredRegister(GClass* StaticClass, const TCHAR* InName);


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


	int32 InternalIndex;

private:
	void AddObject(FString InName, int32 InInternalNumber = -1, int32 SerialNumber = 0);



private:
	TObjectPtr<GObject> OuterPrivate;
	TObjectPtr<GClass>	ClassPrivate;
	FString NamePrivate;
};


void GObjectBaseInit();