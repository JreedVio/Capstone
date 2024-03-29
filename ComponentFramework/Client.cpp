#include "Client.h"
#include "Debug.h"
#include "MMath.h"
#include "SceneManager.h"
#include "RoomScene.h"
#include "Packet.h"
#include "Message.h"
#include "PlateActor.h"
#include <memory>
#include "UIManager.h"
#include "IPAddrMenu.h"


using namespace MATH;

Client::Client() : NetworkUnit(UnitType::CLIENT)
{
}

Client::~Client()
{
    Disconnect();
    if (client != nullptr) enet_host_destroy(client);
    peer = nullptr;
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

    
}

bool Client::Connect(const char* addressStr)
{
    ENetAddress address;
    ENetEvent event;

    if (strcmp(addressStr, "0") == 0) addressStr = "127.0.0.1";

    enet_address_set_host(&address, addressStr);

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

    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        std::cout << "Connection to " << hostName << " succeeded.\n";

        if (!RecieveRoomName(event)) {
            Debug::FatalError("Failed to Recieve Room Name", __FILE__, __LINE__);
            return false;
        }

        // Get scene and actors
        SceneManager* sceneManager = SceneManager::GetInstance();
        sceneManager->CreatePlayers();
        localPlayer = sceneManager->GetLocalPlayer()->GetPawn();
        remotePlayer = sceneManager->GetRemotePlayer()->GetPawn();
        if (localPlayer == nullptr || remotePlayer == nullptr) {
            Debug::FatalError("Failed to get Player Actors", __FILE__, __LINE__);
            return false;
        }
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
    return false;
}

bool Client::RecieveRoomName(ENetEvent& event)
{
    if (enet_host_service(client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_RECEIVE)
    {
        Message msg;
        std::stringstream ss;
        cereal::BinaryInputArchive archive(ss);

        // Put data into streamstring          
        ss.write(reinterpret_cast<const char*>(event.packet->data), event.packet->dataLength);

        // Deserialize it
        archive(msg);

        // Process the packet
        if (msg.header.type == CustomMessageType::RoomName) {

            const char* roomName = reinterpret_cast<const char*>(msg.body.data());

            SceneManager* sceneManager = SceneManager::GetInstance();
            sceneManager->SetNextScene(roomName);

            // Send an acknowledgement
            SendReady();

            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);

            return true;
        }

        /* Clean up the packet now that we're done using it. */
        enet_packet_destroy(event.packet);
    }
    Disconnect();
    return false;
}

void Client::OnDestroy()
{
}

void Client::Disconnect()
{
    if (peer != nullptr) enet_peer_disconnect(peer, 0);
    if (client != nullptr) enet_host_flush(client);
}

void Client::Update()
{
}

void Client::Send(){
    if (peer == nullptr) return;

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

void Client::SendRoomName(const char* roomName) {
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
}

void Client::SendPuzzleSolved() {
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

void Client::SendReady() {
    if (peer == nullptr) return;

    Message msg;
    msg.header.type = CustomMessageType::Ready;

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

void Client::SendObjectPosition(const char* objectName, Vec3 pos) {
    if (peer == nullptr) return;

    Message msg;
    msg.header.type = CustomMessageType::ObjectPosition;

    msg << pos.x << pos.y << pos.z;
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

void Client::SendObjectState(const char* objectName, bool state) {
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

void Client::Recieve()
{
    // Initialize variables
    ENetEvent event;
    Message msg;
    std::stringstream ss;
    cereal::BinaryInputArchive archive(ss);

    int eventStatus = 1;

    /* Wait up to 1000 milliseconds for an event. */
    eventStatus = enet_host_service(client, &event, 0);
    if (eventStatus > 0)
    {
        switch (event.type)
        {
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
            enet_peer_reset(peer);
            peer = nullptr;
            event.peer->data = NULL;

            SceneManager::GetInstance()->SetOpenMainMenu(true);

            break;
        }

    }
}

void Client::ProcessMessage(Message& msg)
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

        Ref<TransformComponent> transform = SceneManager::GetInstance()->GetCurrentScene()->GetActor(objectName)->GetComponent<TransformComponent>();
        transform->SetTransform(Vec3(x, y, z), transform->GetOrientation());
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
        std::dynamic_pointer_cast<PlateActor>(SceneManager::GetInstance()->GetCurrentScene()->GetActor(objectName))->SetIsEnabled(a);
    }
}

//void Client::SendPositionPacket() {
//    if (peer == nullptr) return;
//    float x = 7.0f, y = 8.0f, z = 9.0f;
//
//    std::vector<uint8_t> buffer(sizeof(float) * 3);
//
//    // Copy the float values into the buffer
//    std::memcpy(buffer.data(), &x, sizeof(float));
//    std::memcpy(buffer.data() + sizeof(float), &y, sizeof(float));
//    std::memcpy(buffer.data() + 2 * sizeof(float), &z, sizeof(float));
//
//    // Get a const void* pointer to the buffer
//    const void* data_ptr = static_cast<const void*>(buffer.data());
//    Packet packet(Type::Position, data_ptr, sizeof(data_ptr));
//
//    std::stringstream ss;
//    cereal::BinaryOutputArchive archive(ss);
//    archive(packet);
//
//    // Set pos vector to pos of the Local Player Actor
//    //Vec3 pos = localPlayer->GetComponent<TransformComponent>()->GetPosition();
//
//    std::string str = ss.str();
//
//    std::cout << str.length() << std::endl;
//
//    ENetPacket* tempPacket = enet_packet_create(str.c_str(),
//        str.length() + 1,
//        ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
//    enet_peer_send(peer, 0, tempPacket);
//}
