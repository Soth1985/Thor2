#include <Thor/Framework/ThiObject.h>
#include <Thor/Framework/ThObjectArena.h>
#include <Thor/Framework/ThDelegate.h>
#include <Thor/Framework/ThiFramework.h>
using namespace Thor;

THOR_DECLARE_STRUCT(A);
THOR_DECLARE_STRUCT(B);
THOR_DECLARE_STRUCT(C);

struct A:public ThiObject, public ThDelegateHolder
{
	//int y;
	virtual ThiType* GetType()const
	{
		return ThType<A>::Instance();
	}

	A()
	{
		static int count=0;
		//y=count++;
	}
};

struct B:public A
{
	float h;
	virtual ThiType* GetType()const
	{
		return ThType<B>::Instance();
	}

	B()
	{
		static float count=0.0f;
		h=count++;
		h=99;
	}

    void Func(int*,float)
	{
		int i=0;
	}
};

struct C:public B
{
	ThF32 j;
	virtual ThiType* GetType()const
	{
		return ThType<C>::Instance();
	}

	C()
	{
		static float count=0.0f;
		j=count++;
	}

	THOR_MEMORY_ARENA(C, 3);
};

struct D:public C
{
public:
public:
private:
};

THOR_REG_TYPE(A, THOR_TYPELIST_1(ThiObject));
THOR_REG_TYPE(B, THOR_TYPELIST_1(A));
THOR_REG_TYPE(C, THOR_TYPELIST_1(B));

struct Vis
{
	void Visit(ThRefPtr<C>& obj)
	{
		int i=0;
	}

	void Visit(ThRefPtr<A>& obj)
	{
		int i=0;
	}

	void Visit(ThRefPtr<B>& obj)
	{
		int i=0;
	}
};

void TestVisitor()
{
	ThiObjectPtr objC = new C();
	StaticVisitor< THOR_TYPELIST_3(C, A, B) >::Visit(ThiObjectPtr(objC), Vis(), true);
	ThiObjectPtr objA = new A();
	StaticVisitor< THOR_TYPELIST_3(C, A, B) >::Visit(ThiObjectPtr(objA), Vis());
	ThiObjectPtr objB = new B();
	StaticVisitor< THOR_TYPELIST_3(C, A, B) >::Visit(ThiObjectPtr(objB), Vis());

	C* pC = (C*)GetPointer(objC);
	A* pA = (A*)GetPointer(objA);
	C* cst = static_cast<C*>(pA);
}

void TestAllocator()
{
	C* var = new C();
	
	var = new C();
	var = new C();
	delete var;
	var=C::AllocArray(3);
	C::FreeArray(var);	

	ThSize sz=sizeof D;
	D* dvar= new D[30];
	delete[]dvar;
};
//
struct Tst
{
	typedef void result_type;
	void Func(int*,float)
	{
		int i=0;
	}

	void operator()(int*,float)const
	{
		int i=0;
	}
};

void Func(int*,float)
{
	int i=0;
}
void TestDelegate()
{
	Tst t;
    BPtr bp( new B() );
	ThDelegate<int*,float> del;
	bp->ConnectMethod<B>(del, &B::Func);
	
	//{
		ThDelegateConnectionPtr c2=del.ConnectMethod( &Tst::Func, &t );
		ThDelegateConnectionPtr c1=del.ConnectFunctor( t );
		ThDelegateConnectionPtr c0=del.Connect( &Func );
	//}
	
	int* y=0;
	//std::tr1::ref(0);
	del(y,0.0f);
}

void testAssert()
{
	//while(true)
	{
		THOR_ASSERT(0, "Message");
	}
}


int main()
{
	B bb;
	int offs = offsetof(B,h);
	char* pbb=(char*)&bb;
	pbb+=offs;
	float* val=(float*)pbb;
	
	GetFramework();
	testAssert();
	int *ptr = 0;
	*ptr = 99;
	typedef std::vector<int> vec_t;
	vec_t v(90);
	v.reserve(10);
	
	v.reserve(20);
	TestVisitor();
	//TestAllocator();
	TestDelegate();
	const char* name=ThClassName<A>::GetName();
		
	C c;
	int* p = new int;
	delete[] p;
	ThiType* t=c.GetType();	
	ThString str=c.ToString();
	return 0;
}