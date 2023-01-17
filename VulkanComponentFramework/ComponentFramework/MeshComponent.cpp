#include "MeshComponent.h"

using namespace MATH;

MeshComponent::MeshComponent(Component* parent_, const char* filename_) : Component(parent_) {
    filename = filename_;
}
MeshComponent::~MeshComponent() {
    OnDestroy();
}

bool MeshComponent::OnCreate() {
    return true;
}

void MeshComponent::Render() const {
}

void MeshComponent::OnDestroy() {
}

void MeshComponent::Update(const float deltaTime) {}