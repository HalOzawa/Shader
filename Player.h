#pragma once
#include "Engine/GameObject.h"

class FBX;

class Player :
    public GameObject
{
	FBX* pFbx;
public:
	Player(GameObject* parent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

