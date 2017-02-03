#pragma once

#include <Thor/Core/CoreFwd.h>

namespace Thor
{

struct THOR_FRAMEWORK_DLL ThStringUtilities
{
	static void ToString(ThF32 val, ThChar* result, ThSize bufSize);
	static void ToString(ThF64 val, ThChar* result, ThSize bufSize);
	static void ToString(ThI8 val, ThChar* result, ThSize bufSize);
	static void ToString(ThU8 val, ThChar* result, ThSize bufSize);
	static void ToString(ThI16 val, ThChar* result, ThSize bufSize);
	static void ToString(ThU16 val, ThChar* result, ThSize bufSize);
	static void ToString(ThI32 val, ThChar* result, ThSize bufSize);
	static void ToString(ThU32 val, ThChar* result, ThSize bufSize);
	static void ToString(ThI64 val, ThChar* result, ThSize bufSize);
	static void ToString(ThU64 val, ThChar* result, ThSize bufSize);

	static void ToWideString(ThF32 val, ThWchar* result, ThSize bufSize);
	static void ToWideString(ThF64 val, ThWchar* result, ThSize bufSize);
	static void ToWideString(ThI8 val, ThWchar* result, ThSize bufSize);
	static void ToWideString(ThU8 val, ThWchar* result, ThSize bufSize);
	static void ToWideString(ThI16 val, ThWchar* result, ThSize bufSize);
	static void ToWideString(ThU16 val, ThWchar* result, ThSize bufSize);
	static void ToWideString(ThI32 val, ThWchar* result, ThSize bufSize);
	static void ToWideString(ThU32 val, ThWchar* result, ThSize bufSize);
	static void ToWideString(ThI64 val, ThWchar* result, ThSize bufSize);
	static void ToWideString(ThU64 val, ThWchar* result, ThSize bufSize);

	static bool Parse(const ThChar* str, ThF32& val);
	static bool Parse(const ThChar* str, ThF64& val);
	static bool Parse(const ThChar* str, bool& val);
	static bool Parse(const ThChar* str, ThI8& val);
	static bool Parse(const ThChar* str, ThU8& val);
	static bool Parse(const ThChar* str, ThI16& val);
	static bool Parse(const ThChar* str, ThU16& val);
	static bool Parse(const ThChar* str, ThI32& val);
	static bool Parse(const ThChar* str, ThU32& val);
	static bool Parse(const ThChar* str, ThI64& val);
	static bool Parse(const ThChar* str, ThU64& val);

	static ThString WideStringToUtf8(const ThWideString& str);
	static ThWideString Utf8ToWideString(const ThString& str);
	static ThString WideStringToUtf8(const ThWchar* str);
	static ThWideString Utf8ToWideString(const ThChar* str);
	static void WideStringToUtf8(const ThWchar* str, ThChar* buf, ThSize bufSize);
	static void Utf8ToWideString(const ThChar* ThChar, ThWchar* buf, ThSize bufSize);
};

}
