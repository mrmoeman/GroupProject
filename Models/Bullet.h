#pragma once
//ALEX OWEN 20/04/15 - Bullet Class handles Bullet behaviour
#include "MovingGameObject.h"
class Bullet :
	public MovingGameObject
{
public:
	Bullet(void);
	~Bullet(void);

	Bullet(XMFLOAT3 _position);

	void update(float delta);
	void setStartPoint(XMFLOAT3 _startPoint);
	void stopBullet();

	bool fired;
	XMFLOAT3 startPoint;
	float maxDistance;
};

