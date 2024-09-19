#pragma once

#include "UIObject.h"
#include "../../../Engine/Json/JsonReader.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Game/Objects/Stage/Components/GaugeComponents/Component_HealthGauge.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

// �v���O���X�o�[�̉摜���\����
struct ProgressBarImage{
    string filePath_;       // �t�@�C���p�X
    int handle_;            // �n���h��
    Transform transform_;   // �ʒu�A��]�A�g��k��
    XMFLOAT3 color_;        // �F

    bool Load(string _filepath);    // �摜�̓ǂݍ���
    bool IsAvailable() const;       // �摜���ǂݍ��܂�Ă��邩
};

class UIProgressBar : public UIObject
{
private:
    float* max_;                    // �ő�l
    float* now_;	                // ���ݒl

    ProgressBarImage frameImage_;   // �t���[���摜
    ProgressBarImage gaugeImage_;   // �Q�[�W�摜
public:
    /// <summary> �R���X�g���N�^ </summary>
    UIProgressBar(std::string _name, UIObject* parent, int _layerNum);
    
    /// <summary> ������ </summary>
    void Initialize() override;
    
    /// <summary> �X�V </summary>
    void Update() override;
    
    /// <summary> �`�� </summary>
    void Draw() override;
    
    /// <summary> ��� </summary>
    void Release() override;

    /// <summary> �ۑ� </summary>
    void Save(json& saveObj) override;
    
    /// <summary> �Ǎ� </summary>
    void Load(json& loadObj) override;
    
    /// <summary> ImGui�\�� </summary>
    void DrawData() override;

/*
setter :*/
    /// <summary> �ő�l��ݒ� </summary>
    void SetMax(float* _max) { max_ = _max; }

    /// <summary> ���ݒl��ݒ� </summary>
    void SetNow(float* _now) { now_ = _now; }

    /// <summary> �ő�l�ƌ��ݒl��ݒ� </summary>
    void SetProgress(float* _now ,float* _max){ now_ = _now; max_ = _max; }

private:
    /// <summary> �摜�t�@�C���p�X���G�N�X�v���[���[����擾 </summary>
    bool GetImageFilePathFromExplorer(string& _filePath) const;

};