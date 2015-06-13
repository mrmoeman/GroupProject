////////////////////////////////////////////////////////////////////////////////
// Filename: displayobject.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _DISPLAYOBJECT_H_
#define _DISPLAYOBJECT_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Texture.h"
#include "Model.h"
#include "Shader.h"
#include "FirstPersonCamera.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: DisplayObject
////////////////////////////////////////////////////////////////////////////////
class DisplayObject
{
public:
	DisplayObject(void);
	~DisplayObject(void);

	bool Init(Model* model = NULL, Texture* texture = NULL, Shader* shader = NULL, Texture* normal_map = NULL );

	Model* GetModel();
	void SetModel( Model* model_class );

	int GetVertexCount();

	Texture* GetTexture();
	void SetTexture( Texture* texture_class );

	Shader* GetShader();
	void SetShader( Shader* shader );

	// ALEX OWEN 20/04/15 - function now accepts an XMMATRIX as a parameter instead of
	// floats as gameobject is now calculating the world matrix
	bool Draw(	ID3D11DeviceContext* device_context, 
				ID3D11Buffer* world_const_buffer, 
				const XMMATRIX &worldMatrix );

	bool DrawWeapon(	ID3D11DeviceContext* device_context, 
						ID3D11Buffer* world_const_buffer,
						FirstPersonCamera * camera,
						const XMFLOAT3 &pos,
						const XMFLOAT3 &rot,
						const XMFLOAT3 &scale );

private:
	Model* m_model;
	Texture* m_texture;
	Shader* m_shader;
	Texture* m_normal_map;
};

#endif

