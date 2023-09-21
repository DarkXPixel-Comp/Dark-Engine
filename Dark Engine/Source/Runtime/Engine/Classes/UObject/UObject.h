#pragma once
#include <Core.h>



struct FObjectConstructor
{

};


class DENGINE_API UObject
{
private:

protected:

public:
	//UObject(FObjectConstructor constructor) {}
	UObject() {};
	~UObject() {};
	virtual void BeginPlay() {}
	virtual void Update(float DeltaTime) {}
	virtual void Destroy() {}
};