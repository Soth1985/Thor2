#include <Thor/Core/String/ThStringUtilities.h>
#include <Thor/Core/String/utf8.h>

namespace Thor
{

void ThStringUtilities::ToString(ThF32 val, ThI8* result, ThSize bufSize)
{
	ToString((ThF64)val, result, bufSize);
}

void ThStringUtilities::ToString(ThF64 val, ThI8* result, ThSize bufSize)
{
	if( sprintf(result, "%lf", val) < 0 )
		THOR_ASSERT(0, "Buffer is too small");
}

void ThStringUtilities::ToString(ThI8 val, ThI8* result, ThSize bufSize)
{
	ToString((ThI64)val, result, bufSize);
}

void ThStringUtilities::ToString(ThU8 val, ThI8* result, ThSize bufSize)
{
	ToString((ThU64)val, result, bufSize);
}

void ThStringUtilities::ToString(ThI16 val, ThI8* result, ThSize bufSize)
{
	ToString((ThI64)val, result, bufSize);
}

void ThStringUtilities::ToString(ThU16 val, ThI8* result, ThSize bufSize)
{
	ToString((ThU64)val, result, bufSize);
}

void ThStringUtilities::ToString(ThI32 val, ThI8* result, ThSize bufSize)
{
	ToString((ThI64)val, result, bufSize);
}

void ThStringUtilities::ToString(ThU32 val, ThI8* result, ThSize bufSize)
{
	ToString((ThU64)val, result, bufSize);
}

void ThStringUtilities::ToString(ThI64 val, ThI8* result, ThSize bufSize)
{
	if( sprintf(result, "%lld", val) < 0 )
		THOR_ASSERT(0, "Buffer is too small");
}

void ThStringUtilities::ToString(ThU64 val, ThI8* result, ThSize bufSize)
{
	if( sprintf(result, "%llu", val) < 0 )
		THOR_ASSERT(0, "Buffer is too small");
}

//
void ThStringUtilities::ToWideString(ThF32 val, ThWchar* result, ThSize bufSize)
{
	ToWideString((ThF64)val, result, bufSize);
}

void ThStringUtilities::ToWideString(ThF64 val, ThWchar* result, ThSize bufSize)
{
	if( swprintf(result, bufSize, L"%lf", val) < 0 )
		THOR_ASSERT(0, "Buffer is too small");
}

void ThStringUtilities::ToWideString(ThI8 val, ThWchar* result, ThSize bufSize)
{
	ToWideString((ThI64)val, result, bufSize);
}

void ThStringUtilities::ToWideString(ThU8 val, ThWchar* result, ThSize bufSize)
{
	ToWideString((ThU64)val, result, bufSize);
}

void ThStringUtilities::ToWideString(ThI16 val, ThWchar* result, ThSize bufSize)
{
	ToWideString((ThI64)val, result, bufSize);
}

void ThStringUtilities::ToWideString(ThU16 val, ThWchar* result, ThSize bufSize)
{
	ToWideString((ThU64)val, result, bufSize);
}

void ThStringUtilities::ToWideString(ThI32 val, ThWchar* result, ThSize bufSize)
{
	ToWideString((ThI64)val, result, bufSize);
}

void ThStringUtilities::ToWideString(ThU32 val, ThWchar* result, ThSize bufSize)
{
	ToWideString((ThU64)val, result, bufSize);
}

void ThStringUtilities::ToWideString(ThI64 val, ThWchar* result, ThSize bufSize)
{
	if( swprintf(result, bufSize, L"%lld", val) < 0 )
		THOR_ASSERT(0, "Buffer is too small");
}

void ThStringUtilities::ToWideString(ThU64 val, ThWchar* result, ThSize bufSize)
{
	if( swprintf(result, bufSize, L"%llu", val) < 0 )
		THOR_ASSERT(0, "Buffer is too small");
}
//

ThBool ThStringUtilities::Parse(const ThI8* str, ThF32& val)
{
	char* end;
	ThF64 temp = strtod(str, &end);

	if (str != end)
	{
		val = (ThF32)temp;
		return true;
	}

	return false;
}

ThBool ThStringUtilities::Parse(const ThI8* str, ThF64& val)
{
	char* end;
	ThF64 temp = strtod(str, &end);

	if (str != end)
	{
		val = temp;
		return true;
	}

	return false;
}

ThBool ThStringUtilities::Parse(const ThI8* str, ThI8& val)
{
	char* end;
	ThI32 temp = strtol(str, &end, 10);

	if (str != end)
	{
		val = (ThI8)temp;
		return true;
	}

	return false;
}

ThBool ThStringUtilities::Parse(const ThI8* str, ThU8& val)
{
	char* end;
	ThU32 temp = strtoul(str, &end, 10);

	if (str != end)
	{
		val = (ThU8)temp;
		return true;
	}

	return false;
}

ThBool ThStringUtilities::Parse(const ThI8* str, ThI16& val)
{
	char* end;
	ThI32 temp = strtol(str, &end, 10);

	if (str != end)
	{
		val = (ThI16)temp;
		return true;
	}

	return false;
}

ThBool ThStringUtilities::Parse(const ThI8* str, ThU16& val)
{
	char* end;
	ThU32 temp = strtoul(str, &end, 10);

	if (str != end)
	{
		val = (ThU16)temp;
		return true;
	}

	return false;
}

ThBool ThStringUtilities::Parse(const ThI8* str, ThI32& val)
{
	char* end;
	ThI32 temp = strtol(str, &end, 10);

	if (str != end)
	{
		val = temp;
		return true;
	}

	return false;
}

ThBool ThStringUtilities::Parse(const ThI8* str, ThU32& val)
{
	char* end;
	ThU32 temp = strtoul(str, &end, 10);

	if (str != end)
	{
		val = (ThU32)temp;
		return true;
	}

	return false;
}

ThBool ThStringUtilities::Parse(const ThI8* str, ThI64& val)
{
	/*char* end;
	ThI64 temp = _strtoi64(str, &end, 10);

	if (str != end)
	{
		val = temp;
		return true;
	}*/

	return false;
}

ThBool ThStringUtilities::Parse(const ThI8* str, ThU64& val)
{
	/*char* end;
	ThU64 temp = _strtoui64(str, &end, 10);

	if (str != end)
	{
		val = temp;
		return true;
	}*/

	return false;
}

ThBool ThStringUtilities::Parse(const ThI8* str, ThBool& val)
{
	char* end;
	ThI32 temp = strtol(str, &end, 10);

	if (str != end)
	{
		if (temp != 0)
			val = true;
		else
			val = false;

		return true;
	}

	return false;
}

ThString ThStringUtilities::WideStringToUtf8(const ThWideString& str)
{
	ThString converted;
	utf8::utf16to8(str.begin(), str.end(), std::back_inserter(converted));
	return converted;
}

ThWideString ThStringUtilities::Utf8ToWideString(const ThString& str)
{
	ThWideString converted;
	utf8::utf8to16(str.begin(), str.end(), std::back_inserter(converted));
	return converted;
}

ThString ThStringUtilities::WideStringToUtf8(const ThWchar* str)
{
	ThString converted;
	utf8::utf16to8(str, str + wcslen(str), std::back_inserter(converted));
	return converted;
}

ThWideString ThStringUtilities::Utf8ToWideString(const ThI8* str)
{
	ThWideString converted;
	utf8::utf8to16(str, str + strlen(str), std::back_inserter(converted));
	return converted;
}

void ThStringUtilities::WideStringToUtf8(const ThWchar* str, ThI8* buf, ThSize bufSize)
{
	utf8::utf16to8(str, str + wcslen(str) + 1, buf);
}

void ThStringUtilities::Utf8ToWideString(const ThI8* str, ThWchar* buf, ThSize bufSize)
{
	utf8::utf8to16(str, str + strlen(str) + 1, buf);
}

}