#pragma once

// �C���N���[�h
#include "../../Engine/GameObject/GameObject.h"

class Scene_Splash : public GameObject
{
private:
	float count_;		// �o�ߎ���

public:
	/// <summary> �R���X�g���N�^ </summary>
	Scene_Splash(GameObject* _parent);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> �`�� </summary>
	void Draw() override;

	/// <summary> ��� </summary>
	void Release() override;

	/*
	getter :*/
	/// <summary> �o�ߎ��Ԃ̎擾 </summary>
	float GetCount() const { return count_; }

	/*
	predicate :*/
	/// <summary> �V�[���ؑ։\�� </summary>
	bool IsChangeScene() const;

	/// <summary> �w�莞�Ԍo�߂�����(�b) </summary>
	bool IsCountNow(float _sec) const;
};
