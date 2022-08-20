#ifndef HOOKS_HOOK
#define HOOKS_HOOK

#include <string>
#include <polyhook2/Detour/x64Detour.hpp>
#include <polyhook2/ZydisDisassembler.hpp>

class hook {
	static inline PLH::ZydisDisassembler* dis = new PLH::ZydisDisassembler(PLH::Mode::x64);
	std::string name;
	PLH::x64Detour* detour;
public:
	hook(std::string name);
	auto get_name() -> std::string;
	auto get_dis() -> PLH::ZydisDisassembler&;
	auto get_detour() -> PLH::x64Detour*;
	auto create_detour(const uint64_t fnAddress, const uint64_t fnCallback, uint64_t* userTrampVar) -> PLH::x64Detour*;
	virtual auto apply() -> bool;
	virtual auto remove() -> bool;
};

#endif /* HOOKS_HOOK */
