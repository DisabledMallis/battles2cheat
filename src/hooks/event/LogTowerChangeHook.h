#pragma once
//Spoons is a fucking duck who keeps dming me ofver this bullshit bypass kmill uyotujemnxfl kid i fucking hATE THis shit

#include "../hook.h"

class LogTowerChangeHook : public hook {
public:
	LogTowerChangeHook() : hook("LogTowerChange")
	{}
	auto apply() -> bool override;
	auto remove() -> bool override;
};