#include "Server.h"
#include "Debug.h"
#include "MMath.h"
#include "SceneManager.h"
#include "RoomScene.h"
#include "Message.h"
#include "Packets.h"
#include <string>
#include "cereal/archives/binary.hpp"
#include <cereal/types/vector.hpp>

#include <WS2tcpip.h>

// Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")

using namespace MATH;

enum class CustomMessageType : uint32_t {
    Position,
    Name
};

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

void Server::Send()
{
    // Set pos vector to pos of the Local Player Actor
    Vec3 pos = localPlayer->GetComponent<TransformComponent>()->GetPosition();

    if (peer == nullptr) return;

    ENetPacket* tempPacket = enet_packet_create(pos,
        sizeof(Vec3) + 1,
        ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
    enet_peer_send(peer, 0, tempPacket);
}

void Server::Recieve()
{
    // Initialize variables
    ENetEvent event;
    Vec3 recievedData;
    Message<CustomMessageType> msg;
    //PacketPosition msg(0.0f, 0.0f, 0.0f);
    std::stringstream ss;
    const char* temp;
    std::string tempString;
    cereal::BinaryInputArchive archive(ss);

    int eventStatus = 1;

    /* Wait up to 1000 milliseconds for an event. */
    eventStatus = enet_host_service(server, &event, 14);

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
            // Unpack the received vector
            //std::memcpy(&recievedData, event.packet->data, event.packet->dataLength);

            //std::memcpy(&temp, event.packet->data, event.packet->dataLength);
            
            ss.write(reinterpret_cast<const char*>(event.packet->data), event.packet->dataLength);
            //ss << std::string(event.packet->data, event.packet->dataLength);
            archive(msg);

            if (msg.header.id == CustomMessageType::Position) {
                float x = 0.0f;
                msg >> x;
                //std::cout << x << std::endl;
            }

            //if(msg.header.type == PacketType::Position)

            // Set this vector to Remote Player Actor
            remotePlayer->GetComponent<TransformComponent>()->pos = recievedData;

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
