#pragma once
#include"DirectXMath.h"

namespace UICursor
{

	DirectX::XMINT2 GetPosition();
	void SetPosition(DirectX::XMINT2 pos);

	void Initialize();
	void Update();

	bool IsHide();
	void ToHide(bool isHide);

	void SetSensitivity_Mouse(DirectX::XMFLOAT2 sens);
	void SetSensitivity_Pad(DirectX::XMFLOAT2 sens);

	DirectX::XMFLOAT2 GetSensitivity_Mouse();
	DirectX::XMFLOAT2 GetSensitivity_Pad();
};

