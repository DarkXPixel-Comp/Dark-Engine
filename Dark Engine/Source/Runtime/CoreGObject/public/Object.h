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
T* NewObject(GObject* Outer, const GClass* Class, FString Name = FString(), EObjectFlags Flags = OF_NoFlags)
{
	FStaticConstructorObjectParameters Params(Class);

	Params.Outer = Outer;
	Params.Name = Name;
	Params.Flags = Flags;

	T* Result = nullptr;
	Result = static_cast<T*>(StaticConstructObjectInternal(Params));

	check(Result);
	return Result;

}

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

extern GObject* StaticAllocateObject(const GClass* InClass, GObject* InOuter, FString InName, EObjectFlags InFlags = OF_NoFlags);


class FObjectInitializer
{
public:
	FObjectInitializer();

	FObjectInitializer(GObject* InObject, GObject* InObjectType);

	FObjectInitializer(GObject* InObject, const FStaticConstructorObjectParameters& StaticParams);

	FObjectInitializer(GObject* InObject);

	~FObjectInitializer();

	GObject* CreateDefaultSubobject(GObject* Outer, FString Name, const GClass* ReturnType, const GClass* ClassToCreateByDefault);

	void PostInit();

	FORCEINLINE GObject* GetObj() const
	{
		return Object;
	}


	static FObjectInitializer& Get();

private:
	void ConstructInternal();

public:

private:
	struct FOverrides
	{
		struct FOverrideDetails
		{
			const GClass* Class = nullptr;
			FOverrides* SubOverrides;
		};
	};

	struct FSubObjectsToInit
	{
		void Add(GObject* SubObject, GObject* Template)
		{
			for (int32 Index = 0; Index < SubObjectInits.Num(); ++Index)
			{
				check(SubObjectInits[Index].SubObject != SubObject);
			}
			SubObjectInits.Emplace(SubObject, Template);
		}

		struct FSubObjectInit
		{
			GObject* SubObject;
			GObject* Template;
			FSubObjectInit(GObject* InSubObject, GObject* InTemplate):
				SubObject(InSubObject),
				Template(InTemplate)
			{
			}
		};
		TArray<FSubObjectInit> SubObjectInits = decltype(SubObjectInits)(ArrayReserve(8));
	} ComponentInits;

	FOverrides SubjectOverrides;



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

	GObject* CreateDefaultSubobject(FString Name, GClass* ReturnType, GClass* ClassCreateByDefault);



	template<class T>
	T* CreateDefaultSubobject(FString Name)
	{
		GClass* ReturnType = T::StaticClass();
		return static_cast<T*>(CreateDefaultSubobject(Name, ReturnType, ReturnType));
	}





};





