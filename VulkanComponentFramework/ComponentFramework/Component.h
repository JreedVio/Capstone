#pragma once
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

class Component {
public:
	Component(Ref<Component> parent_):parent(parent_),isCreated(false) {}
	virtual ~Component() = default;
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
protected:
	Ref<Component> parent;
	bool isCreated;
};