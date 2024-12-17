#pragma once
#include "Engine/GameObject.h"
#include "Engine/Quad.h"

class Stage :
    public GameObject
{
    int hModel_;
    Quad* q;
public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

