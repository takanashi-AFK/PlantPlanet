#include "Component_UIEasing.h"

Component_UIEasing::Component_UIEasing(UIObject* p) :easing_(0, 0, 0),p_ui(p),easing_type(Easing::TYPE::STRAIGHT)
{
}

Component_UIEasing::~Component_UIEasing()
{
}

void Component_UIEasing::Save(json& _saveObj)
{
	{
		auto& pos = secTransform_.position_;
		auto& rot = secTransform_.rotate_;
		auto& scale = secTransform_.scale_;

		_saveObj["Second_position_"] = { pos.x,pos.y,pos.z };
		_saveObj["Second_rotate"] = { rot.x,rot.y,rot.z };
		_saveObj["Second_scale"] = { scale.x,scale.y,scale.z };
	}

	{
		_saveObj["Easing_Start"] =  easing_.val0_ ;
		_saveObj["Easing_Goal"] =  easing_.val1_ ;
		_saveObj["Easing_Ratio"] =  easing_.ratio_ ;
		_saveObj["Easing_Type"] =  static_cast<int>(this->easing_type ) ;
	}
}

void Component_UIEasing::Load(json& _loadObj)
{
	auto SetFloat3 = [&](string index, XMFLOAT3& data)
		{
			data = {};
			if (_loadObj.contains(index)) data = { _loadObj[index][0].get<float>(),_loadObj[index][1].get<float>(),_loadObj[index][2].get<float>() };
		};

	auto SetFloat = [&](string index, float& data)
		{
			data = {};
			if (_loadObj.contains(index)) data = _loadObj[index].get<float>();
		};

	auto SetInt = [&](string index, int& data)
		{
			data = {};
			if (_loadObj.contains(index)) data = _loadObj[index].get<int>();
		};

	{
		auto& t = this->secTransform_;
		SetFloat3("Second_position_", t.position_);
		SetFloat3("Second_rotate", t.rotate_);
		SetFloat3("Second_scale", t.scale_);

		auto& e = this->easing_;
		SetFloat("Easing_Start", e.val0_);
		SetFloat("Easing_Goal", e.val1_);
		SetFloat("Easing_Ratio", e.ratio_);

		int temp = NULL;
		SetInt("Easing_Type",temp);

		easing_type = static_cast<Easing::TYPE>(temp);
	}
}

Transform Component_UIEasing::GetValue()
{
	Transform t = {};
	auto CalcFloat3 = [&](XMFLOAT3 ui, XMFLOAT3 me)->XMFLOAT3
		{
			float left = easing_.GetValue(easing_type);
			float right = 1 - left;

			float x = left * ui.x + right * me.x;
			float y = left * ui.y + right * me.y;
			float z = left * ui.z + right * me.z;

			return { x,y,z };
		};

	auto trans_ui = p_ui->GetTransform();
	t.position_ = CalcFloat3(trans_ui.position_, secTransform_.position_);
	t.rotate_ = CalcFloat3(trans_ui.rotate_, secTransform_.rotate_);
	t.scale_ = CalcFloat3(trans_ui.scale_, secTransform_.scale_);

	return t;
}

Easing* Component_UIEasing::GetEasing()
{
	return &easing_;
}
