#pragma once

#include "Entity.h"
#include "Templates/SubClassOf.h"
#include "Logging/LogMacros.h"



DECLARE_LOG_CATEGORY(MeshObjectLog, Log);


class EMeshObject : public EEntity
{
	DECLARE_CLASS_INTINSIC_NO_CTOR_WITH_PROPERTIES(EMeshObject, EEntity,
		{
		DECLARE_USE_SCRIPTS(TRUE);
		DECLARE_PROPERTY_INT32(TestProp, TestProp);
		DECLARE_PROPERTY(TSubClassOf, SubClass, SubClass);
		DECLARE_PROPERTY(TObjectPtr, MeshComponent, MeshComponent);
		},
		{
			DECLARE_FUNCTION(EMeshObject, TestPrint);
		});
public:
	EMeshObject(const FObjectInitializer& ObjectInitalizer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	FORCEINLINE void TestPrint(std::string InputText)
	{
		DE_LOG(MeshObjectLog, Log, TEXT("%s"), *FString(InputText));
	}



private:
	TObjectPtr<class GStaticMeshComponent> MeshComponent;
	TSubClassOf<class GPrimitiveComponent> SubClass;
	int32 TestProp = -7;
};