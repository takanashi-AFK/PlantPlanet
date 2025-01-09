#pragma once
#include "UIObject.h"

class UIImage : public UIObject
{
private:
	string imageFilePath_;	// 画像ファイルパス
	int imageHandle_;		// 画像ハンドル
	bool isDraw_;			// 描画するかどうか
public:
	/// <summary> コンストラクタ </summary>
	UIImage(string _name, UIObject* parent, int _layerNum);
	
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
	/// <summary> 画像ファイルパスを設定 </summary>
	void SetImage(string _imageFilePath);

	/// <summary> 描画するかどうかを設定 </summary>
	void SetEnable(bool _enable) { isDraw_ = _enable; }

/*
getter :*/
	string GetImageFilePath() const { return imageFilePath_; }
/*
predicate :*/
	/// <summary> 描画するかどうかを取得 </summary>
	bool IsEnable() const { return isDraw_; }
};

