#include "GameObject.h"

//ALEX OWEN - 30/03/15 - set default orientation vectors. Our forward is poitive on the z.
const XMVECTORF32 GameObject::FORWARD = {0.0f,0.0f,1.0f,1};
const XMVECTORF32 GameObject::RIGHT = {1.0f,0.0f,0.0f,1};
const XMVECTORF32 GameObject::UP = {0.0f,1.0f,0.0f,1};


//ALEX OWEN 30/03/15 new constructors to handle

GameObject::GameObject(float _positionX, float _positionY, float _positionZ, float _rotationX, float _rotationY, float _rotationZ, float _scaleX, float _scaleY, float _scaleZ)
	: position( _positionX, _positionY, _positionZ), rotation( _rotationX, _rotationY, _rotationZ), scale( _scaleX, _scaleY, _scaleZ)
{
	updateGridPosition();
	transformed = true; 
	updated = true;
	updateVectors();
}

//ALEX OWEN 20/04/15 -  new constructor
GameObject::GameObject(XMFLOAT3 _position, XMFLOAT3 _rotation, XMFLOAT3 _scale)
	: position(_position), rotation(_rotation), scale(_scale)
{
	updateGridPosition();
	transformed = true; 
	updated = true;
	updateVectors();
}

GameObject::~GameObject(void)
{
}

//int GameObject::getObjNum ( ) const
//{
//	return objNum;
//}

//void GameObject::SetPos()
//{

//}


// All functions that set transform values cnagean updated and transformed flag in order
// to recalculate the orientation vectors and world matrix.

void GameObject::setPosition(XMFLOAT3 _objPos)
{
	position = _objPos;
	updateGridPosition();
	updated = true;
	transformed = true;
	//world = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
}

void GameObject::setRotation(XMFLOAT3 _objRot)
{
	rotation = _objRot;
	updated = true;
	transformed = true;
	//world = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
}

// CAC 16/03/15 //
void GameObject::MovingPos(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	updateGridPosition();
	updated = true;
	transformed = true;
}

void GameObject::setPosition(float _x, float _y, float _z)
{
	position.x = _x;
	position.y = _y;
	position.z = _z;
	updated = true;
	transformed = true;
	//world = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
}

void  GameObject::setPositionY(float _y)
{
	position.y = _y;
	updateGridPosition();
	updated = true;
	transformed = true;
}


void GameObject::setRotation(float _x, float _y, float _z)
{
	rotation.x = _x;
	rotation.y = _y;
	rotation.z = _z;
	updated = true;
	transformed = true;
	//world = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
}

void GameObject::setRotationZ(float _z)
{
	rotation.z = _z;
	updated = true;
	transformed = true;
	//world = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
}

void GameObject::setScale(XMFLOAT3 _scale)
{
	scale = _scale;
	updated = true;
	transformed = true;
	//world = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
}

void GameObject::setScale(float _scale)
{
	scale.x = _scale;
	scale.y = _scale;
	scale.z = _scale;
	updated = true;
	transformed = true;
	//world = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
}

void GameObject::setScale(float _x, float _y, float _z)
{
	scale.x = _x;
	scale.y = _y;
	scale.z = _z;
	updated = true;
	transformed = true;
	//world = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
}


//ALEX OWEN - 26/01/15 - set the position with a vector
void GameObject::setPositionVec(FXMVECTOR _position)
{
	position.x = XMVectorGetX(_position);
	position.y = XMVectorGetY(_position);
	position.z = XMVectorGetZ(_position);
	updated = true;
	transformed = true;

}


//ALEX OWEN - 26/01/15 - set the position with a vector
void GameObject::setRotationVec(FXMVECTOR _rotation)
{
	rotation.x = XMVectorGetX(_rotation);
	rotation.y = XMVectorGetY(_rotation);
	rotation.z = XMVectorGetZ(_rotation);
	updated = true;
	transformed = true;

}




float GameObject::getX() const
{
	return position.x;
}

float GameObject::getY() const
{
	return position.y;
}

float GameObject::getZ() const
{
	return position.z;
}

float GameObject::getRX() const
{
	return rotation.x;
}

float GameObject::getRY() const
{
	return rotation.y;
}

float GameObject::getRZ() const
{
	return rotation.z;
}

float GameObject::getSX() const
{
	return scale.x;
}

float GameObject::getSY() const
{
	return scale.y;
}

float GameObject::getSZ() const
{
	return scale.z;
}

