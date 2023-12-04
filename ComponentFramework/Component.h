#pragma once
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

class Component {
public:
	Component() :parent(nullptr), isCreated(false){}
	Component(Component* parent_):parent(parent_),isCreated(false) {}
	virtual ~Component() = default;
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
	virtual void SetParent(Component* parent_) { parent = parent_; }
	virtual Component* GetParent() const { return parent; }
protected:
	Component* parent;
	bool isCreated;
};