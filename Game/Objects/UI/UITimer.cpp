#include "UITimer.h"
#include "../../../Engine/ImGui/imgui.h"


UITimer::UITimer(string _name, UIObject* parent, int _layerNum)
	: UIObject(_name, UI_TIMER, parent, _layerNum), max_(0), count_(0), isActive_(false)
{
}

void UITimer::Initialize()
{
}

void UITimer::Update()
{
	// �J�E���^�[���X�V
	if(isActive_)count_--;

	// �J�E���^�[��0�ɂȂ������A�N�e�B�u�ɂ���
	if (count_ <= 0){
		count_ = 0;
		isActive_ = false;
	}
}

void UITimer::Draw()
{
}

void UITimer::Release()
{
}

void UITimer::Save(json& saveObj)
{
	// �ő�l�̕ۑ�
	saveObj["max"] = max_;
}

void UITimer::Load(json& loadObj)
{
	// �ő�l�̓ǂݍ���
	max_ = loadObj["max"];
	count_ = max_;
}

void UITimer::DrawData()
{
	// �ő�l�̐ݒ�
	ImGui::DragFloat("max", &max_, 1.f, 1.f, 1000.f);
}
