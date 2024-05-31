#pragma once
#include "Object.h"
#include "PropertyMacros.h"
#include "Engine/EngineBaseTypes.h"


class GTest : public GObject
{
	DECLARE_CASETED_CLASS_INTRINSIC_WITH_API_WITH_PROPERTIES(GTest, GObject,
		{
			DECLARE_PROPERTY(uint32, SubObjectInt, SubObjectInt);
		});

public:

	uint32 SubObjectInt = 511;

};


class EEntity : public GObject
{
	//DECLARE_CASETED_CLASS_INTRINSIC_WITH_API(GEntity, GObject);
	DECLARE_CLASS_INTINSIC_NO_CTOR_WITH_PROPERTIES(EEntity, GObject,
		{
			DECLARE_PROPERTY_INT32(Test, Test);
			DECLARE_PROPERTY_INT32(Ahuet, NewProperty);
			DECLARE_PROPERTY(uint32, uintTest, NameUINT, PROPERTY_META(MIN = 7; MAX = 66));
			DECLARE_PROPERTY(GObject, ObjectPtr, ObjectPtr);
		});

public:
	EEntity();

	EEntity(const FObjectInitializer& ObjectInitalizer);

	void Init();

	FEntityTickFunction PrimaryTick;

	int32 Test = 4;
	int32 Ahuet;
	uint32 uintTest;

	TObjectPtr<GTest> ObjectPtr;

private:


};