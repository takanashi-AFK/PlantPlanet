
#include "UIProgressBar.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/Global.h"
#include "../../../Engine/ResourceManager/Image.h"
#include "../../../Engine/DirectX/Direct3D.h"
#include "../Stage/Stage.h"
#include "../../../Game/Objects/Stage/StageObject.h"
#include "../../../Game/Objects/Stage/Components/Component.h"


UIProgressBar::UIProgressBar(string _name, UIObject* parent,int _layerNum)
    :UIObject(_name, UIType::UI_PROGRESSBAR, parent, _layerNum)
{
}

void UIProgressBar::Initialize()
{
    // �t���[���摜�̓Ǎ�
    frameImage_.filePath_ = "Images/Bar_Frame.png";
    frameImage_.handle_ = Image::Load(frameImage_.filePath_);

    // �Q�[�W�摜�̓Ǎ�
    gaugeImage_.filePath_ = "Images/Bar_Gauge.png";
    gaugeImage_.handle_ = Image::Load(gaugeImage_.filePath_);
}

void UIProgressBar::Update()
{
    // �Q�Ƃ���Q�[�W�R���|�[�l���g�����Ȃ��ꍇ�́A�������I��
    if (referenceGauge_.this_ == nullptr) return;

    // �Q�[�W�̍ő�l�ƌ��ݒl���擾
    max_ = referenceGauge_.this_->GetMax();
    now_ = referenceGauge_.this_->GetNow();

    // �Q�[�W�̃X�P�[�����v�Z
    gaugeImage_.transform_ = transform_;
    gaugeImage_.transform_.scale_.x = (now_ / max_) * frameImage_.transform_.scale_.x;  

    // �t���[���̃X�P�[����ݒ�
    frameImage_.transform_ = transform_;
}

void UIProgressBar::Draw()
{
    // �t���[���摜�ƃQ�[�W�摜���ǂݍ��܂�Ă��Ȃ��ꍇ�́A�������I��
    if (frameImage_.handle_ < 0)return;
    if (gaugeImage_.handle_ < 0)return;

    // �Q�[�W�̉摜��`��
    Image::SetTransform(gaugeImage_.handle_,gaugeImage_.transform_);
    Image::Draw(gaugeImage_.handle_, Direct3D::SHADER_BAR, gaugeImage_.color_);

    // �t���[���̉摜��`��
    Image::SetTransform(frameImage_.handle_, frameImage_.transform_);
    Image::Draw(frameImage_.handle_);
}

void UIProgressBar::Release()
{

}

void UIProgressBar::Save(json& saveObj)
{
    // �Q�[�W�摜�̏���ۑ�
    saveObj["gaugeImage_"]["filePath_"] = gaugeImage_.filePath_;
    saveObj["gaugeImage_"]["color_"] = { REFERENCE_XMFLOAT3(gaugeImage_.color_)};
    
    // �t���[���摜�̏���ۑ�
    saveObj["frameImage_"]["filePath_"] = frameImage_.filePath_;

    // �Q�Ƃ���Q�[�W�R���|�[�l���g�̏���ۑ�
    saveObj["referenceGauge_"]["thisName_"] = referenceGauge_.this_->GetName();
}

void UIProgressBar::Load(json& loadObj)
{
    // �Q�[�W�摜�̏���Ǎ�
    if (loadObj.contains("gaugeImage_")) {
		
        // �Q�[�W�摜�̃t�@�C���p�X���擾
        gaugeImage_.filePath_ = loadObj["gaugeImage_"]["filePath_"].get<string>();

        // �Q�[�W�摜��ǂݍ���
        gaugeImage_.handle_ = Image::Load(gaugeImage_.filePath_);

        // �Q�[�W�̐F���擾
		gaugeImage_.color_ = { loadObj["gaugeImage_"]["color_"][0].get<float>(),loadObj["gaugeImage_"]["color_"][1].get<float>(), loadObj["gaugeImage_"]["color_"][2].get<float>() };
	}   

    // �t���[���摜�̏���Ǎ�
    if (loadObj.contains("frameImage_")) {

		// �t���[���摜�̃t�@�C���p�X���擾
		frameImage_.filePath_ = loadObj["frameImage_"]["filePath_"].get<string>();

		// �t���[���摜��ǂݍ���
		frameImage_.handle_ = Image::Load(frameImage_.filePath_);
	}

    // �Q�Ƃ���Q�[�W�R���|�[�l���g�̏���Ǎ�
    if (loadObj.contains("referenceGauge_")) {

		// �Q�[�W�R���|�[�l���g�̖��O���擾
		referenceGauge_.thisName_ = loadObj["referenceGauge_"]["thisName_"].get<string>();
	}
}

