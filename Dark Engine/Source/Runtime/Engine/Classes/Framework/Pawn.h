#pragma once
#include "Framework/Entity.h"


class EPawn : public EEntity
{	
	DECLARE_CLASS_INTINSIC_NO_CTOR(EPawn, EEntity);
public:
	EPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	




};