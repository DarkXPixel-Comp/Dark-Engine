#include "Class.h"
#include "Misc/AssertionMacros.h"

IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(GField);
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(GStruct);
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(GClass);

void GetPrivateStaticClassBody(const TCHAR* Name, GClass*& ReturnClass, void(*RegisterNativeFunc)(), uint32 InSize, uint32 InAlignment, GClass::ClassConstructorType InClassConstructor,
	GClass::StaticClassFunctionType InSuperClassFunc)
{						
	ReturnClass = ::new GClass(EC_StaticConstructor, Name, InSize, InAlignment, InClassConstructor, InSuperClassFunc);

	check(ReturnClass);

	InitializePrivateStaticClass(InSuperClassFunc(), ReturnClass, Name);

	RegisterNativeFunc();

}


void InitializePrivateStaticClass(GClass* SuperStaticClass, GClass* PrivateStaticClass, const TCHAR* Name)
{
	if (SuperStaticClass != PrivateStaticClass)
	{
		PrivateStaticClass->SetSuperStruct(SuperStaticClass);
	}
	else
	{
		PrivateStaticClass->SetSuperStruct(nullptr);
	}

	PrivateStaticClass->RegisterDependencies();
	
	PrivateStaticClass->Register(Name);
}





void GStruct::SetSuperStruct(GStruct* NewSuperStruct)
{
	SuperStruct = NewSuperStruct;
}

void GStruct::RegisterDependencies()
{
	Super::RegisterDependencies();
	if (SuperStruct.Get() != (nullptr))
	{
		SuperStruct->RegisterDependencies();
	}

}




GClass::GClass(EStaticConstructor, FString Name, uint32 InSize, uint32 InAlignment, ClassConstructorType InClassConstructor, StaticClassFunctionType InSuperClassFunc)
{

}


void GClass::SetSuperStruct(GStruct* NewSuperStruct)
{
	Super::SetSuperStruct(NewSuperStruct);


}

GClass* GField::GetOwnerClass() const
{
	GClass* OwnerClass = nullptr;
	GObject* TestObject = const_cast<GField*>(this);
	
	while ((TestObject != nullptr) && (OwnerClass == nullptr))
	{
		OwnerClass = reinterpret_cast<GClass*>(TestObject);
		TestObject = TestObject->GetOuter();
	}

	return nullptr;
}

GStruct* GField::GetOwnerStruct() const
{
	return nullptr;
}
