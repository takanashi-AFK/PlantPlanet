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
    float* max_;    // �ő�l
    float* now_;	    // ���ݒl

    ProgressBarImage frameImage_;   // �t���[���摜
    ProgressBarImage gaugeImage_;   // �Q�[�W�摜
public:
    UIProgressBar(std::string _name, UIObject* parent, int _layerNum);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void Save(json& saveObj) override;
    void Load(json& loadObj) override;
    void DrawData() override;

    void SetMax(float* _max) { max_ = _max; }
    void SetNow(float* _now) { now_ = _now; }
    void SetProgress(float* _now ,float* _max){ now_ = _now; max_ = _max; }

private:
    bool GetImageFilePathFromExplorer(string& _filePath) const;
};