#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

enum SHADER_TYPE
{
	SHADER_2D,
	SHADER_3D,
	SHADER_POINT,
	SHADER_MAX
};

namespace Direct3D
{
	extern ID3D11Device* pDevice;
	extern ID3D11DeviceContext* pContext;

	//初期化
	HRESULT Initialize(int winW, int winH, HWND hWnd);
	//シェーダー準備
	HRESULT InitShader();
	HRESULT InitShader3D(); //simple3D.hlslのための初期化
	HRESULT InitShader2D(); //simple2D.hlslのための初期化
	HRESULT InitPLShader();

	void SetShader(SHADER_TYPE type);

	//描画開始
	void BeginDraw();
	//描画終了
	void EndDraw();
	//解放
	void Release();

	XMFLOAT4 GetLightPos();
	void SetLightPos(XMFLOAT4 pos);

};

