#ifndef CHRONOTIMER_H
#define CHRONOTIMER_H

#include <iostream>
#include <chrono>
#include <thread>

struct ChronoTimer {
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

	ChronoTimer() {
		start = std::chrono::high_resolution_clock::now();
	}
	~ChronoTimer() {
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;
		float ms = duration.count() * 1000.0f;
		std::cout << "Timer took " << ms << "ms" << std::endl;
	}
};


#endif