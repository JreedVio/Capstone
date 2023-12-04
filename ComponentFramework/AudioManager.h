#ifndef AUDIOMANAGE_H
#define AUDIOMANAGE_H

#include <stdio.h>
#include <iostream>
#include <irrKlang.h>
#include <thread>
#include <chrono>
#include <algorithm>

using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

class AudioManager{

private:
	static AudioManager* instance_;
	float masterVol;
	float bgmVol;
	float sfxVol;
	ISound* backgroundMusic;
	ISoundEngine* engine;
	AudioManager();
	void PlayFadedSound(ISound* sound_);
	void fadeBGM();
public:

	~AudioManager();
	static AudioManager* getInstance();
	bool OnCreate();
	void OnDestroy();
	void Update();
	void PlayBGM(const char* fileName_);
	void PlaySoundEffects(const char* fileName_, bool faded = false);
	void RestartBGM();
	void PauseBGM(bool paused_);
	void FadeBGM();
	bool isBGMPaused() { 
		if (backgroundMusic) return backgroundMusic->getIsPaused(); 
		return false;
	}
	

	float getMasterVolume() { return masterVol; }
	float getBGMVolume() { return bgmVol; }
	float getSFXVolume() { return sfxVol; }
	void setMasterVolume(float vol_) {
		engine->setSoundVolume(masterVol);
		masterVol = vol_;
		masterVol = std::clamp(masterVol, 0.0f, 1.0f);
	}
	void setBGMVolume(float vol_) {
		bgmVol = vol_;
		bgmVol = std::clamp(bgmVol, 0.0f, 1.0f);
	}
	void setSFXVolume(float vol_) {
		sfxVol = vol_;
		sfxVol = std::clamp(sfxVol, 0.0f, 1.0f);
	}

};


#endif