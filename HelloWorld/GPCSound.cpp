#include "GPCSound.h"
bool SoundManager::_initialized = false;
FMOD::System* SoundManager::_system = nullptr;
FMOD_RESULT SoundManager::_result = FMOD_OK;
std::string SoundManager::_soundPath = "";
std::vector<FMOD::Sound*> SoundManager::_sounds = {};
std::vector<FMOD::ChannelGroup*> SoundManager::_channelGroups = {};
std::vector<FMOD::Channel*> SoundManager::_channels = {};
void SoundManager::init(const std::string& defaultFilePath, unsigned numChannels)
{
	if (!_initialized) {
		//i followed some documentation https://www.fmod.com/resources/documentation-api?version=2.0&page=white-papers-getting-started.html

		_soundPath = defaultFilePath;

		_result = FMOD::System_Create(&_system);
		checkFmodErrors(_result, "System creation");

		_result = _system->init(numChannels, FMOD_INIT_NORMAL, 0);
		checkFmodErrors(_result, "System initialization");
		_initialized = true;

		for (unsigned i = 0; i < numChannels; i++) {
			FMOD::Channel* channel = nullptr;
			_channels.push_back(channel);
		}
	}
	//else
		//printf("Already initialized Sound Manager!\n");
}

void SoundManager::update()
{
	// clears memory when channel is no longer in use
	_system->update();
}

unsigned SoundManager::load2DSound(const std::string& relativeFilePath)
{
	FMOD::Sound* sound = nullptr;
	_result = _system->createSound(std::string(_soundPath + relativeFilePath).c_str(), FMOD_2D, nullptr, &sound);
	checkFmodErrors(_result, "Sound Creation");

	_sounds.push_back(sound);

	return _sounds.size() -1;
}

unsigned SoundManager::createChannelGroup(const std::string& groupName)
{
	FMOD::ChannelGroup* group = nullptr;
	_result = _system->createChannelGroup(groupName.c_str(), &group);
	checkFmodErrors(_result, "Group Creation");

	_channelGroups.push_back(group);

	return _channelGroups.size() -1;

}

void SoundManager::play2DSound(unsigned soundIndex, unsigned groupIndex)
{
	//find a free channel for FMOD to use
	unsigned index = 0;
	//flag to check if all channels are nullptr
	bool allNullptr = true;
	for (unsigned i = 0; i < _channels.size(); i++)
	{
		if (_channels[i] != nullptr)
		{
			//set index to current index i
			allNullptr = false;
			bool playing = false;

			//check if channel is playing the sound, if it isn't thats out index
			_channels[i]->isPlaying(&playing);
			if (!playing) // if the channel is free
				index = i;
		}
	}
	if (allNullptr)
		index = 0;

	_result = _system->playSound(_sounds[soundIndex], _channelGroups[groupIndex], false, &_channels[index]);

	checkFmodErrors(_result, "Play sound 2D");

}

void SoundManager::checkFmodErrors(FMOD_RESULT& result, const std::string& whereError)
{
	if (result != FMOD_OK)
	{
		//print out the error, and where it happened
		printf("FMOD error! (%d) %s at %s\n", result, FMOD_ErrorString(result), whereError.c_str());

		//break the program
		__debugbreak();
	}
}

Sound2D::Sound2D(const std::string& path,const std::string& groupName)
{
	_sound = SoundManager::load2DSound(path);
	_group = SoundManager::createChannelGroup(groupName);
}

void Sound2D::play()
{
	SoundManager::play2DSound(_sound, _group);
}
