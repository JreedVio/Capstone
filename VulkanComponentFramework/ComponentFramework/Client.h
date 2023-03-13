#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "enet/enet.h"
#include "NetworkUnit.h"
#include "Actor.h"

class Message;

class Client : public NetworkUnit
{
public:
	ENetHost* client;
	ENetPeer* peer;
	Ref<Actor> localPlayer, remotePlayer;




	Client();
	~Client();

	bool ConnectToServer();
	bool OnCreate();
	void OnDestroy();
	void Update();
	void Send();
	void SendRoomName(const char* roomName);
	void AddPosition(Message& msg);
	void AddRotation(Message& msg);
	void AddRoom(Message& msg);
	void SendPositionPacket();
	void Recieve(int tickrate);
	void ProcessMessage(Message& msg);
};

