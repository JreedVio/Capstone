#pragma once

class Component {
public:
	Component(Component* parent_):parent(parent_),isCreated(false) {}
	virtual ~Component() = default;
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
protected:
	Component* parent;
	bool isCreated;
};