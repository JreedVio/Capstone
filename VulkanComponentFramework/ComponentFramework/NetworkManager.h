#pragma once


#include "Client.h"
#include "Server.h"
#include "NetworkUnit.h"


class NetworkManager
{
private:
	NetworkUnit* unit;
	class Timer* timer;
	unsigned int tickrate;
	const char* roomName;
	bool isNetworkRunning;

public:
	NetworkManager();
	~NetworkManager();
	NetworkUnit* GetUnit() { return unit; }
	bool Initialize();
	bool StartNetwork(int isServer);
	void ResetNetwork();
	void OnDestroy();
	void Update();
};

