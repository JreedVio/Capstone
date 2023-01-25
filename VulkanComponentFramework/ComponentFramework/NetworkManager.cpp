#include "NetworkManager.h"
#include "Debug.h"


NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{
    delete unit;
    unit = nullptr;
}

bool NetworkManager::OnCreate()
{
    if (enet_initialize() != 0)
    {
        Debug::Error("An error occurred while initializing ENet.\n", __FILE__, __LINE__);
        return false;
    }
    atexit(enet_deinitialize);
    

    int isServer = 0;

    while (true) {
        std::cout << "Enter 0 if you are a client or 1 if you are a server\n";
        std::cin >> isServer;
        if (isServer == 0) {
            // Create Client
            unit = new Client();
            if (!unit->OnCreate()) return false;

            std::cout << "Created a Client\n";

            break;
        }
        else if (isServer == 1) {
            // Create Server
            unit = new Server();
            if (!unit->OnCreate()) return false;

            std::cout << "Created a Server\n";

            break;
        }
        else {
            std::cout << "Wrong input. Try again\n";
        }
    }
    return true;
}

void NetworkManager::OnDestroy()
{
}

void NetworkManager::Update()
{
    
}
