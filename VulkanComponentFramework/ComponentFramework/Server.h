#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "enet/enet.h"
#include "NetworkUnit.h"


class Server : public NetworkUnit
{
public:

	ENetAddress address;
	ENetHost* server;
	ENetPeer* peer;



	Server();
	~Server();
	bool OnCreate();
	void OnDestroy();
	void Update();
};

