#include "Stage.h"
#include "Model.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), hModel_(-1)
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	q = new Quad();
	q->Initialize();
}

void Stage::Update()
{
	transform_.rotate_.y += 0.5f;
}

void Stage::Draw()
{
	q->Draw(transform_);
}

void Stage::Release()
{
}
