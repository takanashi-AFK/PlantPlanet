
#include "UIProgressBar.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/Global.h"
#include "../../../Engine/ResourceManager/Image.h"
#include "../../../Engine/DirectX/Direct3D.h"
#include "../Stage/Stage.h"
#include "../../../Game/Objects/Stage/StageObject.h"
#include "../../../Game/Objects/Stage/Components/Component.h"

namespace {
    const string DEFAULT_FRAME_IMAGE = "Images/Bar_Frame.png";
    const string DEFAULT_GAUGE_IMAGE = "Images/Bar_Gauge.png";
}

UIProgressBar::UIProgressBar(string _name, UIObject* parent,int _layerNum)
    :UIObject(_name, UIType::UI_PROGRESSBAR, parent, _layerNum), max_(nullptr), now_(nullptr)
{
}

void UIProgressBar::Initialize()
{
    // �t���[����ݒ�
    {
        // �t���[���摜��ǂݍ���
        frameImage_.Load(DEFAULT_FRAME_IMAGE);

        // �t���[����transform��ݒ� ���f�t�H���g�̈ʒu�A��]�A�g��k��
        frameImage_.transform_ = transform_;
    }

    // �Q�[�W��ݒ�
    {
        // �Q�[�W�摜��ǂݍ���
        gaugeImage_.Load(DEFAULT_GAUGE_IMAGE);

        // �Q�[�W��transform��ݒ� ���f�t�H���g�̈ʒu�A��]�A�g��k��
        gaugeImage_.transform_ = transform_;

        // �Q�[�W�̐F��ݒ�
        gaugeImage_.color_ = { 1.0f,1.0f,1.0f };
    }
}

void UIProgressBar::Update()
{
    // �`��ptransform���I�u�W�F�N�g��transform�ōX�V
    frameImage_.transform_ = transform_;
    gaugeImage_.transform_ = transform_;

    // �Q�[�W�̃X�P�[�����Q�Ƃ����l����ɐݒ�
    {
		// �Q�[�W�̃X�P�[����ݒ� �����ݒl/�ő�l
        if (max_ != nullptr || now_ != nullptr)gaugeImage_.transform_.scale_.x = frameImage_.transform_.scale_.x * (*now_ / *max_);

		// �Q�[�W�̃X�P�[����0�ȉ��̏ꍇ�́A0�ɐݒ�
		if (gaugeImage_.transform_.scale_.x <= 0)gaugeImage_.transform_.scale_.x = 0;
	}
}

void UIProgressBar::Draw()
{
    // �Q�[�W�摜���ǂݍ��܂�Ă���ꍇ
    if (gaugeImage_.IsAvailable()) {

		// �Q�[�W�̉摜��`��
        Image::SetTransform(gaugeImage_.handle_, gaugeImage_.transform_);
        Image::Draw(gaugeImage_.handle_, Direct3D::SHADER_BAR, gaugeImage_.color_);
    }

    // �t���[���摜���ǂݍ��܂�Ă���ꍇ
    if (frameImage_.IsAvailable()) {

        // �t���[���̉摜��`��
		Image::SetTransform(frameImage_.handle_, frameImage_.transform_);
		Image::Draw(frameImage_.handle_);
    }
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
}

void UIProgressBar::Load(json& loadObj)
{
    // �Q�[�W�摜�̏���Ǎ�
    if (loadObj.contains("gaugeImage_")) {
		
        // �Q�[�W�摜��ǂݍ���
        if (loadObj["gaugeImage_"].contains("filePath_")) gaugeImage_.Load(loadObj["gaugeImage_"]["filePath_"].get<string>());

        // �Q�[�W�̐F���擾
        if (loadObj["gaugeImage_"].contains("color_"))gaugeImage_.color_ = { loadObj["gaugeImage_"]["color_"][0].get<float>(),loadObj["gaugeImage_"]["color_"][1].get<float>(), loadObj["gaugeImage_"]["color_"][2].get<float>() };
	}   

    // �t���[���摜�̏���Ǎ�
    if (loadObj.contains("frameImage_")) {

        // �t���[���摜��ǂݍ���
        if (loadObj["frameImage_"].contains("filePath_"))frameImage_.Load(loadObj["frameImage_"]["filePath_"].get<string>());
	}
}

