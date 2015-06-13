#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>

class Particle
{
private:

	

public:

	Particle(XMFLOAT3 _Position, XMFLOAT3 _Colour, XMFLOAT3 _Direction, float _Scale);
	~Particle();

	void UpdateParticle(float _ScaleSpeed, float _MovementSpeed, float _FadeSpeed, float _dt);

	float Rotation;
	XMFLOAT3 Position;
	XMFLOAT3 Colour;
	XMFLOAT3 Direction;
	float Scale;
	float Alpha;

	bool ReadyToRemove();

};

#endif