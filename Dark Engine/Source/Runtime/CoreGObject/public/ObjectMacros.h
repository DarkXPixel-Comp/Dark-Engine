#pragma once
#include "Misc/EnumClassFlags.h"
#include "HAL/Platform.h"
#include "DeferredRegistry.h"

extern void GObjectForceRegister(class GObjectBase* Object);


enum EStaticConstructor {EC_StaticConstructor};


enum EObjectFlags
{
	OF_NoFlags = 0,
	OF_ClassDefaultObject = 1 << 0,
	OF_MarkAsNative = 1 << 1,
	OF_ClassDefaultSub╬bject = 1 << 2
};


ENUM_CLASS_FLAGS(EObjectFlags);

#define DECLARE_CLASS(TClass, TSuperClass) \
private:\
	TClass& operator=(TClass&&);\
	TClass& operator=(const TClass&);\
	static class GClass* GetPrivateStaticClass();\
public:	\
	typedef TSuperClass Super;\
	typedef TClass ThisClass;\
	inline static class GClass* StaticClass()  \
	{										   \
		return GetPrivateStaticClass();			 \
	}// \
	//static void StaticRegisterNatives##TClass() {}


#define DEFINE_CLASS_PROPERTIES(TClass, Code) \
public: \
inline static void StaticRegisterNatives##TClass()\
{Code;}

	
#define RELAY_CONSTRUCTOR(TClass, TSuperClass) TClass(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : TSuperClass(ObjectInitializer) {}

#define DEFINE_DEFAULT_CONSTRUCTOR_CALL(TClass)\
		static void __DefaultConstructor(const FObjectInitializer& X) {new ((void*)X.GetObj())TClass;}
#define DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(TClass)\
		static void __DefaultConstructor(const FObjectInitializer& X) {new ((void*)X.GetObj())TClass(X);}

#define DECLARE_CLASS_INTINSIC_NO_CTOR(TClass, TSuperClass) \
	DECLARE_CLASS(TClass, TSuperClass) \
	/*DEFINE_DEFAULT_CONSTRUCTOR_CALL(TClass) \	*/ \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(TClass) \
	DEFINE_CLASS_PROPERTIES(TClass, NULL)


#define DECLARE_CLASS_INTINSIC_NO_CTOR_WITH_PROPERTIES(TClass, TSuperClass, PropertiesCode) \
	DECLARE_CLASS(TClass, TSuperClass) \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(TClass) \
	DEFINE_CLASS_PROPERTIES(TClass, PropertiesCode)

#define DECLARE_CASETED_CLASS_INTRINSIC_WITH_API_WITH_PROPERTIES(TClass, TSuperClass, PropertiesCode)	\
	DECLARE_CLASS(TClass, TSuperClass) \
	RELAY_CONSTRUCTOR(TClass, TSuperClass) \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(TClass) \
	DEFINE_CLASS_PROPERTIES(TClass, PropertiesCode)

#define DECLARE_CASETED_CLASS_INTRINSIC_WITH_API(TClass, TSuperClass)	\
	DECLARE_CLASS(TClass, TSuperClass) \
	RELAY_CONSTRUCTOR(TClass, TSuperClass) \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(TClass) \
	DEFINE_CLASS_PROPERTIES(TClass, NULL)
	


#define IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(TClass)\
FClassRegistrationInfo RegistrationInfo_GClass_##TClass;\
GClass* TClass::GetPrivateStaticClass()\
{\
	if(!RegistrationInfo_GClass_##TClass.InnerSingleton)\
    {			   \
		GetPrivateStaticClassBody((TCHAR*)TEXT(#TClass), \
		RegistrationInfo_GClass_##TClass.InnerSingleton, \
		StaticRegisterNatives##TClass, \
		sizeof(TClass), alignof(TClass), \
		(GClass::ClassConstructorType)InternalConstructor<TClass>, \
		&TClass::Super::StaticClass);	   \
	}\
	return RegistrationInfo_GClass_##TClass.InnerSingleton;\
}



#define IMPLEMENT_INTRINSIC_CLASS(TClass, TSuperClass) \
	class GClass* _Construct_GClass_##TClass(); \
	extern FClassRegistrationInfo RegistrationInfo_GClass_##TClass;\
	struct 	_Construct_GClass_##TClass##_Statics \
	{\
		static GClass* Construct() \
		{  \
			extern class GClass* _Construct_GClass_##TSuperClass();\
			GClass* SuperClass = _Construct_GClass_##TSuperClass(); \
			GClass* Class = TClass::StaticClass();\
			GObjectForceRegister(Class);\
			return Class; \
		}\
	}; \
	GClass* _Construct_GClass_##TClass() \
	{\
		if(!RegistrationInfo_GClass_##TClass.OuterSingleton)\
		{ \
			RegistrationInfo_GClass_##TClass.OuterSingleton = _Construct_GClass_##TClass##_Statics::Construct();\
		} \
		return RegistrationInfo_GClass_##TClass.OuterSingleton;\
	} \
	static FRegisterCompiledInfo AutoInitializer_##TClass(&_Construct_GClass_##TClass, TClass::StaticClass, TEXT(#TClass), RegistrationInfo_GClass_##TClass);\
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(TClass)

