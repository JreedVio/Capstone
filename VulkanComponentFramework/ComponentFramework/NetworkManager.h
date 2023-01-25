#pragma once


#include "Client.h"
#include "Server.h"
#include "NetworkUnit.h"


class NetworkManager
{
private:
	NetworkUnit* unit;
public:
	NetworkManager();
	~NetworkManager();
	bool OnCreate();
	void OnDestroy();
	void Update();
};

