#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "Shared.h"

//check if the string contains only unique characters
bool AllUniqueCharacters(const char* str, bool caseSensitive)
{
    int frequency[127] = {};
    int len = strlen(str);
    
    for (int i = 0; i < len; ++i)
    {
        char c = str[i];
        
        if (!caseSensitive)
            c = tolower(c);
        
        frequency[(int)c]++;
        
        if (frequency[(int)c] > 1)
            return false;
    }
    
    return true;
}

bool AllUniqueCharacters2(const char* str, bool caseSensitive)
{
    int len = strlen(str);
    
    for (int i = 0; i < len; ++i)
    {
        char c = str[i];
        
        if (!caseSensitive)
            c = tolower(c);
        
        for (int j = i + 1; j < len; ++j)
        {
            char c1 = str[j];
            
            if (!caseSensitive)
                c1 = tolower(c1);
            
            if (c == c1)
                return false;
        }
    }
    
    return true;
}

void TestUniqueCharacters()
{
    CHECK( AllUniqueCharacters("Test", true) == true, "Test->case sensitive");
    CHECK( AllUniqueCharacters("Test", false) == false, "Test->case insensitive");
    CHECK( AllUniqueCharacters2("Test", true) == true, "Test->case sensitive 2");
    CHECK( AllUniqueCharacters2("Test", false) == false, "Test->case insensitive 2");
}

//check if b is a circular rotation of a
bool CircularRotation(const char* a, const char* b)
{
    int aLen = strlen(a);
    int bLen = strlen(b);
    
    char* temp = new char[bLen + bLen + 1];
    temp[0]='\0';
    strcat(temp, b);
    strcat(temp, b);
    char* res = strstr(temp, a);
    delete[] temp;
    return res != nullptr;
}

void TestCircularRotation()
{
    CHECK(CircularRotation("ACTGACG", "TGACGAC") == true, "ACTGACG->TGACGAC");
    CHECK(CircularRotation("ACTGACG", "TGACGAG") == false, "ACTGACG->TGACGAG");
}

int EuclidGcd(int a, int b)
{
    assert(a>= b);
    
    if (b == 0)
        return a;
    
    int r = a % b;
    return EuclidGcd(b, r);
}
//check if strings one insert/remove/change character away
bool OneEditAway(const char* a, const char* b)
{
    int aLen = strlen(a);
    int bLen = strlen(b);
    int diff = Abs(aLen - bLen);
    
    if (diff > 1)
        return false;
    
    //make b always the longest string
    if (aLen > bLen)
    {
        Swap(aLen, bLen);
        Swap(a, b);
    }
    
    if (aLen == bLen)
    {
        int changes = 0;
        
        for (int i = 0; i < aLen; ++i)
        {
            if (a[i] != b[i])
            {
                ++changes;
                if (changes > 1)
                    return false;
            }
        }
    }
    else
    {
        int changes = 0;
        int i = 0;
        int j = 0;
        
        while (i < aLen && j < bLen)
        {
            if (a[i] != b[j])
            {
                changes++;
                if (changes > 1)
                    return false;
                ++j;
                continue;
            }
            
            i++;
            j++;
        }
    }
    
    return true;
}

void TestOneEditAway()
{
    CHECK( OneEditAway("pale", "bale") == true, "pale->bale");
    CHECK( OneEditAway("pale", "ba") == false, "pale->ba");
    CHECK( OneEditAway("pale", "pals") == true, "pale->pals");
    CHECK( OneEditAway("pale", "pdle") == true, "pale->pdle");
    CHECK( OneEditAway("pale", "base") == false, "pale->base");
    CHECK( OneEditAway("pale", "paled") == true, "pale->paled");
    CHECK( OneEditAway("pale", "parle") == true, "pale->parle");
    CHECK( OneEditAway("pale", "parle") == true, "pale->parle");
    CHECK( OneEditAway("pale", "parfle") == false, "pale->parfle");
}

int Log10(int x)
{
    static int range[] = {0, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 0x7FFFFFFF};
    return BinaryRange(range, sizeof(range) / sizeof(int), x);
}

int CompressSize(const char* str)
{
    int len = strlen(str);
    
    if (len == 0)
        return 0;
    
    int numSameChars = 1;
    int result = 0;
    char curChar = str[0];
    
    for (int i = 1; i < len + 1; ++i)
    {
        if (str[i] == curChar)
            ++numSameChars;
        else
        {
            result += 1 + Log10(numSameChars) + 1;
            numSameChars = 1;
            curChar = str[i];
        }
    }
    
    return result;
}
//compress string to char->counter
const char* Compress(const char* str)
{
    int len = strlen(str);
    int compressLen = CompressSize(str);
    
    if (compressLen >= len)
        return str;
    
    char* buf = new char[compressLen + 1];
    //buf[0] = str[0];
    buf[0] = '\0';
    int j = 0;
    int numSameChars = 1;
    char curChar = str[0];
    
    for (int i = 1; i < len + 1; ++i)
    {
        if (str[i] == curChar)
            ++numSameChars;
        else
        {
            j+=snprintf(&buf[j], compressLen + 1, "%c%d", curChar, numSameChars);
            numSameChars = 1;
            curChar = str[i];
        }
    }
    
    return buf;
}

void TestCompress()
{
    const char* res = Compress("aaasgggunnnm");
    CHECK (strcmp("a3s1g3u1n3m1", res) != 0, "aaasgggunnnm->a3s1g3u1n3m1");
    res = Compress("aaaaaasggggggunnnm");
    CHECK (strcmp("a6s1g6u1n3m1", res) == 0, "aaaaaasggggggunnnm->a6s1g6u1n3m1");
}

int main()
{
    TestUniqueCharacters();
    TestCircularRotation();
    TestOneEditAway();
    TestCompress();
    return 0;
}