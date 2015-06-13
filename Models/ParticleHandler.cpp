#include "ParticleHandler.h"
#include "ParticleEmitter.h"
#include <list>
#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>

std::list<ParticleEmitter> EmitterList;

void UpdateParticleHandler(float _dt){
	for (std::list<ParticleEmitter>::iterator it = EmitterList.begin(); it != EmitterList.end(); it ++){
		it->Update(_dt);
	}


	std::list<ParticleEmitter>::iterator i = EmitterList.begin();
	while(i != EmitterList.end())
	{
	if(i->ReadyToRemoveEmitter())
		{
		i = EmitterList.erase(i);
		}
	else
		{
		i++;
		}
	}

}

void DrawAllParticles(ID3D11DeviceContext* deviceContext, XMFLOAT3 _PlayerPosition){
	for (std::list<ParticleEmitter>::iterator it = EmitterList.begin(); it != EmitterList.end(); it ++){
		it->Draw(deviceContext, _PlayerPosition);
	}
}

void AddEmitter(XMFLOAT3 _Position, ID3D11Device* d3dDevice_){
	EmitterList.push_back(ParticleEmitter(_Position, d3dDevice_));
}

void AddEmitter(XMFLOAT3 _Position, XMFLOAT3 _Colour, ID3D11Device* d3dDevice_){
	EmitterList.push_back(ParticleEmitter(_Position, _Colour, d3dDevice_));
}

void AddEmitter(XMFLOAT3 _Position, XMFLOAT3 _Colour,int waves, ID3D11Device* d3dDevice_){
	EmitterList.push_back(ParticleEmitter(_Position, _Colour, waves, d3dDevice_));
}

void HammerTime(){

	for (std::list<ParticleEmitter>::iterator it = EmitterList.begin(); it != EmitterList.end(); it ++){
		it->EXTERMINATEdata();
	}


	EmitterList.clear();
}

int GetParticleNumber(){
	int MyInt;
	MyInt = 0;

	for (std::list<ParticleEmitter>::iterator it = EmitterList.begin(); it != EmitterList.end(); it ++){
		MyInt += it->GetNumberParticles();
	}

	return MyInt;
}