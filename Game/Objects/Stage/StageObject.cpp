#include "StageObject.h"

// インクルード
#include "../../../Engine/Global.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/ResourceManager/Model.h"
#include "Stage.h"

#include"atlstr.h"

namespace {
	static bool isShowAddComponentWindow_ = false; // コンポーネント追加ウィンドウの表示フラグ
	static bool isShowSetComponentNameWindow_ = false; // コンポーネント追加ウィンドウの表示フラグ
	static ComponentType selectComponentType_; // 選択されたコンポーネントタイプ

	void DrawAddComponentWindow(StageObject* _holder)
	{
		ImGui::Begin("Components that can be added", &isShowAddComponentWindow_); {
			for (int i = 0; i < ComponentType::Max; i++) {
				if (ImGui::Selectable(ComponentTypeToString((ComponentType)i).c_str(), selectComponentType_ == i)) {

					// 選択しているコンポーネントタイプを代入
					selectComponentType_ = (ComponentType)i;

					// ウィンドウを閉じる
					isShowAddComponentWindow_ = false;

					// コンポーネント名を入力するウィンドウを表示
					isShowSetComponentNameWindow_ = true;	
				}
			}
		}
		ImGui::End();
	}

	void DrawSetComponentNameWindow(StageObject* _object)
	{
		ImGui::Begin("Set Component Name",&isShowSetComponentNameWindow_); {
            static char buffer[256] = "";
			ImGui::InputTextWithHint("##Input", "Input New name...", buffer, IM_ARRAYSIZE(buffer));
			ImGui::SameLine();
			if (ImGui::Button("create")) {

				// コンポーネントを作成
				_object->AddComponent(CreateComponent(buffer, selectComponentType_, _object));

				isShowSetComponentNameWindow_ = false;
				
				// バッファをクリア
				memset(buffer, 0, sizeof(buffer));

			}
		}
		ImGui::End();
	}
}


StageObject::StageObject(string _name, string _modelFilePath, GameObject* _parent)
	:GameObject(_parent, _name), modelFilePath_(_modelFilePath), modelHandle_(-1), myComponents_(), fallSpeed_(1), isOnGround_(false)
	,shaderType_(Direct3D::SHADER_3D), isColliding_(true)
{
}

bool StageObject::AddComponent(Component* _comp)
{
	// ヌルチェック
	if (_comp == nullptr) return false;

	// 初期化
	_comp->ChildIntialize();

	// コンポーネントのvectorに追加する
	myComponents_.push_back(_comp); 
	return true;
}

Component* StageObject::FindComponent(string _name)
{
	Component* result = nullptr;

	for (auto comp : myComponents_) {
		if (comp->GetName() == _name) {
			result = comp;
			break;
		}

		// 子コンポーネントを再帰的に検索
		result = comp->GetChildComponent(_name);
		if (result != nullptr)break;
	}
	return result;
}

vector<Component*> StageObject::FindComponent(ComponentType _type)
{
	vector<Component*> result;

	for (auto comp : myComponents_) {
		if (comp->GetType() == _type)result.push_back(comp);

		// 子コンポーネントを再帰的に検索
		auto childComps = comp->GetChildComponent(_type);
		result.insert(result.end(), childComps.begin(), childComps.end());
	}
	return result;
}

bool StageObject::DeleteComponent(Component* _comp)
{
	// イテレータに、"myComponents_"内で探したいデータを登録
	auto it = std::find(myComponents_.begin(), myComponents_.end(), _comp);

	// イテレータがリストの最後まで到達したら関数終了
	if (it == myComponents_.end()) return false;

	// イテレータのコンポーネントを消す
	myComponents_.erase(it); return true;
}

