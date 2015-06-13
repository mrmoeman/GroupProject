#include "SoundLoader.h"


SoundLoader::SoundLoader()
{
	//Init();
}

SoundLoader::~SoundLoader(){}

bool SoundLoader::Init()
{
	/* std::unordered_map<SOUND_ID, XACTINDEX>sounds;
	 audioBanks_.soundBank->Play( sounds[SOUND_ID::JUMP], 0, 2, 0 );
	 XACTINDEX clipCue = audioBanks_.soundBank->GetCueIndex( "clip" );
	 XACTINDEX clipCue1 = audioBanks_.soundBank->GetCueIndex( "RainSFX" );*/
	


	// NATH Nathaniel smart audio // 25/03/2015
	bool result = false;

	ZeroMemory( &audioBanks_, sizeof( stXACTAudio ) );
	// setting up XACT if soundEngine is the same as false, return false
	result = SetupXACT( &soundEngine_);
    if( result == false )
        return false;
	// load XACT from sound engine audio banks 
    if( LoadXACTAudio( soundEngine_, audioBanks_, "Win/Wave Bank.xwb", "Win/Sound Bank.xsb" ) == false )
        return false;

	//audioBanks_.soundBank->Play( sounds[SOUND_ID::RAIN], 0, 2, 0 );
	//LoadSounds();

}

void SoundLoader::AddSoundFile( SOUND_ID soundID, string fileName)
{	
	// XACT Index pulles the sound file name  
	sounds[soundID] = audioBanks_.soundBank->GetCueIndex(fileName.c_str());
}

void SoundLoader::LoadSounds()
{	
	// load all the sounds in the game
	AddSoundFile( SOUND_ID::RAIN, "clip" );
}

void SoundLoader::PlaySound( SOUND_ID soundID )
{
	// play the sound using the id to find it
	audioBanks_.soundBank->Stop( sounds[soundID], NULL );
	audioBanks_.soundBank->Play( sounds[soundID], 0, 2, 0 );
}

void SoundLoader::UnloadSoundEngine()
{
	  if( soundEngine_ )
    {
        soundEngine_->ShutDown( );
        soundEngine_->Release( );
    }

    if( audioBanks_.soundBankData )
    {
        UnmapViewOfFile( audioBanks_.soundBankData );
        audioBanks_.soundBankData = 0;
    }

    if( audioBanks_.waveBankData )
    {
        UnmapViewOfFile( audioBanks_.waveBankData );
        audioBanks_.waveBankData = 0;
    }
}