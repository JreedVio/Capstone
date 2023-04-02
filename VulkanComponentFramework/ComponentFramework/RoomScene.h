#ifndef ROOMSCENE_H
#define ROOMSCENE_H

#include "Scene.h"
#include "Room.h"
#include <vector>
#include "PlayerController.h"

class LightActor;


class RoomScene : public Scene{

public:
	RoomScene(VulkanRenderer* renderer_);
	RoomScene(VulkanRenderer* renderer_, Ref<Room> room_);
	virtual ~RoomScene();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;

	void SetRoom(Ref<Room> room_) { room = room_; }
	Ref<Room> GetRoom() { return room; }
	virtual std::unordered_map<const char*, Ref<Actor>> GetActorList() override{ 
		return room->GetActorList();
	}
	Ref<Actor> GetActor(const char* name_);	

	void AddActor(const char* name_, Ref<Actor> actor_) { room->AddActor(name_, actor_); }

private:
	Ref<Room> room;
	Ref<PlayerController> remotePlayer;
	Ref<PlayerController> localPlayer;


};

#endif
