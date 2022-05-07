#include <Thor/Core/Containers/ThRefPtr.h>
#include <memory>

using namespace Thor;

struct Test:public ThRefCounted
{
	int val;

	Test()
		:
	val(0)
	{
		//
	}

	Test(int v)
		:
	val(v)
	{
		//
	}

	~Test()
	{
		//
	}
};

struct Test2:public Test
{
	float fVal;

	Test2()
		:
	fVal(0.0f)
	{
		//
	}

	~Test2()
	{
		//
	}
};

int main()
{
	typedef ThRefPtr<Test> ptr_t;
	typedef ThRefPtr<Test2> ptr2_t;
	ptr_t ptr(new Test());
	ptr_t ptr1(ptr);
	ptr_t ptr2(new Test2());
	ptr2_t test2=DynamicCast<Test2>(ptr2);
	ptr_t ptr3(test2);

	if(test2==ptr2)
	{
		int i=0;
	}

	if(ptr2==test2)
	{
		int i=0;
	}
	test2=0;

	if(test2)
	{
		int i=0;
	}
	ptr=ptr1;

	typedef ThWeakPtr<Test> weak_t;
	typedef ThWeakPtr<Test2> weak2_t;

	weak_t weak(ptr);
	weak_t weak2(weak);
	weak_t weak3(test2);

	weak=ptr1;
	ptr2=weak.Lock();
	weak2=weak;
	return 0;
}
