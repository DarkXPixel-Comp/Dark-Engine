#pragma once

#include "Entity.h"
#include "Templates/SubClassOf.h"





class EMeshObject : public EEntity
{
	DECLARE_CLASS_INTINSIC_NO_CTOR_WITH_PROPERTIES(EMeshObject, EEntity, { 
		DECLARE_PROPERTY_INT32(TestProp, TestProp);
		DECLARE_PROPERTY(TSubClassOf, SubClass, SubClass);
		});
public:
	EMeshObject(const FObjectInitializer& ObjectInitalizer = FObjectInitializer::Get());



private:
	TObjectPtr<class GStaticMeshComponent> MeshComponent;
	TSubClassOf<GObjectBase> SubClass;
	int32 TestProp = -7;
};