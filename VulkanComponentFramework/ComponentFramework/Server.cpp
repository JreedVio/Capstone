#include "Server.h"
#include "Debug.h"
#include "MMath.h"
#include "SceneManager.h"
#include "RoomScene.h"
#include "Message.h"
#include "Packet.h"
#include <string>

#include <WS2tcpip.h>

// Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")

using namespace MATH;

Server::Server() : NetworkUnit(UnitType::SERVER)
{
}

Server::~Server()
{
    enet_host_destroy(server);
}

bool Server::OnCreate()
{
    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */
    /* enet_address_set_host (& address, "x.x.x.x"); */

    //enet_address_set_host(&address, "142.214.83.88");
    address.host = ENET_HOST_ANY;
    address.port = 6699;

    const size_t s = 1000;
    char hostName[s];
    char hostIp[s];
    enet_address_get_host_ip(&address, hostIp, s);
    enet_address_get_host(&address, hostName, s);

    std::cout << "Creating a server: " << hostIp << " | " << hostName << std::endl;

    /* Bind the server to port 1234. */
    server = enet_host_create(&address /* the address to bind the server host to */,
        32      /* allow up to 32 clients and/or outgoing connections */,
        2      /* allow up to 2 channels to be used, 0 and 1 */,
        0      /* assume any amount of incoming bandwidth */,
        0      /* assume any amount of outgoing bandwidth */);
    if (server == NULL)
    {
        Debug::Error("An error occurred while trying to create an ENet server host.\n", __FILE__, __LINE__);
        return false;
    }
    std::cout << "Server is created\n";

    // Get scene and actors
    Scene* scene = SceneManager::GetInstance()->GetCurrentScene();
    if (scene == nullptr) {
        Debug::FatalError("Failed to get Current Scene", __FILE__, __LINE__);
        return false;
    }

    localPlayer = scene->GetActor("LocalPlayer");
    remotePlayer = scene->GetActor("RemotePlayer");
    if (localPlayer == nullptr || remotePlayer == nullptr) {
        Debug::FatalError("Failed to get Player Actors", __FILE__, __LINE__);
        return false;
    }
	return true;
}

void Server::OnDestroy()
{
}

void Server::Update()
{   
}

void Server::Send() {
    if (peer == nullptr) return;

    Message msg;
    msg.header.type = CustomMessageType::RotationAndPosition;

    AddRotation(msg);
    AddPosition(msg);

    //Serialize
    std::stringstream ss;
    cereal::BinaryOutputArchive archive(ss);
    archive(msg);

    std::string str = ss.str();

    // Send over the network
    ENetPacket* tempPacket = enet_packet_create(str.c_str(),
        str.length() + 1,
        ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
    enet_peer_send(peer, 0, tempPacket);
}

void Server::AddPosition(Message& msg)
{
    Vec3 pos = localPlayer->GetComponent<TransformComponent>()->GetPosition();

    msg << pos.x << pos.y << pos.z;
}

void Server::AddRotation(Message& msg)
{
    Vec3 ijk = localPlayer->GetComponent<TransformComponent>()->GetOrientation().ijk;
    float w = localPlayer->GetComponent<TransformComponent>()->GetOrientation().w;

    msg << ijk.x << ijk.y << ijk.z << w;
}

void Server::Recieve(int tickrate)
{
    // Initialize variables
    ENetEvent event;
    Message msg;
    std::stringstream ss;
    cereal::BinaryInputArchive archive(ss);

    int eventStatus = 1;

    /* Wait up to 1000 milliseconds for an event. */
    eventStatus = enet_host_service(server, &event, (1000 / tickrate) - 1);

    // Wait for connection, diconnection and package receival
    if (eventStatus > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            printf("A new client connected from %x:%u.\n",
                event.peer->address.host,
                event.peer->address.port);
            if (peer == nullptr)
                peer = event.peer;
            remotePlayer->SetVisible(true);

            break;
        case ENET_EVENT_TYPE_RECEIVE:

            // Put data into streamstring          
            ss.write(reinterpret_cast<const char*>(event.packet->data), event.packet->dataLength);

            // Deserialize it
            archive(msg);

            // Process the packet
            ProcessMessage(msg);

            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);

            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            printf("%x:%u disconnected.\n", event.peer->address.host, event.peer->address.port);
            /* Reset the peer's client information. */
            enet_peer_reset(peer);
            //event.peer->data = NULL;
            break;
        }
    }
}

void Server::ProcessMessage(Message& msg)
{
    if (msg.header.type == CustomMessageType::Position) {
        Vec3 receivedPos;
        msg >> receivedPos.z >> receivedPos.y >> receivedPos.x;
        remotePlayer->GetComponent<TransformComponent>()->pos = receivedPos;
        //std::cout << "Position: " << receivedPos.x << " " << receivedPos.y
        //    << " " << receivedPos.z << std::endl;
    }
    else if (msg.header.type == CustomMessageType::Rotation) {
        float x, y, z, w;
        msg >> w >> z >> y >> x;
        remotePlayer->GetComponent<TransformComponent>()->orientation = Quaternion(w, x, y, z);
        //std::cout << "Rotation: " << x << " " << y << " " << z << " " << w << std::endl;
    }
    else if (msg.header.type == CustomMessageType::RotationAndPosition) {
        Vec3 receivedPos;
        msg >> receivedPos.z >> receivedPos.y >> receivedPos.x;
        remotePlayer->GetComponent<TransformComponent>()->pos = receivedPos;
        //std::cout << "Position: " << receivedPos.x << " " << receivedPos.y
        //    << " " << receivedPos.z << std::endl;

        float x, y, z, w;
        msg >> w >> z >> y >> x;
        remotePlayer->GetComponent<TransformComponent>()->orientation = Quaternion(w, x, y, z);
        //std::cout << "Rotation: " << x << " " << y << " " << z << " " << w << std::endl;
    }
}

/*printf("A packet of length %u containing %s was received from %x:%u on channel %u.\n",
                event.packet->dataLength,
                event.packet->data,
                event.peer->address.host,
                event.peer->address.port,
                event.channelID);*/

/*//enet_packet_resize(tempPacket, strlen("packetfoo") + 1);
//memcpy???
//strcpy_s(reinterpret_cast<char*>(temptemp->data[strlen("packet")]), strlen("packetfoo") + 1, "foo");
//strcpy(reinterpret_cast<char*>(tempPacket->data[strlen("packet")]), "foo");
//strcpy(reinterpret_cast<char*>(temptemp->data[strlen("packet")]), "foo");*/
