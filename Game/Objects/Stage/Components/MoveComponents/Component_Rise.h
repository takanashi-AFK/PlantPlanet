#pragma once
#include "../Component.h"

#include <DirectXMath.h>

// using�f�B���N�e�B�u
using namespace DirectX;

class Component_Rise : public Component
{
private:
	bool isInfinity_;
	bool isFirst_;
	bool isEnd_;
	float height_;
	float riseSpeed_;
	float targetHeight_;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_Rise(string _name, StageObject* _holder, Component* _parent);

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

	void Stop() { isActive_ = false; }

	void SetInfinity(bool _isInfinity) { isInfinity_ = _isInfinity; }

	bool IsActive() { return isActive_; }

	bool IsEnd() { return isEnd_; }

	void SetIsEnd(bool _isEnd) { isEnd_ = _isEnd; }
};

