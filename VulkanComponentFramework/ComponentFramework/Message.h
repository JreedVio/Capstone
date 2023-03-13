/*
	MMO Client/Server Framework using ASIO
	"Happy Birthday Mrs Javidx9!" - javidx9
	Videos:
	Part #1: https://youtu.be/2hNdkYInj4g
	Part #2: https://youtu.be/UbjxGvrDrbw
	License (OLC-3)
	~~~~~~~~~~~~~~~
	Copyright 2018 - 2020 OneLoneCoder.com
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:
	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.
	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.
	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com
	Author
	~~~~~~
	David Barr, aka javidx9, �OneLoneCoder 2019, 2020
*/

#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

enum class CustomMessageType : uint32_t {
	Position,
	Rotation,
	RotationAndPosition,
	RoomName
};

///[OLC_HEADERIFYIER] START "MESSAGE"

// Message Header is sent at start of all messages. The template allows us
// to use "enum class" to ensure that the messages are valid at compile time
struct message_header
{
	CustomMessageType type;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(type); // serialize things by passing them to the archive
	}
};

// Message Body contains a header and a std::vector, containing raw bytes
// of infomation. This way the message can be variable length, but the size
// in the header must be updated.
struct Message
{
	// Header & Body vector
	message_header header{};
	std::vector<uint8_t> body;
	
	void clear() {
		body.clear();
	}

	// This method lets cereal know which data members to serialize
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(header, body); // serialize things by passing them to the archive
	}

	// returns size of entire message packet in bytes
	size_t size() const
	{
		return body.size();
	}

	// Convenience Operator overloads - These allow us to add and remove stuff from
	// the body vector as if it were a stack, so First in, Last Out. These are a 
	// template in itself, because we dont know what data type the user is pushing or 
	// popping, so lets allow them all. NOTE: It assumes the data type is fundamentally
	// Plain Old Data (POD). TLDR: Serialise & Deserialise into/from a vector

	// Pushes any POD-like data into the message buffer
	template<typename DataType>
	friend Message& operator << (Message& msg, const DataType& data)
	{
		// Check that the type of the data being pushed is trivially copyable
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

		// Cache current size of vector, as this will be the point we insert the data
		size_t i = msg.body.size();

		// Resize the vector by the size of the data being pushed
		msg.body.resize(msg.body.size() + sizeof(DataType));

		// Physically copy the data into the newly allocated vector space
		std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

		// Return the target message so it can be "chained"
		return msg;
	}

	// Pulls any POD-like data form the message buffer
	template<typename DataType>
	friend Message& operator >> (Message& msg, DataType& data)
	{
		// Check that the type of the data being pushed is trivially copyable
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

		// Cache the location towards the end of the vector where the pulled data starts
		size_t i = msg.body.size() - sizeof(DataType);

		// Physically copy the data from the vector into the user variable
		std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

		// Shrink the vector to remove read bytes, and reset end position
		msg.body.resize(i);

		// Return the target message so it can be "chained"
		return msg;
	}
};
