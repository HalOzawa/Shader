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
		XMMATRIX    matW; //ワールド変換
		XMMATRIX	matWVP; //スクリーン変換
		XMMATRIX    matNormal; //法線ワールド変換用
		XMFLOAT4	diffuseColor; //RGBの拡散反射係数
		XMFLOAT4    lightPosition;
		XMFLOAT2    diffuseFactor;
		int		    isTextured;
	};
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal; //ノーマル追加(法線ベクトル)
	};

	ID3D11Buffer* pVertexBuffer_; //頂点バッファ用のメモリ
	ID3D11Buffer** pIndexBuffer_; //インデックスバッファ用のメモリ
	ID3D11Buffer* pConstantBuffer_; //コンスタントバッファ用のメモリ
	vector<MATERIAL> pMaterialList_; //マテリアルの配列
	vector<int> indexCount_;

	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_; //マテリアルの個数

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

