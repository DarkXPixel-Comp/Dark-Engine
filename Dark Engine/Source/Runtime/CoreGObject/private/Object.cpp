#include "Object.h"
#include "Class.h"
#include "Containers/Array.h"
#include "Misc/AssertionMacros.h"


IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(GObject);

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

GObject* StaticConstructObjectInternal(const FStaticConstructorObjectParameters& Params)
{
	const GClass* InClass = Params.Class;
	GObject* InOuter = Params.Outer;
	const FString& InName = Params.Name;

	GObject* Result = nullptr;




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

FObjectInitializer::FObjectInitializer(GObject* InObject) : Object(InObject)
{
	ConstructInternal();
}

FObjectInitializer& FObjectInitializer::Get()
{
	FObjectInitializer* Result = AllInitializers.Num() ? AllInitializers.Last() : nullptr;
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


GObject::GObject()
{
	FObjectInitializer* ObjectInitializer = GetCurrentInitializer();

	check(ObjectInitializer);
	check(ObjectInitializer->GetObj() != nullptr && ObjectInitializer->GetObj() != this);

	ObjectInitializer->Object = this;
}

GObject::GObject(const FObjectInitializer& ObjectInitializer)
{
	check(ObjectInitializer.Object != nullptr && ObjectInitializer.Object != this);

	const_cast<FObjectInitializer&>(ObjectInitializer).Object = this;
}
