#pragma once
#include "Actor.h"
#include "Vector.h"
#include "glew.h"

using namespace MATH;

enum class LightStyle {
	DirectionLight = 1,
	PointLight,
	SkyLight,
	SpotLight
};
class LightActor : public Actor{
private:
	LightStyle lightStyle;
	Vec3 pos;
	Vec4 colour;
	float intensity;
	Vec3 fallOff;
	GLuint uboLightDataID;
	const GLuint bindPoint = 1;
public:
	LightActor(Component* parent_, LightStyle lightStyle_, Vec3 pos_, Vec4 colour_,
		float intensity_ = 1.0f, Vec3 fallOff_ = Vec3(0.0f, 0.0f, 0.0f));
	LightActor(Component* parent_);
	~LightActor();
	Vec3 GetPosition() const { return pos; }
	bool OnCreate() override;
	void OnDestroy() override;
	GLuint GetLightID() { return uboLightDataID; }
};

