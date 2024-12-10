#pragma once
#include"DirectXMath.h"

namespace UICursor
{

	DirectX::XMFLOAT2 GetPosition();
	void SetPosition(DirectX::XMFLOAT2 pos);

	void Update();

	bool IsHide();
	void ToHide(bool isHide);

	void SetSensitivity_Mouse(DirectX::XMFLOAT2 sens);
	void SetSensitivity_Pad(DirectX::XMFLOAT2 sens);

	DirectX::XMFLOAT2 GetSensitivity_Mouse();
	DirectX::XMFLOAT2 GetSensitivity_Pad();
};

