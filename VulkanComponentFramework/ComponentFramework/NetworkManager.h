#pragma once


#include "Client.h"
#include "Server.h"
#include "NetworkUnit.h"


class NetworkManager
{
private:
	static NetworkManager* Instance;
	NetworkUnit* unit;
	class Timer* timer;
	unsigned int tickrate;
	const char* roomName;
	bool isNetworkRunning;
	NetworkManager();

public:
	static NetworkManager* GetInstance() {
		if (!Instance) Instance = new NetworkManager();
		return Instance;
	}
	~NetworkManager();
	NetworkUnit* GetUnit() { return unit; }
	bool Initialize();
	bool StartNetwork(int isServer);
	void ResetNetwork();
	void OnDestroy();
	void Update();
};

