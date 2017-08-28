#include <Thor/Core/Containers/ThVector.h>
#include <Thor/Core/Containers/ThHashMap.h>
#include <Thor/Core/Containers/ThHashSet.h>
#include <Thor/Core/Hash/ThSha3.h>
#include <Thor/Core/Hash/ThMurmurHash.h>
#include <Thor/Core/Hash/ThCrc32.h>

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
    ThVector<int> iiVec1 = {2, 9, 8, 7, 3, 1, 0};
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
    
    pVec4.EmplaceBack(88, 99);
    pVec4.Reserve(10);

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
    ThHashMap<std::string, int> m;
	//m.Reserve(10);
	bool r = m.Insert("boo", 0);
	r = m.Insert("hoo", 4);
	r = m.Insert("too", 5);
	r = m.Insert("goo", 6);
	r = m.Insert("foo", 7);

    ThHashMap<std::string, int>::Iterator i = m.Find("hoo");

	m.Rehash(100);
	m.Erase("boo");
    
    Thor::ThPair<std::string, std::string> pp = {"l","o"};
    
    ThHashMap<std::string, int> m2 =
    {
        {"jiu", 9},
        {"sdf", 88},
    };
}

void TestThHashSet()
{
    using namespace Thor;
    ThHashSet<std::string> s;

	s.Insert("a");
	s.Insert("b");

    ThHashSet<std::string>::Iterator i = s.Find("a");
    
    ThHashSet<std::string> s2 =
    {
        "erge",
        "asf",
        "dfvd"
    };
}

void TestSha3()
{
    using namespace Thor;
    using namespace Thor::Hash;
    const ThChar* msg = "TestSha3";
    const ThChar* msg1 = "Extra";
    ThU8 result[32] = {};
    
    Sha3_Ctx ctx;
    Sha3_256_Init(&ctx);
    Sha3_Update(&ctx, (ThU8*)msg, strlen(msg));
    Sha3_Update(&ctx, (ThU8*)msg1, strlen(msg1));
    Sha3_Final(&ctx, result);
    printf("sha3 = ");
    for (int i = 0; i < 32; ++i)
    {
        printf("%02x", result[i]);
    }
    printf("\n");
}

void TestMurmur3()
{
    using namespace Thor;
    using namespace Thor::Hash;
    const ThChar* msg = "TestMurmurH";
    printf("murmur3 = ");
    ThU32 result = Murmur3((ThU8*)msg, strlen(msg), 10);
    printf("%u\n", result);
    printf("murmur2 = ");
    result = Murmur2((ThU8*)msg, strlen(msg), 10);
    printf("%u\n", result);
    printf("murmur2_64 = ");
    ThU64 result_64 = Murmur2_64((ThU8*)msg, strlen(msg), 10);
    printf("%llu\n", result_64);
}

void TestCrc32()
{
    using namespace Thor;
    using namespace Thor::Hash;
    const ThChar* msg = "TestCrc32";
    printf("crc32 = ");
    ThU32 result = Crc32((ThU8*)msg, strlen(msg), 0);
    printf("%u", result);
    printf("\n");
}

int main()
{
	TestThVector();
	TestThHashMap();
	TestThHashSet();
    TestSha3();
    TestMurmur3();
    TestCrc32();
	return 0;
}
