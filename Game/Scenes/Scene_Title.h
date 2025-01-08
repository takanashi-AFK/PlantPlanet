#pragma once

#include"../../Engine/GameObject/GameObject.h"

class Scene_Title:public GameObject
{
private:
	bool isFirstSelectButton_;
public:
	/// <summary> コンストラクタ </summary>
	Scene_Title(GameObject* parent);

	/// <summary> 初期化 </summary>
	void Initialize() override;

	/// <summary> 更新 </summary>
	void Update() override;

	/// <summary> 描画 </summary>
	void Draw() override;

	/// <summary> 解放 </summary>
	void Release() override;
};

