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

	bool OnCreate();
	bool RecieveRoomName(ENetEvent& event);
	void OnDestroy();
	void Disconnect();
	void Update();
	void Send();
	void SendRoomName(const char* roomName);
	void SendPuzzleSolved();
	void AddPosition(Message& msg);
	void AddRotation(Message& msg);
	//void SendPositionPacket();
	void Recieve(int tickrate);
	void ProcessMessage(Message& msg);
};

