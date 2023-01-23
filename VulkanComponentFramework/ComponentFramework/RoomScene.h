#ifndef ROOMSCENE_H
#define ROOMSCENE_H

#include "Scene.h"

class Room;

class RoomScene : public Scene{

public:

	explicit RoomScene(Renderer* renderer_, Ref<Room> room_);
	virtual ~RoomScene();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;

private:
	Ref<Room> room;
};

#endif
