#include "UIButton.h"
#include "../../../Engine/ResourceManager/Image.h"
#include "../../../Engine/DirectX/Input.h"
#include "../../../Engine/DirectX/EasingFunction.h"

UIButton::UIButton(string _name ,GameObject* _parent)
	:UIObject("UIButton", _parent), UIButtonPict_(-1), isButtonPushed_(false)
{
}

void UIButton::Initialize()
{
	//�{�^����������Ă��Ȃ���Ԃɂ���
	isButtonPushed_ = false;
	//�摜�̓ǂݍ���
	UIButtonPict_ = Image::Load("Assets/Image/TestButton.png");
	//�摜�̓ǂݍ��݂Ɏ��s�����ꍇ
	assert(UIButtonPict_ >= 0);
	//�摜�̃T�C�Y���擾
	size_ = Image::GetTextureSize(UIButtonPict_);
}

void UIButton::Update() 
{
	DrawGUI();
	//�}�E�X�̈ʒu���擾
	mousePos_ = Input::GetMousePosition();
}

void UIButton::Draw()
{
	Image::SetTransform(UIButtonPict_, transform_);
	Image::Draw(UIButtonPict_);
}

void UIButton::Release()
{

}

void UIButton::Save(json& _saveUIobj)
{
}

void UIButton::Load(json& _loadUIobj)
{
}

void UIButton::DrawGUI()
{
	std::vector<std::string> easingFunc;
	static int currentEasing = 0;
	//for(const auto& pair : EaseFunc)
	//	easingFunc.push_back(pair.first);

}

void UIButton::SetImage()
{
}

bool UIButton::MouseInArea(XMFLOAT3 mousePos)
{
	return false;
}

void UIButton::ClickButton()
{
}
