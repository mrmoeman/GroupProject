////////////////////////////////////////////////////////////////////////////////
// Filename: Texture.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Texture.h"


Texture::Texture(void)
{
	m_texture = NULL;
}

Texture::~Texture(void)
{
}

bool Texture::Init(ID3D11Device* device, char* filename)
{
	HRESULT result;

	// Load the texture in.
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void Texture::Release()
{
	// Release the texture resource.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = NULL;
	}

	return;
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_texture;
}

void Texture::SetTexture(ID3D11ShaderResourceView* texture)
{
	m_texture = texture;
}

void Texture::Render(ID3D11DeviceContext* deviceContext)
{
	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &m_texture);
}

void Texture::RenderNMAP(ID3D11DeviceContext* deviceContext)
{
	// Set shader texture resource in the pixel shader for the normal map.
	deviceContext->PSSetShaderResources(1, 1, &m_texture);
}
