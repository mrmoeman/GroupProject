

#include "ParticleEmitter.h"
#include <list>
#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>
#include "Particle.h"
#include <math.h>
#include <time.h>
#include "MyMath.h"

ParticleEmitter::ParticleEmitter(XMFLOAT3 Position, ID3D11Device* d3dDevice_)
{
	StartPosition = Position;
	ParticleColour = XMFLOAT3(0,1,0);
	ParticleDirection = XMFLOAT3(0,1,0);

	MovementSpeed = 0.2;
	FadeSpeed = 0.0002;
	ScaleSpeed = -0.0007;
	CurrentWave = 0;

	ParticleScale = 1;

	ParticlesPerWave = 15;
	NumberWaves = 50000;
	WaveTime = 250;
	DirectionVariance = 50;

	timer = 0;

	///BUFFER
	HRESULT d3dResult;

	VertexType PartcileVertices[] =
    {
        { XMFLOAT3(  10.0f, 0.0f , 10.0f ), ParticleColour, XMFLOAT3(  0.0f, 1.0f , 0.0f ) },
        { XMFLOAT3(  10.0f, 0.0f , -10.0f ), ParticleColour , XMFLOAT3(  0.0f, 1.0f , 0.0f )},
        { XMFLOAT3( -10.0f , -0.0f , -10.0f ), ParticleColour , XMFLOAT3(  0.0f, 1.0f , 0.0f )},

        { XMFLOAT3( -10.0f , -0.0f , -10.0f ), ParticleColour , XMFLOAT3(  0.0f, 1.0f , 0.0f )},
        { XMFLOAT3( -10.0f ,  0.0f, 10.0f ), ParticleColour , XMFLOAT3(  0.0f, 1.0f , 0.0f )},
        { XMFLOAT3(  10.0f,  0.0f, 10.0f ), ParticleColour , XMFLOAT3(  0.0f, 1.0f , 0.0f )},
    };

	D3D11_BUFFER_DESC vertexDesc;
    ZeroMemory( &vertexDesc, sizeof( vertexDesc ) );
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof( VertexType ) * 6;

    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory( &resourceData, sizeof( resourceData ) );
    resourceData.pSysMem = PartcileVertices;

    d3dResult = d3dDevice_->CreateBuffer( &vertexDesc, &resourceData, &vertexBufferParticle_ );

	D3D11_BUFFER_DESC constDesc;
	ZeroMemory( &constDesc, sizeof( constDesc ) );
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof( XMMATRIX );
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &worldCB_ );

	

}
ParticleEmitter::ParticleEmitter(XMFLOAT3 Position, XMFLOAT3 Colour,ID3D11Device* d3dDevice_)
{
	StartPosition = Position;
	ParticleColour = Colour;
	ParticleDirection = XMFLOAT3(0,1,0);

	MovementSpeed = 100;
	FadeSpeed = 0.1;
	ScaleSpeed = -0.35;
	CurrentWave = 0;

	ParticleScale = 1;

	ParticlesPerWave = 5;
	NumberWaves = 3;
	WaveTime = 2;
	DirectionVariance = 50;

	timer = 0;

	///BUFFER
	HRESULT d3dResult;

	VertexType PartcileVertices[] =
    {
        { XMFLOAT3(  10.0f, 0.0f , 10.0f ), ParticleColour, XMFLOAT3(  0.0f, 1.0f , 0.0f ) },
        { XMFLOAT3(  10.0f, 0.0f , -10.0f ), ParticleColour , XMFLOAT3(  0.0f, 1.0f , 0.0f )},
        { XMFLOAT3( -10.0f , -0.0f , -10.0f ), ParticleColour , XMFLOAT3(  0.0f, 1.0f , 0.0f )},

        { XMFLOAT3( -10.0f , -0.0f , -10.0f ), ParticleColour , XMFLOAT3(  0.0f, 1.0f , 0.0f )},
        { XMFLOAT3( -10.0f ,  0.0f, 10.0f ), ParticleColour , XMFLOAT3(  0.0f, 1.0f , 0.0f )},
        { XMFLOAT3(  10.0f,  0.0f, 10.0f ), ParticleColour , XMFLOAT3(  0.0f, 1.0f , 0.0f )},
    };

	D3D11_BUFFER_DESC vertexDesc;
    ZeroMemory( &vertexDesc, sizeof( vertexDesc ) );
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof( VertexType ) * 6;

    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory( &resourceData, sizeof( resourceData ) );
    resourceData.pSysMem = PartcileVertices;

    d3dResult = d3dDevice_->CreateBuffer( &vertexDesc, &resourceData, &vertexBufferParticle_ );

	D3D11_BUFFER_DESC constDesc;
	ZeroMemory( &constDesc, sizeof( constDesc ) );
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof( XMMATRIX );
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &worldCB_ );

	

}
ParticleEmitter::ParticleEmitter(XMFLOAT3 Position, XMFLOAT3 Colour, int waves, ID3D11Device* d3dDevice_){
StartPosition = Position;
	ParticleColour = Colour;
	ParticleDirection = XMFLOAT3(0,1,0);

	MovementSpeed = 100;
	FadeSpeed = 0.1;
	ScaleSpeed = -0.35;
	CurrentWave = 0;

	ParticleScale = 1;

	ParticlesPerWave = 5;
	NumberWaves = waves;
	WaveTime = 2;
	DirectionVariance = 50;

	timer = 0;

	///BUFFER
	HRESULT d3dResult;

	VertexType PartcileVertices[] =
    {
        { XMFLOAT3(  10.0f, 0.0f , 10.0f ), ParticleColour, XMFLOAT3(  0.0f, 1.0f , 0.0f ) },
        { XMFLOAT3(  10.0f, 0.0f , -10.0f ), ParticleColour , XMFLOAT3(  0.0f, 1.0f , 0.0f )},
        { XMFLOAT3( -10.0f , -0.0f , -10.0f ), ParticleColour , XMFLOAT3(  0.0f, 1.0f , 0.0f )},

        { XMFLOAT3( -10.0f , -0.0f , -10.0f ), ParticleColour , XMFLOAT3(  0.0f, 1.0f , 0.0f )},
        { XMFLOAT3( -10.0f ,  0.0f, 10.0f ), ParticleColour , XMFLOAT3(  0.0f, 1.0f , 0.0f )},
        { XMFLOAT3(  10.0f,  0.0f, 10.0f ), ParticleColour , XMFLOAT3(  0.0f, 1.0f , 0.0f )},
    };

	D3D11_BUFFER_DESC vertexDesc;
    ZeroMemory( &vertexDesc, sizeof( vertexDesc ) );
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof( VertexType ) * 6;

    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory( &resourceData, sizeof( resourceData ) );
    resourceData.pSysMem = PartcileVertices;

    d3dResult = d3dDevice_->CreateBuffer( &vertexDesc, &resourceData, &vertexBufferParticle_ );

	D3D11_BUFFER_DESC constDesc;
	ZeroMemory( &constDesc, sizeof( constDesc ) );
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof( XMMATRIX );
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &worldCB_ );
}
ParticleEmitter::~ParticleEmitter(){}
ParticleEmitter::ParticleEmitter(){}

