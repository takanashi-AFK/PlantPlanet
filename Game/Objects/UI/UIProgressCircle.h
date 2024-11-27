#pragma once
#include "UIObject.h"

class UIProgressCircle : public UIObject
{
private:
	float max_;                    // 最大値
	float now_;	                // 現在値

	float startAngle_;              // 開始角度
	float endAngle_;                // 終了角度

	string imageFilePath_;          // 画像ファイルパス
	int imageHandle_;               // 画像ハンドル
	XMFLOAT3 color_;                // 色

public:
	/// <summary> コンストラクタ </summary>
	UIProgressCircle(std::string _name, UIObject* parent, int _layerNum);

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
	void SetProgress(float* _now, float* _max) { now_ = _now; max_ = _max; }

	/// <summary> 開始角度を設定 </summary>
	void SetStartAngle(float _angle) { startAngle_ = _angle; }

	/// <summary> 終了角度を設定 </summary>
	void SetEndAngle(float _angle) { endAngle_ = _angle; }

	/// <summary> 色を設定 </summary>
	void SetColor(XMFLOAT3 _color) { color_ = _color; }

	/// <summary> 使用する画像を設定 </summary>
	bool SetImage(string _imageFilePath);

/*
getter :*/
	/// <summary> 最大値を取得 </summary>
	float GetMax() const { return max_; }

	/// <summary> 現在値を取得 </summary>
	float GetNow() const { return now_; }

	/// <summary> 開始角度を取得 </summary>
	float GetStartAngle() const { return startAngle_; }

	/// <summary> 終了角度を取得 </summary>
	float GetEndAngle() const { return endAngle_; }

	/// <summary> 色を取得 </summary>
	XMFLOAT3 GetColor() const { return color_; }

	/// <summary> 画像ファイルパスを取得 </summary>
	string GetImageFilePath() { return imageFilePath_; }

	/// <summary> 画像ハンドルを取得 </summary>
	int GetImageHandle() { return imageHandle_; }

private:
	/// <summary> 画像ファイルパスをエクスプローラーから取得 </summary>
	string GetPNGFilePath();

};

