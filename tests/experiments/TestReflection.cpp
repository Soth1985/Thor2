#include <Thor/Core/ThiObject.h>
#include <Thor/Core/ThObjectUtils.h>

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
	void Visit(ThRefPtr<C> obj)
	{
		int i=0;
	}

	void Visit(ThRefPtr<A> obj)
	{
		int i=0;
	}

	void Visit(ThRefPtr<B> obj)
	{
		int i=0;
	}
};

void TestVisitor()
{
	ThiObjectPtr objC = new C();
    Vis Visitor;
	StaticVisitor< THOR_TYPELIST_3(C, A, B) >::Visit(objC, Visitor, true);
	ThiObjectPtr objA = new A();
	StaticVisitor< THOR_TYPELIST_3(C, A, B) >::Visit(objA, Visitor);
	ThiObjectPtr objB = new B();
	StaticVisitor< THOR_TYPELIST_3(C, A, B) >::Visit(objB, Visitor);

	C* pC = (C*)GetPointer(objC);
	A* pA = (A*)GetPointer(objA);
	C* cst = static_cast<C*>(pA);
}

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

void TestAssert()
{
	//while(true)
	{
		THOR_ASSERT(0, "Message");
	}
}


int main()
{
	
	TestAssert();
	TestVisitor();
	TestDelegate();
	const char* name=ThClassName<A>::GetName();
		
	C c;
	ThiType* t=c.GetType();	
	ThString str=c.ToString();
	return 0;
}