void UIProgressBar::DrawData()
{
    // �`����̐ݒ�
    if (ImGui::TreeNode("draw")) {
        
        // �t���[���̐ݒ�
        if(ImGui::TreeNode("frame")){

			// �t���[���摜�̕ύX
			ImGui::Text("image : %s", frameImage_.filePath_.c_str()); ImGui::SameLine();
			if (ImGui::SmallButton("...")) {

                // �摜�t�@�C����I�����ēǂݍ���
				string inputFilePath{};
				if (GetImageFilePathFromExplorer(inputFilePath)) frameImage_.Load(inputFilePath);
			}

			ImGui::TreePop();
		}

        // �Q�[�W�̐ݒ�
        if (ImGui::TreeNode("gauge")) {

			// �Q�[�W�摜�̕ύX
			ImGui::Text("image : %s", gaugeImage_.filePath_.c_str()); ImGui::SameLine();
			if (ImGui::SmallButton("...")) {

                // �摜�t�@�C����I�����ēǂݍ���
                string inputFilePath{};
                if (GetImageFilePathFromExplorer(inputFilePath)) gaugeImage_.Load(inputFilePath);
			}

			// �Q�[�W�̐F��ݒ�
			ImGui::ColorEdit3("color", &gaugeImage_.color_.x);

			ImGui::TreePop();
		}
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("value")) {
        
		// �ő�l�ƌ��ݒl��ݒ�
		if(max_ != nullptr)ImGui::DragFloat("Max", max_);
		if(now_ != nullptr)ImGui::DragFloat("Now", now_);

		ImGui::TreePop();
    }
}

bool UIProgressBar::GetImageFilePathFromExplorer(string& _filePath) const
{
    //���݂̃J�����g�f�B���N�g�����o���Ă���
    char defaultCurrentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

    // �ǉ�����I�u�W�F�N�g�̃��f���t�@�C���p�X��ݒ�
    string filePath{}; {
        // �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
        OPENFILENAME ofn; {
            TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
            ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
            ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
            ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
            ofn.lpstrFile[0] = '\0'; // ������
            ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
            ofn.lpstrFilter = TEXT("PNG�t�@�C��(*.png)\0*.png\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iFBX�t�@�C���̂ݕ\���j
            ofn.nFilterIndex = 1; // �����I������t�B���^�[
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
            ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
        }

        // �t�@�C����I������_�C�A���O�̕\��
        if (GetOpenFileName(&ofn) == TRUE) {
            // �t�@�C���p�X���擾
            filePath = ofn.lpstrFile;

            // �J�����g�f�B���N�g������̑��΃p�X���擾
            filePath = FileManager::GetAssetsRelativePath(filePath);

            // ���������"\\"��"/"�ɒu��
            FileManager::ReplaceBackslashes(filePath);

            // �f�B���N�g����߂�
            SetCurrentDirectory(defaultCurrentDir);
        }
        else {
            // �f�B���N�g����߂�
			SetCurrentDirectory(defaultCurrentDir);
			return false;
        }
    }

    _filePath = filePath;
    return true;

}

bool ProgressBarImage::Load(string _filepath)
{
    // �t�@�C���p�X��ݒ�
    filePath_ = _filepath;

    // �摜�̓ǂݍ���
    handle_ = Image::Load(_filepath);

    // �摜�̓ǂݍ��݂ɐ����������ǂ�����Ԃ�
    return handle_ >= 0;
}

bool ProgressBarImage::IsAvailable() const
{
    // �摜�̃n���h�����L�����ǂ�����Ԃ�
	return handle_ >= 0;
}
