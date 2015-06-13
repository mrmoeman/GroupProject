#include "Xact3Audio.h"
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

#ifndef _SOUND_LOADER_H_
#define _SOUND_LOADER_H_

enum SOUND_ID
{
	RAIN,
	BGMUSIC
};

class SoundLoader
{
public:
	SoundLoader();
	~SoundLoader();
	
	bool Init();

	void UnloadSoundEngine();
	void LoadSounds( );
	void AddSoundFile(SOUND_ID soundID, string fileName);
	void PlaySound( SOUND_ID soundID );
	

	stXACTAudio audioBanks_;
	IXACT3Engine* soundEngine_;

private:
	unordered_map<SOUND_ID, XACTINDEX>sounds;

};



#endif