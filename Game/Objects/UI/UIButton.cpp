#include "UIButton.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/ResourceManager/Image.h"
#include "../../../Engine/DirectX/Direct3D.h"
#include "../../../Engine/DirectX/Input.h"
#include "../../../Engine/Global.h"

using namespace FileManager;

UIButton::UIButton(string _name, GameObject* parent)
	: UIObject(_name, UIType::UI_BUTTON, parent)
{
}

void UIButton::Initialize()
{
}

void UIButton::Update()
{
}

void UIButton::Draw()
{
}

void UIButton::Release()
{
}

void UIButton::Save(json& saveObj)
{
	saveObj["imageFilePath_"] = imageFilePath_;
}

void UIButton::Load(json& loadObj)
{
	if(loadObj.contains("imageFilePath_"))imageFilePath_ = loadObj["imageFilePath_"].get<string>();
}

void UIButton::DrawData()
{
    if (ImGui::TreeNode("imageFilePath_")) {

		ImGui::Text("imageFilePath_:%s", imageFilePath_.c_str());
		ImGui::SameLine();

        if (ImGui::SmallButton("...")) {
            //���݂̃J�����g�f�B���N�g�����o���Ă���
            char defaultCurrentDir[MAX_PATH];
            GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

            // �ǉ�����I�u�W�F�N�g�̃��f���t�@�C���p�X��ݒ�
            {
                // �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
                OPENFILENAME ofn; {
                    TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
                    ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
                    ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
                    ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
                    ofn.lpstrFile[0] = '\0'; // ������
                    ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
                    ofn.lpstrFilter = TEXT("PNG�t�@�C��(*.fbx)\0*.png\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iFBX�t�@�C���̂ݕ\���j
                    ofn.nFilterIndex = 1; // �����I������t�B���^�[
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
                    ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
                }

                // �t�@�C����I������_�C�A���O�̕\��
                if (GetOpenFileName(&ofn) == TRUE) {
                    // �t�@�C���p�X���擾
                    imageFilePath_ = ofn.lpstrFile;

                    // �J�����g�f�B���N�g������̑��΃p�X���擾
                    imageFilePath_ = FileManager::GetAssetsRelativePath(imageFilePath_);

                    // ���������"\\"��"/"�ɒu��
                    FileManager::ReplaceBackslashes(imageFilePath_);

                    // �f�B���N�g����߂�
                    SetCurrentDirectory(defaultCurrentDir);

                    // �摜��ǂݍ���
                    SetImage(imageFilePath_);
                }
                else {
                    return;
                }
            }
        }
        ImGui::TreePop();
    }

}

void UIButton::SetImage(string _imageFilePath)
{
	imageFilePath_ = _imageFilePath;
	imageHandle_ = Image::Load(_imageFilePath);
}

bool UIButton::OnClick()
{
    // �}�E�X�̍��W���擾
    XMFLOAT2 mousePos = { Input::GetMousePosition().x,Input::GetMousePosition().y };

    // �}�E�X�̍��W���摜�̍��W�ɕϊ�
    ConvertToImageCoordinates(mousePos);

    //�}�E�X�̃|�W�V�������摜�̗̈�ɓ����Ă��邩���N���b�N�������ꂽ��
    return IsMouseOver(mousePos) && Input::IsMouseButtonDown(0);
}

bool UIButton::IsMouseOver(XMFLOAT2 _mousePosition)
{
	// �}�E�X�J�[�\���̍��W���擾
	XMFLOAT2 imagesize = Image::GetSize(imageHandle_);
    float imageHalfWidth = imagesize.x /2;
    float imageHalfHeight = imagesize.y /2;

    XMFLOAT2 center = { transform_.position_.x,transform_.position_.y };

    float scaleX = transform_.scale_.x;
    float scaleY = transform_.scale_.y;

    float top = center.y - ((imageHalfHeight * 2) * scaleY);
    float bottom = center.y + ((imageHalfHeight * 2) * scaleY);
    float left = center.x - ((imageHalfWidth * 2) * scaleX);
    float right = center.x + ((imageHalfWidth * 2) * scaleX);

    // ����͈͓��Ƀ}�E�X�J�[�\���������Ă��邩�ǂ�����Ԃ�
    return (_mousePosition.x >= left && _mousePosition.x <= right && _mousePosition.y >= top && _mousePosition.y <= bottom);
	return false;
}

void UIButton::ConvertToImageCoordinates(XMFLOAT2& _position)
{
    int scWidth = Direct3D::screenWidth_;
    int scHeight = Direct3D::screenHeight_;

#ifdef _DEBUG
    scWidth = scWidth * 0.7;
    scHeight = scHeight * 0.7;
#endif // _DEBUG

    // �}�E�X�̍��W���摜�̍��W�ɕϊ�
    _position.x = (float)(_position.x * 2.0f) / (float)scWidth - 1.0f;
    _position.y = 1.0f - (float)(_position.y * 2.0f) / (float)scHeight;

    ImGui::Text("mousePos_ x:%f y:%f", _position.x, _position.y);
    ImGui::Text("screenWidth_:%d screenHeight_:%d", scWidth, scHeight);
}