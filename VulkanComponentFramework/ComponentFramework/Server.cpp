#include "Server.h"
#include "Debug.h"
#include "MMath.h"
#include "SceneManager.h"
#include "RoomScene.h"
#include "Room.h"
#include "Message.h"
#include "Packet.h"
#include "PlateActor.h"
#include <string>
#include <memory>

#include <WS2tcpip.h>

// Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")

using namespace MATH;

Server::Server() : NetworkUnit(UnitType::SERVER), isClientReady(false)
{
}

Server::~Server()
{
    Disconnect();
    if (server != nullptr) enet_host_destroy(server);
    peer = nullptr;
    isClientReady = false;
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
    SceneManager* sceneManager = SceneManager::GetInstance();
    sceneManager->CreatePlayers();
    localPlayer = sceneManager->GetLocalPlayer()->GetPawn();
    remotePlayer = sceneManager->GetRemotePlayer()->GetPawn();
    if (localPlayer == nullptr || remotePlayer == nullptr) {
        Debug::FatalError("Failed to get Player Actors", __FILE__, __LINE__);
        return false;
    }
	return true;
}

void Server::OnDestroy()
{
}

void Server::Disconnect()
{
    if (peer != nullptr) enet_peer_disconnect(peer, 0);
    if (server != nullptr) enet_host_flush(server);
}

void Server::Update()
{   
}

void Server::Send() {
    if (peer == nullptr || !isClientReady) return;

    Message msg;
    msg.header.type = CustomMessageType::RotationAndPosition;

    Vec3 ijk = localPlayer->GetComponent<TransformComponent>()->GetOrientation().ijk;
    float w = localPlayer->GetComponent<TransformComponent>()->GetOrientation().w;
    msg << ijk.x << ijk.y << ijk.z << w;

    Vec3 pos = localPlayer->GetComponent<TransformComponent>()->GetPosition();
    msg << pos.x << pos.y << pos.z;

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

void Server::SendRoomName(const char* roomName) {
    if (peer == nullptr) return;

    Message msg;
    msg.header.type = CustomMessageType::RoomName;

    msg.AddCharArray(roomName, std::strlen(roomName));

    //Serialize
    std::stringstream ss;
    cereal::BinaryOutputArchive archive(ss);
    archive(msg);

    std::string str = ss.str();

    // Send over the network
    ENetPacket* tempPacket = enet_packet_create(str.c_str(),
        str.length() + 1,
        ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, tempPacket);
    SendObjectPosition("Plate");
}

void Server::SendPuzzleSolved()
{
    if (peer == nullptr) return;

    Message msg;
    msg.header.type = CustomMessageType::PuzzleSolved;

    //Serialize
    std::stringstream ss;
    cereal::BinaryOutputArchive archive(ss);
    archive(msg);

    std::string str = ss.str();

    // Send over the network
    ENetPacket* tempPacket = enet_packet_create(str.c_str(),
        str.length() + 1,
        ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, tempPacket);
}

void Server::SendObjectPosition(const char* objectName) {
    if (peer == nullptr) return;

    Message msg;
    msg.header.type = CustomMessageType::ObjectPosition;

    msg << 10.0f << 20.0f << 30.0f;
    msg.AddCharArray(objectName, std::strlen(objectName));
    msg << std::strlen(objectName) + 1;

    //Serialize
    std::stringstream ss;
    cereal::BinaryOutputArchive archive(ss);
    archive(msg);

    std::string str = ss.str();

    // Send over the network
    ENetPacket* tempPacket = enet_packet_create(str.c_str(),
        str.length() + 1,
        ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, tempPacket);
}

void Server::SendObjectState(const char* objectName, bool state) {
    if (peer == nullptr) return;

    Message msg;
    msg.header.type = CustomMessageType::ObjectState;

    msg << state;
    msg.AddCharArray(objectName, std::strlen(objectName));
    msg << std::strlen(objectName) + 1;

    //Serialize
    std::stringstream ss;
    cereal::BinaryOutputArchive archive(ss);
    archive(msg);

    std::string str = ss.str();

    // Send over the network
    ENetPacket* tempPacket = enet_packet_create(str.c_str(),
        str.length() + 1,
        ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, tempPacket);
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
    eventStatus = enet_host_service(server, &event, 33);

    // Wait for connection, diconnection and package receival
    if (eventStatus > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            if (peer == nullptr) {
                peer = event.peer;
                SendRoomName(SceneManager::GetInstance()->GetCurrentScene()->GetSceneName());
                remotePlayer->SetVisible(true);
                std::cout << "Accepted connection from " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
            }
            else {
                // Decline the connection from all other clients
                std::cout << "Declined connection from " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
                enet_peer_disconnect(event.peer, 0);
            }

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
            if (peer == event.peer) {
                enet_peer_reset(peer);
                peer = nullptr;
                event.peer->data = NULL;
                remotePlayer->SetVisible(false);
            }
            break;
        }
    }
}

void Server::ProcessMessage(Message& msg)
{
    if (msg.header.type == CustomMessageType::RotationAndPosition) {
        Vec3 receivedPos;
        msg >> receivedPos.z >> receivedPos.y >> receivedPos.x;
        remotePlayer->GetComponent<TransformComponent>()->pos = receivedPos;

        float x, y, z, w;
        msg >> w >> z >> y >> x;
        remotePlayer->GetComponent<TransformComponent>()->orientation = Quaternion(w, x, y, z);
        remotePlayer->GetComponent<TransformComponent>()->SetTransform(receivedPos, Quaternion(w, x, y, z));
    }
    else if (msg.header.type == CustomMessageType::RoomName) {

        const char* roomName = reinterpret_cast<const char*>(msg.body.data());

        SceneManager* sceneManager = SceneManager::GetInstance();
        sceneManager->GetCurrentScene()->SetStatus(ROOMTRANSIT);
        sceneManager->SetNextScene(roomName);
    }
    else if (msg.header.type == CustomMessageType::PuzzleSolved) {
        dynamic_cast<RoomScene*>(SceneManager::GetInstance()->GetCurrentScene())->GetRoom()->SetSolved(true);
    }
    else if (msg.header.type == CustomMessageType::Ready) {
        isClientReady = true;
    }
    else if (msg.header.type == CustomMessageType::ObjectPosition) {
        int size;
        msg >> size;

        std::vector<char> name;
        for (int i = 0; i < size; i++)
        {
            char letter;
            msg >> letter;
            name.push_back(letter);
        }
        std::reverse(name.begin(), name.end());
        const char* objectName = reinterpret_cast<const char*>(name.data());

        float x, y, z;
        msg >> z >> y >> x;
        std::cout << "Object " << objectName << " changed pos to ";
        std::cout << x << " " << y << " " << z << std::endl;
    }
    else if (msg.header.type == CustomMessageType::ObjectState) {
        int size;
        msg >> size;

        std::vector<char> name;
        for (int i = 0; i < size; i++)
        {
            char letter;
            msg >> letter;
            name.push_back(letter);
        }
        std::reverse(name.begin(), name.end());
        const char* objectName = reinterpret_cast<const char*>(name.data());

        bool a;
        msg >> a;
        std::cout << "Object " << objectName << " changed bool to ";
        std::cout << a << std::endl;
        std::dynamic_pointer_cast<PlateActor>(SceneManager::GetInstance()->GetCurrentScene()->GetActor(objectName))->SetIsEnabled(a);
    }
}
