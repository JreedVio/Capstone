#ifndef ROOM_H
#define ROOM_H

#include "Actor.h"
#include "DoorActor.h"
#include <unordered_map>

class Room {

protected:
	int width;
	int length;
	int height;

	float roomTime;
	float currentTime;

	Ref<DoorActor> door;
	bool puzzleSolved;

	std::unordered_map<const char*, Ref<Actor>> actorList;
	std::unordered_map<const char*, Ref<Actor>> wallList; //Not used, keep it for now

	void TimeOver();

public:
	Room(int width_, int length_, int height_);
	~Room();
	virtual std::unordered_map<const char*, Ref<Actor>> GetActorList() { return actorList; }
	virtual std::unordered_map<const char*, Ref<Actor>> GetWallList() { return wallList; }

	virtual bool OnCreate();
	virtual void Update(float deltaTime);
	virtual void OnDestroy();
	virtual void CheckPuzzle();
	bool IsSolved() { return puzzleSolved; }
	void OpenDoor() { door->SetIsOpened(true); }
	void SetRoomTime(float roomTime_);
	void SetDoor(Ref<DoorActor> door_) { door = door_; }

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

	Ref<Actor> GetActor(const char* name_) {
		for (auto actor_ : GetActorList()) {
			if (strcmp(actor_.first, name_) == 0) {
				return actor_.second;
			}
		}
		return nullptr;
	}

};

#endif

