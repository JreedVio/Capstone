#include "NetworkManager.h"
#include "Debug.h"
#include "ChronoTimer.h"
#include "SceneManager.h"
#include "Timer.h"



NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{
    delete unit;
    unit = nullptr;

    if (timer) {
        delete timer;
        timer = nullptr;
    }
}

bool NetworkManager::OnCreate()
{
    timer = new Timer();
    if (timer == nullptr) {
        Debug::FatalError("Failed to initialize Timer object", __FILE__, __LINE__);
        return false;
    }

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

            break;
        }
        else if (isServer == 1) {
            // Create Server
            unit = new Server();
            if (!unit->OnCreate()) return false;

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
    timer->Start();

    while (SceneManager::GetInstance()->GetIsRunning()) {
        {
            //ChronoTimer chronoTimer;

            timer->UpdateFrameTicks();

            if (unit == nullptr) return;

            unit->Send();
            unit->Recieve();
        }
        SDL_Delay(timer->GetSleepTime(64));
    }
}
