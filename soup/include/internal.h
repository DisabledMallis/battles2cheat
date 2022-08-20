#pragma once

template<const char* SIG, typename RETURN_TYPE, typename... ARGS>
RETURN_TYPE callFastcall(ARGS... args) {
	typedef RETURN_TYPE(__fastcall internalFunc)(ARGS...);
	internalFunc* pInternalFunc = (internalFunc*)memory::FindSig(SIG);
	if (pInternalFunc == nullptr) {
		ilog(LogLevel::Error, "Internal function call failure! Sig: %s", SIG);
	}
	RETURN_TYPE val = pInternalFunc(args...);
	if (val == nullptr) {
		ilog(LogLevel::Warning, "Internal function call returned null, but was expected to return a value: %s", SIG);
	}
	return val;
}

static const char* defaultTowerIds[] = {
	"Quincy",
	"DartMonkey",
	"TackShooter",
	"BombShooter"
};
static const char* defaultTowerNames[] = {
	"Quincy",
	"Dart Monkey",
	"Tack Shooter",
	"Bomb Shooter"
};