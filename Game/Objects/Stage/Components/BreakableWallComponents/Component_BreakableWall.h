#pragma once
#include "../Component.h"
class Component_BreakableWall :
    public Component
{
private:
	StageObject* playerObjects_;
	bool isPlayerContains;

public:
	Component_BreakableWall(string _name, StageObject* _holder, Component* _parent);
	void Initialize() override;
	void Update() override;
	void Release() override;
	void OnCollision(GameObject* _target, Collider* _collider) override;
	void Save(json& _saveObj) override;
	void Load(json& _loadObj) override;
	void DrawData() override;

	bool IsPlayerContains() { return isPlayerContains; }

};

