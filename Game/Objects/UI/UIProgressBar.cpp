#include "UIProgressBar.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/Global.h"
#include "../../../Engine/ResourceManager/Image.h"
#include "../../../Engine/DirectX/Direct3D.h"

UIProgressBar::UIProgressBar(string _name, GameObject* parent)
	:UIObject(_name, UIType::UI_PROGRESSBAR, parent),
    gaugeMaxValue_(100.0f),  // �����l��ݒ�
    gaugeNowValue_(50.0f),  // �����l��ݒ�
    gaugeAnimValue_(0.0f)  // �����l��ݒ�
{
}

void UIProgressBar::Initialize()
{
}

void UIProgressBar::Update()
{
}

void UIProgressBar::Draw()
{
}

void UIProgressBar::Release()
{
}

void UIProgressBar::Save(json& saveObj)
{
}

void UIProgressBar::Load(json& loadObj)
{
}

void UIProgressBar::DrawData()
{
}

void UIProgressBar::SetImage(string _imageFilePath)
{
}

void UIProgressBar::SetGaugeMaxValue(float maxValue)
{
}

void UIProgressBar::SetGaugeCurrentValue(float currentValue)
{
}

void UIProgressBar::SetGaugeAnimValue()
{
}

