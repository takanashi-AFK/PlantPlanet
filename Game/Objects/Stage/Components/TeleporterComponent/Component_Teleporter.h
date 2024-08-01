#pragma once
#include "../../Components/Component.h"
#include "../../../../../Engine/SceneManager.h"
#include "../../Stage.h"

enum ChangeType
{
	CHANGE_TELEPORT = 0,
	CHANGE_SCENE,
	CHANGE_JSON,
	CHANGE_Max,
};

class Component_Teleporter : public Component
{
private:
	bool isActive_;
	XMFLOAT3 teleportPos_;
	SCENE_ID changeSceneID_;
	string changeJsonPath_;
	StageObject* target_;

	Stage* pStage_;

	ChangeType changeType_;
public:
		/// <summary>
	    /// �R���X�g���N�^
	 	/// </summary>
	 	/// <param name="_holder">�R���|�[�l���g�̕ێ���</param>
	 	Component_Teleporter(string _name, StageObject* _holder, Component* _parent);
	 
	 	/// <summary>
	 	/// ������
	 	/// </summary>
	 	void Initialize() override;
	 
	 	/// <summary>
	 	/// �X�V
	 	/// </summary>
	 	void Update() override;
	 
	 	/// <summary>
	 	/// ���
	 	/// </summary>
	 	void Release() override;
	 
	 	/// <summary>
	 	/// �ۑ�
	 	/// </summary>
	 	/// <param name="_saveObj">�f�[�^�̕ۑ����</param>
	 	void Save(json& _saveObj) override;
	 
	 	/// <summary>
	 	/// �Ǎ�
	 	/// </summary>
	 	/// <param name="_loadObj">�f�[�^�̓Ǎ����</param>
	 	void Load(json& _loadObj) override;
	 
	 	/// <summary>
	 	/// ImGui�p�l���\��
	 	/// </summary>
	 	void DrawData()override;
	 
	 	/// <summary>
	 	/// �e���|�[�g
	 	/// </summary>
	 	void Teleport();	

		/// <summary>
		/// ���s
		/// </summary>
		/// <param name="_ch">�ǂ̌`���ŕω����邩</param>
		void Execute(ChangeType _ch);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_target"></param>
		void SetTarget(StageObject* _target) { target_ = _target; }
};

