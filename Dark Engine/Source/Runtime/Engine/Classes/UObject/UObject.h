#pragma once
#include <Core.h>
#include <Containers/String/DarkString.h>
#include <list>



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
	~UObject() { Destroy(); };
	virtual void BeginPlay();
	virtual void Update(float DeltaTime);
	virtual void Destroy();
	virtual void SetOwner(UObject* owner) final;
	virtual UObject* GetOwner() final;

protected:
	template <typename T>
	T* CreateDefaultSubObject(FString name);
	template<typename T>
	T* Cast(UObject*);

private:
	UObject* m_owner = nullptr;
	std::list<UObject*>	m_objects;


};

template<typename T>
inline T* UObject::CreateDefaultSubObject(FString name)
{
	UObject* object = new T;
	object->SetOwner(this);
	m_objects.push_back(object);
	return reinterpret_cast<T*>(object);
}

template<typename T>
inline T* UObject::Cast(UObject* obj)
{
	return reinterpret_cast<T*>(obj);
}
