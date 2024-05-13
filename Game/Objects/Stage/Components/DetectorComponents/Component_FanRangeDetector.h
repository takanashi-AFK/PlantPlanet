#pragma once

#include "../Component.h"
#include <DirectXMath.h>

using namespace DirectX;

class Component_FanRangeDetector : public Component
{
private:
	StageObject* target_;	// �ΏۃI�u�W�F�N�g
	float length_;			// ����
	float angle_;			// �p�x
	XMVECTOR direction_;	// ����
	bool isContains_;		// �����蔻��t���O

public:
	Component_FanRangeDetector(StageObject* _holder);

	void Initialize() override;
	void Update() override;
	void Release() override;

	void Save(json& _saveObj) override;
	void Load(json& _loadObj) override;
	void DrawData() override;

	// �͈͓��ɓ����Ă��邩�ǂ����𔻒�
	bool IsContains();
};

