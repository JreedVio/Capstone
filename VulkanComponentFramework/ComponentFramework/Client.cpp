#include "Client.h"
#include "Debug.h"
#include "MMath.h"
#include "SceneManager.h"
#include "RoomScene.h"
#include "Packets.h"
#include "Message.h"
#include "cereal/archives/binary.hpp"
#include <cereal/types/vector.hpp>

using namespace MATH;

enum class CustomMessageType : uint32_t {
    Position,
    Name
};

Client::Client() : NetworkUnit(UnitType::CLIENT)
{
}

Client::~Client()
{
    if (client != nullptr) enet_host_destroy(client);
}

bool Client::ConnectToServer()
{
    return true;
}

bool Client::OnCreate()
{
    client = enet_host_create(NULL /* create a client host */,
        1 /* only allow 1 outgoing connection */,
        2 /* allow up 2 channels to be used, 0 and 1 */,
        0 /* assume any amount of incoming bandwidth */,
        0 /* assume any amount of outgoing bandwidth */);
    if (client == NULL)
    {
        Debug::Error("An error occurred while trying to create an ENet client host.\n", __FILE__, __LINE__);
        return false;
    }

    /// Connect To Server

    ENetAddress address;
    ENetEvent event;
    /* Connect to some.server.net:1234. */
    enet_address_set_host(&address, "127.0.0.1"); 

    const size_t s = 1000;
    char hostName[s];
    char hostIp[s];
    enet_address_get_host_ip(&address, hostIp, s);
    enet_address_get_host(&address, hostName, s);

    std::cout << "Trying to connect to: " << hostIp << " | " << hostName << std::endl;

    address.port = 6699;
    /* Initiate the connection, allocating the two channels 0 and 1. */
    peer = enet_host_connect(client, &address, 2, 0);
    if (peer == NULL)
    {
        Debug::Error("No available peers for initiating an ENet connection.\n", __FILE__, __LINE__);
        return false;
    }

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

    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        std::cout << "Connection to " << hostName << " succeeded.\n";
        remotePlayer->SetVisible(true);

        return true;
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset(peer);
        std::cout << "Connection to " << hostName << " failed.\n";

        return false;
    }
}

void Client::OnDestroy()
{
}

void Client::Update()
{
}

void Client::Send()
{
    if (peer == nullptr) return;

    Message<CustomMessageType> msg;
    msg.header.id = CustomMessageType::Position;
    float x = 240.0f;
    msg << x;
    // header is 18 bytes

    //PacketPosition msg(10.0f, 20.0f, 50.0f);
    // 12 bytes

    std::stringstream ss;
    cereal::BinaryOutputArchive archive(ss);
    archive(msg);

    // Set pos vector to pos of the Local Player Actor
    //Vec3 pos = localPlayer->GetComponent<TransformComponent>()->GetPosition();

    //const void* sendingData = ss.str().c_str();
    std::string str = ss.str();

    std::cout << str.length() << std::endl;

    ENetPacket* tempPacket = enet_packet_create(str.c_str(),
        str.length() + 1,
        ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
    enet_peer_send(peer, 0, tempPacket);
}

void Client::Recieve()
{
    // Initialize variables
    ENetEvent event;
    Vec3 recievedData;
    int eventStatus = 1;

    /* Wait up to 1000 milliseconds for an event. */
    eventStatus = enet_host_service(client, &event, 14);
    if (eventStatus > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_RECEIVE:
            // Unpack the received vector
            std::memcpy(&recievedData, event.packet->data, event.packet->dataLength);

            // Set this vector to Remote Player Actor
            remotePlayer->GetComponent<TransformComponent>()->pos = recievedData;

            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);

            break;
        }
    }
}

//enet_peer_disconnect(peer, 0);
    ///* Allow up to 3 seconds for the disconnect to succeed
    // * and drop any packets received packets.
    // */
    //while (enet_host_service(client, &event, 3000) > 0)
    //{
    //    switch (event.type)
    //    {
    //    case ENET_EVENT_TYPE_RECEIVE:
    //        enet_packet_destroy(event.packet);
    //        break;
    //    case ENET_EVENT_TYPE_DISCONNECT:
    //        std::cout << "Disconnection succeeded.\n";
    //        //return false;
    //    }
    //}
    ///* We've arrived here, so the disconnect attempt didn't */
    ///* succeed yet.  Force the connection down.             */
    //enet_peer_reset(peer);
