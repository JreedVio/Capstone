#ifndef PLATEPUZZLEROOM_H
#define PLATEPUZZLEROOM_H

#include <memory>
#include "Room.h"

class PlatePuzzleRoom : public Room{

private:

	Ref<Actor> plate1;
	Ref<Actor> plate2;
	const char* plate1Name;
	const char* plate2Name;

public:
	PlatePuzzleRoom(int width_, int length_, int height_);
	~PlatePuzzleRoom();

	virtual bool OnCreate() override;
	virtual void Update(float deltaTime) override;
	virtual void OnDestroy() override;
	virtual void CheckPuzzle() override;

	void SetPlate1Name(const char* plate1_) { plate1Name = plate1_; }
	Ref<Actor> GetPlate1() { return plate1; }
	void SetPlate2Name(const char* plate2_) { plate2Name = plate2_; }
	Ref<Actor> GetPlate2() { return plate2; }

};

#endif

