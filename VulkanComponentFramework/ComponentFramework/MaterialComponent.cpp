#include "MaterialComponent.h"
MaterialComponent::MaterialComponent(Component* parent_, const char* filename_):Component(parent_) {
	filename = filename_;
}

MaterialComponent::~MaterialComponent() {
	OnDestroy();
}

bool MaterialComponent::OnCreate() {
	return true;
}

void MaterialComponent::OnDestroy() {	
}
void MaterialComponent::Update(const float deltaTime) {}
void MaterialComponent::Render()const {}