////////////////////////////////////////////////////////////////////////////////
// Filename: Model.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODEL_H_
#define _MODEL_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <xnamath.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: Model ( virtual base class )
////////////////////////////////////////////////////////////////////////////////
class Model
{
public:
	Model(void);
	virtual ~Model(void);

	virtual bool Init(ID3D11Device* device, char* obj_file_name = NULL) = 0;
	virtual void Release(void);

	ID3D11Buffer* const* GetVertexBuffer();
	int GetVertexCount(void);

	virtual void Render(ID3D11DeviceContext* deviceContext) = 0;

protected:
	ID3D11Buffer *m_vertexBuffer;
	int m_vertexCount;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: Model2D
////////////////////////////////////////////////////////////////////////////////
class Model2D : public Model
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	Model2D(void);
	~Model2D(void);

	bool Init(ID3D11Device* device, char* obj_file_name = NULL);
	void Release(void);

	void Render(ID3D11DeviceContext* deviceContext);
};

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelVTN
////////////////////////////////////////////////////////////////////////////////
class ModelVTN : public Model
{
private:

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

public:
	ModelVTN(void);
	~ModelVTN(void);

	bool Init(ID3D11Device* device, char* obj_file_name = NULL);
	void Release(void);

	void Render(ID3D11DeviceContext* deviceContext);
};

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelNMAP
////////////////////////////////////////////////////////////////////////////////
class ModelNMAP : public Model
{
private:

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 bitangent;
	};

public:
	ModelNMAP(void);
	~ModelNMAP(void);

	bool Init(ID3D11Device* device, char* obj_file_name = NULL);
	void Release(void);

	void Render(ID3D11DeviceContext* deviceContext);
};

#endif