void ParticleEmitter::Update(float _dt){
	timer+= 1 * (_dt*100);

	if(timer >= WaveTime && CurrentWave < NumberWaves){
		for(int i = 0; i < ParticlesPerWave; i++){
			//int MyInt;
			//MyInt = 1;
			ParticleList.push_back(Particle(StartPosition, ParticleColour, XMFLOAT3((float)((rand() %1000))/500 - 1,(float)((rand() %1000))/500 - 1,(float)((rand() %1000))/500 - 1), ParticleScale));
		}
		timer = 0;
		CurrentWave++;
	}



	for (std::list<Particle>::iterator it = ParticleList.begin(); it != ParticleList.end(); it ++){
		it->UpdateParticle(ScaleSpeed, MovementSpeed, FadeSpeed, _dt);
	}

	std::list<Particle>::iterator i = ParticleList.begin();
	while(i != ParticleList.end())
	{
	if(i->ReadyToRemove())
		{
		i = ParticleList.erase(i);
		}
	else
		{
		i++;
		}
	}

}

void ParticleEmitter::Draw(ID3D11DeviceContext* deviceContext, XMFLOAT3 _PlayerPosition){
	for (std::list<Particle>::iterator it = ParticleList.begin(); it != ParticleList.end(); it ++){

		// Puts the vertex and index buffers on the graphics pipeline
		unsigned int stride;
		unsigned int offset;
		// Set vertex buffer stride and offset.
		stride = sizeof(VertexType); 
		offset = 0;

		// create matrices to create a single world matrix for the GameObject's transform
		XMMATRIX PartRot_mat = XMMatrixRotationRollPitchYaw(0, it->Rotation,0);
		XMMATRIX scale_mat = XMMatrixScaling(it->Scale, it->Scale, it->Scale);
		//XMMATRIX rotation_mat = XMMatrixRotationRollPitchYaw(X_Rot, Y_Rot,0);
		XMMATRIX rotation_mat = RotateToFace(it->Position, _PlayerPosition);
		XMMATRIX position_mat = XMMatrixTranslation(it->Position.x, it->Position.y, it->Position.z);
		// 1) scale 
		// 2) rotate 
		// 3) position
		XMMATRIX world_mat = XMMatrixTranspose(PartRot_mat * scale_mat * rotation_mat * position_mat );
		// update the world matrix on the graphics card
		deviceContext->UpdateSubresource( worldCB_, 0, 0, &world_mat, 0, 0 );
		// set the world matrix on the vertex shader
		deviceContext->VSSetConstantBuffers( 0, 1, &worldCB_ );





		// Set the vertex buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetVertexBuffers(0, 1, &vertexBufferParticle_, &stride, &offset);
		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		deviceContext->Draw(6, 0 );
	}
}

void ParticleEmitter::setEmitterSettings(int _PartPerWave, int Waves, int Time){
	ParticlesPerWave = _PartPerWave;
	NumberWaves = Waves;
	WaveTime = Time;
}

void ParticleEmitter::setScale(float _Scale){
	ParticleScale = _Scale;
}

void ParticleEmitter::setScaleSpeed(float _ScaleSpeed){
	ScaleSpeed = _ScaleSpeed;
}

void ParticleEmitter::setFadeSpeed(float _FadeSpeed){
	FadeSpeed = _FadeSpeed;
}

void ParticleEmitter::setMovementSpeed(float _MovementSpeed){
	MovementSpeed = _MovementSpeed;
}

void ParticleEmitter::setPosition(XMFLOAT3 _Position){
	StartPosition = _Position;
}

void ParticleEmitter::setColour(XMFLOAT3 _Colour){
	ParticleColour = _Colour;
}

void ParticleEmitter::setDirection(XMFLOAT3 _Direction){
	ParticleDirection = _Direction;
}

void ParticleEmitter::EXTERMINATEdata(){
	ParticleList.clear();
}

int ParticleEmitter::GetNumberParticles(){
	return ParticleList.size();
}

bool ParticleEmitter::ReadyToRemoveEmitter(){
	if(ParticleList.size() == 0 && CurrentWave > 0){
		return true;
	}
	else return false;
}


