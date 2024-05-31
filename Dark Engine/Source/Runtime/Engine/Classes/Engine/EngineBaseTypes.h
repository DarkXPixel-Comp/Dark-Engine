#pragma once


enum ELevelTick
{
	LevelTick_TimeOnly = 0,
	LevelTick_ViewportsOnly = 1,
	LevelTick_All = 2,
	LevelTick_PauseTick = 3
};


enum ETickGroup
{
	TG_PrePhysics,
	TG_DuringPhysics,
	TG_EndPhysics
};


struct FTickFunction
{
	ETickGroup TickGroup;
	float TickInterval;

	bool bCanEverTick;
	bool bStartWithTickEnable;
};


struct FEntityTickFunction : public FTickFunction
{												
	class EEntity* Target;
};

struct GBaseComponentTickFunction : public FTickFunction
{
	class GBaseComponent* Target;
};