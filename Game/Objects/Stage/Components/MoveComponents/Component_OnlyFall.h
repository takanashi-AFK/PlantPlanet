#pragma once
#include "../Component.h"

#include <DirectXMath.h>

// using�f�B���N�e�B�u
using namespace DirectX;

class Component_OnlyFall : public Component
{

private:
	bool isActive_;
	bool isFirst_;
	float startHeight_;
	float fallDistance_;
	float fallSpeed_;
	float height_;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_OnlyFall(string _name, StageObject* _holder, Component* _parent);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;
	
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;
	
	/// <summary>
	/// �J��
	/// </summary>
	void Release() override;
	
	/// <summary>
	/// �ۑ�
	/// </summary>
	/// <param name="_saveObj">�ۑ����</param>
	void Save(json& _saveObj) override;
	
	/// <summary>
	/// �Ǎ�
	/// </summary>
	/// <param name="_loadObj">�Ǎ����</param>
	void Load(json& _loadObj) override;	

	/// <summary>
	/// ImGui�p�l���\��
	/// </summary>
	void DrawData() override;


	void Execute() { isActive_ = true; }
};

