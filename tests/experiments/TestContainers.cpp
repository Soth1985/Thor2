#include <Thor/Core/Containers/ThVector.h>
#include <Thor/Core/Containers/ThHashMap.h>
#include <Thor/Core/Containers/ThHashSet.h>

class Probe
{
public:
	int x;
	float y;

	Probe()
		:
	x(0),
	y(0.0f)
	{

	}

	Probe(int x_, float y_)
		:
	x(x_),
	y(y_)
	{

	}

	Probe(const Probe& copy)
	{
		x = copy.x;
		y = copy.y;
	}

	~Probe()
	{

	}

	const Probe& operator=(const Probe& rhs)
	{
		return *this;
	}
};

void TestThVectorConstructors()
{
    using namespace Thor;
    ThVector<int> iVec;
    ThVector<int> iVec1(5);
    ThVector<int> iVec2(5u, 7);
    ThVector<Probe> pVec;
    ThVector<Probe> pVec1(5);
    ThVector<Probe> pVec2(5u, Probe(1, 2.0f));
    ThVector<Probe> mvVec(std::move(pVec2));

	int iv[5];
	Probe pv[5];

	for (int i = 0; i < 5; ++i)
	{
		iv[i] = i;
		pv[i].x = i;
		pv[i].y = i;
	}

    ThVector<int> iVec3(iv, &iv[5]);
    ThVector<int> iVec4(iVec3);
    ThVector<Probe> pVec3(pv, &pv[5]);
    ThVector<Probe> pVec4(pVec3);

    ThVector<std::string> sVec;

	sVec.PushBack("a");
	sVec.PushBack("b");
	sVec.PushBack("c");
	sVec.PushBack("d");
	sVec.PushBack("e");
	sVec.PushBack("f");

	sVec.Erase(&sVec[2]);
}

void TestThVector()
{
	TestThVectorConstructors();	
}

void TestThHashMap()
{
    using namespace Thor;
	ThHashMap<ThString, int> m;
	//m.Reserve(10);
	m.GrowSize(2);
	bool r = m.Insert("boo", 0);
	r = m.Insert("hoo", 4);
	r = m.Insert("too", 5);
	r = m.Insert("goo", 6);
	r = m.Insert("foo", 7);

	ThHashMap<ThString, int>::Iterator i = m.Find("hoo");

	m.Rehash(100);
	m.Erase("boo");
}

void TestThHashSet()
{
    using namespace Thor;
	ThHashSet<ThString> s;

	s.Insert("a");
	s.Insert("b");

	ThHashSet<ThString>::Iterator i = s.Find("a");
}

int main()
{
	TestThVector();
	TestThHashMap();
	TestThHashSet();
	return 0;
}
