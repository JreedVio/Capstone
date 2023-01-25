#include "Server.h"
#include "Debug.h"

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

    enet_address_set_host(&address, "172.17.1.203");
    address.port = 7777;
    //address.host = ENET_HOST_ANY;

    const size_t s = 1000;
    char hostName_[s] = "asd";
    enet_address_get_host(&address, hostName_, s);

    std::cout << hostName_;

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

    // Wait for connection and diconnection

    ENetEvent event;

    while (true) {
        /* Wait up to 1000 milliseconds for an event. */
        while (enet_host_service(server, &event, 1000) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                printf("A new client connected from %x:%u.\n",
                    event.peer->address.host,
                    event.peer->address.port);
                /* Store any relevant client information here. */
                //event.peer->data = "Client information";
                //std::cin >> 
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                printf("A packet of length %u containing %s was received from %x:%u on channel %u.\n",
                    event.packet->dataLength,
                    event.packet->data,
                    event.peer->address.host,
                    event.peer->address.port,
                    event.channelID);
                /* Clean up the packet now that we're done using it. */
                enet_packet_destroy(event.packet);

                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                printf("%x:%u disconnected.\n", event.peer->address.host, event.peer->address.port);
                /* Reset the peer's client information. */
                //event.peer->data = NULL;
                return true;
                break;
            }
        }
    }

	return true;
}

void Server::OnDestroy()
{
}

void Server::Update()
{

}
