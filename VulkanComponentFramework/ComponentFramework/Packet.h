// It was done with help from ChatGPT
#pragma once

#include <cstdint>
#include <cstring>
#include <array>
#include <optional>
#include "cereal/archives/binary.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/optional.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/types/variant.hpp"

// Packet types
enum class Type : uint8_t {
    Data_Int = 0,
    Data_Float = 1,
    Control = 2,
    Position = 3,
    PlayerInfo = 4
};

class Packet {
private:
    // Packet header struct
    struct Header {
        Type packet_type;

        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(packet_type); // serialize things by passing them to the archive
        }

    } m_header;

    // Packet data union
    union DataUnion {
        int32_t i_data;
        float f_data;
        float position_data[3];
        float player_info_data[4];
    } m_data_union;

    // Packet data
    std::shared_ptr<uint8_t> m_data;
    size_t m_data_size = 0;

public:
    // it needs to optional
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(m_header, m_data, m_data_size,
            cereal::make_nvp("i_data", m_data_union.i_data),
            cereal::make_nvp("f_data", m_data_union.f_data),
            cereal::make_nvp("position_data", m_data_union.position_data),
            cereal::make_nvp("player_info_data", m_data_union.player_info_data)); // serialize things by passing them to the archive
    }

    // Default constructor
    Packet() = default;

    // Constructor to create a packet with data and a header
    Packet(Type packet_type_, const void* data, size_t data_size)
    {
        if (packet_type_ == Type::Position) {
            const auto* floats = reinterpret_cast<const float*>(data);
            setPositionData(floats[0], floats[1], floats[2]);
        }
        m_header.packet_type = packet_type_;
        // Allocate memory for the packet data
        m_data_size = data_size;
        m_data = std::make_shared<uint8_t>(m_data_size);
        // Copy the data into the packet
        std::memcpy(m_data.get(), data, m_data_size);
    }

    // Destructor to free the memory used by the packet data
    ~Packet() {
        //delete[] m_data;
    }

    // Accessors for the packet header fields
    Type getPacketType() const { return static_cast<Type>(m_header.packet_type); }

    // Accessors for the packet data
    int32_t getIntData() const { return m_data_union.i_data; }
    void setIntData(int32_t data) { m_data_union.i_data = data; }

    float getFloatData() const { return m_data_union.f_data; }
    void setFloatData(float data) { m_data_union.f_data = data; }

    const float* getPositionData() const { return m_data_union.position_data; }
    void setPositionData(float x, float y, float z) {
        m_data_union.position_data[0] = x;
        m_data_union.position_data[1] = y;
        m_data_union.position_data[2] = z;
    }

    const float* getPlayerInfoData() const { return m_data_union.player_info_data; }
    void setPlayerInfoData(float x, float y, float z, int32_t score) {
        m_data_union.player_info_data[0] = x;
        m_data_union.player_info_data[1] = y;
        m_data_union.player_info_data[2] = z;
        m_data_union.player_info_data[3] = score;
    }

    const uint8_t* getData() const { return m_data.get(); }
    size_t getDataSize() const { return m_data_size; }
};