#include "ObjectBase.h"
#include "Containers/UnordoredMap.h"
#include "Misc/AssertionMacros.h"
#include "Containers/Array.h"
#include "Class.h"
#include "DeferredRegistry.h"

struct FPendingRegistrant
{
	GObjectBase* Object = nullptr;
	FPendingRegistrant* NextAutoRegister = nullptr;

	FPendingRegistrant(GObjectBase* InObject):
		Object(InObject)
	{}
};

static FPendingRegistrant* GFirstPendingRegistrant = nullptr;
static FPendingRegistrant* GLastPendingRegistrant = nullptr;



static void GetPendingAutoRegistrants(TArray<FPendingRegistrant>& OutRegistrants)
{
	FPendingRegistrant* NextPendingRegistrant = GFirstPendingRegistrant;
	GFirstPendingRegistrant = nullptr;
	GLastPendingRegistrant = nullptr;


	while (NextPendingRegistrant)
	{
		FPendingRegistrant* PendingRegistrant = NextPendingRegistrant;
		OutRegistrants.Add(*PendingRegistrant);
		NextPendingRegistrant = PendingRegistrant->NextAutoRegister;
		delete PendingRegistrant;
	}
}


struct FPendingRegistrantInfo
{
	const TCHAR* Name;
	FPendingRegistrantInfo(const TCHAR* InName) :
		Name(InName)
	{

	}

	static TMap<GObjectBase*, FPendingRegistrantInfo>& GetMap()
	{
		static TMap<GObjectBase*, FPendingRegistrantInfo> RegistrantInfo;
		return RegistrantInfo;
	}
};


GObjectBase::GObjectBase() : ClassPrivate(NoInit), OuterPrivate(NoInit), NamePrivate(NoInit)
{

}

GObjectBase::GObjectBase(GClass* InClass, GObject* InOuter,FString InName, int32 InIndex, int32 InSerialNumber, EObjectFlags InFlags) :
	ClassPrivate(InClass),
	OuterPrivate(InOuter),
	NamePrivate(InName),
	Flags(InFlags)
{
	check(ClassPrivate);

	AddObject(InName, InIndex, InSerialNumber);
}

GObjectBase::GObjectBase(EObjectFlags InFlags) : Flags(InFlags), InternalIndex(-1), ClassPrivate(nullptr), OuterPrivate(nullptr)
{

}

bool GObjectBase::IsDefaultSubobject() const
{
	return !EnumHasAnyFlags(Flags, OF_ClassDefaultObject) && GetOuter() && (EnumHasAnyFlags(GetOuter()->GetFlags(), OF_ClassDefaultObject));
}

void GObjectBase::DeferredRegister(GClass* StaticClass, const TCHAR* InName)
{
	OuterPrivate = nullptr;
	ClassPrivate = StaticClass;


	AddObject(InName);




}


void GObjectBase::Register(const TCHAR* InName)
{
	TMap<GObjectBase*, FPendingRegistrantInfo>& MapRegistrants = FPendingRegistrantInfo::GetMap();

	FPendingRegistrant* PendingRegistration = new FPendingRegistrant(this);

	MapRegistrants.emplace(this, FPendingRegistrantInfo(InName));

	if (GLastPendingRegistrant)
	{
		GLastPendingRegistrant->NextAutoRegister = PendingRegistration;
	}
	else
	{
		GFirstPendingRegistrant = PendingRegistration;
	}

	GLastPendingRegistrant = PendingRegistration;

}

GObject* GObjectBase::GetTypedOuter(GClass* Class) const
{
	GObject* Result = nullptr;

	for (GObject* NextOuter = GetOuter(); Result == nullptr && NextOuter != nullptr; NextOuter = NextOuter->GetOuter())
	{
		if (NextOuter->Is(Class))
		{
			Result = NextOuter;
		}
	}
	return Result;
}

void GObjectBase::AddObject(FString InName, int32 InIndex, int32 InSerialNumber)
{
	NamePrivate = InName;

	GGObjectArray.AllocateObjectIndex(this);



}


void GObjectForceRegister(GObjectBase* Object)
{
	TMap<GObjectBase*, FPendingRegistrantInfo>& PendingRegistrants = FPendingRegistrantInfo::GetMap();

	auto It = PendingRegistrants.find(Object);

	if (It != PendingRegistrants.end())
	{
		const TCHAR* Name = It->second.Name;
		PendingRegistrants.erase(Object);
		Object->DeferredRegister(GClass::StaticClass(), Name);
	}

}


bool IsClassChildOf(const GClass* Class, const GClass* TestClass)
{
	return Class->IsChildOf(TestClass);
}

void GObjectBaseInit()
{
	FClassDeferredRegistry& ClassRegistry = FClassDeferredRegistry::Get();

	ClassRegistry.DoPendingInnerRegistrations();
	ClassRegistry.DoPendingOuterRegistrations([](FClassDeferredRegistry::FRegistrant& Registrant)
		{
			GClass* Class = Registrant.OuterRegisterFn();
			Class->GetDefaultObject();
		});


	TArray<FPendingRegistrant> PendingRegistrants;
	GetPendingAutoRegistrants(PendingRegistrants);

	for (int32 i = 0; i < PendingRegistrants.Num(); ++i)
	{
		const FPendingRegistrant& PendingRegistrant = PendingRegistrants[i];

		GObjectForceRegister(PendingRegistrant.Object);

		GetPendingAutoRegistrants(PendingRegistrants);
	}

	


}
