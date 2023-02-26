#ifndef ROOM_H
#define ROOM_H

#include "Actor.h"
#include "DoorActor.h"
#include <unordered_map>

class Room{

private:
	int width;
	int length;
	int height;

	float roomTime;
	float currentTime;

	std::unordered_map<const char*, Ref<Actor>> actorList;
	std::unordered_map<const char*, Ref<Actor>> wallList; //Not used, keep it for now

	void TimeOver();

public:
	Room(int width_, int length_, int height_);
	~Room();
	virtual std::unordered_map<const char*, Ref<Actor>> GetActorList() { return actorList; }
	virtual std::unordered_map<const char*, Ref<Actor>> GetWallList() { return wallList; }

	virtual void Update(float deltaTime);
	virtual void OnDestroy();
	void SetRoomTime(float roomTime_);

	template<typename ActorTemplate>
	void AddActor(const char* name_, Ref<ActorTemplate> actor_) {
		if (std::dynamic_pointer_cast<Actor>(actor_)) {
			actorList[name_] = actor_;
		}
		else {
#ifdef _DEBUG
			std::cerr << "WARNING: Trying to add a non-actor type to actor list - ignored\n";
#endif
		}
	}

//	template<typename ActorTemplate>
//	void AddWall(const char* name_, Ref<ActorTemplate> actor_) {
//		if (std::dynamic_pointer_cast<Actor>(actor_)) {
//			wallList[name_] = actor_;
//			actorList[name_] = actor_;
//		}
//		else {
//#ifdef _DEBUG
//			std::cerr << "WARNING: Trying to add a non-actor type to actor list - ignored\n";
//#endif
//		}
//	}

};

#endif