bool StageObject::DeleteAllComponent()
{
	// リスト内にある要素をすべて削除
	myComponents_.clear();return true;
}
void StageObject::OnGround(float _fallSpeed)
{
	// オブジェクトが既に地面に設置されているなら処理を抜ける
	if (!isOnGround_) return;

	float closestDist = FLT_MAX;  // 最短距離を保持
	bool dataHit = false;         // レイが地面に当たったかどうか

	// ステージオブジェクトを取得
	Stage* pStage = (Stage*)FindObject("Stage");
	if (pStage == nullptr) return;

	vector<StageObject*> stageObj = pStage->GetStageObjects();

	// モデルのサイズを考慮した足元位置（中心位置からモデル高さの半分下）
	float modelHeight = 3.5f;  // モデルの高さ
	XMFLOAT3 rayStart = transform_.position_ + onGroundOffset_;

	// 全てのオブジェクトに対してレイキャストを行う
	for (auto obj : stageObj) {
		// 自分自身のオブジェクトをスキップ
		if (obj->GetObjectName() == this->objectName_ || !obj->isColliding_)
			continue;

		int hGroundModel = obj->modelHandle_;

		// レイキャストデータの準備
		RayCastData data;
		data.start = rayStart;             // レイの発射位置（足元）
		data.dir = XMFLOAT3(0, -1, 0);     // レイの方向（下方向）

		// レイを発射して地面との距離を測定
		Model::RayCast(hGroundModel, &data);

		// レイが地面に当たったら
		if (data.hit) {
			// 最短距離の地面に更新
			if (data.dist < closestDist) {
				closestDist = data.dist;
				dataHit = true;
			}
		}
	}

	// レイが地面に当たっていれば、最短距離に基づいて位置を調整
	if (dataHit) {
		transform_.position_.y -= (closestDist - modelHeight / 2);
	}
}



void StageObject::CollisionWall()
{
	if (!isCollisionWall_) return;

	Stage* pStage = static_cast<Stage*>(FindObject("Stage"));
	if (pStage == nullptr) return;

	vector<StageObject*> stageObj = pStage->GetStageObjects();

	// 全てのオブジェクトに対して水平方向のレイキャストを行う
	for (auto obj : stageObj) {
		if (obj->GetObjectName() == this->objectName_ || !obj->isColliding_)
			continue;

		int hGroundModel = obj->modelHandle_;
		if (hGroundModel < 0) continue;

		std::vector<XMFLOAT3> directions = {
			XMFLOAT3(1, 0, 0),  // right
			XMFLOAT3(-1, 0, 0), // left
			XMFLOAT3(0, 0, 1),  // forward
			XMFLOAT3(0, 0, -1)  // backward
		};

		for (const auto& dir : directions) {
			RayCastData data;
			data.start = transform_.position_; // レイの発射位置
			data.start.y = transform_.position_.y + 0.5f;
			data.dir = dir; // レイの方向
			Model::RayCast(hGroundModel, &data); // レイを発射

			if (data.dist < 0.6f && data.dist > 0.0f) {
				if (dir.x != 0) {
					transform_.position_.x -= dir.x * (0.6f - data.dist);
				}
				else if (dir.z != 0) {
					transform_.position_.z -= dir.z * (0.6f - data.dist);
				}
			}
		}
	}
}
void StageObject::PlayAnimation(int _startFrame, int _endFrame, float _speed)
{
	Model::SetAnimFrame(modelHandle_,_startFrame, _endFrame, _speed);

}

void StageObject::Initialize()
{
	// モデルの読み込み
	modelHandle_ = Model::Load(modelFilePath_);
	assert(modelHandle_ >= 0);

	// 保有するコンポーネントの初期化処理
	for (auto comp : myComponents_)comp->ChildIntialize();
}

void StageObject::Update()
{
	// 保有するコンポーネントの更新処理
	for (auto comp : myComponents_)comp->ChildUpdate();
	OnGround(fallSpeed_);
	CollisionWall();
}

void StageObject::Draw()
{
	Direct3D::SetShader(shaderType_);

	// モデルの描画
	Model::SetTransform(modelHandle_, transform_);
	Model::Draw(modelHandle_);

	Direct3D::SetShader(Direct3D::SHADER_3D);

	int mh = Model::Load("Models/Player/blasterA.fbx");
	auto temppos = Model::GetBonePosition(modelHandle_, "mixamorig:LeftHand");
	//temppos = holder_->GetWorldScale();


	Transform temptrans = {};
	temptrans.position_ = temppos;
	temptrans.scale_ = Model::GetBoneScale (modelHandle_, "mixamorig:LeftHand");

	temptrans.rotate_ = Model::GetBoneRotation(modelHandle_, "mixamorig:LeftHand") ;

	Direct3D::SetShader(Direct3D::SHADER_3D);
	Model::SetTransform(mh, temptrans);
	Model::Draw(mh);
}

