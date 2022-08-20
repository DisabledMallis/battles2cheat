#pragma once

#include "../hook.h"

class SNetRequestQueueBloonSetEventHook : public hook {
public:
	SNetRequestQueueBloonSetEventHook() : hook("SNetRequestQueueBloonSetEvent::SNetRequestQueueBloonSetEvent") {};
	auto apply() -> bool override;
	auto remove() -> bool override;
};