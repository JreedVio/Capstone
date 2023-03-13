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



	Server();
	~Server();
	bool OnCreate();
	void OnDestroy();
	void Update();
	void Send();
	void SendRoomName(const char* roomName);
	void AddPosition(Message& msg);
	void AddRotation(Message& msg);
	void Recieve(int tickrate);
	void ProcessMessage(Message& msg);
};

