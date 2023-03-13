#pragma once


#include "Client.h"
#include "Server.h"
#include "NetworkUnit.h"


class NetworkManager
{
private:
	NetworkUnit* unit;
	class Timer* timer;
	int isServer;
	unsigned int tickrate;
	const char* roomName;

public:
	NetworkManager();
	~NetworkManager();
	NetworkUnit* GetUnit() { return unit; }
	bool OnCreate();
	void OnDestroy();
	void Update();
	void SetUnitType(int type_) { isServer = type_; }
};

