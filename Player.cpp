#include "Player.h"
#include "Engine/FBX.h"

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), pFbx(nullptr)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	pFbx = new FBX;
	pFbx->Load("Assets//Oden.fbx");
	transform_.scale_ = { 0.7, 0.7, 0.7 };
}

void Player::Update()
{
	transform_.rotate_.y += 0.1;
}

void Player::Draw()
{
	pFbx->Draw(transform_);
}

void Player::Release()
{
	pFbx->Release();
	delete pFbx;
}
