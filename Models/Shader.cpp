////////////////////////////////////////////////////////////////////////////////
// Filename: Shader.h
////////////////////////////////////////////////////////////////////////////////
#include "Shader.h"
#include <fstream>
#include <DxErr.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: Shader ( virtual base class )
////////////////////////////////////////////////////////////////////////////////
Shader::Shader(void)
{
	m_vertexShader = NULL;
	m_pixelShader = NULL;
	m_layout = NULL;
	m_sampler = NULL;
}

Shader::~Shader(void)
{
}

void Shader::Release(void)
{
	// Release the layout.
	if(m_layout)
	{
		m_layout->Release();
		m_layout = NULL;
	}

	// Release the pixel shader.
	if(m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = NULL;
	}

	// Release the vertex shader.
	if(m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = NULL;
	}

	// Release the sampler.
	if(m_sampler)
	{
		m_sampler->Release();
		m_sampler = NULL;
	}
}

void Shader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, char* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Log a message to notify the user to check the text file for compile errors.
	DXTRACE_MSG( "Error compiling shader.  Check shader-error.txt for message." );

	return;
}

void Shader::Render(ID3D11DeviceContext* deviceContext)
{
	// activate the input layout on the graphics card
	deviceContext->IASetInputLayout( m_layout );

	// activate the vertex shader on the graphics card
	deviceContext->VSSetShader( m_vertexShader, 0, 0 );
	
	// activate the pixel shader on the graphics card
	deviceContext->PSSetShader( m_pixelShader, 0, 0 );

	// activate the sampler on the graphics card
	deviceContext->PSSetSamplers( 0, 1, &m_sampler );
}

////////////////////////////////////////////////////////////////////////////////
// Class name: Shader2D
////////////////////////////////////////////////////////////////////////////////
Shader2D::Shader2D(void) : Shader()
{
}

Shader2D::~Shader2D(void)
{
}

