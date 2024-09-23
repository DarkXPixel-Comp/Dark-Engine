#pragma once

#include "Entity.h"
#include "Templates/SubClassOf.h"





class EMeshObject : public EEntity
{
	DECLARE_CLASS_INTINSIC_NO_CTOR_WITH_PROPERTIES(EMeshObject, EEntity, { 
		DECLARE_PROPERTY_INT32(TestProp, TestProp);
		DECLARE_PROPERTY(TSubClassOf, SubClass, SubClass);
		DECLARE_PROPERTY(TObjectPtr, MeshComponent, MeshComponent);
		});
public:
	EMeshObject(const FObjectInitializer& ObjectInitalizer = FObjectInitializer::Get());

	virtual void BeginPlay() override;



private:
	TObjectPtr<class GStaticMeshComponent> MeshComponent;
	TSubClassOf<class GPrimitiveComponent> SubClass;
	int32 TestProp = -7;
};