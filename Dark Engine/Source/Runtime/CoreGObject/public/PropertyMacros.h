#pragma once
#include "HAL/Platform.h"
#include "Object.h"
#include "Class.h"
#include "Templates/SubClassOf.h"
#include "Templates/DarkTemplate.h"


#define DECLARE_PROPERTY(TypeProperty, PropertyName, Name, Meta) \
{ \
	using PropertyType = GProperty;\
	GStruct* CurrentStruct = (GStruct*)StaticClass(); \
	PropertyType* Property = NewObject<PropertyType>(StaticClass(), TEXT(#Name));\
	Property->SetType(GProperty::EType::P##TypeProperty);\
	Property->SetOffsetProperty(DE_OFFSETOF(ThisClass, PropertyName));\
	CurrentStruct->RegisterProperty((GProperty*)Property);\
	Meta;\
}


#define DECLARE_PROPERTY_OBJECT()
#define DECLARE_PROPERTY_INT32(PointerToProperty, Name) \
	DECLARE_PROPERTY(int32, PointerToProperty, Name)


#define PROPERTY_META(Code) \
{\
	int64 MAX = 0; \
	int64 MIN = 0; \
	float MoveSpeed = 1.f;\
	Code; \
	Property->SetMinMaxValue(MIN, MAX);\
	Property->SetMoveSpeed(MoveSpeed);\
}

#define DECLARE_SETTER(Func)	\
{	  \
	void(GObject::*Setter)(void*, uint64) = (GProperty::FuncPtr)&ThisClass::__##Func##Setter_Impl;\
	Property->SetSetter(Setter);\
}  \


#define IMPLEMENT_SETTER(Name, ArgType) \
	void __##Name##Setter_Impl(void* Data, uint64 Size)	 \
{			  \
	check(Size == sizeof(ArgType));\
	Name(*((ArgType*)Data));\
}