#pragma once
#include "gameobject.h"
class MovingGameObject : public GameObject
{
public:
	// ALEX OWEN 20/04/15 - constructor with default values
	MovingGameObject(XMFLOAT3 _position = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 _rotation = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 _scale = XMFLOAT3(0.0f, 0.0f, 0.0f));
	~MovingGameObject(void);

	// ALEX OWEN 20/04/15 update now virtual to allow the function to be called for all gameobject types
	virtual void update(float delta);
	
	void setVelocity(XMFLOAT3 _velocity);
	void setVelocity(XMVECTOR _velocity);
	void setMaxSpeed(float _maxSpeed);
	XMVECTOR getVelocityVec();
	XMFLOAT3 getVelocityDirection();
	float getCurrentSpeed();
	float getMaxSpeed();
	void Hit(float);
	void Attack();
	float gethp() {return hp_; };
	void SetHP(float);

private:
	XMFLOAT3 velocity;
	float maxSpeed;
	float hp_;
	float attack_;
};

