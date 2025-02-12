#pragma once
#include "UIObject.h"
#include "../../../Engine/Global.h"

class Text;

enum TIMER_DRAW_TYPE {
	TEXT,
	CIRCLE_GAUGE,
};

struct CircleGauge
{
/*
value :*/
	int imageHandle = -1;	// 画像ハンドル
	string imageFilePath;	// 画像ファイルパス
	float startAngle = 90;	// 開始角度
	float endAngle = 90;	// 終了角度
/*
function :*/
	/// <summary> 読込 </summary>
	bool Load(string imageFilePath);

	/// <summary> 更新 </summary>
	void Update(float progress);

	/// <summary> 描画 </summary>
	void Draw(Transform _transform);
};

class UITimer : public UIObject
{
private:
	float count_;				// 現在値
	float max_;					// 最大値

	bool isActive_;				// タイマーが動いているか
	
	TIMER_DRAW_TYPE drawType_;	// 描画タイプ
	Text* pText_;				// テキストオブジェクト
	float size_;				// テキストのサイズ
	
	CircleGauge circleGauge_;	// ゲージ
	bool isOpenChangeFontWindow_;

	struct FontData { string filePath; XMINT2 charSize; int rowLength; } fontData_;	// フォントデータ

public:
	/// <summary> コンストラクタ </summary>
	UITimer(string _name, UIObject* parent, int _layerNum);

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

	/// <summary> タイマーの開始 </summary>
	void StartTimer() { isActive_ = true; }

	/// <summary> タイマーの停止 </summary>
	void StopTimer() { isActive_ = false; }

	/// <summary> タイマーの終了 </summary>
	void EndTimer() { count_ = 0; isActive_ = false; }

/*
setter :*/
	/// <summary> タイマーの設定 ※秒 </summary>
	void SetTimer(float _sec) { max_ = _sec * FPS; count_ = max_; }

	/// <summary> タイマーのリセット </summary>
	void ResetTimer() { count_ = max_; isActive_ = false; }

	/// <summary> タイマーのリスタート </summary>
	void RestartTimer() { count_ = max_; isActive_ = true; }

/*
getter :*/
	/// <summary> タイマーの取得 </summary>
	float GetTimer() const { return count_; }

	/// <summary> 秒数の取得 </summary>
	float GetSeconds() const { return count_ / FPS; }

	/// <summary> 最大値の取得 </summary>
	float GetMaxTimer() const { return max_; }

/*
predicate :*/
	/// <summary> タイマーが0になったか </summary>
	bool IsEnd() const { return count_ <= 0; }

	/// <summary> タイマーが最大値か </summary>
	bool IsMax() const { return count_ >= max_; }

private:
	/// <summary> テキストの描画 </summary>
	void DrawTextTimer();

	/// <summary> ゲージの描画 </summary>
	void DrawCircleGaugeTimer();

	/// <summary> ファイルパスから画像を取得 </summary>
	bool GetImageFilePathFromExplorer(string& _filePath) const;

	/// <summary> フォントファイルパスをエクスプローラから取得 </summary>
	bool GetFontFilePathFromExplorer(string& _filePath) const;
	void ChangeFontWindow();

};

