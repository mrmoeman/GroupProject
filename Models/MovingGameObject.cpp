#include "MovingGameObject.h"
#include "ModelsDemo.h"

// ALEX OWEN 20/04/15 - Constructor with deafult values theat calls it's paremt constructor
MovingGameObject::MovingGameObject(XMFLOAT3 _position, XMFLOAT3 _rotation,XMFLOAT3 _scale)
	: GameObject(_position, _rotation, _scale)
{
	maxSpeed =0.1f;
	hp_ = 0.0f;
	attack_ = 0.0f;
	velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//state = 3;
}


MovingGameObject::~MovingGameObject(void)
{
}

// ALEX OWEN 20/04/15 - update function now uses delta and updates oreientation vectors
void MovingGameObject::update(float delta)
{
	XMVECTOR position = getPositionVec();
	XMVECTOR _velocity = getVelocityVec();

	
	if(!(XMVectorGetX(_velocity) == 0.0f && XMVectorGetY(_velocity) == 0.0f && XMVectorGetZ(_velocity) == 0.0f))
		setPositionVec(position + _velocity);

	//ALEX OWEN - 30/03/15 - get the angle between the default forward and your current velocity.
	XMVECTOR angleVec = XMVector3AngleBetweenVectors(FORWARD, getVelocityVec());
	if(!(XMVectorGetX(angleVec) == 0.0f && XMVectorGetY(angleVec) == 0.0f && XMVectorGetZ(angleVec) == 0.0f))
	setRotation(getRX(),XMVectorGetZ(angleVec), getRZ());
	
	updateVectors();
	//updated = true;
	//turn object to face velocity. May need to change if objects are not always facing the dirtection
	// we are moving in. Getting pushed back etc.
	//Rot.x = XMVectorGetZ(XMVector3Normalize(_velocity));
	//setRotVec(XMVECTOR(XMVector3Normalize(_velocity)));

	//setRotationVec(_velocity);

	//OutputDebugString("updating");
}

void MovingGameObject::setVelocity(XMFLOAT3 _velocity)
{
	velocity = _velocity;
}

void MovingGameObject::setVelocity(XMVECTOR _velocity)
{
	velocity.x = XMVectorGetX(_velocity);
	velocity.y = XMVectorGetY(_velocity);
	velocity.z = XMVectorGetZ(_velocity);
}

void MovingGameObject::setMaxSpeed(float _maxSpeed)
{
	maxSpeed = _maxSpeed;
}

XMVECTOR MovingGameObject::getVelocityVec()
{
	return XMVectorSet(velocity.x, velocity.y, velocity.z, 1);
}

// ALEX OWEN - 16/02/15 - Returns the direction of the velocity
XMFLOAT3 MovingGameObject::getVelocityDirection()
{
	XMVECTOR direction = XMVector3Normalize(XMVECTOR(getVelocityVec()));
	return XMFLOAT3(XMVectorGetX(direction), XMVectorGetY(direction), XMVectorGetZ(direction));
}

float MovingGameObject::getCurrentSpeed()
{
	return sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z + velocity.z);
}
////////////////////////////////////////////////////////////////////////////////

float MovingGameObject::getMaxSpeed()
{
	return maxSpeed;
}

void MovingGameObject::Hit(float dmg)
{
	hp_ -= dmg;

	if( hp_ <= 0.0 ) 
	{
		hp_ = 0.0f;
	}
}

void MovingGameObject::Attack()
{
	HitPlayer(attack_);
}

void MovingGameObject::SetHP(float hp)
{
	hp_ = hp;
}

//void adjustHealth( float val )