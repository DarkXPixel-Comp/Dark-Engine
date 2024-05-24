#pragma once
#include "Object.h"
#include "ObjectMacros.h"


class GStruct;
class GClass;
class GObject;



void InitializePrivateStaticClass(GClass* SuperStaticClass, GClass* PrivateStaticClass, GClass* WithinStaticClass, const TCHAR* Name);



class GField : public GObject
{
	DECLARE_CASETED_CLASS_INTRINSIC_WITH_API(GField, GObject);



	GClass* GetOwnerClass() const;

	GStruct* GetOwnerStruct() const;

};


class GStruct : public GField
{
	DECLARE_CASETED_CLASS_INTRINSIC_WITH_API(GStruct, GField);
public:
	void SetSuperStruct(GStruct* NewSuperStruct);

	void RegisterDependencies();


private:
	TObjectPtr<GStruct>	SuperStruct;


};

class GClass : public GStruct
{
	DECLARE_CASETED_CLASS_INTRINSIC_WITH_API(GClass, GStruct);

public:
	typedef void		(*ClassConstructorType)				(const FObjectInitializer&);
	typedef GClass* (*StaticClassFunctionType)();

	//GClass(const FObjectInitializer& ObjectInitalizer);
	GClass(GClass* InSuperClass, const FObjectInitializer& ObjectInitalizer);
	GClass(EStaticConstructor, FString Name, uint32 InSize, uint32 InAlignment, ClassConstructorType InClassConstructor, StaticClassFunctionType InSuperClassFunc, StaticClassFunctionType InWithinClassFunc);

	TObjectPtr<GClass> ClassWithin;


	void SetSuperStruct(GStruct* NewSuperStruct);

	template<class T>
	void InternalConstructor(const FObjectInitializer& ObjectInitializer)
	{
		T::__DefaultConstructor(ObjectInitializer);
	}

};



void GetPrivateStaticClassBody(
	const TCHAR* Name,
	GClass*& ReturnClass,
	void(*RegisterNativeFunc)(),
	uint32 InSize,
	uint32 InAlignment,
	GClass::ClassConstructorType InClassConstructor,
	GClass::StaticClassFunctionType InSuperClassFunc, GClass::StaticClassFunctionType InWithinClassFunc
);