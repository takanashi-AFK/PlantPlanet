#include "UICursor.h"
#include "../../../Engine/DirectX/Input.h"
#include "../../../Engine/Global.h"
#include "../../../Engine/ImGui/imgui.h"

namespace UICursor
{
	XMINT2 position_;
	XMINT2 prevPos_;
	bool isHide_ = false;
	XMFLOAT2 sensitivityPad_ = { 10.f,10.f };
	XMFLOAT2 sensitivityMouse_ = { 1.f,1.f };

	DirectX::XMINT2 GetPosition()
	{
		return position_;
	}

	void SetPosition(DirectX::XMINT2 pos)
	{
		position_ = pos;
	}

	void Initialize()
	{
		POINT mousePos;
		GetCursorPos(&mousePos);

		position_ = { mousePos.x,mousePos.y };
		SetCursorPos(position_.x, position_.y);
		prevPos_ = position_;
	}

	void Update()
	{

		XMINT2 vecCursor = { (int)(Input::GetPadStickL(0).x),(int)(Input::GetPadStickL(0).y) };

		POINT mousePos;
		GetCursorPos(&mousePos);

		position_ = { mousePos.x,mousePos.y };

		if (vecCursor.x > .0f || vecCursor.y > .0f)
		{
			
			position_ =
			{
				position_.x + (int)(vecCursor.x * sensitivityPad_.x) ,
				position_.y + (int)(-vecCursor.y * sensitivityPad_.y)
			};

			//SetCursorPos(position_.x, position_.y);
		}

		prevPos_ = position_;

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