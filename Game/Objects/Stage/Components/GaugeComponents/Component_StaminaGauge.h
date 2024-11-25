#pragma once
#include "Component_Gauge.h"
class Component_Timer;
class Component_StaminaGauge :public Component_Gauge
{
	private:
	float stamina_;	//前回のスタミナ
	float shaderChangeTime_;	//シェーダーを変更する時間
	bool isLock_;	//ロックフラグ
	bool isUseStamina_;	//スタミナを使用したか
	Component_Timer* timer;

	public:
		/// <summary> コンストラクタ </summary>
		Component_StaminaGauge(string _name, StageObject* _holder, Component* _parent);

		/// <summary> 初期化 </summary>
		void Initialize() override;

		/// <summary> 更新 </summary>
		void Update() override;

		/// <summary> 解放 </summary>
		void Release() override;

		/// <summary>
		/// スタミナを消費する
		/// </summary>
		/// <param name="_useValue">何単位消費するか</param>
		/// <returns>消費できたらtrue </returns>
		void UseStamina(float _useValue);

		bool CanUseStamina(float _useValue);

		/// <summary> スタミナを回復する </summary>
		void RecoverStamina(float _recoverValue);

		/// <summary> ロック </summary>
		void RecoverLock() { isLock_ = true; }

		/// <summary> アンロック </summary>
		void RecoverUnlock() { isLock_ = false; }


};

