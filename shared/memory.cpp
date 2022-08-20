#include "memory.h"
#include <DbgHelp.h>
#include <logger.h>

uintptr_t memory::FindMLvlPtr(uintptr_t baseAddr, std::vector<unsigned int> offsets){
    uintptr_t addr = baseAddr;
	for (int I = 0; I < offsets.size(); I++) {
		addr = *(uintptr_t*)(addr);
		if ((uintptr_t*)(addr) == nullptr)
			return addr;
		addr += offsets[I];
	}
	return addr;
}

auto memory::getModuleBase() -> long long {
    return (long long)GetModuleHandleA(nullptr);
}

auto memory::getModuleBaseHandle() -> HMODULE {
    return GetModuleHandleA(nullptr);
}

auto memory::getBaseModuleSize() -> long long {
    MODULEINFO info;
    GetModuleInformation(GetCurrentProcess(), getModuleBaseHandle(), &info, sizeof(info));
    return info.SizeOfImage;
}

auto memory::getBaseModuleEnd() -> long long {
    return getModuleBase() + getBaseModuleSize();
}

void memory::SetThisModule(HMODULE mod) {
	thisMod = mod;
}
auto memory::GetThisModule() -> HMODULE {
	return thisMod;
}

auto memory::GetModule(const char* moduleName) -> HMODULE
{
	return GetModuleHandleA(moduleName);
}

auto memory::GetModuleSize(const char* moduleName) -> size_t
{
	MODULEINFO info;
	GetModuleInformation(GetCurrentProcess(), GetModule(moduleName), &info, sizeof(info));
	return info.SizeOfImage;
}

auto memory::GetModuleEnd(const char* moduleName) -> size_t
{
	return ((size_t)GetModule(moduleName)) + GetModuleSize(moduleName);
}

auto memory::DemangleSymbol(std::string mangled) -> std::string
{
	char* undecorated_name = (char*)_malloca(mangled.size() + 1);
	UnDecorateSymbolName(mangled.c_str() + 1, undecorated_name, mangled.size(), UNDNAME_COMPLETE);
	return undecorated_name;
}

auto memory::FindSig(const char* pattern) -> uintptr_t {
    return FindSig(getModuleBase(), getBaseModuleEnd(), pattern);
}
struct SearchedSig {
	size_t rangeStart;
	size_t rangeEnd;
	std::string pattern;
	uintptr_t result;
	SearchedSig(size_t rangeStart, size_t rangeEnd, std::string pattern, uintptr_t result) {
		this->rangeStart = rangeStart;
		this->rangeEnd = rangeEnd;
		this->pattern = pattern;
		this->result = result;
	}
};

std::vector<SearchedSig> alreadySearched = std::vector<SearchedSig>();
auto memory::FindSig(long long rangeStart, long long rangeEnd, const char* pattern) -> uintptr_t {
	for(int i = 0; i < alreadySearched.size(); i++) {
		SearchedSig searched = alreadySearched[i];
		if(searched.pattern == std::string(pattern) && searched.rangeStart == rangeStart && searched.rangeEnd == rangeEnd) {
			return searched.result;
		}
	}
	std::string sanitizedPat = pattern;
	int skips = 0;
	while (sanitizedPat[0] == '?' && sanitizedPat[1] == '?') {
		sanitizedPat = sanitizedPat.substr(3);
		skips++;
		ilog("Sig skips: %d", skips);
		ilog("Current sig: %s", sanitizedPat.c_str());
	}
    const char* pat = sanitizedPat.c_str();
    long long firstMatch = 0;
    for (long long pCur = rangeStart; pCur < rangeEnd; pCur++) {
        if (!*pat) return firstMatch;
        if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
            if (!firstMatch) firstMatch = pCur;
            if (!pat[2]) {
				firstMatch += -skips;
				alreadySearched.push_back(SearchedSig(rangeStart, rangeEnd, std::string(pattern), firstMatch));
				return firstMatch;
			};
            if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
            else pat += 2;
        } else {
            pat = sanitizedPat.c_str();
            firstMatch = 0;
        }
    }
	MessageBoxA(nullptr, pattern, "SCAN FAILURE", MB_OK);
    return 0;
}