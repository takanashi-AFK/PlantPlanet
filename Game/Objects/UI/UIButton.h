#pragma once
#include "UIObject.h"
#include"../../../Engine/DirectX/Direct3D.h"

class UIButton : public UIObject
{
private:
	string imageFilePath_; // 画像ファイルパス
	int imageHandle_;		// 画像ハンドル
	Direct3D::SHADER_TYPE shaderType_;

	int arrayPlaceX_;
	int arrayPlaceY_;
	bool isSetShader_;

public:
	/// <summary> コンストラクタ </summary>
	UIButton(string _name, UIObject* parent, int _layerNum);
	~UIButton() override;

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

	void SetArrayPlace(int x, int y);
	void GetArrayPlace(int* x, int* y) const;

	string GetImageFilePath() const { return imageFilePath_; }

	void SetShader(Direct3D::SHADER_TYPE type);
	Direct3D::SHADER_TYPE  GetShader()const;

	bool GetIsMouseOverThisButton();
/*
setter :*/
	/// <summary> 画像ファイルパスを設定 </summary>
	void SetImage(string _imageFilePath);

/*
predicate :*/
	/// <summary> ボタンがクリックされたか </summary>
	bool OnClick();

	/// <summary> マウスがボタンの上にあるか </summary>
	bool IsMouseOver(XMFLOAT2 _position);

	static constexpr int ALPHA_MAX = 256;
	static constexpr int ALPHA_MIDDLE = 192;

private:
	/// <summary> 画像座標を画面座標に変換 </summary>
	void ConvertToImageCoordinates(XMFLOAT2& _position);
};

