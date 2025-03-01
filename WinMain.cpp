//インクルード
#include <Windows.h>
#include <stdlib.h>
#include "Engine/Direct3D.h"
#include "Engine/Camera.h"
#include "Engine/Transform.h"
#include "Engine/RootJob.h"

using namespace Direct3D;

#pragma comment(lib, "winmm.lib")

//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//定数宣言
const wchar_t* WIN_CLASS_NAME = L"SampleGame";  //ウィンドウクラス名
const wchar_t* APP_NAME = L"サンプルゲーム"; //アプリケーション名
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

RootJob* pRootJob = nullptr;

//エントリーポイント
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	//ウィンドウクラス（設計図）を作成
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);             //この構造体のサイズ
	wc.hInstance = hInstance;                   //インスタンスハンドル
	wc.lpszClassName = WIN_CLASS_NAME;            //ウィンドウクラス名
	wc.lpfnWndProc = WndProc;                   //ウィンドウプロシージャ
	wc.style = CS_VREDRAW | CS_HREDRAW;         //スタイル（デフォルト）
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //アイコン
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);   //小さいアイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //マウスカーソル
	wc.lpszMenuName = NULL;                     //メニュー（なし）
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //背景（白）
	RegisterClassEx(&wc); //クラスを登録

	//ウィンドウサイズの計算
	//(表示領域をWINDOW_WIDTHxWINDOW_HEIGHTに指定するための計算)
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	int winW = winRect.right - winRect.left;     //ウィンドウ幅
	int winH = winRect.bottom - winRect.top;     //ウィンドウ高さ

	//ウィンドウを作成
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,         //ウィンドウクラス名
		APP_NAME,     //タイトルバーに表示する内容
		WS_OVERLAPPEDWINDOW, //スタイル（普通のウィンドウ）
		CW_USEDEFAULT,       //表示位置左（おまかせ）
		CW_USEDEFAULT,       //表示位置上（おまかせ）
		winW,                 //ウィンドウ幅
		winH,                 //ウィンドウ高さ
		NULL,                //親ウインドウ（なし）
		NULL,                //メニュー（なし）
		hInstance,           //インスタンス
		NULL                 //パラメータ（なし）
	);

	//ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);

	//Direct3D初期化
	HRESULT hr = Direct3D::Initialize(winW, winH, hWnd);
	if (FAILED(hr)) {
		return 0;
	}

	Camera::Initialize();

	pRootJob = new RootJob(nullptr);
	pRootJob->Initialize();

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Quadの初期化に失敗", NULL, MB_OK);
		return 0;
	}

	//メッセージループ（何か起きるのを待つ）
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//メッセージあり
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//メッセージなし
		else
		{ //このカッコ内が1フレーム
			static DWORD startTime = timeGetTime();
			DWORD nowTime = timeGetTime();
			static DWORD lastUpdateTime = nowTime;
			static DWORD countFps = 0;
			//timeGetTime() Windowsが起動してからの時間をミリ秒
			//char str[16];
			if (nowTime - startTime >= 1000) {
				countFps = 0;
				startTime = nowTime;
			}
			std::wstring str;
			//wsprintf(str.data(), L"%u", nowTime - startTime);
			countFps++;

			wsprintf(str.data(), L"%u", countFps);
			SetWindowText(hWnd, str.c_str());

			//カメラを更新
			Camera::Update();
			//ルートジョブからつながるすべてのオブジェクトをUpdateする
			pRootJob->UpdateSub();

			//ゲームの処理
			Direct3D::BeginDraw();
			//ルートジョブからつながるすべてのオブジェクトをDrawする
			pRootJob->DrawSub();

			////描画処理
			Direct3D::EndDraw();
		}
	}
	pRootJob->ReleaseSub();
	Direct3D::Release();
	return 0;
}

//ウィンドウプロシージャ（何かあった時に呼ばれる関数）
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY: //ウィンドウが閉じられたイベント
		PostQuitMessage(0);  //プログラム終了
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
