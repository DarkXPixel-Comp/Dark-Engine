#pragma once


#include "SceneComponent.h"


class GPrimitiveComponent : public GSceneComponent
{
	DECLARE_CLASS_INTINSIC_NO_CTOR_WITH_PROPERTIES(GPrimitiveComponent, GSceneComponent);

public:
	GPrimitiveComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	~GPrimitiveComponent();

	constexpr virtual bool ShouldCreateRenderState() const override { return true; }

	virtual void CreateRenderState() override;



};