void StageObject::Release()
{
	// 保有するコンポーネントの開放処理
	for (auto comp : myComponents_) {
		comp->ChildRelease();
		delete comp;
	}
	myComponents_.clear();
}

void StageObject::OnCollision(GameObject* _target, Collider* _collider)
{
	// 保有するコンポーネントの衝突処理
	for (auto comp : myComponents_)comp->ChildOnCollision(_target, _collider);
}

void StageObject::Save(json& _saveObj)
{
	// 自身の変形行列情報を保存
	_saveObj["position_"] = { REFERENCE_XMFLOAT3(transform_.position_) };
	_saveObj["rotate_"] = { REFERENCE_XMFLOAT3(transform_.rotate_) };
	_saveObj["scale_"] = { REFERENCE_XMFLOAT3(transform_.scale_)};
	_saveObj["fallSpeed_"] = fallSpeed_;
	_saveObj["isOnGround_"] = isOnGround_;
	_saveObj["isCollisionWall_"] = isCollisionWall_;
	_saveObj["onGroundOffset_"] = { REFERENCE_XMFLOAT3(onGroundOffset_) };
	_saveObj["isColliding_"] = isColliding_;
	
	// 自身のモデルのファイルパスを保存
	_saveObj["modelFilePath_"] = modelFilePath_;

	// 陰影の表示フラグを保存
	_saveObj["shaderType_"] = (int)shaderType_;

	// 接地処理の情報を保存
	_saveObj["isOnGround_"] = isOnGround_;
	_saveObj["isCollisionWall_"] = isCollisionWall_;
	_saveObj["fallSpeed_"] = fallSpeed_;

	// コンポーネント情報を保存
	for (auto comp : myComponents_)comp->ChildSave(_saveObj["myComponents_"][comp->GetName()]);
}

void StageObject::Load(json& _loadObj)
{
	// 現在のすべてのコンポーネント情報を削除
	this->DeleteAllComponent();

	// 変形行列情報を読込
	for (auto& itr : _loadObj.items())
	{
		auto str = itr.key();
		int temp = 0;
	}
	if (_loadObj.contains("position_"))transform_.position_ = { _loadObj["position_"][0].get<float>(),_loadObj["position_"][1].get<float>(), _loadObj["position_"][2].get<float>() };
	if (_loadObj.contains("rotate_"))transform_.rotate_ = { _loadObj["rotate_"][0].get<float>(),_loadObj["rotate_"][1].get<float>(), _loadObj["rotate_"][2].get<float>() };
	if (_loadObj.contains("scale_"))transform_.scale_ = { _loadObj["scale_"][0].get<float>(),_loadObj["scale_"][1].get<float>(), _loadObj["scale_"][2].get<float>() };
	if (_loadObj.contains("fallSpeed_"))fallSpeed_ = _loadObj["fallSpeed_"];
	if (_loadObj.contains("isOnGround_"))isOnGround_ = _loadObj["isOnGround_"];
	if (_loadObj.contains("isCollisionWall_"))isCollisionWall_ = _loadObj["isCollisionWall_"];
	if (_loadObj.contains("onGroundOffset_"))onGroundOffset_ = { _loadObj["onGroundOffset_"][0].get<float>(),_loadObj["onGroundOffset_"][1].get<float>(), _loadObj["onGroundOffset_"][2].get<float>() };
	if (_loadObj.contains("isColliding_"))isColliding_ = _loadObj["isColliding_"];
	// モデルのファイルパスを読込
	modelFilePath_ = _loadObj["modelFilePath_"];

	// 陰影の表示フラグを読込
	if(_loadObj.contains("shaderType_"))shaderType_ = _loadObj["shaderType_"];

	// 接地処理の情報を読込
	if (_loadObj.contains("isOnGround_"))isOnGround_ = _loadObj["isOnGround_"];
	if (_loadObj.contains("isCollisionWall_"))isCollisionWall_ = _loadObj["isCollisionWall_"];
	if (_loadObj.contains("fallSpeed_"))fallSpeed_ = _loadObj["fallSpeed_"];

	// コンポーネントインスタンスを生成
	for (auto& obj : _loadObj["myComponents_"]) {
		
		// コンポーネントを生成
		Component* comp = CreateComponent(obj["name_"], obj["type_"], this);

		// 子コンポーネントを生成
		for (auto& child : obj["childComponents_"])comp->AddChildComponent(CreateComponent(child["name_"], child["type_"], this));

		// "myComponents_"に追加
		this->AddComponent(comp);
	}

	// コンポーネント情報を読込
	for (auto comp : myComponents_)comp->ChildLoad(_loadObj["myComponents_"][comp->GetName()]);
}

