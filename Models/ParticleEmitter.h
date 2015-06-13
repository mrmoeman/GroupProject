#ifndef _PARTICLE_EMITTER_H_
#define _PARTICLE_EMITTER_H_

#include <list>
#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>
#include "Particle.h"

class ParticleEmitter
{
private:

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT3 colour;
		XMFLOAT3 normal;
	};

	ID3D11Buffer* vertexBufferParticle_;

	XMFLOAT3 StartPosition;
	XMFLOAT3 ParticleDirection;
	XMFLOAT3 ParticleColour;

	float ParticleScale;
	int ParticlesPerWave;
	int WaveTime;
	int NumberWaves;
	float MovementSpeed;
	float FadeSpeed;
	float ScaleSpeed;

	float DirectionVariance;

	int CurrentWave;
	float timer;

	std::list<Particle> ParticleList;

	ID3D11Buffer* worldCB_;

public:
	ParticleEmitter();
	ParticleEmitter(XMFLOAT3 Position, ID3D11Device* d3dDevice_);
	ParticleEmitter(XMFLOAT3 Position, XMFLOAT3 Colour,ID3D11Device* d3dDevice_);
	ParticleEmitter(XMFLOAT3 Position, XMFLOAT3 Colour, int waves, ID3D11Device* d3dDevice_);
	~ParticleEmitter();


	void setDirection(XMFLOAT3 _Direction);
	void setColour(XMFLOAT3 _Colour);
	void setPosition(XMFLOAT3 _Position);
	void setMovementSpeed(float _MovementSpeed);
	void setFadeSpeed(float _FadeSpeed);
	void setScaleSpeed(float _ScaleSpeed);
	void setScale(float _Scale);

	void Update(float _dt);
	void Draw(ID3D11DeviceContext* deviceContext, XMFLOAT3 _PlayerPosition);
	void setEmitterSettings(int _PartPerWave, int Waves, int Time);

	void EXTERMINATEdata();

	int GetNumberParticles();

	bool ReadyToRemoveEmitter();



};

#endif