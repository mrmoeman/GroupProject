#include "Particle.h"
#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>

Particle::Particle(XMFLOAT3 _Position, XMFLOAT3 _Colour, XMFLOAT3 _Direction, float _Scale){

	Position = _Position;
	Colour = _Colour;
	Direction = _Direction;
	Scale = _Scale;
	Alpha = 1;
	Rotation = 0;

}
Particle::~Particle(){}

void Particle::UpdateParticle(float _ScaleSpeed, float _MovementSpeed, float _FadeSpeed, float _dt){
	Position.x += Direction.x * _MovementSpeed * _dt;
	Position.y += Direction.y * _MovementSpeed * _dt;
	Position.z += Direction.z * _MovementSpeed * _dt;

	Scale += _ScaleSpeed * _dt;
	Alpha -= _FadeSpeed * _dt;

	Rotation+=1 * _dt;
}

bool Particle::ReadyToRemove(){
	if(Scale <= 0 || Alpha <=0){
		return true;
	}
	else return false;
}