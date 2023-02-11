#ifndef ROOM_H
#define ROOM_H

#include "Actor.h"
#include <unordered_map>

class Room{

private:
	int width;
	int length;
	int height;
	std::unordered_map<const char*, Ref<Actor>> actorList;

public:
	Room(int width_, int length_, int height_);
	~Room();
	virtual std::unordered_map<const char*, Ref<Actor>> GetActorList() { return actorList; }
	virtual void OnDestroy();


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

};

#endif

