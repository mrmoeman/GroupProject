#pragma once

#include "Movinggameobject.h"
#include "Animation.h"

class AnimatedGameObject :
	public MovingGameObject
{
public:
	AnimatedGameObject(XMFLOAT3 _position = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 _rotation = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 _scale = XMFLOAT3(0.0f, 0.0f, 0.0f), Animation* _animator = NULL);
	~AnimatedGameObject(void);

	void updateani(float delta);

	Animation* animator;
};

