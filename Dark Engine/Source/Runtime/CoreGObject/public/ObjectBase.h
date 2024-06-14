#pragma once
#include "Containers/DarkString.h"
#include "ObjectPtr.h"
#include "GObjectArray.h"
#include "ObjectMacros.h"
#include "DeferredRegistry.h"


class GClass;
class GObject;



bool IsClassChildOf(const GClass* Class, const GClass* TestClass);



class GObjectBase
{
protected:
	GObjectBase();
public:
	GObjectBase(GClass* InClass, GObject* InOuter, FString InName, int32 InIndex, int32 InSerialNumber);
	GObjectBase(EObjectFlags InFlags);

	bool IsDefaultSubobject() const;


	void DeferredRegister(GClass* StaticClass, const TCHAR* InName);

	EObjectFlags GetFlags() const { return Flags; }

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

	const FString& GetName() const
	{
		return NamePrivate;
	}



	template<typename OtherClass>
	bool Is(OtherClass SomeBase)
	{
		const GClass* SomeBaseClass = SomeBase;

		const GClass* ThisClass = GetClass();

		return IsClassChildOf(ThisClass, SomeBaseClass);
	}

	GObject* GetTypedOuter(GClass* Class) const;


	template<typename T>
	T* GetTypedOuter() const
	{
		return (T*)(GetTypedOuter(T::StaticClass()));
	}


	int32 InternalIndex;

private:
	void AddObject(FString InName, int32 InInternalNumber = -1, int32 SerialNumber = 0);



private:
	TObjectPtr<GObject> OuterPrivate;
	TObjectPtr<GClass>	ClassPrivate;
	FString NamePrivate;
	EObjectFlags Flags;
};



void GObjectBaseInit();

extern void GObjectForceRegister(GObjectBase* Object);