#pragma once

//#ifndef _PARTICLE_HANDLER_H_
//#define _PARTICLE_HANDLER_H_

#include <list>
#include "ParticleEmitter.h"
#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>


void UpdateParticleHandler(float _dt);
void DrawAllParticles(ID3D11DeviceContext* deviceContext, XMFLOAT3 _PlayerPosition);
void AddEmitter(XMFLOAT3 _Position, ID3D11Device* d3dDevice_);
void AddEmitter(XMFLOAT3 _Position, XMFLOAT3 _Colour, ID3D11Device* d3dDevice_);
void AddEmitter(XMFLOAT3 _Position, XMFLOAT3 _Colour,int waves, ID3D11Device* d3dDevice_);
void HammerTime();
int GetParticleNumber();

extern std::list<ParticleEmitter> EmitterList;

//#endif