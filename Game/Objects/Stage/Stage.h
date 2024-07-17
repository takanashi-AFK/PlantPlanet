#pragma once

// �C���N���[�h
#include "../../../Engine/GameObject/GameObject.h"
#include "../../../Engine/Json/JsonReader.h"
#include <vector>

// using�錾
using std::vector;

// �O���錾
class StageObject;
class Component;

/// <summary>
/// �X�e�[�W���Ǘ�����N���X
/// </summary>
class Stage : public GameObject
{
private:
	friend class GameEditor;		// StageEditor�N���X���t�����h�錾
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
	/// <param name="_saveObj">�ۑ��f�[�^</param>
	void Save(json& _saveObj);

	/// <summary>
	/// �Ǎ�
	/// </summary>
	/// <param name="_loadObj">�Ǎ��f�[�^</param>
	void Load(json& _loadObj);

	/// <summary>
	/// ImGui�p�l���\��
	/// </summary>
	void DrawData();

	/// <summary>
	/// �I�u�W�F�N�g�����X�g�ɒǉ�
	/// </summary>
	/// <param name="_obj">�ǉ�����I�u�W�F�N�g</param>
	void AddStageObject(StageObject* _obj);

	/// <summary>
	/// �I�u�W�F�N�g�����X�g����폜
	/// </summary>
	/// <param name="_obj">�폜����I�u�W�F�N�g</param>
	void DeleteStageObject(StageObject* _obj);

	/// <summary>
	/// ���X�g���̃I�u�W�F�N�g��S�č폜
	/// </summary>
	void DeleteAllStageObject();

	/// <returns>
	/// �I�u�W�F�N�g���X�g���擾
	/// </returns>
	vector<StageObject*>& GetStageObjects() { return objects_; }

	Component* FindComponent(string _name);
};

