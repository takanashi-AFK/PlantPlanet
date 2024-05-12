#pragma once
#include "../Component.h"

class Component_CircleRangeDetector : public Component
{
private:
	
	StageObject* target_;  // �ΏۃI�u�W�F�N�g
	float radius_;  // ���a
	bool isContains_;    // �����蔻��t���O

public:
	Component_CircleRangeDetector(StageObject* _holder);

	void Initialize() override;
	void Update() override;
	void Release() override;

	void Save(json& _saveObj) override;
	void Load(json& _loadObj) override;
	void DrawData() override;
// setter
	void SetRadius(float _radius) { radius_ = _radius; }
	void SetTarget(StageObject* _target) { target_ = _target; }

// getter
	float GetRadius() { return radius_; }
	bool IsContains() { return isContains_; }
	StageObject* GetTarget() { return target_; }

private:
	// �͈͓��ɓ����Ă��邩�ǂ����𔻒�
	bool Contains();

};

