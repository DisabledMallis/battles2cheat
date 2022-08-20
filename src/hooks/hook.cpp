#include "hook.h"

hook::hook(std::string name)
{
    this->name = name;
}
auto hook::get_name() -> std::string
{
    return this->name;
}
auto hook::get_dis() -> PLH::ZydisDisassembler&
{
    return *dis;
}
auto hook::get_detour() -> PLH::x64Detour* {
	return this->detour;
}
auto hook::create_detour(uint64_t fnAddress, uint64_t fnCallback, uint64_t* userTrampVar) -> PLH::x64Detour* {
	this->detour = new PLH::x64Detour(fnAddress, fnCallback, userTrampVar);
	return this->detour;
}
auto hook::apply() -> bool
{
    return false;
}
auto hook::remove() -> bool
{
    return false;
}