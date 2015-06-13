#include "Animation.h"

/*
class Animation
{
	float m_frame_timer;
	float m_frame_speed;
	unsigned int m_current_frame;
	unsigned int m_num_frames;
	std::vector<DisplayObject*> m_frames;
	
public:
	Animation(void);
	~Animation(void);

	void AddFrame( DisplayObject* frame );
	void AddFrames( std::vector<DisplayObject*> frames );
	void SetFramesPerSec( float frames_per_sec );
	void SetFrameSpeed( float frame_speed_delta );

	void Play( unsigned int frame_num = 0 );
	void Pause();

	void Update( float delta );
	DisplayObject* GetCurrentFrame();
};
*/


Animation::Animation(void)
{
	m_frame_timer = 0;
	m_frame_delay = 1;
	m_current_frame = 0;
	m_num_frames = 0;
	m_paused = false;
}

Animation::~Animation(void)
{
}

void Animation::AddFrame( DisplayObject* frame )
{
	if ( frame != NULL )
	{
		m_frames.push_back( frame );
		++m_num_frames;
	}
}

void Animation::AddFrames( std::vector<DisplayObject*> frames )
{
	m_frames.reserve( m_frames.size() + frames.size() );

	std::vector<DisplayObject*>::iterator it = frames.begin();

	while ( it != frames.end() )
	{
		DisplayObject* frame = *it;
		if ( frame != NULL )
		{
			m_frames.push_back( frame );
			++m_num_frames;
		}
	}
}

void Animation::SetFramesPerSec( float frames_per_sec )
{
	if ( frames_per_sec < 0.1f ) frames_per_sec = 0.1f;
	m_frame_delay = 1.0f / frames_per_sec;
	m_frame_timer = 0;
}

void Animation::SetFrameDelay( float frame_delay_delta )
{
	m_frame_delay = frame_delay_delta;
	m_frame_timer = 0;
}

void Animation::AdjustFrameDelay( float speed_adj )
{
	m_frame_delay += speed_adj;
	if ( m_frame_delay <= 0.0001f ) m_frame_delay = 0.0001f;
}

void Animation::Play( int frame_num )
{
	if ( frame_num == -1 )
	{
		m_paused = false; return;
	}
	if ( (unsigned int)frame_num < m_num_frames ) m_current_frame = frame_num;
	m_frame_timer = 0;
	m_paused = false;
}

void Animation::Pause( bool pause )
{
	m_paused = pause;
}

void Animation::Update( float delta )
{
	if ( delta > m_frame_delay || delta < 0 )
	{
		m_frame_timer = 0;
		if ( ++m_current_frame == m_num_frames )
		{
			m_current_frame = 0;
		}

		return;
	}

	m_frame_timer += delta;
	if ( m_frame_timer >= m_frame_delay )
	{
		m_frame_timer -= m_frame_delay;
		if ( ++m_current_frame == m_num_frames )
		{
			m_current_frame = 0;
		}
	}
}

DisplayObject* Animation::GetCurrentFrame()
{
	if ( m_frames.empty() ) return NULL;
	return m_frames[m_current_frame];
}
