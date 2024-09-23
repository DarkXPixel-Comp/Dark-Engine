#pragma once


#include "SceneComponent.h"
#include "PxRigidDynamic.h"
#include "PxPtr.h"
#include "PrimitiveSceneProxy.h"


struct FPrimitiveComponentId
{
	FPrimitiveComponentId() : PrimIdValue(UINT64_MAX)
	{
	}
	FORCEINLINE bool IsValid() const
	{
		return PrimIdValue != UINT64_MAX;
	}

	FORCEINLINE bool operator==(const FPrimitiveComponentId& Other) const
	{
		return PrimIdValue == Other.PrimIdValue;
	}

	static uint64 GetHash(FPrimitiveComponentId Id)
	{
		return std::hash<uint64>{}(Id.PrimIdValue);
	}


	uint64 PrimIdValue;
};


struct FPrimitiveSceneInfoData
{
	FPrimitiveSceneProxy* SceneProxy = nullptr;
	float LastRenderTime = 0;
	FPrimitiveComponentId PrimitiveSceneId;



	static uint64 NextPrimitiveId;

	FPrimitiveSceneInfoData()
	{
		PrimitiveSceneId.PrimIdValue = ++NextPrimitiveId;
	}
};



class GPrimitiveComponent : public GSceneComponent
{
	DECLARE_CLASS_INTINSIC_NO_CTOR_WITH_PROPERTIES(GPrimitiveComponent, GSceneComponent,
		{
			DECLARE_PROPERTY(float, Mass, Mass, PROPERTY_META(DECLARE_SETTER(SetMass)));
		});

public:
	GPrimitiveComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	~GPrimitiveComponent();

	constexpr virtual bool ShouldCreateRenderState() const override { return true; }
	constexpr virtual bool ShouldCreatePhysicState() const override { return true; }

	virtual void SetWorldLocation(FVector NewLocation) override;

	virtual void CreateRenderState() override;
	virtual void CreatePhysicState() override;

	FPrimitiveSceneInfoData& GetSceneData() { return SceneData; }

	virtual FMatrix GetRenderMatrix() const;

	FMatrix RenderMatrix = FMatrix::Identity;


	IMPLEMENT_SETTER(SetMass, float);

	virtual void SetMass(float NewMass)
	{
		NewMass = NewMass >= 0 ? NewMass : 0;
		Mass = NewMass;
		RigidDynamic->setMass(Mass);
	}

	virtual FPrimitiveSceneProxy* CreateSceneProxy()
	{
		/*SceneProxy = new FPrimitiveSceneProxy(this, GetName());
		SceneData.SceneProxy = SceneProxy;*/
		return nullptr;
	}

	FPrimitiveSceneProxy* SceneProxy = nullptr;
	FPrimitiveSceneProxy* GetSceneProxy() const { check(SceneProxy == SceneData.SceneProxy); return SceneData.SceneProxy; }

protected:
	TPxPtr<physx::PxRigidDynamic> RigidDynamic;
	FPrimitiveSceneInfoData SceneData;
	float Mass = 5.f;


private:
	bool bUpdatePrimitiveSceneDescription = false;


};

