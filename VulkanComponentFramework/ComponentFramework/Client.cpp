#include "Client.h"
#include "Debug.h"
#include "MMath.h"

using namespace MATH;

Client::Client() : NetworkUnit(UnitType::CLIENT)
{
}

Client::~Client()
{
    enet_host_destroy(client);
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

    std::cout << "Host Ip: " << hostIp << " | Host Name: " << hostName << std::endl;

    address.port = 27015;
    /* Initiate the connection, allocating the two channels 0 and 1. */
    peer = enet_host_connect(client, &address, 2, 0);
    if (peer == NULL)
    {
        Debug::Error("No available peers for initiating an ENet connection.\n", __FILE__, __LINE__);
        return false;
    }
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        std::cout << "Connection to " << hostName << " succeeded.\n";
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
    // Game loop (Not Working)

    ENetEvent event;
    Vec3 newData;
    Vec3 pos = Vec3(100.0f, 1100.0f, 2200.0f);
    int eventStatus = 1;
        
    /* Wait up to 1000 milliseconds for an event. */
    eventStatus = enet_host_service(client, &event, 1000);
    if (eventStatus > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_RECEIVE:
            std::memcpy(&newData, event.packet->data, event.packet->dataLength);
            std::cout << newData.x << " " << newData.y << " " << newData.z << std::endl;


            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);

            break;
        }
    }
    ENetPacket* tempPacket = enet_packet_create(pos,
        sizeof(Vec3) + 1,
        ENET_PACKET_FLAG_RELIABLE);

    enet_peer_send(peer, 0, tempPacket); 
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
