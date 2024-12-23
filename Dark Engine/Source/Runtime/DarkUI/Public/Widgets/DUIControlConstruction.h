#pragma once
#include "Containers/DarkString.h"
#include <type_traits>


#define DUI_DECLARE_WIDGET(WidgetType, ParentType)

#define DUI_DECLARE_WIDGET_API(WidgetType, ParentType, ApiDefine) \
	private: \
		using Super = ParentType; \
		using ThisClass = WidgetType;	 \
		static const FDUIWidgetClassData& GetPrivateWidgetClass() \
		{ \
			static FDUIWidgetClassData WidgetClassDataInstance = FDUIWidgetClassData(std::type_identity<ParentType>(), #WidgetType); \
			return WidgetClassDataInstance; \
		} \
	public: \
		static const FDUIWidgetClassData& StaticWidgetClass() {return GetPrivateWidgetClass();} \
		virtual const FDUIWidgetClassData& GetWidgetClass() const override {return GetPrivateWidgetClass(); }	\
	private:
	


class FDUIWidgetClassData
{
	friend class FDUIControlledConstruction;
private:
	FDUIWidgetClassData(const FString& InWidgetTypeName) :
		WidgetType(InWidgetTypeName)
	{}

public:
	template <typename InWidgetParentType>
	FDUIWidgetClassData(std::type_identity<InWidgetParentType>, const FString& InWidgetTypeName) :
		WidgetType(InWidgetTypeName)
	{}

	FString GetWidgetType() const { return WidgetType; }
private:
	FString WidgetType;

};


class FDUIControlledConstruction
{
public:
	FDUIControlledConstruction() = default;
	virtual ~FDUIControlledConstruction() = default;

public:
	static const FDUIWidgetClassData& StaticWidgetClass()
	{
		static FDUIWidgetClassData WidgetClassDataInstance = FDUIWidgetClassData(TEXT("FDUIControlledConstruction"));
		return WidgetClassDataInstance;
	}
	virtual const FDUIWidgetClassData& GetWidgetClass() const = 0;
	
public:
	void* operator new(const size_t InSize)
	{
		return malloc(InSize);
	}

	void* operator new(const size_t InSize, void* Addr)
	{
		return Addr;
	}

	void operator delete(void* Mem)
	{
		free(Mem);
	}
};
			