#pragma once
#include <Core.h>
#include <Containers/DarkString.h>
#include <list>
#include "Containers/Array.h"

#define ADD_VARIABLE(VAR) AddVariable<decltype(VAR)>(#VAR, &VAR)



struct FObjectConstructor
{

};





enum ETypeProperties
{
	None,
	Integer,
	Float,
	String,
	Pointer,
	Vector2,
	Vector3,
	Vector4,
	Array,
	Boolean,
	Custom
};




struct FObjectProperties
{
	FObjectProperties(FString InName, ETypeProperties InTypeProperties, void* InPropertiesPtr) : 
		Name(InName),
		TypeProperties(InTypeProperties),
		Properties(InPropertiesPtr)	
	{
	}

	FString Name;
	ETypeProperties TypeProperties;
	void* Properties;
};



class DENGINE_API UObject
{
private:

protected:

public:
	//UObject(FObjectConstructor constructor) {}
	UObject() { Name = "UObject" + std::to_string(CountObjects++); };
	~UObject() { Destroy(); };
	virtual void BeginPlay();
	virtual void Update(float DeltaTime);
	virtual void Destroy();
	virtual void SetOwner(UObject* owner) final;
	virtual UObject* GetOwner() final;
	virtual void SetName(FString InName) final { Name = InName; }
	virtual FString GetName() { return Name; }
	virtual const TArray<FObjectProperties>& GetProperties() const { return ObjectProperties; }

	template<typename T>
	void AddVariable(FString Name, void* Variable)
	{

	}

	UObject* GetOuter() const
	{
		return m_owner;
	}



protected:
	template <typename T>
	T* CreateDefaultSubObject(FString name);
	template<typename T>
	T* Cast(UObject*);

	void AddProperties(FObjectProperties Properties);

private:
	UObject* m_owner = nullptr;
	std::list<UObject*>	m_objects;
	static uint64 CountObjects;
	FString Name;

	TArray<FObjectProperties> ObjectProperties;


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
