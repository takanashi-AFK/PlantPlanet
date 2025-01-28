#pragma once
#include "UIObject.h"
#include"../../../Engine/DirectX/Direct3D.h"

class UIButton : public UIObject
{
private:
	string imageFilePath_; // 画像ファイルパス
	int imageHandle_;		// 画像ハンドル
	Direct3D::SHADER_TYPE shaderType_;

	string defaultImageFilePath_; // デフォルト画像ファイルパス
	int defaultImageHandle_;		// デフォルト画像ハンドル

	string selectImageFilePath_; // 選択画像ファイルパス
	int selectImageHandle_;		// 選択画像ハンドル

	int arrayPlaceX_;
	int arrayPlaceY_;
	bool isAutoShader_;
	bool isChangeImage_;
	bool isSelectable_;

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


/*
setter :*/
	/// <summary> 画像ファイルパスを設定 </summary>
	void SetImage(string _imageFilePath);
	void SetShader(Direct3D::SHADER_TYPE type);
	void SetArrayPlace(int x, int y);
	void SetAutoShader(bool flag) { isAutoShader_ = flag; };
	void SetSelctable(bool flag) { isSelectable_ = flag; };
/*
getter */

	//この配列の中にあるボタンで一番上かつ選択されているボタンを取得
	static UIButton* GetTopSelectingUI(vector<UIObject*> list);

	void GetArrayPlace(int* x, int* y) const;

	string GetImageFilePath() const { return imageFilePath_; }

	Direct3D::SHADER_TYPE  GetShader()const;

	// このボタンにマウスが乗っているか
	bool GetIsMouseOverThisButton();

	bool IsSelectable() const { return isSelectable_; };
/*
setter :*/
	/// <summary> 画像ファイルパスを設定 </summary>

	void SetSelectImage(string _imageFilePath);
	void SetDefaultImage(string _imageFilePath);
/*
predicate :*/
	/// <summary> ボタンがクリックされたか </summary>
	bool OnClick();

	/// <summary> マウスがボタンの上にあるか </summary>
	bool IsMouseOver(XMFLOAT2 _position);

	void UnselectShader();
	void SelectShader();
private:
	/// <summary> 画像座標を画面座標に変換 </summary>
	void ConvertToImageCoordinates(XMFLOAT2& _position);
};

