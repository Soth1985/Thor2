#pragma once

#include <Thor/Framework/FrameworkFwd.h>

namespace Thor
{

struct THOR_FRAMEWORK_DLL ThStringUtilities
{
	static void ToString(ThF32 val, ThI8* result, ThSize bufSize);
	static void ToString(ThF64 val, ThI8* result, ThSize bufSize);
	static void ToString(ThI8 val, ThI8* result, ThSize bufSize);
	static void ToString(ThU8 val, ThI8* result, ThSize bufSize);
	static void ToString(ThI16 val, ThI8* result, ThSize bufSize);
	static void ToString(ThU16 val, ThI8* result, ThSize bufSize);
	static void ToString(ThI32 val, ThI8* result, ThSize bufSize);
	static void ToString(ThU32 val, ThI8* result, ThSize bufSize);
	static void ToString(ThI64 val, ThI8* result, ThSize bufSize);
	static void ToString(ThU64 val, ThI8* result, ThSize bufSize);

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

	static ThBool Parse(const ThI8* str, ThF32& val);
	static ThBool Parse(const ThI8* str, ThF64& val);
	static ThBool Parse(const ThI8* str, ThBool& val);
	static ThBool Parse(const ThI8* str, ThI8& val);
	static ThBool Parse(const ThI8* str, ThU8& val);
	static ThBool Parse(const ThI8* str, ThI16& val);
	static ThBool Parse(const ThI8* str, ThU16& val);
	static ThBool Parse(const ThI8* str, ThI32& val);
	static ThBool Parse(const ThI8* str, ThU32& val);
	static ThBool Parse(const ThI8* str, ThI64& val);
	static ThBool Parse(const ThI8* str, ThU64& val);

	static ThString WideStringToUtf8(const ThWideString& str);
	static ThWideString Utf8ToWideString(const ThString& str);
	static ThString WideStringToUtf8(const ThWchar* str);
	static ThWideString Utf8ToWideString(const ThI8* str);
	static void WideStringToUtf8(const ThWchar* str, ThI8* buf, ThSize bufSize);
	static void Utf8ToWideString(const ThI8* str, ThWchar* buf, ThSize bufSize);
};

}