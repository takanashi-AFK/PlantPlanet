#pragma once
#include"../../Engine/GameObject/GameObject.h"

class Scene_End :public GameObject
{
private:
	float count_;		// 経過時間

public:
	/// <summary> コンストラクタ </summary>
	Scene_End(GameObject* parent_);

	/// <summary> 初期化 </summary>
	void Initialize() override;

	/// <summary> 更新 </summary>
	void Update() override;

	/// <summary> 描画 </summary>
	void Draw() override;

	/// <summary> 解放 </summary>
	void Release() override;
};


