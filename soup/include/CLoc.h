#ifndef SOUP_INCLUDE_CLOC
#define SOUP_INCLUDE_CLOC

#include <nuv.h>
#include <memory.h>

class CLoc {
public:
	nuv::string* TranslateText(nuv::string* translatedText, nuv::string* locText, size_t maybeDialect) {
		typedef nuv::string* (__fastcall func_t)(class CLoc*, nuv::string*, nuv::string*, size_t);
		func_t* func = (func_t*)memory::FindSig("40 ?? 53 56 57 41 ?? 41 ?? 41 ?? 41 ?? 48 ?? ?? ?? ?? 48 81 ?? ?? ?? ?? ?? 49 8B");
		return func(this, translatedText, locText, maybeDialect);
	}
};

#endif // !SOUP_INCLUDE_CLOC
