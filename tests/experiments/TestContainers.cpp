#include <Thor/Framework/Containers/ThVector.h>
#include <Thor/Framework/Containers/ThHashMap.h>
#include <Thor/Framework/Containers/ThHashSet.h>

using namespace Thor;

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
	ThVector<int> iVec, iVec1(5), iVec2(5u, 7);
	ThVector<Probe> pVec, pVec1(5), pVec2(5u, Probe(1, 2.0f));

	int iv[5];
	Probe pv[5];

	for (int i = 0; i < 5; ++i)
	{
		iv[i] = i;
		pv[i].x = i;
		pv[i].y = i;
	}

	ThVector<int> iVec3(iv, &iv[5]), iVec4(iVec3);
	ThVector<Probe> pVec3(pv, &pv[5]), pVec4(pVec3);

	ThVector<ThString> sVec;

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