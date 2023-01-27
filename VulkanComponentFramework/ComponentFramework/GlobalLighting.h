#pragma once

#include "Matrix.h"
#include "Component.h"
#include "Actor.h"

using namespace MATH;

constexpr int MAX_LIGHTS = 24;



struct GlobalLighting
{
	Vec4 position[2];
	Vec4 diffuse[2];
};

class LightActor : public Actor
{
	
	Vec4 position;
	Vec4 diffuse;

public:

	LightActor(Component* parent_ = nullptr, Vec4 position_ = Vec4(0.0f, 0.0f, 0.0f, 0.0f), Vec4 diffuse_ = Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	~LightActor();

	Vec4 GetPosition() const { return position; }

	Vec4 GetDiffuse() const { return diffuse; }

	void SetPosition(const Vec4& pos) { position = pos; }

	void SetDiffuse(const Vec4& diff) { diffuse = diff; }

};
