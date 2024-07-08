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
  //�Q�Ƃ��Ă���R���|�[�l���g�����邩�ǂ���
  //  if (referenceGauge_.this_ == nullptr)
		//// ���ꂪ���� �Ȃ��refereceGauge��null�Ȃ̂�referenceGauge��holderName_���H
  //      referenceGauge_.this_ = (Component_Gauge*)((StageObject*)FindObject(referenceGauge_.holderName_))->FindComponent(referenceGauge_.thisName_);
    if(referenceGauge_.this_ == nullptr)return;
    //�Q�Ƃ���Q�[�W�R���|�[�l���g����l���擾
    if (referenceGauge_.this_ != nullptr) {
        gaugeMaxValue_ = referenceGauge_.this_->GetMax();
        gaugeNowValue_ = referenceGauge_.this_->GetNow();
    }
    
}

void UIProgressBar::Draw()
{
    if (imageHandle_ < 0) return;
    
   // �ݒ肳�ꂽ�l����ό`�s��`transform_`�̒l���v�Z
   // �Q�[�W�̃X�P�[�����v�Z
    transGauge_ = transform_;
    transGauge_.scale_.x = (gaugeNowValue_ / gaugeMaxValue_) * transFrame_.scale_.x;
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

// ������������������������������������������������������������
// �Q�[�W�R���|�[�l���g�����擾
// ������������������������������������������������������������
 // �Q�[�W�R���|�[�l���g�����擾
    vector<gauge> gauges;

    // �X�e�[�W�����擾
    vector<StageObject*> objects = ((Stage*)FindObject("Stage"))->GetStageObjects();

    //�X�e�[�W���̃I�u�W�F�N�g���Q�Ƃ��Ă��ׂẴQ�[�W�R���|�[�l���g���擾
    for (auto obj : objects) {
        //HP�Q�[�W��T���Ď擾
        for (auto healthGaugeComp : obj->FindComponent(HealthGauge)) {
            //�z��ɃI�u�W�F�N�g�ƃR���|�[�l���g����ɂ܂Ƃ߂ē����H
            auto gaugeComp = dynamic_cast<Component_Gauge*>(healthGaugeComp);
            gauges.push_back({ obj,gaugeComp,obj->GetObjectName(),gaugeComp->GetName() });
        }
        //�Z�Q�[�W...
    }
    static int select = 0;

    if (gauges.empty())return;
    //�R���{�{�b�N�X��gauge�̃R���e�i�ɂ���select�Ԗڂ̃R���|�[�l���g�𕶎���ɕϊ����Ď󂯎��H
    if (ImGui::BeginCombo("gauge_", gauges[select].thisName_.c_str())) {
        for (int i = 0; i < gauges.size(); i++) {
            //select�����݂̃C���f�b�N�Xi�Ɠ��������ǂ������`�F�b�N�B
            bool is_selected = (select == i);
            //is_selected��true�Ȃ�v�f����\������
            if (ImGui::Selectable(gauges[i].thisName_.c_str(), is_selected))
                select = i;
            if (is_selected)
                //�I�����ꂽ�A�C�e���Ƀt�H�[�J�X���Z�b�g
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }


    //�����o�ϐ��� struct gauge referenceGauge_�ɑI�����ꂽ�Q�[�W��ݒ�
    referenceGauge_ = gauges[select];

    gaugeMaxValue_ = referenceGauge_.this_->GetMax();
    gaugeNowValue_ = referenceGauge_.this_->GetNow();

    ImGui::Text("%f,%f", gaugeMaxValue_, gaugeNowValue_);


    //vector<gauge> gauges;

    //// �X�e�[�W�����擾
    //vector<StageObject*> objects = ((Stage*)FindObject("Stage"))->GetStageObjects();

    ////�X�e�[�W���̃I�u�W�F�N�g���Q�Ƃ��Ă��ׂẴQ�[�W�R���|�[�l���g���擾
    //for (auto obj : objects) {
    //    //HP�Q�[�W��T���Ď擾
    //    for (auto healthGaugeComp : obj->FindComponent(HealthGauge)) {
    //        //�z��ɃI�u�W�F�N�g�ƃR���|�[�l���g����ɂ܂Ƃ߂ē����H
    //        auto gaugeComp = dynamic_cast<Component_Gauge*>(healthGaugeComp);
    //        gauges.push_back({ obj,gaugeComp,obj->GetObjectName(),gaugeComp->GetName() });
    //    }
    //    //�Z�Q�[�W...
    //}
    //static int select = 0;

    //if (gauges.empty())return;
    ////�R���{�{�b�N�X��gauge�̃R���e�i�ɂ���select�Ԗڂ̃R���|�[�l���g�𕶎���ɕϊ����Ď󂯎��H
    //if (ImGui::BeginCombo("gauge_", gauges[select].thisName_.c_str())) {
    //    for (int i = 0; i < gauges.size(); i++) {
    //        //select�����݂̃C���f�b�N�Xi�Ɠ��������ǂ������`�F�b�N�B
    //        bool is_selected = (select == i);
    //        //is_selected��true�Ȃ�v�f����\������
    //        if (ImGui::Selectable(gauges[select].thisName_.c_str(), is_selected))
    //            select = i;
    //        if (is_selected)
    //            //�I�����ꂽ�A�C�e���Ƀt�H�[�J�X���Z�b�g
    //            ImGui::SetItemDefaultFocus();
    //    }
    //    ImGui::EndCombo();
    //}


    ////�����o�ϐ��� struct gauge referenceGauge_�ɑI�����ꂽ�Q�[�W��ݒ�
    //referenceGauge_ = gauges[select];

    //gaugeMaxValue_ = referenceGauge_.this_->GetMax();
    //gaugeNowValue_ = referenceGauge_.this_->GetNow();

    //ImGui::Text("%f,%f", gaugeMaxValue_, gaugeNowValue_);

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

