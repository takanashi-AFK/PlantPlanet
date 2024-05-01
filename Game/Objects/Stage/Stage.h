#pragma once

// �C���N���[�h
#include "../../../Engine/GameObject/GameObject.h"
#include "../../../Engine/Json/JsonReader.h"
#include <vector>

// using�錾
using std::vector;

// �O���錾
class StageObject;

// �X�e�[�W���Ǘ�����N���X
class Stage : public GameObject
{
private:
	vector<StageObject*> objects_;	// ���g���ۗL����X�e�[�W�I�u�W�F�N�g�Q

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_parent">�e�I�u�W�F�N�g</param>
	Stage(GameObject* _parent);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ���
	/// </summary>
	void Release() override;

	/// <summary>
	/// �ۑ�
	/// </summary>
	/// <param name="_saveObj"></param>
	void Save(json& _saveObj);
};

