#include "strconvert.h"
#include <codecvt>

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
std::string strconvert::ws_to_s(std::wstring text)
{
	return converter.to_bytes(text);
}

std::wstring strconvert::s_to_ws(std::string text)
{
	return converter.from_bytes(text);
}