//ALEX OWEN - 16/02/15 - returns position as an XMFLOAT3
XMFLOAT3 GameObject::getPosition() const
{
	return position;
}

//ALEX OWEN - 26/01/15 - return position as vector
XMVECTOR GameObject::getPositionVec() const
{
	return XMVectorSet(position.x, position.y, position.z, 1);
}


//ALEX OWEN - 26/01/15 - return rotation as vector
XMVECTOR GameObject::getRotationVec() const
{
	return XMVectorSet(rotation.x, rotation.y, rotation.z, 1);
}

//ALEX OWEN 30/03/15 access forward Vector
XMVECTOR GameObject::getForwardVector()
{	
	if(updated) updateVectors();
	XMVECTOR _forwardVector = XMVectorSet(forwardVector.x, forwardVector.y, forwardVector.z, 0);
	return _forwardVector;
}

//ALEX OWEN 30/03/15 access right Vector
XMVECTOR GameObject::getRightVector()
{
	if(updated) updateVectors();
	XMVECTOR _rightVector = XMVectorSet(rightVector.x, rightVector.y, rightVector.z, 0);
	return _rightVector;
}

//ALEX OWEN 30/03/15 access up Vector
XMVECTOR GameObject::getUpVector()
{
	if(updated) updateVectors();
	XMVECTOR _upVector = XMVectorSet(upVector.x, upVector.y, upVector.z, 0);
	return _upVector;
}

//ALEX OWEN - 30/03/15
XMMATRIX GameObject::getPositionMatrix() const
{
	XMMATRIX translationMatrix = XMMatrixTranslation(position.x, position.y, position.z);
	return translationMatrix;
}

//ALEX OWEN - 30/03/15
XMMATRIX GameObject::getRotationMatrix() const
{
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	
	return rotationMatrix;
}

//ALEX OWEN 10/04/15
XMMATRIX GameObject::getWorldMatrix()
{
	if(!transformed) return XMLoadFloat4x4(&worldMatrix);
	// create matrices to create a single world matrix for the GameObject's transform
	XMMATRIX scaleMatrix = XMMatrixScaling( scale.x, scale.y, scale.z );
	XMMATRIX rotationMatrix = getRotationMatrix();
	XMMATRIX positionMatrix = getPositionMatrix();
	
	// 1) scale 
	// 2) rotate 
	// 3) position
	
	XMMATRIX _worldMatrix = XMMatrixTranspose( scaleMatrix * rotationMatrix * positionMatrix );
	XMStoreFloat4x4(&worldMatrix,_worldMatrix);
	transformed = false;
	return _worldMatrix;
}

// ZS-->04/02/2015 DisplayObject setter
void GameObject::SetDisplayObject( DisplayObject * display_object )
{
	m_display_object = display_object;
}

// ZS-->04/02/2015 DisplayObject getter
DisplayObject * GameObject::GetDisplayObject()
{
	return m_display_object;
}

//ALEX OWEN - 30/03/15 
void GameObject::updateVectors()
{
	XMMATRIX rotationMatrix = getRotationMatrix();
	FXMVECTOR _forwardVector = XMVector3Transform(FORWARD,rotationMatrix);
	FXMVECTOR _rightVector = XMVector3Transform(RIGHT,rotationMatrix);
	FXMVECTOR _upVector = XMVector3Transform(UP,rotationMatrix);

	XMFLOAT3* _temp = &forwardVector;
	XMStoreFloat3(_temp,_forwardVector);
	_temp = &rightVector;
	XMStoreFloat3(_temp,_rightVector);
	_temp = &upVector;
	XMStoreFloat3(_temp,_upVector);
	updated = false;

	//forwardVector = XMStoreFloat4(_forwardVector)
	//forwardVector.x = XMVectorGetX(
}

void GameObject::updateGridPosition()
{
	// CAC 16/03/15 //
	int a = (int)position.x/100;
	int b = (int)position.z/100;

	//levelArray[a][b] = 1;
}

// ZS-->16/03/2015 added Draw function
//ALEX OWEN - 10/04/15 - draw function now passes a world matrix instead of individual values. That
// way the gameobject can decide whether the world matrix needs to be recalculated or not
bool GameObject::Draw( ID3D11DeviceContext* device_context, ID3D11Buffer* world_const_buffer )
{
	if (m_display_object)
	{
		return m_display_object->Draw( device_context, world_const_buffer, getWorldMatrix() );
	}

	return false;
}



