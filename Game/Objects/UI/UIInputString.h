#pragma once

#include "UIObject.h"

class Text;

class UIInputString : public UIObject
{
private:
	string currentInput_;				// 入力された文字列
	bool isComplete_;					// 入力完了フラグ
	std::map<int, char> keyToCharMap_;	// スキャンコードと対応する文字

	Text* pText_;						// テキストオブジェクト
	float size_;						// テキストのサイズ
	bool isOpenChangeFontWindow_;		// フォント変更ウィンドウを開くかどうか

	struct FontData { string filePath; XMINT2 charSize; int rowLength; } fontData_;	// フォントデータ

public:
	/// <summary> コンストラクタ </summary>
	UIInputString(string _name, UIObject* parent, int _layerNum);

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
	/// <summary> テキストのサイズを設定 </summary>
	void SetSize(float size) { size_ = size; }

	/// <summary> 入力完了フラグを設定 </summary>
	void SetComplete(bool isComplete) { this->isComplete_ = isComplete; }

	/// <summary> 入力された文字列を設定 </summary>
	void SetInputString(string input) { currentInput_ = input; }

/*
getter :*/
	/// <summary> テキストのサイズを取得 </summary>
	float GetSize() const { return size_; }

	/// <summary> 入力された文字列を取得 </summary>
	string GetInputString() const { return currentInput_; }

/*
predicate :*/
	/// <summary> 入力完了フラグを取得 </summary>
	bool IsComplete() const{ return isComplete_; }

private:
	/// <summary> フォント変更ウィンドウ </summary>
	void ChangeFontWindow();

	/// <summary> フォントファイルパスをエクスプローラから取得 </summary>
	bool GetFontFilePathFromExplorer(string& _filePath) const;

	/// <summary> キーマッピングの初期化 </summary>
	void InitializeKeyMapping();
};

