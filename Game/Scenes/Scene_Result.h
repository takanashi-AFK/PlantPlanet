#pragma once
#include"../../Engine/GameObject/GameObject.h"

class Scene_Result :public GameObject
{
private:
	int scoreNum_;	// �X�R�A
	int layerNumberCount_;
public:
	/// <summary> �R���X�g���N�^ </summary>
	Scene_Result(GameObject* parent_);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> �`�� </summary>
	void Draw() override;

	/// <summary> ��� </summary>
	void Release() override;
	
private:
	/// <summary> �X�R�A�̌v�Z </summary>
	int CalculateScore(bool isCleared, int remainingTime, int remainingHP);
};

	