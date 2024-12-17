#include "Input.h"

namespace Input
{
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;
	BYTE keyState[256] = { 0 }; //���݂̊e�L�[�̏��
	BYTE prevKeyState[256];    //�O�t���[���ł̊e�L�[�̏��

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
		//�L�[�{�[�h
		memcpy(prevKeyState, keyState, sizeof(prevKeyState)); //�R�s�[
		pKeyDevice->Acquire();
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);
		//�}�E�X
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
		//���͉����ĂāA�O��͉����ĂȂ�
		if (keyState[keyCode] == keyCode && prevKeyState[keyCode] != keyCode)
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		//�O��͉����ĂāA���͉����ĂȂ�
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

	//�}�E�X�̃{�^�����������������ׂ�i�������ςȂ��͖����j
	bool IsMouseButtonDown(int buttonCode)
	{
		//���͉����ĂāA�O��͉����ĂȂ�
		if (IsMouseButton(buttonCode) && !(prevMouseState_.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	//�}�E�X�̃{�^�����������������ׂ�
	bool IsMouseButtonUp(int buttonCode)
	{
		//�������ĂȂ��āA�O��͉����Ă�
		if (!IsMouseButton(buttonCode) && !(prevMouseState_.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	//�}�E�X�J�[�\���̈ʒu���擾
	XMVECTOR GetMousePosition()
	{
		POINT mousePos;
		GetCursorPos(&mousePos);
		XMVECTOR result = XMVectorSet((float)mousePos.x, (float)mousePos.y, 0, 0);
		return result;
	}

	//���̃t���[���ł̃}�E�X�̈ړ��ʂ��擾
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
