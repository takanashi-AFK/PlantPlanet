#pragma once

// �C���N���[�h
#include "StageObject.h"
#include<array>

/// <summary>
/// �X�J�C�X�t�B�A���Ǘ�����N���X
/// </summary>
class SkySphere : public StageObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_parent">�e�I�u�W�F�N�g</param>
	SkySphere(GameObject* _parent);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	enum class TIME_ZONE
	{
		MORNING ,
		NIGHT,

		AMOUNT
	};

	void SetTimeZone(TIME_ZONE tz);
	TIME_ZONE GetTimeZone();

	void SetLights();

private:

	TIME_ZONE timeZone_;

	inline static std::array<int, static_cast<int>(TIME_ZONE::AMOUNT)> modelHandles_ = {};

	inline static std::array<XMFLOAT4, static_cast<int>(TIME_ZONE::AMOUNT)> globalLightColors_ =
	{
		XMFLOAT4{.4f,.4f,.4f,1.f},
				{.4f,.5f,.6f,.5f},
	};
	inline static std::array<XMFLOAT4, static_cast<int>(TIME_ZONE::AMOUNT)> AmbientLightColors_ =
	{
		XMFLOAT4{.15f,.15f,.15f,.6f},
				{.2f,.2f,.25f,.2f}
	};
};

