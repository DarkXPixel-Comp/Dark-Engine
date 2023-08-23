#pragma once



struct FObjectConstructor
{

};


class UObject
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