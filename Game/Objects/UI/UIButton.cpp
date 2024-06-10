#include "UIButton.h"

#include "../../../Engine/ResourceManager/Image.h"
#include "../../../Engine/DirectX/Input.h"
#include "../../../Engine/DirectX/EasingFunction.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/DirectX/Direct3D.h"

using namespace Direct3D;

UIButton::UIButton(GameObject* _parent)
	:UIObject("UiButton", _parent), UIButtonPict_(-1)
{

}

void UIButton::Initialize()
{
	UIButtonPict_ = Image::Load("Images/TestButton.png");
	assert(UIButtonPict_ >= 0);

	// �摜�̑傫�����擾
	size_ = Image::GetSize(UIButtonPict_);

	// �E�B���h�E��ł̉摜�̑傫���ɕϊ�
	size_.x /= Direct3D::screenWidth_;
	size_.y /= Direct3D::screenHeight_;

}

void UIButton::Update()
{
	// �}�E�X�̍��W���擾
	XMFLOAT3 mousePos_ = Input::GetMousePosition();

	// �}�E�X�̍��W���摜�̍��W�ɕϊ�
	ConvertToImageCoordinates(mousePos_);
	ImGui::Text("mousePos_ x:%f y:%f", mousePos_.x, mousePos_.y);

	// �}�E�X���摜�̗̈���ɓ����Ă��邩�ǂ���
	ImGui::Text("isInArea = %s", MouseInArea(mousePos_) ? "true" : "false");


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

bool UIButton::MouseInArea(XMFLOAT3 mousePos)
{
	// �}�E�X�̍��W���摜�̗̈���ɓ����Ă��邩�ǂ���
	return (mousePos.x > -(size_.x * transform_.scale_.x) / 2 && mousePos.x < (size_.x * transform_.scale_.x) / 2
		&& mousePos.y > -(size_.y * transform_.scale_.y) / 2 && mousePos.y < (size_.y * transform_.scale_.y) / 2);
}

bool UIButton::ClickButton()
{
	// �}�E�X�̍��W���擾
	XMFLOAT3 mousePos = Input::GetMousePosition();

	// �}�E�X�̍��W���摜�̍��W�ɕϊ�
	ConvertToImageCoordinates(mousePos);

	//�}�E�X�̃|�W�V�������摜�̗̈�ɓ����Ă��邩���N���b�N�������ꂽ��
	return MouseInArea(mousePos) && Input::IsMouseButtonDown(0);
}

void UIButton::ConvertToImageCoordinates(XMFLOAT3& _position)
{
	int scWidth = Direct3D::screenWidth_;
	int scHeight = Direct3D::screenHeight_;

#ifdef _DEBUG
	scWidth = scWidth * 0.7;
	scHeight = scHeight * 0.7;
#endif // _DEBUG


	// �}�E�X�̍��W���摜�̍��W�ɕϊ�
	_position.x = (float)(_position.x - ((float)scWidth) / 2) / (float)scWidth;
	_position.y = (float)(_position.y - (float)scHeight / 2) / (float)scHeight;

	ImGui::Text("mousePos_ x:%f y:%f", _position.x, _position.y);
	ImGui::Text("screenWidth_:%d screenHeight_:%d", scWidth, scHeight);
}

void UIButton::DrawGui()
{
	std::vector<std::string> easingFunc;
	static int currentEasingFunc = 0;
	for (const auto& pair : EaseFunc)
		easingFunc.push_back(pair.first);

	if (ImGui::BeginCombo("Ease Function", easingFunc[currentEasingFunc].c_str())) {
		for (int i = 0; i < easingFunc.size(); i++) {
			bool FuncSelected = (currentEasingFunc == i);
			if (ImGui::Selectable(easingFunc[i].c_str(), FuncSelected)) {
				currentEasingFunc = i;
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("Apply Easing")) {
		auto selectedEasingFunc = EaseFunc[easingFunc[currentEasingFunc]];


	}

	// ���g�̕ό`����`��
	if (ImGui::TreeNode("transform_")) {
		ImGui::DragFloat3("position_", &transform_.position_.x, 0.1f);
		ImGui::DragFloat3("rotate_", &transform_.rotate_.x, 1.f, -360.f, 360.f);
		ImGui::DragFloat3("scale_", &transform_.scale_.x, 0.1f, 0.f, LONG_MAX);
		ImGui::TreePop();
	}

}



