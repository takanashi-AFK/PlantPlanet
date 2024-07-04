#include "UIProgressBar.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/Global.h"
#include "../../../Engine/ResourceManager/Image.h"
#include "../../../Engine/DirectX/Direct3D.h"
#include "../Stage/Stage.h"
#include "../../../Game/Objects/Stage/StageObject.h"
#include "../../../Game/Objects/Stage/Components/Component.h"


UIProgressBar::UIProgressBar(string _name, GameObject* parent)
	:UIObject(_name, UIType::UI_PROGRESSBAR, parent),
    gaugeMaxValue_(100.0f),  // �����l��ݒ�
    gaugeNowValue_(50.0f)  // �����l��ݒ�
{
}

void UIProgressBar::Initialize()
{
    pictGaugeHandle_ = Image::Load("Images/Bar_Gauge.png");
    assert(pictGaugeHandle_ >= 0);

    pictFrameHandle_ = Image::Load("Images/Bar_Frame.png");
    assert(pictFrameHandle_ >= 0);

}

void UIProgressBar::Update()
{
    if (healthGauge_ == nullptr)healthGauge_ = (Component_HealthGauge*)FindObject(targetName_);
    if (healthGauge_ == nullptr)return;

    if (healthGauge_ != nullptr) {
        gaugeMaxValue_ = healthGauge_->GetMax();
        gaugeNowValue_ = healthGauge_->GetHP();
    }

    // �ݒ肳�ꂽ�l����ό`�s��`transform_`�̒l���v�Z
    // �Q�[�W�̃X�P�[�����v�Z
    transGauge_ = transform_;
    transGauge_.scale_.x = (gaugeNowValue_ / gaugeMaxValue_) * transFrame_.scale_.x;
    
}

void UIProgressBar::Draw()
{
    if (imageHandle_ < 0) return;
    
    

    // �Q�[�W�̉摜��`��
    Image::SetTransform(pictGaugeHandle_, transGauge_);
    Image::Draw(pictGaugeHandle_,Direct3D::SHADER_BAR,color_);
    
    transFrame_ = transform_;

    Image::SetTransform(pictFrameHandle_, transFrame_);
    Image::Draw(pictFrameHandle_);
}

void UIProgressBar::Release()
{
}

void UIProgressBar::Save(json& saveObj)
{
    if (target_ != nullptr)saveObj["target_"] = target_->GetObjectName();
    saveObj["imageFilePath_"] = imageFilePath_;
    saveObj["gaugeMaxValue_"] = gaugeMaxValue_;
    saveObj["gaugeNowValue_"] = gaugeNowValue_;
    saveObj["color_"] = { REFERENCE_XMFLOAT3(color_) };
}

void UIProgressBar::Load(json& loadObj)
{
    if (loadObj.contains("imageFilePath_")) {
        imageFilePath_ = loadObj["imageFilePath_"].get<string>();
        SetImage(imageFilePath_);
    }
    if (loadObj.contains("target_"))targetName_ = loadObj["target_"];
    gaugeMaxValue_ = loadObj["gaugeMaxValue_"].get<float>();
    gaugeNowValue_ = loadObj["gaugeNowValue_"].get<float>();
    color_ = { loadObj["color_"][0].get<float>(),loadObj["color_"][1].get<float>(), loadObj["color_"][2].get<float>() };
}

void UIProgressBar::DrawData()
{
    // ������������������������������������
    // �v���O���X�o�[�̃t���[���摜��ݒ�
    // ������������������������������������
    if (ImGui::TreeNode("imageFilePath_")) {

        // ���݂̃t�@�C���p�X��\��
        ImGui::Text("imageFilePath_:%s", imageFilePath_.c_str());
        ImGui::SameLine();

        // �t�@�C���p�X���擾
        if (ImGui::SmallButton("...")) {
            // ���݂̃J�����g�f�B���N�g�����o���Ă���
            char defaultCurrentDir[MAX_PATH];
            GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

            // �u�t�@�C�����J���v�_�C�A���O�̐ݒ�
            OPENFILENAME ofn; {
                TCHAR szFile[MAX_PATH] = {};
                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.lpstrFile = szFile;
                ofn.lpstrFile[0] = '\0';
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = TEXT("PNG�t�@�C��(*.fbx)\0*.png\0���ׂẴt�@�C��(*.*)\0*.*\0");
                ofn.nFilterIndex = 1;
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                ofn.lpstrInitialDir = TEXT(".");
            }

            // �t�@�C���I���_�C�A���O�̕\��
            if (GetOpenFileName(&ofn) == TRUE) {
                // �t�@�C���p�X���擾
                imageFilePath_ = ofn.lpstrFile;
                imageFilePath_ = FileManager::GetAssetsRelativePath(imageFilePath_);
                FileManager::ReplaceBackslashes(imageFilePath_);
                SetCurrentDirectory(defaultCurrentDir);
                SetImage(imageFilePath_);
            }
            else {
                return;
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }


    // ������������������������������������
    // �Q�[�W�\���F��ݒ�
    // ������������������������������������
    ImVec4 inputCol = { REFERENCE_XMFLOAT3(color_),0 };
    ImGui::ColorEdit4("Color", (float*)&inputCol);
    color_ = {REFERENCE_XMFLOAT3(inputCol)};


    // ������������������������������������
    // �Q�Ƃ���Q�[�W�R���|�[�l���g��ݒ�
    // ������������������������������������

    // �Q�[�W�R���|�[�l���g�̖��O���擾�i���ׂāj
    vector<string> gaugeCompNames;

    // �R���|�[�l���g�̖��O��ǉ����Ă�����
    gaugeCompNames.push_back("null");
    auto stage = (Stage*)FindObject("Stage");
    if (stage) {
        for (auto obj : stage->GetStageObjects()) {
            for (auto component : obj->FindComponent(HealthGauge)) {
                //�X�e�[�W���̂��ׂẴI�u�W�F�N�g�̖��O���擾
                gaugeCompNames.push_back(obj->GetObjectName());
                break;
            }
        }
    }

    // �擾�������O�̃��X�g���Q�Ƃ���ImGui�őI���ł���悤��
    static int select = 0;

    if (ImGui::BeginCombo("target_", gaugeCompNames[select].c_str())) {
        for (int i = 0; i < gaugeCompNames.size(); i++) {
            bool is_selected = (select == i);
            if (ImGui::Selectable(gaugeCompNames[i].c_str(), is_selected)) select = i;
            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    // �I�����ꂽindex���疼�O������o��
    targetName_ = gaugeCompNames[select];

    // �Q�[�W�̐���
    ImGui::DragFloat("Current Value", &gaugeNowValue_, 1.f, gaugeMaxValue_);
    ImGui::DragFloat("Max Value", &gaugeMaxValue_, 1.f);
    ImGui::ProgressBar(gaugeNowValue_ / gaugeMaxValue_, ImVec2(0.0f, 0.0f));

}


void UIProgressBar::SetImage(string _imageFilePath)
{
    imageFilePath_ = _imageFilePath;
    pictFrameHandle_ = Image::Load(_imageFilePath);
}

void UIProgressBar::SetGaugeMaxValue(float _maxValue)
{
    gaugeMaxValue_ = _maxValue;
}

void UIProgressBar::SetGaugeCurrentValue(float _nowValue) {
    gaugeNowValue_ = _nowValue;
}

