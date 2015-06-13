////////////////////////////////////////////////////////////////////////////////
// Filename: Shader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SHADER_H_
#define _SHADER_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11async.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: Shader ( virtual base class )
////////////////////////////////////////////////////////////////////////////////
class Shader
{
public:
	Shader(void);
	virtual ~Shader(void);

	virtual bool Init(ID3D11Device* device) = 0;
	virtual void Release();

	virtual void Render(ID3D11DeviceContext* deviceContext);

protected:
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, char* shaderFilename);

protected:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampler;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: Shader2D
////////////////////////////////////////////////////////////////////////////////
class Shader2D : public Shader
{
public:
	Shader2D(void);
	virtual ~Shader2D(void);

	bool Init(ID3D11Device* device );
};

////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderVTN
////////////////////////////////////////////////////////////////////////////////
class ShaderVTN : public Shader
{
public:
	ShaderVTN(void);
	virtual ~ShaderVTN(void);

	bool Init(ID3D11Device* device );
};

////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderNMAP
////////////////////////////////////////////////////////////////////////////////
class ShaderNMAP : public Shader
{
public:
	ShaderNMAP(void);
	virtual ~ShaderNMAP(void);

	bool Init(ID3D11Device* device );
};

#endif