void UIProgressBar::DrawData()
{
    // ������������������������������������
    // �v���O���X�o�[�̃t���[���摜��ݒ�
    // ������������������������������������
    if (ImGui::TreeNode("frame_")) {

        // ���݂̃t�@�C���p�X��\��
        ImGui::Text("imageFilePath_:%s", frameImage_.filePath_.c_str());
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
                frameImage_.filePath_ = ofn.lpstrFile;
                frameImage_.filePath_ = FileManager::GetAssetsRelativePath(frameImage_.filePath_);
                FileManager::ReplaceBackslashes(frameImage_.filePath_);
                SetCurrentDirectory(defaultCurrentDir);

                // �t���[���摜��ǂݍ���
                frameImage_.handle_ = Image::Load(frameImage_.filePath_);
            }
            else {
                ImGui::TreePop();
                return;
            }
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("gauge_")) {

        // ������������������������������������
        // �Q�[�W�\���F��ݒ�
        // ������������������������������������
        ImVec4 inputCol = { REFERENCE_XMFLOAT3(gaugeImage_.color_),0 };
        ImGui::ColorEdit4("Color", (float*)&inputCol);
        gaugeImage_.color_ = { REFERENCE_XMFLOAT3(inputCol) };

        // ������������������������������������������������������������
        // �Q�ƃQ�[�W�R���|�[�l���g��I��
        // ������������������������������������������������������������
        vector<gauge> gauges;
        vector<StageObject*> objects = ((Stage*)FindObject("Stage"))->GetStageObjects();

        //�X�e�[�W���̃I�u�W�F�N�g���Q�Ƃ��Ă��ׂẴQ�[�W�R���|�[�l���g���擾
        for (auto obj : objects) {
            //HP�Q�[�W��T���Ď擾
            auto healthGaugeComp = obj->FindComponent(HealthGauge);
            if (healthGaugeComp.empty() == false)
                for (auto healthGaugeComp : healthGaugeComp) {
                    //�z��ɃI�u�W�F�N�g�ƃR���|�[�l���g����ɂ܂Ƃ߂ē����H
                    auto gaugeComp = dynamic_cast<Component_Gauge*>(healthGaugeComp);
                    if (gaugeComp == nullptr)continue;
                    gauges.push_back({ obj,gaugeComp,obj->GetObjectName(),gaugeComp->GetName() });
                }
            //�Z�Q�[�W...
        }
        static int select = 0;

        // select �͈̔͂��`�F�b�N
        if (select >= gauges.size()) {
            select = 0;
        }

        //�R���{�{�b�N�X��gauge�̃R���e�i�ɂ���select�Ԗڂ̃R���|�[�l���g�𕶎���ɕϊ����Ď󂯎��H
        if (gauges.empty() == false)
            if (ImGui::BeginCombo("gauge_", referenceGauge_.thisName_.c_str())) {
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

                // select �͈̔͂��`�F�b�N���Ă���ݒ�
                if (select < gauges.size()) {
                    referenceGauge_ = gauges[select];
                }
            }

        ImGui::TreePop();
    }

    // �Q�[�W�̐���
    ImGui::DragFloat("Current Value", &now_, 1.f, max_);
    ImGui::DragFloat("Max Value", &max_, 1.f);
    ImGui::ProgressBar(now_ / max_, ImVec2(0.0f, 0.0f));

}


void UIProgressBar::SetGaugeMaxValue(float _maxValue)
{
    max_ = _maxValue;
}

void UIProgressBar::SetGaugeCurrentValue(float _nowValue) {
    now_ = _nowValue;
}
