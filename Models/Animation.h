#pragma once

#include "DisplayObject.h"
#include <vector>

class Animation
{
	bool m_paused;
	float m_frame_timer;
	float m_frame_delay;
	unsigned int m_current_frame;
	unsigned int m_num_frames;
	std::vector<DisplayObject*> m_frames;
	
public:
	Animation(void);
	~Animation(void);

	void AddFrame( DisplayObject* frame );
	void AddFrames( std::vector<DisplayObject*> frames );
	void SetFramesPerSec( float frames_per_sec );
	void SetFrameDelay( float frame_delay_delta );
	void AdjustFrameDelay( float frame_delay_adj );
	
	void Pause( bool pause );
	void Play( int frame_num = -1 );

	void Update( float delta );
	DisplayObject* GetCurrentFrame();
};

