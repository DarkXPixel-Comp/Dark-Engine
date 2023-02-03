#pragma once

//#include <assert.h>
#include <vector>


#define FUNC_DECLARE_MULTICAST_DELEGATE(DelegateName, ReturnType, ...) typedef TMultiCastDelegate<ReturnType, __VA_ARGS__> DelegateName
#define FUNC_DECLARE_DELEGATE(DelegateName, ReturnType, ...) typedef TDelegate<ReturnType, __VA_ARGS__> DelegateName


#define DECLARE_DELEGATE( DelegateName ) FUNC_DECLARE_DELEGATE( DelegateName, void )
#define DECLARE_MULTICAST_DELEGATE( DelegateName ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void )

#define DECLARE_DELEGATE_OneParam( DelegateName, Param1Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type )
#define DECLARE_MULTICAST_DELEGATE_OneParam( DelegateName, Param1Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type )

#define DECLARE_DELEGATE_TwoParams( DelegateName, Param1Type, Param2Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type, Param2Type )
#define DECLARE_MULTICAST_DELEGATE_TwoParams( DelegateName, Param1Type, Param2Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type, Param2Type )

#define DECLARE_DELEGATE_ThreeParams( DelegateName, Param1Type, Param2Type, Param3Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type )
#define DECLARE_MULTICAST_DELEGATE_ThreeParams( DelegateName, Param1Type, Param2Type, Param3Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type )

#define DECLARE_DELEGATE_FourParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type )
#define DECLARE_MULTICAST_DELEGATE_FourParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type )

#define DECLARE_DELEGATE_FiveParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type )
#define DECLARE_MULTICAST_DELEGATE_FiveParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type )

#define DECLARE_DELEGATE_SixParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type )
#define DECLARE_MULTICAST_DELEGATE_SixParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type )

#define DECLARE_DELEGATE_SevenParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type )
#define DECLARE_MULTICAST_DELEGATE_SevenParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type )

#define DECLARE_DELEGATE_EightParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type, Param8Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type, Param8Type )
#define DECLARE_MULTICAST_DELEGATE_EightParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type, Param8Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type, Param8Type )

#define DECLARE_DELEGATE_NineParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type, Param8Type, Param9Type ) FUNC_DECLARE_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type, Param8Type, Param9Type )
#define DECLARE_MULTICAST_DELEGATE_NineParams( DelegateName, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type, Param8Type, Param9Type ) FUNC_DECLARE_MULTICAST_DELEGATE( DelegateName, void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type, Param6Type, Param7Type, Param8Type, Param9Type )







class IContainer {
public: virtual void Call() = 0;
};


template<typename InRetValType, typename ...ParamTypes>
class IIContainer
{
public:
	virtual InRetValType Call(ParamTypes...) = 0;

};




template <typename UserClass, typename InRetValType, typename... ParamTypes>
class TMethodContainter : public IIContainer<InRetValType, ParamTypes...>
{
	using FFuncPtr = InRetValType(UserClass::*)(ParamTypes...);
public:
	TMethodContainter(UserClass* cl, FFuncPtr func) : uClass(cl), pFunc(func)
	{

	}


	InRetValType Call(ParamTypes... params)
	{
		return (uClass->*pFunc)(params...);
	}


private:

	FFuncPtr pFunc;
	UserClass* uClass;
};

template<typename InRetValType, typename... ParamTypes>
class TFuncContainer : public IIContainer<InRetValType, ParamTypes...>
{
	using FFuncPtr = InRetValType(*)(ParamTypes...);

public:
	TFuncContainer(FFuncPtr func) : pFunc(func) {}

	InRetValType Call(ParamTypes... params)
	{
		return pFunc(params...);
	}

private:
	FFuncPtr pFunc;

};

template <typename InRetValType, typename... ParamTypes>
class TDelegate
{
public:
	void Bind(InRetValType(*func)(ParamTypes...))
	{
		container = new TFuncContainer<InRetValType, ParamTypes...>(func);


	}
	template<typename UserClass>
	void Bind(UserClass* inUserObject, InRetValType(UserClass::* func)(ParamTypes...))
	{
		container = new TMethodContainter<UserClass, InRetValType, ParamTypes...>(inUserObject, func);

	}




	InRetValType BroadCast(ParamTypes... params)
	{
		if (container)
		{
			return container->Call();
		}
	}

	IIContainer<InRetValType, ParamTypes...>* container;

};

template <typename InRetValType, typename... ParamTypes>
class TMultiCastDelegate
{
	using FFuncPtr = InRetValType(*)(ParamTypes...);

public:
	void Bind(InRetValType(*func)(ParamTypes...))
	{
		IIContainer<InRetValType, ParamTypes...>* temp = new TFuncContainer<InRetValType, ParamTypes...>(func);

		containers.push_back(temp);

		pFunc = func;
	}
	template<typename UserClass>
	void Bind(UserClass* inUserObject, InRetValType(UserClass::* func)(ParamTypes...))
	{
		//container = new TMethodContainter<UserClass, InRetValType, ParamTypes...>(inUserObject, func);

		//containers.push_back(new TMethodContainter < UserClass, InRetValType, ParamTypes...)(inUserObject, func);

		IIContainer<InRetValType, ParamTypes...>* temp = new TMethodContainter<UserClass, InRetValType, ParamTypes...>(inUserObject, func);

		containers.push_back(temp);
	}




	void BroadCast(ParamTypes... params)
	{
		for (auto& i : containers)
		{
			i->Call(params...);
		}

	}




	




	std::vector<IIContainer<InRetValType, ParamTypes...>*> containers;

	IIContainer<InRetValType, ParamTypes...>* container;
	FFuncPtr pFunc;
};





