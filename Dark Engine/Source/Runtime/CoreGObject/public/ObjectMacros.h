#pragma once
//#include "ObjectBase.h"
//#include "Class.h"


enum EStaticConstructor {EC_StaticConstructor};



#define DECLARE_CLASS(TClass, TSuperClass) \
private:\
	TClass& operator=(TClass&&);\
	TClass& operator=(const TClass&);\
	static class GClass* GetPrivateStaticClass();\
public:	\
	typedef TSuperClass Super;\
	typedef TClass ThisClass;\
	__forceinline static const class GClass* StaticClass()  \
	{										   \
		return GetPrivateStaticClass();			 \
	}  \
	void StaticRegisterNatives##TClass() {}
	
#define RELAY_CONSTRUCTOR(TClass, TSuperClass) TClass(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : TSuperClass(ObjectInitializer) {}

#define DECLARE_CASETED_CLASS_INTRINSIC_WITH_API(TClass, TSuperClass)	\
	DECLARE_CLASS(TClass, TSuperClass) \
	RELAY_CONSTRUCTOR(TClass, TSuperClass)
	
/*
void GetPrivateStaticClassBody(
	const TCHAR* Name,
	GClass*& ReturnClass,
	void(*RegisterNativeFunc)(),
	uint32 InSize,
	uint32 InAlignment,
	GClass::ClassConstructorType InClassConstructor,
	GClass::StaticClassFunctionType InSuperClassFunc, GClass::StaticClassFunctionType InWithinClassFunc
);*/


#define DEFINE_DEFAULT_CONSTRUCTOR_CALL(TClass)\
		static void __DefaultConstructor(const FObjectInitializer& X) {new ((void*)X.GetObj())TClass;}


#define IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(TClass)\
FClassRegistrationInfo RegistrationInfo_GClass_##TClass;\
GClass* TClass::GetPrivateClass()\
{\
	if(!RegistrationInfo_GClass_##TClass.InnerSingleton)\
    {			   \
		GetPrivateStaticClassBody((TCHAR*)TEXT(#TClass) + 1, \
		RegistrationInfo_GClass_##TClass.InnerSingleton, \
		StaticRegisterNatives##TClass(), \
		sizeof(TClass), alignof(TClass), \
		(GClass::ClassConstructorType)InternalConstructor<TClass>, \
		&TClass::Super::StaticClass,   \
		&TClass::WithinClass::StaticClass);	   \
	}\
}\

