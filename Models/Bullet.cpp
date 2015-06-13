#include "Bullet.h"

// ALEX OWEN 20/04/15 - Bullet class for handling bullet behaviour
Bullet::Bullet(void)
{
	MovingGameObject::setMaxSpeed(5.0f);
	fired = false;
	startPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
	maxDistance = 1500;
}


Bullet::~Bullet(void)
{

}
/*
Bullet::Bullet(XMFLOAT3 _position): MovingGameObject(_position)
{
	fired = false;
	startPoint = XMFLOAT3(_posX, _posY, _posZ);
	maxDistance = 1500;
}
*/

void Bullet::setStartPoint(XMFLOAT3 _startPoint)
{
	startPoint = _startPoint;
}

void Bullet::update(float delta)
{
	MovingGameObject::update(delta);
	XMVECTOR newPosition = getPositionVec();
	XMFLOAT3* startPointPointer = &startPoint;
	XMVECTOR startPointVec = XMLoadFloat3(startPointPointer);
	XMVECTOR difference = XMVectorSubtract(newPosition,startPointVec);
    XMVECTOR length = XMVector3Length(difference);
	float distance = 0.0f;
	XMStoreFloat(&distance,length);
	if(distance > maxDistance) stopBullet();
}

void Bullet::stopBullet()
{
	fired = false;
	startPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

