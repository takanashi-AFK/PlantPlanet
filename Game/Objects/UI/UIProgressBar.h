#pragma once

#include "UIObject.h"
#include "../../../Engine/Json/JsonReader.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Game/Objects/Stage/Components/GaugeComponents/Component_HealthGauge.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class UIProgressBar : public UIObject
{
private:
    float max_, now_;  // �ő�l�ƌ��ݒl���i�[���郁���o�ϐ�

    struct FrameImage {
        string filePath_;
        int handle_;
        Transform transform_;
    }frameImage_;

    struct GaugeImage {
        string filePath_;
        int handle_;
        Transform transform_;
        XMFLOAT3 color_;
    }gaugeImage_;

    Component_HealthGauge* healthGauge_; // HealthGauge�̃|�C���^��ǉ�

    struct gauge {
        StageObject* holder_;
        Component_Gauge* this_;
        string holderName_;
        string thisName_;
    }referenceGauge_;

public:
    UIProgressBar(std::string _name, GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void Save(json& saveObj) override;
    void Load(json& loadObj) override;
    void DrawData() override;

    void SetGaugeMaxValue(float _maxValue);
    void SetGaugeCurrentValue(float _nowValue);
   
    void SetGauge(Component_HealthGauge* _gauge) { healthGauge_ = _gauge; }
};