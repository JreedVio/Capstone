#include "NetworkManager.h"
#include "Debug.h"
#include "ChronoTimer.h"
#include "SceneManager.h"
#include "Timer.h"

NetworkManager* NetworkManager::Instance(nullptr);

NetworkManager::NetworkManager() : tickrate(30), isNetworkRunning(false){
}

NetworkManager::~NetworkManager()
{
    if (unit) {
        delete unit;
        unit = nullptr;
    }

    if (timer) {
        delete timer;
        timer = nullptr;
    }
}

bool NetworkManager::Initialize()
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

    return true;
}

bool NetworkManager::StartServer()
{
    // Create Server
    unit = new Server();
    if (!unit->OnCreate()) return false;
    
    isNetworkRunning = true;
    return true;
}

bool NetworkManager::StartClient(const char* address){
    // Create Client
    unit = new Client();
    if (!unit->OnCreate()) return false;
    if (!dynamic_cast<Client*>(unit)->Connect(address)) return false;

    isNetworkRunning = true;
    return true;
}

void NetworkManager::ResetNetwork()
{
    isNetworkRunning = false;

    if (unit) {
        unit->Disconnect();
        delete unit;
        unit = nullptr;
    }
}

void NetworkManager::OnDestroy()
{}

void NetworkManager::Update()
{
    timer->Start();

    while (isNetworkRunning && SceneManager::GetInstance()->GetIsRunning() &&
           SceneManager::GetInstance() != nullptr) {
        {
            //ChronoTimer chronoTimer;

            //timer->UpdateFrameTicks();

            if (unit == nullptr) return;

            unit->Send();
            unit->Recieve(tickrate);
        }
        //SDL_Delay(timer->GetSleepTime(tickrate));
    }
}
