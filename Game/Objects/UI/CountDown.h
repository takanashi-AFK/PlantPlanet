#pragma once
#include "../../../../Engine/GameObject/GameObject.h"

// �C���N���[�h
#include<vector>

class CountDown : public GameObject
{
private:
	std::vector<int> countDownImages_;	// �J�E���g�_�E���摜
	float count_;						// �o�ߎ���

public:
	/// <summary> �R���X�g���N�^ </summary>
	CountDown(GameObject* _parent);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> �`�� </summary>
	void Draw() override;

	/// <summary> ��� </summary>
	void Release() override;
/*
predicate :*/
	/// <summary> �J�E���g�_�E�����I�������� </summary>
	bool IsFinished() const { return count_ <= 0; }
};
