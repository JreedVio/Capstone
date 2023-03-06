#pragma once

#include <cstdint>

enum class PacketType {
	Position,
	Angle
};

struct Header {
	PacketType type;
};

struct Packet {
	Header header;
};

struct PacketPosition : public Packet {
	float x, y, z;

	PacketPosition(float x_, float y_, float z_) : 
		x(x_), y(y_), z(z_) {
		header.type = PacketType::Position;
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(x, y, z); // serialize things by passing them to the archive
	}
};

struct PacketAngle : public Packet {
	int angle;

	PacketAngle(int angle_) :
		angle(angle_) {
		header.type = PacketType::Angle;
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(angle); // serialize things by passing them to the archive
	}
};
