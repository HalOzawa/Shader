#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include <vector>
#include "Texture.h"
#include "Transform.h"

using namespace DirectX;

//�R���X�^���g�o�b�t�@�[:�A�v��������A�V�F�[�_�[�ɖ��t���[�������������
struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
	XMMATRIX    matW;
};

//���_���
struct VERTEX
{
	XMVECTOR position;
	XMVECTOR uv;
	XMVECTOR normal; //�m�[�}���ǉ�(�@���x�N�g��)
};

class Quad
{
protected:
	int vertexNum_;
	std::vector<VERTEX> vertices_;
	int indexNum_;
	std::vector<int> index_;

	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;    //�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@

	Texture* pTexture_;
public:
	Quad();
	~Quad();
	HRESULT Initialize();
	void Draw(Transform& transform);
	void Release();
private:
	virtual void InitVertexData();
	HRESULT CreateVertexBuffer();
	virtual void InitIndexData();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();
	HRESULT LoadTexture();

	void PassDataToCB(Transform& transform);
	void SetBufferToPipeline();
};

