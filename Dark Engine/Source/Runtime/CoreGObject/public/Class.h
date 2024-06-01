#pragma once
#include "Object.h"
#include "ObjectMacros.h"


class GStruct;
class GClass;
class GObject;



void InitializePrivateStaticClass(GClass* SuperStaticClass, GClass* PrivateStaticClass, const TCHAR* Name);



/*struct Test
{
	int k = 7;
};


using PropPtr = int(Test::*);


int main()
{
	PropPtr test1 = &Test::k;

	Test ex;

	ex.k = 5;

	std::cout << (Test*)(nullptr)->*test1;
}
*/

template<class T>
struct TProperty
{
	void* PropertyPointer = nullptr;
	uint64 SizeProperty = 0;	
};



class GProperty: public GObject
{
	DECLARE_CLASS_INTINSIC_NO_CTOR(GProperty, GObject);
public:
	GProperty() {}
	GProperty(const FObjectInitializer& ObjectInitializer) {}

	void SetOffsetProperty(uint64 InOffset)
	{
		Offset = InOffset;
	}

	void SetMinMaxValue(int64 InMin, int64 InMax)
	{
		MinValue = InMin;
		MaxValue = InMax;
	}

	void SetMoveSpeed(float InMoveSpeed)
	{
		MoveSpeed = InMoveSpeed;
	}

	void* GetValue(GObject* Object)
	{
		return (void*)(((char*)Object) + Offset);
	}

	int32& GetInt32(GObject* Object)
	{
		return *(int32*)(((char*)Object) + Offset);
	}

	//virtual void SetPropertyPointer(void* Ptr) {};

	enum class EType
	{
		uint8,
		int8,
		uint16,
		int16,
		uint32,
		int32,
		uint64,
		int64,
		GObject,
		FVector,
		FRotator
	};

	EType GetType() const { return Type; }
	void SetType(EType InType) { Type = InType; }


	int64 MinValue = 0;
	int64 MaxValue = 0;
	float MoveSpeed = 1.f;

protected:
	EType Type;
	uint64 Offset = 0;
};

class GPropertyInt32 : public GProperty
{
	DECLARE_CLASS_INTINSIC_NO_CTOR(GPropertyInt32, GProperty);
public:
	GPropertyInt32();
	GPropertyInt32(const FObjectInitializer& ObjectInitializer);

	
private:


};




class GField : public GObject
{
	DECLARE_CASETED_CLASS_INTRINSIC_WITH_API(GField, GObject);


public:
	GField(EStaticConstructor, EObjectFlags = OF_NoFlags);


	GClass* GetOwnerClass() const;

	GStruct* GetOwnerStruct() const;

};


class GStruct : public GField
{
	DECLARE_CASETED_CLASS_INTRINSIC_WITH_API(GStruct, GField);
public:
	GStruct(int32 InSize, int32 InAlignment, EObjectFlags InFlags = OF_NoFlags);

	void RegisterProperty(GProperty* NewProperty);


	int32 GetPropertiesSize() const { return PropertiesSize; }
	int32 GetAlignment() const { return Alignment; }

	bool IsChildOf(const GStruct* SomeBase) const;


	GStruct* GetSuperStruct() const { return SuperStruct; }
	void SetSuperStruct(GStruct* NewSuperStruct);

	void RegisterDependencies();

	const TArray<TObjectPtr<GProperty>>& GetProperties() const
	{
		return Properties;
	}


private:
	TObjectPtr<GStruct>	SuperStruct;
	TArray<TObjectPtr<GProperty>> Properties;
	int32 PropertiesSize = 0;
	int32 Alignment = 0;


};

class GClass : public GStruct
{
	DECLARE_CASETED_CLASS_INTRINSIC_WITH_API(GClass, GStruct);

public:
	typedef void		(*ClassConstructorType)				(const FObjectInitializer&);
	typedef GClass* (*StaticClassFunctionType)();

	ClassConstructorType ClassConstructor;

	//GClass(const FObjectInitializer& ObjectInitalizer);
	GClass(GClass* InSuperClass, const FObjectInitializer& ObjectInitalizer);
	GClass(EStaticConstructor, FString Name, uint32 InSize, uint32 InAlignment, ClassConstructorType InClassConstructor, StaticClassFunctionType InSuperClassFunc);

	GClass* GetSuperClass() const
	{
		return (GClass*)GetSuperStruct();
	}

	GObject* CreateDefaultObject();

	GObject* GetDefaultObject() const
	{
		const_cast<GClass*>(this)->CreateDefaultObject();
		return ClassDefaultObject;
	}


	TObjectPtr<GObject>	ClassDefaultObject;
	//TObjectPtr<GClass> ClassWithin;

	virtual void SetupObjectInitializer(FObjectInitializer&) const {}



	void SetSuperStruct(GStruct* NewSuperStruct);

};



void GetPrivateStaticClassBody(
	const TCHAR* Name,
	GClass*& ReturnClass,
	void(*RegisterNativeFunc)(),
	uint32 InSize,
	uint32 InAlignment,
	GClass::ClassConstructorType InClassConstructor,
	GClass::StaticClassFunctionType InSuperClassFunc
);