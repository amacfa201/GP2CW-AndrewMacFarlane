#pragma once

#include <glm\glm.hpp>
#include <AL\al.h>
#include <AL\alc.h>
#include <vector>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <SDL\SDL.h>
#include <glm/gtc/type_ptr.hpp>

class gameAudio
{
	struct data {
		int sourceIDNum, bufferIDNum;
		char* audioBuffer;
		std::string audioName;
		data(unsigned int sI, unsigned int bI, char* b, const char* n) // needs changed still
		{
			this->sourceIDNum = sI;
			this->bufferIDNum = bI;
			this->audioBuffer = b;
			this->audioName = n;
		}

	};

	struct Vector3
	{
		float x, y, z;
	};
	std::vector<data> datas;
	ALCcontext* alContext;
	ALCdevice *alDevice;
	bool isBigEndian(); //idk wtf this is refactor later
	int changeToInt(char* buffer, int length);
	char* loadWAVFile(const char* fn, int& chan, int& samplerate, int& bps, int& size);

public:
	gameAudio();
	~gameAudio();
	unsigned int loadInSoundFx(const char* filename);
	void deleteSoundFx(unsigned int id);
	void playSoundFx(unsigned int id);
	void playSoundFx(unsigned int id, glm::vec3& pos);
	void stopSoundFx(unsigned int id);
	void setAudioListener(glm::vec3& pos, glm::vec3& camLookAt);

private:

};

