#pragma once
#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#include <vector>

#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

using std::vector;

class Texture;

class FBX
{
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse;
		XMFLOAT4 specular;
		XMFLOAT4 shininess;
		XMFLOAT4 ambient;
		XMFLOAT4 factor;
	};

	struct CONSTANT_BUFFER
	{
		XMMATRIX    matW; //���[���h�ϊ�
		XMMATRIX	matWVP; //�X�N���[���ϊ�
		XMMATRIX    matNormal; //�@�����[���h�ϊ��p
		XMFLOAT4	diffuseColor; //RGB�̊g�U���ˌW��
		XMFLOAT4    lightPosition;
		XMFLOAT2    diffuseFactor;
		int		    isTextured;
	};
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal; //�m�[�}���ǉ�(�@���x�N�g��)
	};

	ID3D11Buffer* pVertexBuffer_; //���_�o�b�t�@�p�̃�����
	ID3D11Buffer** pIndexBuffer_; //�C���f�b�N�X�o�b�t�@�p�̃�����
	ID3D11Buffer* pConstantBuffer_; //�R���X�^���g�o�b�t�@�p�̃�����
	vector<MATERIAL> pMaterialList_; //�}�e���A���̔z��
	vector<int> indexCount_;

	int vertexCount_;	//���_��
	int polygonCount_;	//�|���S����
	int materialCount_; //�}�e���A���̌�

	void InitVertex(fbxsdk::FbxMesh* mesh);
	void InitIndex(fbxsdk::FbxMesh* mesh);
	void IntConstantBuffer();
	void InitMaterial(fbxsdk::FbxNode* pNode);
public:
	FBX();
	HRESULT Load(std::string fileName);
	void    Draw(Transform& transform);
	void    Release();
};

