#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include "Dx11DemoBase.h"
#include "DisplayObject.h"
#include "LevelArray.h"
#include <xnamath.h>
#define ARRAY_WIDTH 40
#define ARRAY_HEIGHT 40

//static int objNum=0;

#pragma once
class GameObject
{
	/*
	// ZS-->04/02/2015 added a DisplayObject*
	DisplayObject *m_display_object;
	XMFLOAT3 Pos;
	XMFLOAT3 Rot;
	XMFLOAT3 Scale;
	*/

public:
	GameObject(float _positionX, float _positionY, float _positionZ, float _rotationX, float _rotationY, float _rotationZ, float _scaleX, float _scaleY, float _scaleZ);
	// ALEX OWEN 20/04/15 - Constructor with default values
	GameObject(XMFLOAT3 _position = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 _rotation = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 _scale = XMFLOAT3(0.0f, 0.0f, 0.0f));
	virtual ~GameObject(void);
	DisplayObject *m_display_object;
	//void SetPos();
	void setPosition(XMFLOAT3 _objPos);
	//cat
	void MovingPos(float x, float y, float z);
	void setPosition(float _x, float _y, float _z);
	void setPositionY(float _y);
	void setRotation(XMFLOAT3 _objRot);
	void setRotation(float _x, float _y, float _z);
	void setRotationZ(float _z);
	void setScale(XMFLOAT3 _scale);
	void setScale(float _scale);
	void setScale(float _x, float _y, float _z);
	void setPositionVec(XMVECTOR _pos);
	void setRotationVec(XMVECTOR _rot);

	float getX() const;
	float getY() const;
	float getZ() const;
	float getRX() const;
	float getRY() const;
	float getRZ() const;
	float getSX() const;
	float getSY() const;
	float getSZ() const;

	XMFLOAT3 getPosition() const;
	XMVECTOR getPositionVec() const;
	XMVECTOR getRotationVec() const;

	//Alex Owen - 30/03/15 - Returns matrices
	XMMATRIX getRotationMatrix() const;
	XMMATRIX getPositionMatrix() const;
	XMMATRIX getWorldMatrix();

	//Alex Owen - 30/03/15. Getters for orientation vectors.
	XMVECTOR getForwardVector();
	XMVECTOR getRightVector();
	XMVECTOR getUpVector();

	//Alex Owen - 30/03/15 - function for updating Orientation vectors
	void updateVectors();

	//ALEX OWEN - CCTA 27/04 -
	void updateGridPosition();

	// ZS-->04/02/2015 added getter/setter for the DisplayObject*
	void SetDisplayObject( DisplayObject *display_object );
	DisplayObject * GetDisplayObject();

	// ZS-->16/03/2015 added Draw function
	bool Draw( ID3D11DeviceContext* device_context, ID3D11Buffer* world_const_buffer );

	//Alex Owen 02/03/15 default orientation reference vectors
	static const XMVECTORF32 FORWARD;
	static const XMVECTORF32 RIGHT;
	static const XMVECTORF32 UP;

	// Variables are now protected
protected:
	
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
	// Orientation Vectors
	XMFLOAT3 forwardVector;
	XMFLOAT3 rightVector;
	XMFLOAT3 upVector;
	// World Matrix stored so it does not need to be recalculated if it has not transformed
	XMFLOAT4X4 worldMatrix;
	bool updated;
	bool transformed;
};

#endif

