#include "Client.h"
#include "Debug.h"
#include "MMath.h"
#include "SceneManager.h"
#include "RoomScene.h"
#include "Packet.h"
#include "Message.h"

using namespace MATH;

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

void Client::Send(){
    if (peer == nullptr) return;

    Message msg;
    msg.header.type = CustomMessageType::RotationAndPosition;

    AddRotation(msg);
    AddPosition(msg);

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

void Client::AddPosition(Message& msg)
{
    Vec3 pos = localPlayer->GetComponent<TransformComponent>()->GetPosition();
    
    msg << pos.x << pos.y << pos.z;
}

void Client::AddRotation(Message& msg)
{
    Vec3 ijk = localPlayer->GetComponent<TransformComponent>()->GetOrientation().ijk;
    float w = localPlayer->GetComponent<TransformComponent>()->GetOrientation().w;

    msg << ijk.x << ijk.y << ijk.z << w;
}

void Client::SendPositionPacket() {
    if (peer == nullptr) return;
    float x = 7.0f, y = 8.0f, z = 9.0f;

    std::vector<uint8_t> buffer(sizeof(float) * 3);

    // Copy the float values into the buffer
    std::memcpy(buffer.data(), &x, sizeof(float));
    std::memcpy(buffer.data() + sizeof(float), &y, sizeof(float));
    std::memcpy(buffer.data() + 2 * sizeof(float), &z, sizeof(float));

    // Get a const void* pointer to the buffer
    const void* data_ptr = static_cast<const void*>(buffer.data());
    Packet packet(Type::Position, data_ptr, sizeof(data_ptr));

    std::stringstream ss;
    cereal::BinaryOutputArchive archive(ss);
    archive(packet);

    // Set pos vector to pos of the Local Player Actor
    //Vec3 pos = localPlayer->GetComponent<TransformComponent>()->GetPosition();

    std::string str = ss.str();

    std::cout << str.length() << std::endl;

    ENetPacket* tempPacket = enet_packet_create(str.c_str(),
        str.length() + 1,
        ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
    enet_peer_send(peer, 0, tempPacket);
}

void Client::Recieve(int tickrate)
{
    // Initialize variables
    ENetEvent event;
    Message msg;
    std::stringstream ss;
    cereal::BinaryInputArchive archive(ss);

    int eventStatus = 1;

    /* Wait up to 1000 milliseconds for an event. */
    eventStatus = enet_host_service(client, &event, (1000 / tickrate) - 1);
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
        }
    }
}

void Client::ProcessMessage(Message& msg)
{
    if (msg.header.type == CustomMessageType::Position) {
        Vec3 receivedPos;
        msg >> receivedPos.z >> receivedPos.y >> receivedPos.x;
        remotePlayer->GetComponent<TransformComponent>()->pos = receivedPos;
        //std::cout << "Position: " << receivedPos.x << " " << receivedPos.y
        //    << " " << receivedPos.z << std::endl;
    }
    else if (msg.header.type == CustomMessageType::Rotation) {
        float x, y, z, w;
        msg >> w >> z >> y >> x;
        remotePlayer->GetComponent<TransformComponent>()->orientation = Quaternion(w, x, y, z);
        //std::cout << "Rotation: " << x << " " << y << " " << z << " " << w << std::endl;
    }
    else if (msg.header.type == CustomMessageType::RotationAndPosition) {
        Vec3 receivedPos;
        msg >> receivedPos.z >> receivedPos.y >> receivedPos.x;
        remotePlayer->GetComponent<TransformComponent>()->pos = receivedPos;
        //std::cout << "Position: " << receivedPos.x << " " << receivedPos.y
        //    << " " << receivedPos.z << std::endl;

        float x, y, z, w;
        msg >> w >> z >> y >> x;
        remotePlayer->GetComponent<TransformComponent>()->orientation = Quaternion(w, x, y, z);
        //std::cout << "Rotation: " << x << " " << y << " " << z << " " << w << std::endl;
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
