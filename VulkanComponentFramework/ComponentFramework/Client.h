#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "enet/enet.h"
#include "NetworkUnit.h"

class Client : public NetworkUnit
{
public:
	ENetHost* client;
	ENetPeer* peer;


	Client();
	~Client();

	bool ConnectToServer();
	bool OnCreate();
	void OnDestroy();
	void Update();
};
