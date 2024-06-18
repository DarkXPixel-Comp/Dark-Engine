#include "Class.h"
#include "Misc/AssertionMacros.h"

IMPLEMENT_INTRINSIC_CLASS(GField, GObject);
IMPLEMENT_INTRINSIC_CLASS(GStruct, GField);
IMPLEMENT_INTRINSIC_CLASS(GClass, GStruct);
IMPLEMENT_INTRINSIC_CLASS(GProperty, GObject);
IMPLEMENT_INTRINSIC_CLASS(GPropertyInt32, GProperty);


TArray<GClass*> AllClasses;


//IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(GField);
//IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(GStruct);
//IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(GClass);

void GetPrivateStaticClassBody(const TCHAR* Name, GClass*& ReturnClass, void(*RegisterNativeFunc)(), uint32 InSize, uint32 InAlignment, GClass::ClassConstructorType InClassConstructor,
	GClass::StaticClassFunctionType InSuperClassFunc)
{	
	ReturnClass = (GClass*)malloc(sizeof(GClass));
	ReturnClass = ::new(ReturnClass) GClass(EC_StaticConstructor, Name, InSize, InAlignment, InClassConstructor, InSuperClassFunc);

	check(ReturnClass);

	InitializePrivateStaticClass(InSuperClassFunc(), ReturnClass, Name);

	ReturnClass->RegisterSuperProperties();
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

	AllClasses.Add(PrivateStaticClass);
}





GStruct::GStruct(int32 InSize, int32 InAlignment, EObjectFlags InFlags) : 
	GField(EC_StaticConstructor, InFlags),
	SuperStruct(nullptr),
	PropertiesSize(InSize),
	Alignment(InAlignment)
{

}

void GStruct::RegisterSuperProperties()
{
	if(SuperStruct)
	{
		for (auto& i : SuperStruct->GetProperties())
		{
			Properties.Add(i);
		}
	}
}

void GStruct::RegisterProperty(GProperty* NewProperty)
{
	Properties.Add(NewProperty);
}

bool GStruct::IsChildOf(const GStruct* SomeBase) const
{
	check(this);

	if (SomeBase == nullptr)
	{
		return false;
	}

	for (const GStruct* TempStruct = this; TempStruct; TempStruct = TempStruct->GetSuperStruct())
	{
		if (TempStruct == SomeBase)
		{
			return true;
		}
	}
	return false;
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




GClass::GClass(EStaticConstructor, FString Name, uint32 InSize, uint32 InAlignment, ClassConstructorType InClassConstructor, StaticClassFunctionType InSuperClassFunc):
	GStruct(InSize, InAlignment),
	ClassConstructor(InClassConstructor)
{

}

const TArray<GClass*>& GClass::GetAllClasses()
{
	return AllClasses;
}


GObject* GClass::CreateDefaultObject()
{
	if (ClassDefaultObject == nullptr)
	{
		GClass* ParentClass = GetSuperClass();
		GObject* ParentDefaultObject = nullptr;
		if (ParentClass)
		{
			GObjectForceRegister(ParentClass);
			ParentDefaultObject = ParentClass->GetDefaultObject();
		}


		if (ParentDefaultObject != nullptr || this == GObject::StaticClass())
		{
			if (ClassDefaultObject == nullptr)
			{
				ClassDefaultObject = StaticAllocateObject(this, GetOuter(), "", OF_ClassDefaultObject);
				check(ClassDefaultObject);


				(*ClassConstructor)(FObjectInitializer(ClassDefaultObject, ParentDefaultObject));
			}
		}
	}


	return ClassDefaultObject;
}


void GClass::SetSuperStruct(GStruct* NewSuperStruct)
{
	Super::SetSuperStruct(NewSuperStruct);


}

GField::GField(EStaticConstructor, EObjectFlags InFlags) : GObject(EC_StaticConstructor, InFlags)
{

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

GPropertyInt32::GPropertyInt32() : Super()
{
	Type = EType::Pint32;
}

GPropertyInt32::GPropertyInt32(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Type = EType::Pint32;
}
