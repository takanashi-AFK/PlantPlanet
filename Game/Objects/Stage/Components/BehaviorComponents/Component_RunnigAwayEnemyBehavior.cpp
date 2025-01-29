#include "Component_RunnigAwayEnemyBehavior.h"

#include "../../../../../Engine/Global.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../Stage.h"
#include "../../StageObject.h" 
#include<cmath>
#include<format>
#include "../GaugeComponents/Component_HealthGauge.h"
#include "../../../../Plants/PlantCollection.h"
#include "../PlantComponents/Component_Plant.h"
#include<algorithm>

Component_RunnigAwayEnemyBehavior::Component_RunnigAwayEnemyBehavior(string _name, StageObject* _holder, Component* _parent)
	:Component(_holder, _name, RunningawayEnemy, _parent), points_{}, isFlowerSpawned_(false), destinationPointIterator_{},moveAmount_{}
{
	nowProcess_ = [this]() {RunawayProcess(); };
}

Component_RunnigAwayEnemyBehavior::~Component_RunnigAwayEnemyBehavior()
{
}

void Component_RunnigAwayEnemyBehavior::Initialize()
{
	holder_->AddCollider(new SphereCollider({ 0,0,0 }, 1.f));
	holder_->SetObjectType(StageObject::TYPE_ENEMY);

	if (!FindChildComponent("HealthGauge")) AddChildComponent(CreateComponent("HealthGauge", HealthGauge, holder_, this));

	destinationPointIterator_ = points_.begin();
}

void Component_RunnigAwayEnemyBehavior::Update()
{
	if (IsDead()) nowProcess_ = [this]() {DeadProcess(); };
	nowProcess_();
}

void Component_RunnigAwayEnemyBehavior::Release()
{
}

void Component_RunnigAwayEnemyBehavior::Save(json& _saveObj)
{
	_saveObj["MoveAmount"] = moveAmount_;
	_saveObj["Point_Num"] = points_.size();
	_saveObj["DropFlowerName"] = dropFlowerName_;
	int index = 0;
	for (auto& itr : points_) {

		_saveObj[std::format("Point{}", index).c_str()] = { itr.x,itr.y,itr.z };
		++index;
	}
}

void Component_RunnigAwayEnemyBehavior::Load(json& _loadObj)
{
	if (_loadObj.contains("MoveAmount")) moveAmount_ = _loadObj["MoveAmount"].get<float>();
	if (_loadObj.contains("DropFlowerName"))	dropFlowerName_ = _loadObj["DropFlowerName"].get<string>();

	int list_max = 0;
	
	if (!_loadObj.contains("Point_Num")) return;
	list_max = _loadObj["Point_Num"].get<int>();

	for (auto i = 0u; i < list_max; ++i) {

		auto str = std::format("Point{}", i);
		if (_loadObj.contains(str.c_str()))
		{
			points_.push_back({
				_loadObj[str.c_str()][0].get<float>(),
				_loadObj[str.c_str()][1].get<float>(),
				_loadObj[str.c_str()][2].get<float>()
				});
		}
	}

	destinationPointIterator_ = points_.begin();
}

void Component_RunnigAwayEnemyBehavior::DrawData()
{
	ImGui::Checkbox("isActive_", &isActive_);
	
	static int selectFlower = 0;
	unordered_map<int, PlantData> plant = PlantCollection::GetPlants();
	if (plant.size() == 0)ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "No plant data!"); \
	else {
		// コンボボックス
		if (ImGui::BeginCombo("FlowerSetData", plant[selectFlower].name_.c_str())) {
			for (const auto& [key, data] : plant) { // `unordered_map` をループ
				bool is_selected = (selectFlower == key); // 現在選択中か確認
				if (ImGui::Selectable(data.name_.c_str(), is_selected)) {
					selectFlower = key; // キーを更新
					dropFlowerName_ = data.name_;
				}
				if (is_selected) {
					ImGui::SetItemDefaultFocus(); // デフォルトフォーカス
				}
			}
			ImGui::EndCombo();
		}
	}

	ImGui::DragFloat("MoveAmount     :", &moveAmount_, 0.01, .0f);

	if (ImGui::Button("Add"))
	{
		points_.push_back(XMFLOAT3(0, 0, 0));
	}

	ImGui::SameLine();

	static int insertIndex = 0;
	int max = ((std::max)(0, static_cast<int>(points_.size() - 1)));

	ImGui::PushItemWidth(20);
	ImGui::DragInt("Index:", &insertIndex, 1, 0, max);
	ImGui::PopItemWidth();

	insertIndex = std::clamp(insertIndex, 0, max);

	ImGui::SameLine();

	if (ImGui::Button("Insert"))
	{
		auto pos = std::next(points_.begin(), insertIndex);
		points_.insert(pos, XMFLOAT3(0,0,0));
	}

	int index = 0;

	ImGui::PushItemWidth(200);
	for (auto itr = points_.begin(); itr != points_.end();) {

		auto eraseButton = std::format("Erase {: 2}", index);
		if (ImGui::Button(eraseButton.c_str()))
		{
			itr = points_.erase(itr);

			destinationPointIterator_ = points_.end();
			continue;
		}

		ImGui::SameLine();

		auto str = std::format("Point{: 2} Pos", index);
		ImGui::DragFloat3(str.c_str(), &itr->x, 0.1f);

		++index;
		++itr;
	}
	ImGui::PopItemWidth();

}

