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

public:
	NetworkManager();
	~NetworkManager();
	bool OnCreate();
	void OnDestroy();
	void Update();
	void SetUnitType(int type_) { isServer = type_; }
};

