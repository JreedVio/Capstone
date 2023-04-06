#pragma once

#include "VMath.h"

using namespace MATH;

enum class UnitType {
	NONE,
	SERVER,
	CLIENT
};

class NetworkUnit
{
public:
	NetworkUnit() : unitType(UnitType::NONE) {}
	NetworkUnit(UnitType type_) : unitType(type_) {}
	virtual ~NetworkUnit() {}
	virtual bool OnCreate() = 0;
	virtual void Disconnect() = 0;
	virtual void Send() = 0;
	virtual void Recieve() = 0;
	virtual void Update() = 0;
	virtual void OnDestroy() = 0;
	virtual void SendRoomName(const char* roomName) = 0;
	virtual void SendPuzzleSolved() = 0;
	virtual void SendObjectPosition(const char* objectName, Vec3 pos) = 0;
	virtual void SendObjectState(const char* objectName, bool state) = 0;
	UnitType unitType;
};

