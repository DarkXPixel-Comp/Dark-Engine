#pragma once



enum ETickGroup
{
	TG_PrePhysics,
	TG_DuringPhysics,
	TG_EndPhysics
};


struct FTickFunction
{
	ETickGroup TickGroup;

	bool bCanEverTick;
	float TickInterval;
};


struct FEntityTickFunction : public FTickFunction
{												
	class EEntity* Target;


};