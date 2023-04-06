#include "NetworkManager.h"
#include "Debug.h"
#include "ChronoTimer.h"
#include "SceneManager.h"
#include "Timer.h"

NetworkManager* NetworkManager::Instance(nullptr);

NetworkManager::NetworkManager() : tickrate(60), isNetworkRunning(false){
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
    //timer->Start();

    std::chrono::steady_clock::time_point sendTime = std::chrono::steady_clock::now();
    float time = 1000000.0f / tickrate;
    std::chrono::microseconds sendInterval((int)time);

    while (isNetworkRunning && SceneManager::GetInstance()->GetIsRunning() &&
           SceneManager::GetInstance() != nullptr && unit != nullptr) {
        {
            //ChronoTimer chronoTimer;
            //timer->UpdateFrameTicks();

            // Limit Send function to tickrate
            if (std::chrono::steady_clock::now() >= sendTime) { 
                unit->Send();
                sendTime = std::chrono::steady_clock::now() + sendInterval;
            }
            unit->Recieve();
        }
        //SDL_Delay(timer->GetSleepTime(tickrate));
    }
}
