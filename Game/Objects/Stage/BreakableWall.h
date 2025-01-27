#pragma once
#include "Stage.h"
#include "StageObject.h"

class BreakableWall : public StageObject
{

public:
	BreakableWall(GameObject* _parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnCollision(GameObject* _target, Collider* _collider) override;
	void Save(json& _saveObj) override;
	void Load(json& _loadObj) override;
	void DrawData() override;

};

