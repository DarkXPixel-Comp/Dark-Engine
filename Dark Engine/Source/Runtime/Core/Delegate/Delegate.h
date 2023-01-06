#pragma once

//#include <assert.h>


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







//
////   онтейнер дл€ хранени€ до 2-х аргументов.
//struct NIL {};
//class IArguments { public: virtual ~IArguments() {} };
//	template< class T1 = NIL, class T2 = NIL >
//	 class Arguments : public IArguments
//	 {
//		public: Arguments() {}
//		public: Arguments(T1 i_arg1) :
//		arg1(i_arg1) {}
//		public: Arguments(T1 i_arg1, T2 i_arg2) :
//				arg1(i_arg1), arg2(i_arg2) {}
//				public: T1 arg1; T2 arg2;
//	 };
//
//						 //   онтейнер дл€ хранени€ указател€ на метод.
//class IContainer {
//public: virtual void Call(IArguments*) = 0; };			
//template< class T, class M > class Container : public IContainer {};
//						 //  —пециализаци€ дл€ метода без аргументов.
//			
//template< class T >
//class Container< T, void (T::*)(void) > : public IContainer
//{
//	typedef void (T::* M)(void);
//	public: Container(T* c, M m) : m_class(c), m_method(m) {}
//	private: T* m_class; M m_method;
//	public: void Call(IArguments* i_args)
//	{
//		 (m_class->*m_method)();
//						 }
//						 };
//
//						 //  —пециализаци€ дл€ метода с одним аргументом.
//						 template< class T, class A1 >
//						 class Container< T, void (T::*)(A1) > : public IContainer
//						 {
//							 typedef void (T::* M)(A1);
//							 typedef Arguments<A1> A;
//						 public: Container(T* c, M m) : m_class(c), m_method(m) {}
//						 private: T* m_class; M m_method;
//						 public: void Call(IArguments* i_args)
//						 {
//							 A* a = dynamic_cast<A*>(i_args);
//							 assert(a);
//							 if (a) (m_class->*m_method)(a->arg1);
//						 }
//						 };
//
//						 //  —пециализаци€ дл€ метода с двум€ аргументами
//						 template< class T, class A1, class A2 >
//						 class Container< T, void (T::*)(A1, A2) > : public IContainer
//						 {
//							 typedef void (T::* M)(A1, A2);
//							 typedef Arguments<A1, A2> A;
//						 public: Container(T* c, M m) : m_class(c), m_method(m) {}
//						 private: T* m_class; M m_method;
//						 public: void Call(IArguments* i_args)
//						 {
//							 A* a = dynamic_cast<A*>(i_args);
//							 assert(a);
//							 if (a) (m_class->*m_method)(a->arg1, a->arg2);
//						 }
//						 };
//
//						 //  —обственно делегат.
//						 class Delegate
//						 {
//						 public:
//
//							 Delegate() : m_container(0) {}
//							 ~Delegate() { if (m_container) delete m_container; }
//
//							 template< class T, class U > void Connect(T* i_class, U i_method)
//							 {
//								 if (m_container) delete m_container;
//								 m_container = new Container< T, U >(i_class, i_method);
//							 }
//
//							 void operator()()
//							 {
//								 m_container->Call(&Arguments<>());
//							 }
//
//							 template< class T1 > void operator()(T1 i_arg1)
//							 {
//								 m_container->Call(&Arguments< T1 >(i_arg1));
//							 }
//
//							 template< class T1, class T2 > void operator()(T1 i_arg1, T2 i_arg2)
//							 {
//								 m_container->Call(&Arguments< T1, T2 >(i_arg1, i_arg2));
//							 }
//
//							 IContainer* m_container;
//						 };
//
//class Victim { public: void Foo() {} void Bar(int) {} };
//
//					 int main()
//					 {
//						 Victim test_class;
//						 Delegate test_delegate;
//						 test_delegate.Connect(&test_class, &Victim::Foo);
//						 test_delegate();
//						 test_delegate.Connect(&test_class, &Victim::Bar);
//						 test_delegate(10);
//						 return 0;
//					 }
//
//
//
//
//
