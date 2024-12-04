#pragma once

#include "UIObject.h"
#include "../../../Engine/Json/JsonReader.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Game/Objects/Stage/Components/GaugeComponents/Component_HealthGauge.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

// プログレスバーの画像情報構造体
struct ProgressBarImage{
    string filePath_;       // ファイルパス
    int handle_;            // ハンドル
    Transform transform_;   // 位置、回転、拡大縮小
    XMFLOAT3 color_;        // 色

    bool Load(string _filepath);    // 画像の読み込み
    bool IsAvailable() const;       // 画像が読み込まれているか
};

class UIProgressBar : public UIObject
{
private:
    float max_;                    // 最大値
    float now_;	                // 現在値

    ProgressBarImage frameImage_;   // フレーム画像
    ProgressBarImage gaugeImage_;   // ゲージ画像
public:
    /// <summary> コンストラクタ </summary>
    UIProgressBar(std::string _name, UIObject* parent, int _layerNum);
    
    /// <summary> 初期化 </summary>
    void Initialize() override;
    
    /// <summary> 更新 </summary>
    void Update() override;
    
    /// <summary> 描画 </summary>
    void Draw() override;
    
    /// <summary> 解放 </summary>
    void Release() override;

    /// <summary> 保存 </summary>
    void Save(json& saveObj) override;
    
    /// <summary> 読込 </summary>
    void Load(json& loadObj) override;
    
    /// <summary> ImGui表示 </summary>
    void DrawData() override;

/*
setter :*/
    /// <summary> 最大値を設定 </summary>
    void SetMax(float _max) { max_ = _max; }

    /// <summary> 現在値を設定 </summary>
    void SetNow(float _now) { now_ = _now; }

    /// <summary> 最大値と現在値を設定 </summary>
    void SetProgress(float _now ,float _max){ now_ = _now; max_ = _max; }

private:
    /// <summary> 画像ファイルパスをエクスプローラーから取得 </summary>
    bool GetImageFilePathFromExplorer(string& _filePath) const;

};