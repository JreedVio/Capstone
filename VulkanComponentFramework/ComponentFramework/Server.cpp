#include "Server.h"
#include "Debug.h"
#include "MMath.h"


#include <WS2tcpip.h>

// Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")

using namespace std;
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

    std::cout << "Host Ip: " << hostIp << " | Host Name: " << hostName << std::endl;

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

	return true;
}

void Server::OnDestroy()
{
}

void Server::Update()
{
    // Wait for connection and diconnection

    ENetEvent event;
    Vec3 pos;

    /*SceneManager* sceneManager = SceneManager::GetInstance();
    Ref<Actor> mario = sceneManager->GetCurrentScene()->GetActor("Mario1");
    if (mario != nullptr) {
        Ref<TransformComponent> transform = mario->GetComponent<TransformComponent>();
        pos = transform->GetPosition();
    }
    else {*/
        pos = Vec3(0, 0, 0);
    //}

    Vec3 recievedData;
    int eventStatus = 1;
    
    /* Wait up to 1000 milliseconds for an event. */
    eventStatus = enet_host_service(server, &event, 1000);
        
    if (eventStatus > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            printf("A new client connected from %x:%u.\n",
                event.peer->address.host,
                event.peer->address.port);
            if(peer == nullptr)
                peer = event.peer;

            /* Store any relevant client information here. */

            //enet_packet_resize(tempPacket, strlen("packetfoo") + 1);
            //memcpy???
            //strcpy_s(reinterpret_cast<char*>(temptemp->data[strlen("packet")]), strlen("packetfoo") + 1, "foo");
            //strcpy(reinterpret_cast<char*>(tempPacket->data[strlen("packet")]), "foo");
            //strcpy(reinterpret_cast<char*>(temptemp->data[strlen("packet")]), "foo");

            break;
        case ENET_EVENT_TYPE_RECEIVE:
            std::memcpy(&recievedData, event.packet->data, event.packet->dataLength);
            std::cout << recievedData.x << " " << recievedData.y << " " << recievedData.z << std::endl;

            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);

            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            printf("%x:%u disconnected.\n", event.peer->address.host, event.peer->address.port);
            /* Reset the peer's client information. */
            //event.peer->data = NULL;
            //return true;
            break;
        }
    }
    if (peer != nullptr) {
        ENetPacket* tempPacket = enet_packet_create(pos,
            sizeof(Vec3) + 1,
            ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
        enet_peer_send(peer, 0, tempPacket);
        //std::cout << "Send a packet\n";
    }
}

/*printf("A packet of length %u containing %s was received from %x:%u on channel %u.\n",
                event.packet->dataLength,
                event.packet->data,
                event.peer->address.host,
                event.peer->address.port,
                event.channelID);*/
