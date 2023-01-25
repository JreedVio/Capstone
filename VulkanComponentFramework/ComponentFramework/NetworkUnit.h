#pragma once

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
	virtual void OnDestroy() = 0;
	UnitType unitType;
};

