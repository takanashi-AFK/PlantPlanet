#pragma once
#include "Image.h"

class Text
{
private:
	int hPict_;						// ピクチャハンドル
	unsigned int width_, height_;	// 1文字分の幅と高さ
	char fileName_[MAX_PATH];		// ファイル名
	unsigned int rowLength_;		// 1行に並ぶ文字数

public:
	/// <summary> コンストラクタ </summary>
	Text();

	/// <summary> デストラクタ </summary>
	~Text();

	/// <summary> 初期化 </summary>
	HRESULT Initialize();

	//初期化（オリジナルの画像を使いたい時）
	//引数：fileName	画像ファイル名
	//引数：charWidth	画像の１文字の幅（ピクセル）
	//引数：charHeight	画像の１文字の高さ（ピクセル）
	//引数：rowLength	画像の中に横に何文字並んでいるか
	//戻値：成功／失敗
	HRESULT Initialize(const char* fileName, const unsigned int charWidth, const unsigned int charHeight, const unsigned int rowLength);

	//描画（文字列）
	//引数：x, y	表示位置（左上）
	//引数：str		表示したい文字列
	void Draw(int x, int y, const char* str ,uint8_t alpha = 255);

	//描画（整数値）
	//引数：x, y	表示位置（左上）
	//引数：value	表示したい値
	void Draw(int x, int y, int value ,uint8_t alpha = 255);

	///	<summary> 解放 </summary>
	void Release();

/*
setter :*/
	void SetScale(float sc) { scale = sc; }
/*
getter :*/
	unsigned int GetWidth() const { return width_; }
	unsigned int GetHeight()const  { return height_; }
	string GetFileName()const { return fileName_; }
	unsigned int GetRowLength()const { return rowLength_; }

private:
	float scale;
};

