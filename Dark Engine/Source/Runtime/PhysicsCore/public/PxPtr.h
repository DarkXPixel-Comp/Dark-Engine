#pragma once



template<class T>
class TPxPtr
{
public:
	TPxPtr(T* InPtr) : Ptr(InPtr) {}
	TPxPtr() {}
	~TPxPtr()
	{
		Release();
	}

	TPxPtr& operator=(T* InPtr)
	{
		Release();
		Ptr = InPtr;
		return *this;
	}

	T& operator*() const
	{
		return *Ptr;
	}

	T* operator->() const
	{
		return Ptr;
	}

	operator T* () const
	{
		return Ptr;
	}

	void Release()
	{
		if (Ptr)
		{
			T* Temp = Ptr;
			Ptr = nullptr;
			Temp->release();
		}
	}

private:
	T* Ptr = nullptr;

};

template<class T>
class TPxRefCountedPtr
{
	template<class U> friend class TPxRefCountedPtr;
public:
	template<class U>
	TPxRefCountedPtr(U* InPtr) : Ptr(InPtr)
	{
		AddRef();
	}
	
	TPxRefCountedPtr(const TPxRefCountedPtr& Other) : Ptr(Other.Ptr)
	{

	}
	template<class U>
	TPxRefCountedPtr(const class TPxRefCountedPtr<U>& Other)  : Ptr(Other.Ptr)
	{
		AddRef();
	}

	TPxRefCountedPtr(TPxRefCountedPtr&& Other) : Ptr(nullptr)
	{
		if (this != reinterpret_cast<TPxRefCountedPtr*>(&reinterpret_cast<unsigned char&>(Other)))
		{
			//Swap()
		}

	}



	TPxRefCountedPtr() : Ptr(nullptr)
	{

	}

	TPxRefCountedPtr(decltype(__nullptr)) : Ptr(nullptr)
	{

	}



	void Reset()
	{
		if (Ptr)
		{
			Release();
			Ptr = nullptr;
		}
	}


private:
	void Release()
	{
		T* Temp = Ptr;

		if (Temp)
		{
			Ptr = nullptr;
			Temp->release();
		}
	}

	void AddRef() const
	{
		if (Ptr)
		{
			Ptr->acquireReference();
		}
	}



private:
	T* Ptr = nullptr;
};