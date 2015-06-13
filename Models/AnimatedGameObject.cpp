#include "AnimatedGameObject.h"


AnimatedGameObject::AnimatedGameObject(XMFLOAT3 _position, XMFLOAT3 _rotation, XMFLOAT3 _scale, Animation* _animator)
	:MovingGameObject(_position, _rotation, _scale)
{
	animator = _animator;
}


AnimatedGameObject::~AnimatedGameObject(void)
{

}

void AnimatedGameObject::updateani(float delta)
{
	animator->Update(delta);
	MovingGameObject::update(delta);
}
