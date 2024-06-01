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
			DECLARE_PROPERTY(GObject, SubClass, SubClass);
			DECLARE_PROPERTY(GObject, BaseComponent, BaseComponent);
			DECLARE_PROPERTY(GObject, SceneComponent, RenderComponent);
		});

public:
	EEntity();

	EEntity(const FObjectInitializer& ObjectInitalizer);

	void AddOwnedComponent();

	class GBaseComponent* AddComponentByClass(GClass* Class);

	void FinishAddComponent(GBaseComponent* NewComponent);

	void Init();

	FEntityTickFunction PrimaryTick;

	

private:
	int32 Test;
	TObjectPtr<GTest> SubClass;
	TObjectPtr<GBaseComponent> BaseComponent;
	TObjectPtr<class GSceneComponent>	SceneComponent;


};