void Component_RunnigAwayEnemyBehavior::OnCollision(GameObject* _target, Collider* _collider)
{
}

void Component_RunnigAwayEnemyBehavior::RunawayProcess()
{
	//目的地が未設定なら処理を終える
	if (destinationPointIterator_ == points_.end())
	{
		destinationPointIterator_ = points_.begin();
		return;
	}

	auto [length, vec] = GetLengthAndVectorToDestination();

	//目的地が移動量以下ならそのまま目的地にピッタリ同じ位置にする
	if (length <= moveAmount_)holder_->SetPosition(*destinationPointIterator_);
	else MoveTo(vec,moveAmount_);

	ExchangeRotate(vec);

	CheckDestination();
}

void Component_RunnigAwayEnemyBehavior::DeadProcess()
{
	Stage* pStage = (Stage*)(holder_->FindObject("Stage"));
	if (pStage == nullptr)return;
	pStage->DeleteStageObject(holder_);

	if (isFlowerSpawned_ == true)return;
	if (dropFlowerName_ == "")return;

	std::unordered_map<int, PlantData> plantData = PlantCollection::GetPlants();
	if (plantData.size() <= 0)return;

	for (auto& plant : plantData) {
		if (plant.second.name_ == dropFlowerName_) {
			// その場に花を生成


			StageObject* plantObject = CreateStageObject("EnemyDropPlant1", plant.second.modelFilePath_, pStage);

			// 当たり判定を設定
			plantObject->SetIsColliding(false);

			// プラントコンポーネントを作成
			Component_Plant* plantComponent = (Component_Plant*)CreateComponent("Plant", Plant, plantObject, this);

			// 植物情報を設定
			plantComponent->SetData(plant.second);

			// コンポーネントを追加
			plantObject->AddComponent(plantComponent);

			// サイズを設定
			plantObject->SetScale({ 0.3f,0.3f,0.3f });
			plantObject->SetPosition(holder_->GetPosition());

			// 属性を設定
			plantObject->SetObjectType(StageObject::TYPE_PLANT);

			// ステージに追加
			((Stage*)holder_->GetParent())->AddStageObject(plantObject);

			isFlowerSpawned_ = true;
			return;
		}
	}
}

void Component_RunnigAwayEnemyBehavior::CheckDestination()
{
	using std::next;
	//現在地と目的地が違うなら抜ける
	if ((*destinationPointIterator_) != holder_->GetPosition()) return;

	//次のイテレータがあるならそれを目的地にし、最後ならば最初に戻る(移動はループする)
	destinationPointIterator_ = next(destinationPointIterator_) != points_.end() ?
		next(destinationPointIterator_) : points_.begin();
}

void Component_RunnigAwayEnemyBehavior::MoveTo(XMFLOAT3 vec, float amount)
{
	holder_->SetPosition(holder_->GetPosition() + (vec * amount));
}

std::tuple<float, XMFLOAT3> Component_RunnigAwayEnemyBehavior::GetLengthAndVectorToDestination()
{
	using std::pow;
	using std::sqrtf;

	auto pos = holder_->GetPosition();
	auto diff = (*destinationPointIterator_) - pos;

	auto&[x, y, z] = diff;

	float length = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	auto vec = diff / length;

	return { length,vec };
}

bool Component_RunnigAwayEnemyBehavior::IsDead()
{
	Component_HealthGauge* hp = dynamic_cast<Component_HealthGauge*>(GetChildComponent("HealthGauge"));
	if (!hp)return false;

	if (hp->GetNow() <= 0)return true;

	return false;
}

void Component_RunnigAwayEnemyBehavior::ExchangeRotate(XMFLOAT3 vec)
{
	holder_->SetRotateY(XMConvertToDegrees(std::atan2f(vec.x, vec.z)));
}
