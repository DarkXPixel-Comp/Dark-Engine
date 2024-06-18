#pragma once
#include "Object.h"
#include "PropertyMacros.h"
#include "Engine/EngineBaseTypes.h"
#include "Math/MathFwd.h"


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
		//	DECLARE_PROPERTY(GObject, SubClass, SubClass);
		//	DECLARE_PROPERTY(GObject, BaseComponent, BaseComponent);
		//	DECLARE_PROPERTY(GObject, SceneComponent, RenderComponent);
		});

public:
	EEntity();

	EEntity(const FObjectInitializer& ObjectInitalizer);

	void DispatchBeginPlay();

	virtual void BeginPlay();



	void AddOwnedComponent(class GBaseComponent* Component);

	class GBaseComponent* AddComponentByClass(GClass* Class);

	void FinishAddComponent(class GBaseComponent* NewComponent);

	void Init();

	void SetOwner(EEntity* InOwner);

	virtual	void PreRegisterAllComponents();

	virtual void RegisterAllComponents();

	void SetRootComponent(class GSceneComponent* NewRootComponent);

		
	class FWorld* GetWorld() const;
	class GLevel* GetLevel() const;

	template<class T>
	void GetComponents(TArray<T*>& OutComponents) const
	{
		OutComponents.Empty();
		ForEachComponent_Internal<T>(T::StaticClass(), [&OutComponents](T* InComp)
			{
				OutComponents.Add(InComp);
			});
	}


	template<class T, typename Func>
	void ForEachComponent_Internal(TSubClassOf<GBaseComponent> ComponentClass, Func InFunc) const
	{
		for (auto OwnedComponent : OwnedComponents)
		{
			if (OwnedComponent)
			{
				if (OwnedComponent->Is(ComponentClass))
				{
					InFunc(static_cast<T*>(OwnedComponent));
				}
			}

		}


	}


	bool IsBeginPlay() const { return bBeginPlay; }

	
	void PostSpawnInitialize(const FTransform& SpawnTransform, EEntity* InOwner);

	FEntityTickFunction PrimaryTick;

	class GSceneComponent* GetRootComponent() const { return RootComponent; }

	

private:
	bool bBeginPlay = false;

	TObjectPtr<EEntity>	Owner;
	TArray<TObjectPtr<EEntity>>	Children;
	TObjectPtr<GSceneComponent>	RootComponent;
	TSet<TObjectPtr<GBaseComponent>> OwnedComponents;



	int32 Test;
	TObjectPtr<GTest> SubClass;
	TObjectPtr<GBaseComponent> BaseComponent;
	TObjectPtr<class GSceneComponent>	SceneComponent;


};