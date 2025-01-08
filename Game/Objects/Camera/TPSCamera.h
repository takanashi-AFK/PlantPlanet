#pragma once
#include "../../../Engine/GameObject/GameObject.h"
#include "../../../Engine/Json/JsonReader.h"

class TPSCamera : public GameObject
{
private:
	XMFLOAT2 angle_;		// 回転角度
	float sensitivity_;		// 感度
	GameObject* pTarget_;	// ターゲット
	string targetName_;		// ターゲット名
	bool isActive_;			// 有効無効
	float targetHeight_;	// カメラの高さ
	float targetDistance_;	// カメラの距離
	XMVECTOR prevAxis_;		// 前回の軸
	std::list<int> hModels_;

public:
	/// <summary> コンストラクタ </summary>
	TPSCamera(GameObject* parent);

	/// <summary> 初期化 </summary>
	void Initialize() override;
	
	/// <summary> 更新 </summary>
	void Update() override;
	
	/// <summary> 描画 </summary>
	void Draw() override;
	
	/// <summary> 解放 </summary>
	void Release() override;

	/// <summary>　保存 </summary>
	void Save(json& saveObj);

	/// <summary>　読込 </summary>
	void Load(json& loadObj);

	/// <summary> ImGui表示 </summary>
	void DrawData();

/*
setter :*/
	/// <summary> カメラの有効無効の設定 </summary>
	void SetActive(bool isActive) { isActive_ = isActive; }
	
	/// <summary> カメラの感度の設定 </summary>
	void SetSensitivity(float sensitivity) { sensitivity_ = sensitivity; }

	/// <summary> カメラの高さの設定 </summary>
	void SetTargetHeight(float targetHeight) { targetHeight_ = targetHeight; }

	/// <summary> カメラの距離の設定 </summary>
	void SetTargetDistance(float targetDistance) { targetDistance_ = targetDistance; }

	/// <summary> ターゲットの設定 </summary>
	void SetTarget(GameObject* target) { pTarget_ = target; }

	/// <summary> ターゲット名の設定 </summary>
	void SetTargetName(string targetName) { targetName_ = targetName; }

	/// <summary> 回転角度の設定 </summary>
	void SetAngle(XMFLOAT2 angle) { angle_ = angle; }

	void SetRayCastList(std::list<int> list);
/*
getter :*/
	/// <summary> カメラの感度の取得 </summary>
	float GetSensitivity() const { return sensitivity_; }

	/// <summary> カメラの高さの取得 </summary>
	float GetTargetHeight() const { return targetHeight_; }

	/// <summary> カメラの距離の取得 </summary>
	float GetTargetDistance() const { return targetDistance_; }

	/// <summary> ターゲットの取得 </summary>
	GameObject* GetTarget() const { return pTarget_; }

	/// <summary> ターゲット名の取得 </summary>
	string GetTargetName() const { return targetName_; }
	
	/// <summary> 回転角度の取得 </summary>
	XMFLOAT2 GetAngle() const { return angle_; }

	std::list<int> GetRayCastList();
/*
predicate :*/
	/// <summary> カメラが有効かどうか </summary>
	bool IsActive() const { return isActive_; }

	/// <summary> ターゲットが設定されているかどうか </summary>
	bool IsTarget() const { return pTarget_ != nullptr; }

private:
	/// <summary> 回転角度の計算 </summary>
	void CalcRotateAngle(XMFLOAT3 _mouseMove,XMFLOAT3 _padMove);

	/// <summary> カメラの位置と注視点の計算 </summary>
	void CalcCameraPositionAndTarget(XMFLOAT3& _cameraPosition,XMFLOAT3& _camaraTarget);
};

