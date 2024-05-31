#pragma once
#include "ObjectMacros.h"
#include "ObjectBase.h"
#include "Misc/AssertionMacros.h"


class GObject;
class GClass;


extern void InitGObject();
extern void ExitGObject();




struct FStaticConstructorObjectParameters
{  
	FStaticConstructorObjectParameters(const GClass* InClass);

	const GClass* Class;
	GObject* Outer;
	FString Name;
	EObjectFlags Flags = OF_NoFlags;


};


GObject* StaticConstructObjectInternal(const FStaticConstructorObjectParameters& Params);


template<class T>
T* NewObject(GObject* Outer)
{
	FStaticConstructorObjectParameters Params(T::StaticClass());
	Params.Outer = Outer;

	T* Result = nullptr;

	Result = static_cast<T*>(StaticConstructObjectInternal(Params));


	check(Result);

	return Result;
}



template<class T>
T* NewObject(GObject* Outer, FString Name, EObjectFlags Flags = OF_NoFlags)
{
	FStaticConstructorObjectParameters Params(T::StaticClass());
	Params.Outer = Outer;
	Params.Name = Name;

	T* Result = nullptr;

	Result = static_cast<T*>(StaticConstructObjectInternal(Params));


	check(Result);

	return Result;
}



class FObjectInitializer
{
public:
	FObjectInitializer();

	FObjectInitializer(GObject* InObject, GObject* InObjectType);

	FObjectInitializer(GObject* InObject, const FStaticConstructorObjectParameters& StaticParams);


	FObjectInitializer(GObject* InObject);

	FORCEINLINE GObject* GetObj() const
	{
		return Object;
	}


	static FObjectInitializer& Get();

private:
	void ConstructInternal();

public:
	~FObjectInitializer();




private:
	GObject* Object; //<- Object to initialize
	GObject* ObjectType;//<- Object to copy properties


	GObject* LastConstructedObject = nullptr;//<- Previously constructed object

private:
	friend class GObject;

};


template<class T>
void InternalConstructor(const FObjectInitializer& X)
{
	T::__DefaultConstructor(X);
}

/*\brief Game Object
* Base class for all game objects in Engine
* 
*/
class GObject : public GObjectBase
{
	DECLARE_CLASS(GObject, GObject);
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(GObject);
	DEFINE_CLASS_PROPERTIES(GObject, NULL);
	typedef GObject WithinClass;
public:
	GObject();
	GObject(const FObjectInitializer& ObjectInitializer);
	GObject(EStaticConstructor, EObjectFlags = OF_NoFlags);

	//void RegisterProperty()





};





