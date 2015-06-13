////////////////////////////////////////////////////////////////////////////////
// Filename: displayobject.cpp
////////////////////////////////////////////////////////////////////////////////
#include "DisplayObject.h"

DisplayObject::DisplayObject(void)
{
	m_model = NULL;
	m_texture = NULL;
	m_shader = NULL;
	m_normal_map = NULL;
}

DisplayObject::~DisplayObject(void)
{
}

bool DisplayObject::Init(Model* model, Texture* texture, Shader* shader, Texture* normal_map)
{
	m_model = model;
	m_texture = texture;
	m_shader = shader;
	m_normal_map = normal_map;

	return true;
}

Model* DisplayObject::GetModel() { return m_model; }
void DisplayObject::SetModel( Model* model_class ) { m_model = model_class; }
int DisplayObject::GetVertexCount() { return m_model->GetVertexCount(); }

Texture* DisplayObject::GetTexture() { return m_texture; }
void DisplayObject::SetTexture( Texture* texture_class ) { m_texture = texture_class; }

Shader* DisplayObject::GetShader() { return m_shader; }
void DisplayObject::SetShader( Shader* shader ) { m_shader = shader; }

bool DisplayObject::Draw(ID3D11DeviceContext* device_context, ID3D11Buffer* world_const_buffer, const XMMATRIX &worldMatrix )
{
	// activate the GameObject's shader on the graphics card
	m_shader->Render( device_context );

	// activate the GameObject's vertex buffer on the graphics card
	m_model->Render( device_context );

	// activate the GameObject's texture buffer on the graphics card
	m_texture->Render( device_context );

	// if there is a normal map texture then we assume we are using the NMAP shader
	if ( m_normal_map ) m_normal_map->RenderNMAP( device_context );

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//ALEX OWEN - 10/04/15 matrices now calculated in GameObject
	/*
	// create matrices to create a single world matrix for the GameObject's transform
	XMMATRIX scale_mat = XMMatrixScaling( scale.x, scale.y, scale.z );
	XMMATRIX rotation_mat = XMMatrixRotationRollPitchYaw( rot.x, rot.y, rot.z );
	XMMATRIX position_mat = XMMatrixTranslation( pos.x, pos.y, pos.z );
	
	// 1) scale 
	// 2) rotate 
	// 3) position
	XMMATRIX world_mat = XMMatrixTranspose( scale_mat * rotation_mat * position_mat );
	*/

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	//ALEX OWEN 10/04/15 
	// update the world matrix on the graphics card
	device_context->UpdateSubresource( world_const_buffer, 0, 0, &worldMatrix, 0, 0 );

	// set the world matrix on the vertex shader
	device_context->VSSetConstantBuffers( 0, 1, &world_const_buffer );

	// draw the number of vertices
	device_context->Draw( m_model->GetVertexCount(), 0 );

	return true;
}

bool DisplayObject::DrawWeapon(	ID3D11DeviceContext* device_context, 
								ID3D11Buffer* world_const_buffer,
								FirstPersonCamera * camera,
								const XMFLOAT3 &pos,
								const XMFLOAT3 &rot,
								const XMFLOAT3 &scale )
{
	// activate the GameObject's shader on the graphics card
	m_shader->Render( device_context );

	// activate the GameObject's vertex buffer on the graphics card
	m_model->Render( device_context );

	// activate the GameObject's texture buffer on the graphics card
	m_texture->Render( device_context );

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	// we need a determinant to inverse the camera view matrix
	XMVECTOR determinant;

	// inverse the view matrix
    XMMATRIX invertView = XMMatrixInverse(&determinant, camera->GetViewMatrix());

	// create matrices to create a single world matrix for the object's transform
	XMMATRIX scale_mat = XMMatrixScaling( scale.x, scale.y, scale.z );
	XMMATRIX rotation_mat = XMMatrixRotationRollPitchYaw( rot.x, rot.y, rot.z );
	XMMATRIX position_mat = XMMatrixTranslation( pos.x, pos.y, pos.z );
	
	//world matrix for the gun
	XMMATRIX world_mat = XMMatrixTranspose( scale_mat * rotation_mat * position_mat * invertView );
	//XMMATRIX world_mat = XMMatrixTranspose(position_mat * invertView );

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	// update the world matrix on the graphics card
	device_context->UpdateSubresource( world_const_buffer, 0, 0, &world_mat, 0, 0 );

	// set the world matrix on the vertex shader
	device_context->VSSetConstantBuffers( 0, 1, &world_const_buffer );

	// draw the number of vertices
	device_context->Draw( m_model->GetVertexCount(), 0 );

	return true;
} 