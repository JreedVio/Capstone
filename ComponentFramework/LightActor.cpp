#include "LightActor.h"
#include "UBO_Padding.h"

LightActor::LightActor(Component* parent_, LightStyle lightStyle_, Vec3 pos_, Vec4 colour_, 
	float intensity_, Vec3 fallOff_): Actor(parent_), lightStyle(lightStyle_), pos(pos_), colour(colour_)
{

}

LightActor::LightActor(Component* parent_) : Actor(parent_)
{
	lightStyle = LightStyle::DirectionLight;
	pos = Vec3(0.0f, 0.0f, 0.0f);
	colour = Vec4(0.0f, 0.0f, 255.0f, 0.0f);
	intensity = 10.0f;
	fallOff = Vec3(0.0f, 0.0f, 0.0f);
	
}

LightActor::~LightActor()
{
	OnDestroy();
}
bool LightActor::OnCreate()
{
	if (isCreated) return isCreated;
	size_t buffer_size = UBO_PADDING::VEC3 + UBO_PADDING::VEC4;
	glGenBuffers(1, &uboLightDataID);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLightDataID);
	glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	size_t offset = 0;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Vec3), pos);
	offset = UBO_PADDING::VEC3;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Vec4), colour);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, uboLightDataID);

	isCreated = true;
	return isCreated;
}

void LightActor::OnDestroy()
{
	glDeleteBuffers(1, &uboLightDataID);
	isCreated = false;
}

