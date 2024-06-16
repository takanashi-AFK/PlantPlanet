#pragma once

// �C���N���[�h
#include "Component_Attack.h"
#include <DirectXMath.h>

using namespace DirectX;

/// <summary>
/// ���Z���������U���R���|�[�l���g
/// </summary>
class Component_ShootAttack :public Component_Attack
{
private:
	float shootingSpeed_;			// �e�̑��x
	XMVECTOR shootingDirection_;	// �e�̕���
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�R���|�[�l���g�̕ێ���</param>
	Component_ShootAttack(string _name, StageObject* _holder, Component* _parent);
	
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
	/// <param name="_savebj">�f�[�^�̕ۑ����</param>
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


	void SetShootingSpeed(float _speed) { shootingSpeed_ = _speed; }
	void SetShootingDirection(XMVECTOR _direction) { shootingDirection_ = _direction; }

};