void StageObject::DrawData()
{
	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクト名を表示
	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	ImGui::Text(this->objectName_.c_str());
	ImGui::SameLine();

	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクトの削除ボタン
	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	if(ImGui::SmallButton("delete"))((Stage*)FindObject("Stage"))->DeleteStageObject(this);
	ImGui::Separator();

	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// 接地処理
	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	if (ImGui::TreeNode("OnGround")) {
		ImGui::Checkbox("isOnGround", &isOnGround_);
		ImGui::SameLine();
		ImGui::Checkbox("isCollisionWall",&isCollisionWall_);
		ImGui::Checkbox("isColliding_" ,&isColliding_);
		ImGui::DragFloat("fallSpeed", &fallSpeed_, 0.1f, 0.f, 1.f);
		ImGui::DragFloat3("onGroundOffset", &onGroundOffset_.x, 0.1f);
		ImGui::TreePop();
	}

	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// シェードの表示
	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	if (ImGui::TreeNode("shade")) {
		
		// fix: 存在しないシェーダーを入力可能なため修正が必要
		// fix: シェーダータイプがint型で表示されるため修正が必要

		constexpr uint8_t shader3d_MAX = Direct3D::SHADER_TYPE::SHADER_MAX;

#ifdef _DEBUG

		mutex_.lock();
		{
			ImGui::InputInt("shaderType_", (int*)&shaderType_);
			shaderType_ = shaderType_ < 0 ?static_cast<Direct3D::SHADER_TYPE>( Direct3D::SHADER_MAX-1) :static_cast<Direct3D::SHADER_TYPE>(shaderType_ % shader3d_MAX);
		}
		mutex_.unlock();

#endif

		CString shaderString(Direct3D::GetShaderName(shaderType_).c_str());
		ImGui::Text(shaderString.GetString());
		ImGui::TreePop();
	}

	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクトの名前を変更
	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	if (ImGui::TreeNode("name")) {
		ImGui::Text("Current name : %s", this->objectName_.c_str());
		char buffer[256] = "";
		if (ImGui::InputTextWithHint("##Input", "Input New name...", buffer, IM_ARRAYSIZE(buffer)))
			this->objectName_ = buffer;
		ImGui::TreePop();
	}

	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// 自身の変形情報を描画
	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	if (ImGui::TreeNode("transform_")) {
		ImGui::DragFloat3("position_", &transform_.position_.x, 0.1f);
		ImGui::DragFloat3("rotate_", &transform_.rotate_.x, 1.f, -360.f, 360.f);
		ImGui::DragFloat3("scale_", &transform_.scale_.x, 0.1f, 0.f, LONG_MAX);
		ImGui::TreePop();
	}

	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// コンポーネントの情報を描画
	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	if (ImGui::TreeNode("myComponents_")) {
		ImGui::SameLine();

		// コンポーネント追加ボタンを描画
		if (ImGui::SmallButton("AddComponent")) isShowAddComponentWindow_ = true;

		// 保有するコンポーネントの情報を描画
		for (auto comp : myComponents_)comp->ChildDrawData();
		ImGui::TreePop();
	}

	// コンポーネント追加ウィンドウを描画
	if (isShowAddComponentWindow_)DrawAddComponentWindow(this);
	if (isShowSetComponentNameWindow_)DrawSetComponentNameWindow(this);

}

StageObject* CreateStageObject(string _name, string _modelFilePath, GameObject* _parent)
{
	// オブジェクトのインスタンスを生成
	StageObject* obj = new StageObject(_name, _modelFilePath, _parent);
	
	// 初期化
	obj->Initialize();
	
	// 親オブジェクトのリストに追加
	if (_parent != nullptr)_parent->PushBackChild(obj);
	
	return obj;
}