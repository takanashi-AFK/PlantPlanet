#pragma once

#include "../Component.h"
#include <DirectXMath.h>
class Component_Fall : public Component
{
private:
	
	float fallSpeed_;//�~�����x
	float fallSpeedplus_;//�~�����x���グ����
	float riseSpeed_;//�㏸���x
	 DirectX::XMFLOAT3 startRisePosition_;
	 DirectX::XMFLOAT3 startFallPosition_;
	DirectX::XMFLOAT3 localPosition;
	bool isFirstTime_ = true;
	bool isRising_ = false;
	bool isActive_;
public:
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	///<param name="_holder">�ۗL��</param>
	Component_Fall(StageObject* _holder);

	///<summary>
	/// ������
	///</summary>
	void Initialize() override;

	///<summary>
	/// �X�V
	///</summary>
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
	/// ImGui�\��
	/// </summary>
	void DrawData() override;

	/// <summary>
	/// �U���J�n
	/// </summary>
	void Execute() { isActive_ = true; }
};