bool Shader2D::Init(ID3D11Device* device)
{
	HRESULT result;
	ID3D10Blob* errorMessage = NULL;

	///////////////////
	// VERTEX SHADER //
	///////////////////

	ID3D10Blob* vertexShaderBuffer = NULL;

	// Compile the vertex shader code.
	result = D3DX11CompileFromFile(
		"TextTextureMap.fx",			// filepath
		NULL,							// macro
		NULL,							// includes
		"VS_Main",						// function name
		"vs_4_0",						// shader profile
		D3D10_SHADER_ENABLE_STRICTNESS, // flags 1
		0,								// flags 2
		NULL,							// thread pump
		&vertexShaderBuffer,			// shader buffer
		&errorMessage,					// error blob
		NULL							// hresult
	);

	if(FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if(errorMessage) OutputShaderErrorMessage(errorMessage, "TextTextureMap.fx");

		// If there was nothing in the error message then it simply could not find the shader file itself.
		else DXTRACE_MSG( "Missing Shader File 'TextTextureMap.fx'" );

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(
		vertexShaderBuffer->GetBufferPointer(), 
		vertexShaderBuffer->GetBufferSize(), 
		NULL, 
		&m_vertexShader
	);

	if(FAILED(result))
	{
		// There was an error creating the vertex shader
		 DXTRACE_MSG( "Error creating the vertex shader" );

		// Release the vertex shader buffer if it exists already
		if( vertexShaderBuffer ) vertexShaderBuffer->Release( );

		return false;
	}

	// Create a D3D11_INPUT_ELEMENT_DESC for the vertex layout description
	D3D11_INPUT_ELEMENT_DESC polygonLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    unsigned int numElements = ARRAYSIZE( polygonLayout );

	// Create the vertex layout description
    result = device->CreateInputLayout( 
		polygonLayout, 
		numElements, 
		vertexShaderBuffer->GetBufferPointer( ), 
		vertexShaderBuffer->GetBufferSize( ), 
		&m_layout 
	);

    vertexShaderBuffer->Release( );
	vertexShaderBuffer = NULL;

    if( FAILED( result ) )
    {
        // There was an error creating the vertex input layout
		DXTRACE_MSG( "Error creating the vertex input layout" );

        return false;
    }

	//////////////////
	// PIXEL SHADER //
	//////////////////

	ID3D10Blob* pixelShaderBuffer = NULL;

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(
		"TextTextureMap.fx",			// filepath
		NULL,							// macro
		NULL,							// includes
		"PS_Main",						// function name
		"ps_4_0",						// shader profile
		D3D10_SHADER_ENABLE_STRICTNESS, // flags 1
		0,								// flags 2
		NULL,							// thread pump
		&pixelShaderBuffer,				// shader buffer
		&errorMessage,					// error blob
		NULL							// hresult
	);

	if(FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if(errorMessage) OutputShaderErrorMessage(errorMessage, "TextTextureMap.fx");

		// If there was nothing in the error message then it simply could not find the shader file itself.
		else DXTRACE_MSG( "Missing Shader File 'TextTextureMap.fx'" );

		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(), 
		pixelShaderBuffer->GetBufferSize(), 
		NULL, 
		&m_pixelShader
	);

	pixelShaderBuffer->Release();
	pixelShaderBuffer = NULL;

	if(FAILED(result))
	{
		// There was an error creating the pixel shader
		DXTRACE_MSG( "Error creating the pixel shader" );

		return false;
	}

	/////////////
	// SAMPLER //
	/////////////

	// Create a sampler description
	D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the sampler
    result = device->CreateSamplerState( &samplerDesc, &m_sampler );

    if( FAILED( result ) )
    {
        // There was an error creating the pixel shader
		DXTRACE_MSG( "Error creating sampler state" );

        return false;
    }

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderVTN
////////////////////////////////////////////////////////////////////////////////
ShaderVTN::ShaderVTN(void) : Shader()
{
}

ShaderVTN::~ShaderVTN(void)
{
}

bool ShaderVTN::Init(ID3D11Device* device)
{
	HRESULT result;
	ID3D10Blob* errorMessage = NULL;

	///////////////////
	// VERTEX SHADER //
	///////////////////

	ID3D10Blob* vertexShaderBuffer = NULL;

	// Compile the vertex shader code.
	result = D3DX11CompileFromFile(
		"TextureMap.fx",				// filepath
		NULL,							// macro
		NULL,							// includes
		"VS_Main",						// function name
		"vs_4_0",						// shader profile
		D3D10_SHADER_ENABLE_STRICTNESS, // flags 1
		0,								// flags 2
		NULL,							// thread pump
		&vertexShaderBuffer,			// shader buffer
		&errorMessage,					// error blob
		NULL							// hresult
	);

	if(FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if(errorMessage) OutputShaderErrorMessage(errorMessage, "TextureMap.fx");

		// If there was nothing in the error message then it simply could not find the shader file itself.
		else DXTRACE_MSG( "Missing Shader File 'TextureMap.fx'" );

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(
		vertexShaderBuffer->GetBufferPointer(), 
		vertexShaderBuffer->GetBufferSize(), 
		NULL, 
		&m_vertexShader
	);

	if(FAILED(result))
	{
		// There was an error creating the vertex shader
		DXTRACE_MSG( "Error creating the vertex shader" );

		// Release the vertex shader buffer if it exists already
		if( vertexShaderBuffer ) vertexShaderBuffer->Release( );

		return false;
	}

	// Create a D3D11_INPUT_ELEMENT_DESC for the vertex layout description
	D3D11_INPUT_ELEMENT_DESC polygonLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    unsigned int numElements = ARRAYSIZE( polygonLayout );

	// Create the vertex layout description
    result = device->CreateInputLayout( 
		polygonLayout, 
		numElements, 
		vertexShaderBuffer->GetBufferPointer( ), 
		vertexShaderBuffer->GetBufferSize( ), 
		&m_layout 
	);

    vertexShaderBuffer->Release( );
	vertexShaderBuffer = NULL;

    if( FAILED( result ) )
    {
        // There was an error creating the vertex input layout
		DXTRACE_MSG( "Error creating the vertex input layout" );

        return false;
    }

	//////////////////
	// PIXEL SHADER //
	//////////////////

	ID3D10Blob* pixelShaderBuffer = NULL;

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(
		"TextureMap.fx",				// filepath
		NULL,							// macro
		NULL,							// includes
		"PS_Main",						// function name
		"ps_4_0",						// shader profile
		D3D10_SHADER_ENABLE_STRICTNESS, // flags 1
		0,								// flags 2
		NULL,							// thread pump
		&pixelShaderBuffer,				// shader buffer
		&errorMessage,					// error blob
		NULL							// hresult
	);

	if(FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if(errorMessage) OutputShaderErrorMessage(errorMessage, "TextureMap.fx");

		// If there was nothing in the error message then it simply could not find the shader file itself.
		else DXTRACE_MSG( "Missing Shader File 'TextureMap.fx'" );

		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(), 
		pixelShaderBuffer->GetBufferSize(), 
		NULL, 
		&m_pixelShader
	);

	pixelShaderBuffer->Release();
	pixelShaderBuffer = NULL;

	if(FAILED(result))
	{
		// There was an error creating the pixel shader
		DXTRACE_MSG( "Error creating the pixel shader" );

		return false;
	}

	/////////////
	// SAMPLER //
	/////////////

	// Create a sampler description
	D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the sampler
    result = device->CreateSamplerState( &samplerDesc, &m_sampler );

    if( FAILED( result ) )
    {
        // There was an error creating the pixel shader
		DXTRACE_MSG( "Error creating sampler state" );

        return false;
    }

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderNMAP
////////////////////////////////////////////////////////////////////////////////
ShaderNMAP::ShaderNMAP(void) : Shader()
{
}

ShaderNMAP::~ShaderNMAP(void)
{
}

bool ShaderNMAP::Init(ID3D11Device* device)
{
	HRESULT result;
	ID3D10Blob* errorMessage = NULL;

	///////////////////
	// VERTEX SHADER //
	///////////////////

	ID3D10Blob* vertexShaderBuffer = NULL;

	// Compile the vertex shader code.
	result = D3DX11CompileFromFile(
		"NormalMap.fx",					// filepath
		NULL,							// macro
		NULL,							// includes
		"VS_Main",						// function name
		"vs_4_0",						// shader profile
		D3D10_SHADER_ENABLE_STRICTNESS, // flags 1
		0,								// flags 2
		NULL,							// thread pump
		&vertexShaderBuffer,			// shader buffer
		&errorMessage,					// error blob
		NULL							// hresult
	);

	if(FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if(errorMessage) OutputShaderErrorMessage(errorMessage, "NormalMap.fx");

		// If there was nothing in the error message then it simply could not find the shader file itself.
		else DXTRACE_MSG( "Missing Shader File 'NormalMap.fx'" );

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(
		vertexShaderBuffer->GetBufferPointer(), 
		vertexShaderBuffer->GetBufferSize(), 
		NULL, 
		&m_vertexShader
	);

	if(FAILED(result))
	{
		// There was an error creating the vertex shader
		DXTRACE_MSG( "Error creating the vertex shader" );

		// Release the vertex shader buffer if it exists already
		if( vertexShaderBuffer ) vertexShaderBuffer->Release( );

		return false;
	}

	// Create a D3D11_INPUT_ELEMENT_DESC for the vertex layout description
	D3D11_INPUT_ELEMENT_DESC polygonLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    unsigned int numElements = ARRAYSIZE( polygonLayout );

	// Create the vertex layout description
    result = device->CreateInputLayout( 
		polygonLayout, 
		numElements, 
		vertexShaderBuffer->GetBufferPointer( ), 
		vertexShaderBuffer->GetBufferSize( ), 
		&m_layout 
	);

    vertexShaderBuffer->Release( );
	vertexShaderBuffer = NULL;

    if( FAILED( result ) )
    {
        // There was an error creating the vertex input layout
		DXTRACE_MSG( "Error creating the vertex input layout" );

        return false;
    }

	//////////////////
	// PIXEL SHADER //
	//////////////////

	ID3D10Blob* pixelShaderBuffer = NULL;

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(
		"NormalMap.fx",					// filepath
		NULL,							// macro
		NULL,							// includes
		"PS_Main",						// function name
		"ps_4_0",						// shader profile
		D3D10_SHADER_ENABLE_STRICTNESS, // flags 1
		0,								// flags 2
		NULL,							// thread pump
		&pixelShaderBuffer,				// shader buffer
		&errorMessage,					// error blob
		NULL							// hresult
	);

	if(FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if(errorMessage) OutputShaderErrorMessage(errorMessage, "NormalMap.fx");

		// If there was nothing in the error message then it simply could not find the shader file itself.
		else DXTRACE_MSG( "Missing Shader File 'NormalMap.fx'" );

		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(), 
		pixelShaderBuffer->GetBufferSize(), 
		NULL, 
		&m_pixelShader
	);

	pixelShaderBuffer->Release();
	pixelShaderBuffer = NULL;

	if(FAILED(result))
	{
		// There was an error creating the pixel shader
		DXTRACE_MSG( "Error creating the pixel shader" );

		return false;
	}

	/////////////
	// SAMPLER //
	/////////////

	// Create a sampler description
	D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the sampler
    result = device->CreateSamplerState( &samplerDesc, &m_sampler );

    if( FAILED( result ) )
    {
        // There was an error creating the pixel shader
		DXTRACE_MSG( "Error creating sampler state" );

        return false;
    }

	return true;
}