#include "GlobalLighting.h"

LightActor::LightActor(Component* parent_, Vec4 position_, Vec4 diffuse_):
	Actor(parent_), position(position_), diffuse(diffuse_)
{	

}

LightActor::~LightActor()
{
}
