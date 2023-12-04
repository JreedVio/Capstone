#ifndef PHYSICSPUZZLEROOM_H
#define PHYSICSPUZZLEROOM_H


#include "Room.h"
#include <vector>
#include <memory>

class UserInterface;

class PhysicsPuzzleRoom : public Room{

public:

	PhysicsPuzzleRoom(int width_, int length_, int height_);	
	~PhysicsPuzzleRoom();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void CheckPuzzle() override;	

private:
		
	Ref<Actor> Cube1;
	Ref<Actor> Cube2;
	Ref<Actor> plate1;
	Ref<Actor> plate2;
	std::vector<Ref<Actor>> Cubes;


};

#endif
