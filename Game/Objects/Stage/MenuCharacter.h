#pragma once
#include "StageObject.h"

class MenuCharacter :public StageObject
{
public:
	/// <summary> コンストラクタ </summary>
	MenuCharacter(GameObject* _parent);

	/// <summary> 初期化 </summary>
	void Initialize() override;

	/// <summary> 更新 </summary>
	void Update() override;

	/// <summary> 描画 </summary>
	void Draw() override;

	/// <summary> 解放 </summary>
	void Release() override {};

};

