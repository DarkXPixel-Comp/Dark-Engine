#include "Object.h"
#include "Class.h"
#include "Containers/Array.h"
#include "Misc/AssertionMacros.h"
#include "HAL/DarkMemory.h"

struct FClassParams
{
	GClass* (*ClassNoRegisterFunc)();
	uint32 NumFunctions;
	uint32 NumProperties;
};



void ConstructGClass(GClass*& OutClass, const FClassParams& Params)
{
	if (OutClass)
	{
		return;
	}

	GClass* NewClass = Params.ClassNoRegisterFunc();
	OutClass = NewClass;

	GObjectForceRegister(NewClass);

	//GClass* SuperClass = NewClass->GetSu
}



IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(GObject);
GClass* _Construct_GClass_GObject_NoRegister()
{
	return GObject::StaticClass();
}

const FClassParams _Construct_GClass_GObject_Statics_Class_Params = {&GObject::StaticClass};
GClass* _Construct_GClass_GObject()
{
	if (!RegistrationInfo_GClass_GObject.OuterSingleton)
	{
		ConstructGClass(RegistrationInfo_GClass_GObject.OuterSingleton, _Construct_GClass_GObject_Statics_Class_Params);
	}
	return RegistrationInfo_GClass_GObject.OuterSingleton;
}


//FObjectInitializer* CurrentInitializer = nullptr;

TArray<FObjectInitializer*>	AllInitializers;

FObjectInitializer* GetCurrentInitializer()
{
	FObjectInitializer* Result = AllInitializers.Num() ? AllInitializers.Last() : nullptr;
	return Result;
}

void InitGObject()
{
	GObjectBaseInit();
}



GObject* StaticAllocateObject(const GClass* InClass, GObject* InOuter, FString InName, EObjectFlags InFlags)
{
	GObject* Result = nullptr;
	bool bSubObject = false;
	const bool bCreatingCDO = (InFlags & OF_ClassDefaultObject);


	if (InName == FString())
	{
		InName = FString::PrintF(TEXT("%s%i"), *InClass->GetName(), GGObjectArray.CurrentMaxIndex);
	}

	int32 Index = -1;
	int32 SerialNumber = 0;
	int32 TotalSize = InClass->GetPropertiesSize();

	if(!Result)
	{
		Result = (GObject*)malloc(TotalSize);
	}
	else
	{
		if (!bCreatingCDO && !Result->IsDefaultSubobject())
		{
			Index = Result->InternalIndex;
			Result->~GObject();
		}
		else
		{
			bSubObject = true;
		}
	}
	

	if (!bSubObject)
	{
		FMemory::Memzero(Result, TotalSize);
		new ((void*)Result) GObjectBase(const_cast<GClass*>(InClass), InOuter, InName, Index, SerialNumber);
	}


	return Result;
}


GObject* StaticConstructObjectInternal(const FStaticConstructorObjectParameters& Params)
{
	const GClass* InClass = Params.Class;
	GObject* InOuter = Params.Outer;
	EObjectFlags InFlags = Params.Flags;
	const FString& InName = Params.Name;

	GObject* Result = StaticAllocateObject(InClass, InOuter, InName, InFlags);

	if (true)
	{
		(*InClass->ClassConstructor)(FObjectInitializer(Result, Params));
	}


	return Result;
}



FStaticConstructorObjectParameters::FStaticConstructorObjectParameters(const GClass* InClass):
	Class(InClass)
{

}



FObjectInitializer::FObjectInitializer() : Object(nullptr)
{
	ConstructInternal();
}

FObjectInitializer::FObjectInitializer(GObject* InObject, GObject* InObjectType) : 
	Object(InObject),
	ObjectType(InObjectType)
{
	ConstructInternal();
}

FObjectInitializer::FObjectInitializer(GObject* InObject, const FStaticConstructorObjectParameters& StaticParams) :
	Object(InObject),
	ObjectType(nullptr)

{
	ConstructInternal();

}

FObjectInitializer::FObjectInitializer(GObject* InObject) : Object(InObject)
{
	ConstructInternal();
}

FObjectInitializer& FObjectInitializer::Get()
{		  
	FObjectInitializer* Result = AllInitializers.Num() ? AllInitializers.Last() : nullptr;
	check(Result->GetObj() != nullptr);
	return *Result;
}

void FObjectInitializer::ConstructInternal()
{
	AllInitializers.Push(this);

	if (Object)
	{
		Object->GetClass()->SetupObjectInitializer(*this);
	}
}

FObjectInitializer::~FObjectInitializer()
{
	PostInit();

	check(AllInitializers.Last() == this);
	AllInitializers.PopBack();
}

GObject* FObjectInitializer::CreateDefaultSubobject(GObject* Outer, FString Name, const GClass* ReturnType, const GClass* ClassToCreateByDefault)
{
	GObject* Result = nullptr;

	const GClass* OverrideClass = ClassToCreateByDefault;

	if (OverrideClass)
	{
		GObject* Template = OverrideClass->GetDefaultObject();
		EObjectFlags SubobjectFlags = OF_ClassDefaultSubÎbject;

		const bool bOwnerTemplateInNotCDO = !EnumHasAnyFlags(Outer->GetFlags(), OF_ClassDefaultObject);

		FStaticConstructorObjectParameters Params(OverrideClass);
		Params.Outer = Outer;
		Params.Name = Name;
		Params.Flags = SubobjectFlags;
	
		Result = StaticConstructObjectInternal(Params);

	}




	return Result;
}

void FObjectInitializer::PostInit()
{
	Object->PostInit();
	Object = nullptr;
}


GObject::GObject()
{
	FObjectInitializer* ObjectInitializer = GetCurrentInitializer();

	check(ObjectInitializer);
	check(!(ObjectInitializer->GetObj() != nullptr && ObjectInitializer->GetObj() != this));

	ObjectInitializer->Object = this;
}

GObject::GObject(const FObjectInitializer& ObjectInitializer)
{
	void* Test = this;
	bool Test1 = ObjectInitializer.Object != nullptr;
	bool Test2 = ObjectInitializer.Object != this;

	check(!(ObjectInitializer.Object != nullptr && ObjectInitializer.Object != this));

	const_cast<FObjectInitializer&>(ObjectInitializer).Object = this;
}

GObject::GObject(EStaticConstructor, EObjectFlags InFlags) : GObjectBase(InFlags)
{
}

GObject* GObject::CreateDefaultSubobject(FString Name, GClass* ReturnType, GClass* ClassCreateByDefault)
{
	check(AllInitializers.Num());
	FObjectInitializer* CurrentInitializer = AllInitializers.Last();

	return CurrentInitializer->CreateDefaultSubobject(this, Name, ReturnType, ClassCreateByDefault);
}
