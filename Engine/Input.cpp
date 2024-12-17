#include "Input.h"

namespace Input
{
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;
	BYTE keyState[256] = { 0 }; //現在の各キーの状態
	BYTE prevKeyState[256];    //前フレームでの各キーの状態

	LPDIRECTINPUTDEVICE8 pMosuDevice = nullptr;
	DIMOUSESTATE mouseState_;
	DIMOUSESTATE prevMouseState_;

	void Initialize(HWND hWnd)
	{
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);

		pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		pDInput->CreateDevice(GUID_SysMouse, &pMosuDevice, nullptr);
		pMosuDevice->SetDataFormat(&c_dfDIKeyboard);
		pMosuDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	}

	void Update()
	{
		//キーボード
		memcpy(prevKeyState, keyState, sizeof(prevKeyState)); //コピー
		pKeyDevice->Acquire();
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);
		//マウス
		memcpy(&prevMouseState_, &mouseState_, sizeof(prevMouseState_));
		pMosuDevice->Acquire();
		pMosuDevice->GetDeviceState(sizeof(mouseState_), &mouseState_);
	}

	bool IsKey(int keyCode)
	{
		if (keyState[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		//今は押してて、前回は押してない
		if (keyState[keyCode] == keyCode && prevKeyState[keyCode] != keyCode)
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		//前回は押してて、今は押してない
		if (prevKeyState[keyCode] == keyCode && keyState[keyCode] != keyCode)
		{
			return true;
		}
		return false;
	}

	bool IsMouseButton(int buttonCode)
	{
		if (mouseState_.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	//マウスのボタンを今押したか調べる（押しっぱなしは無効）
	bool IsMouseButtonDown(int buttonCode)
	{
		//今は押してて、前回は押してない
		if (IsMouseButton(buttonCode) && !(prevMouseState_.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	//マウスのボタンを今放したか調べる
	bool IsMouseButtonUp(int buttonCode)
	{
		//今押してなくて、前回は押してる
		if (!IsMouseButton(buttonCode) && !(prevMouseState_.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	//マウスカーソルの位置を取得
	XMVECTOR GetMousePosition()
	{
		POINT mousePos;
		GetCursorPos(&mousePos);
		XMVECTOR result = XMVectorSet((float)mousePos.x, (float)mousePos.y, 0, 0);
		return result;
	}

	//そのフレームでのマウスの移動量を取得
	XMVECTOR GetMouseMove()
	{
		XMVECTOR result = XMVectorSet((float)mouseState_.lX, (float)mouseState_.lY, (float)mouseState_.lZ, 0);
		return result;
	}

	void SetMousePosition(int x, int y)
	{
	}

	void Release()
	{
		SAFE_RELEASE(pDInput);
		SAFE_RELEASE(pKeyDevice);
		SAFE_RELEASE(pMosuDevice);
	}
}
