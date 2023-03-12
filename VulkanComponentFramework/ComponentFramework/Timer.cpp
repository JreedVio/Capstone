#include "SDL.h"
#include "Timer.h"
#include <iostream>

#define MAX_INTERVAL 0.1f


Timer::Timer() {
	prevTicks = 0;
	currTicks = 0;
}

Timer::~Timer() {}

void Timer::UpdateFrameTicks() {
	prevTicks = currTicks;
	currTicks = SDL_GetTicks();
}

void Timer::Start() {
	prevTicks = SDL_GetTicks();
	currTicks = SDL_GetTicks();
}

float Timer::GetDeltaTime() const {
	float tickInterval = float(currTicks - prevTicks);

	if (tickInterval / 1000.0f >= MAX_INTERVAL) {
		tickInterval = 0.0f;
	}

	return (tickInterval / 1000.0f);
}

unsigned int Timer::GetSleepTime(const unsigned int fps) const {
	unsigned int milliSecsPerFrame = 1000 / fps;
	if (milliSecsPerFrame == 0) {
		return 0;
	}

	if (fps == 64) {
		//std::cout << "YO";
	}

	unsigned int sleepTime = milliSecsPerFrame - (SDL_GetTicks() - currTicks);
	if (sleepTime > milliSecsPerFrame) {
		//return milliSecsPerFrame; 
		// Add more delay if it already takes too long?
		//Makes no sense to me:)
		return 0;
	}
	//std::cout << sleepTime << std::endl;
	return sleepTime;
}

float Timer::GetCurrentTicks() const{
	return float(currTicks - prevTicks);
}
