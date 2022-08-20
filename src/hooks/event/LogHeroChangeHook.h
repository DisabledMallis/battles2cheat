#pragma once
//Spoons is a fucking duck who keeps dming me ofver this bullshit bypass kmill uyotujemnxfl kid i fucking hATE THis shit

#include "../hook.h"

class LogHeroChangeHook : public hook {
public:
	LogHeroChangeHook() : hook("LogHeroChange")
	{}
	auto apply() -> bool override;
	auto remove() -> bool override;
};