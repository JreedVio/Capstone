#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "enet/enet.h"
#include "NetworkUnit.h"
#include "Actor.h"

class Message;

class Server : public NetworkUnit
{
public:

	ENetAddress address;
	ENetHost* server;
	ENetPeer* peer;
	Ref<Actor> localPlayer, remotePlayer;
	bool isClientReady;



	Server();
	~Server();
	bool OnCreate();
	void OnDestroy();
	void Disconnect();
	void Update();
	void Send();
	void SendRoomName(const char* roomName);
	void SendObjectPosition(const char* objectName);
	void SendObjectState(const char* objectName, bool state);
	void SendPuzzleSolved();
	void Recieve(int tickrate);
	void ProcessMessage(Message& msg);
};

