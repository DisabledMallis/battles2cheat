#pragma once

#include "Game.h"
#include "player.h"

namespace reflang {
	class IClass
	{
	public:
		char pad_0000[16]; //0x0000
	}; //Size: 0x0090
	template<typename T>
	class Class : public IClass {
	public:
		T* pT; //0x0010
	};
	template<typename T>
	class Reference
	{
	public:
		char pad_0000[8]; //0x0000
		T* pT; //0x0008
	}; //Size: 0x0010
};