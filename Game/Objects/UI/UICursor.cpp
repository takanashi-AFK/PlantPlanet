#include "UICursor.h"
#include "../../../Engine/DirectX/Input.h"
#include "../../../Engine/Global.h"
#include "../../../Engine/ImGui/imgui.h"

namespace UICursor
{
	XMFLOAT2 position_ = { Direct3D::screenWidth_ / 2.f, Direct3D::screenHeight_ / 2.f };
	bool isHide_ = false;
	XMFLOAT2 sensitivityPad_ = { 10.f,10.f };
	XMFLOAT2 sensitivityMouse_ = { 1.f,1.f };

	DirectX::XMFLOAT2 GetPosition()
	{
		return position_;
	}

	void SetPosition(DirectX::XMFLOAT2 pos)
	{
		position_ = pos;
	}

	void Update()
	{
		SetCursorPos(position_.x, position_.y);

		if (isHide_)	return;

		auto vecCursor = Input::GetMouseMove();
		position_ =
		{
			position_.x + (vecCursor.x*sensitivityMouse_.x) ,
			position_.y + (vecCursor.y*sensitivityMouse_.y) 
		};

		vecCursor = Input::GetPadStickL(0);
		position_ =
		{
			position_.x + (vecCursor.x * sensitivityPad_.x) ,
			position_.y + (-vecCursor.y * sensitivityPad_.y)
		};
	}

	bool IsHide()
	{
		return isHide_;
	}

	void ToHide(bool isHide)
	{
		isHide_ = isHide;
		ShowCursor(!isHide_);
	}

	void SetSensitivity_Mouse(DirectX::XMFLOAT2 sens)
	{
		sensitivityMouse_ = sens;
	}

	void SetSensitivity_Pad(DirectX::XMFLOAT2 sens)
	{
		sensitivityPad_ = sens;
	}

	DirectX::XMFLOAT2 GetSensitivity_Mouse()
	{
		return sensitivityMouse_;
	}
	DirectX::XMFLOAT2 GetSensitivity_Pad()
	{
		return sensitivityPad_;
	}
}