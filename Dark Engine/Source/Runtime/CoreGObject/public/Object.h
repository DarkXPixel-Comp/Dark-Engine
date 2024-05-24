#pragma once
#include "ObjectMacros.h"
#include "ObjectBase.h"


class GObject;


extern void InitGObject();
extern void ExitGObject();




struct FStaticConstructorObjectParameters
{  
	FStaticConstructorObjectParameters(const GClass* InClass);

	const GClass* Class;
	GObject* Outer;
	FString Name;


};



template<typename T>
T* NewObject(GObject* Outer, FString Name)
{
	//FStaticConstructorObjectParameters Params()



}



class FObjectInitializer
{
public:
	FObjectInitializer();

	FObjectInitializer(GObject* InObject, GObject* InObjectType);

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

};


/*\brief Game Object
* Base class for all game objects in Engine
* 
*/
class GObject : public GObjectBase
{
	DECLARE_CLASS(GObject, GObjectBase);


	GObject();
	GObject(const FObjectInitializer& ObjectInitializer);
	GObject(EStaticConstructor);



};





