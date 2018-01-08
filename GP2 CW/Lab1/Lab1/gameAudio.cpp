#include "gameAudio.h"


gameAudio::gameAudio()
{
	alDevice = alcOpenDevice(NULL); //open sound card
	if (alDevice == NULL)
	{
		std::cout << "cannot open sound card" << std::endl; //cannot open sound card error
	}

	alContext = alcCreateContext(alDevice, NULL); //create and AL context

	if (alContext == NULL)
	{
		std::cout << "cannot open context" << std::endl; //cannot open AL error
	}

	alcMakeContextCurrent(alContext); //makes AL the current context
}

gameAudio::~gameAudio()
{
	//Destructor 
	//Deltes buffers etc
	for (unsigned int i = 0; i < datas.size(); i++)
	{
		alDeleteSources(1, (unsigned int*)&datas[i].sourceIDNum);
		if (datas[i].bufferIDNum != -1)
		{
			alDeleteBuffers(1, (unsigned int*)&datas[i].bufferIDNum);
			delete[] datas[i].audioBuffer;
		}
	}
	alcDestroyContext(alContext);
	alcCloseDevice(alDevice);
}

bool gameAudio::isBigEndian()
{
	//determines audio type for given system
	int a = 1;
	return !((char*)&a)[0];
}

int gameAudio::changeToInt(char* buffer, int length)
{
	int a = 0;
	if (!isBigEndian())
		for (int i = 0; i < length; i++)
			((char*)&a)[i] = buffer[i];
	else
		for (int i = 0; i < length; i++)
			((char*)&a)[3 - i] = buffer[i];
	return a;
}

char* gameAudio::loadWAVFile(const char* fn, int& chan, int& samplerate, int& bps, int& size)
{
	//Loads in wav files via buffers
	char buffer[4];
	std::ifstream in(fn, std::ios::binary);
	in.read(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 4); //WAV
	in.read(buffer, 4);	//fmt
	in.read(buffer, 4); //16 bit
	in.read(buffer, 2); //1
	in.read(buffer, 2);
	chan = changeToInt(buffer, 2);
	in.read(buffer, 4);
	samplerate = changeToInt(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	bps = changeToInt(buffer, 2);
	in.read(buffer, 4);
	in.read(buffer, 4);
	size = changeToInt(buffer, 4);

	char* soundData = new char[size];
	in.read(soundData, size);
	return soundData;
}

unsigned int gameAudio::loadInSoundFx(const char* filename)
{
	//used to initialise files
	bool found = false;
	unsigned int sourceID, bufferID;
	char* soundData = NULL;
	for (unsigned int i = 0; i < datas.size(); i++)
	{
		if (datas[i].audioName == filename && datas[i].bufferIDNum != -1)
		{
			bufferID = datas[i].bufferIDNum;
			found = true;
			break;
		}
	}
	if (!found)
	{
		int channel, sampleRate, bps, size;
		soundData = loadWAVFile(filename, channel, sampleRate, bps, size);
		unsigned int format;
		alGenBuffers(1, &bufferID);

		if (channel == 1)
		{
			if (bps == 8)
			{
				format = AL_FORMAT_MONO8;
			}
			else
			{
				format = AL_FORMAT_MONO16;
			}
		}
		else
		{
			if (bps == 8)
			{
				format = AL_FORMAT_STEREO8;
			}
			else
			{
				format = AL_FORMAT_STEREO16;
			}

		}

		alBufferData(bufferID, format, soundData, size, sampleRate);
	}
	alGenSources(1, &sourceID);
	alSourcei(sourceID, AL_BUFFER, bufferID);
	alSourcef(sourceID, AL_REFERENCE_DISTANCE, 1.0f);
	datas.push_back(data(sourceID, (!found ? bufferID : -1), soundData, filename));
	return sourceID;
}

void gameAudio::deleteSoundFx(unsigned int id)
{}

void gameAudio::playSoundFx(unsigned int id)
{
	alSourcePlay(id);
}
void gameAudio::playSoundFx(unsigned int id, glm::vec3& pos)
{
	//plays sound from a given position
	alSource3f(id, AL_POSITION, pos.x, pos.y, pos.z);
	alSourcePlay(id);
}
void gameAudio::stopSoundFx(unsigned int id)
{
	//stops sound from playing 
	alSourceStop(id);
}
void gameAudio::setAudioListener(glm::vec3& pos, glm::vec3& camLookAt)
{
	//sets postion of audio listener

	alListener3f(AL_POSITION, pos.x, pos.y, -pos.z);
	float orinet[6] = { camLookAt.x,camLookAt.y,camLookAt.z,0,1,0 };
	alListenerfv(AL_ORIENTATION, orinet);
}