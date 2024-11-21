#include "EnemyGenerator.h"
#include<algorithm>
#include<random>
#include"../Objects/Stage/StageObject.h"
#include<format>
#include"../../Engine/ImGui/imgui.h"
#include"../../Engine/Global.h"
#include"../../Engine/Collider/SphereCollider.h"
#include"../../Engine/Collider/BoxCollider.h"

EnemyGenerator::EnemyGenerator(XMFLOAT3 pos):Generator(pos),parent_(nullptr),information_(this)
{
	switch (information_.RangeType)
	{
	case RANGE_TYPE::BOX: collider_ = new BoxCollider({ 0,0,0 }, { information_.radiusX*2,information_.radiusY*2 ,information_.radiusZ*2 }); break;
	case RANGE_TYPE::SPHERE: collider_ = new SphereCollider({ 0,0,0 }, 1.0f); break;
	}
}

EnemyGenerator::~EnemyGenerator()
{
	delete collider_;
}

void* EnemyGenerator::Pop()
{
	auto enemy = enemies_.size() <= 0 ?
		nullptr:enemies_[enemies_.size() -1];
	enemies_.pop_back();

	return enemy;
}

bool EnemyGenerator::isEmpty()
{
	return enemies_.empty();
}

void EnemyGenerator::Draw()
{
	auto SerchInDirectory = [](string& fileName,string append)
		{
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			string filter;
			filter.append(append);
			filter.push_back('\0');
			filter.append("*.");
			filter.append(append);
			filter.push_back('\0');


			// �ǉ�����I�u�W�F�N�g�̃��f���t�@�C���p�X��ݒ�
			{
				// �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
				OPENFILENAME ofn; {
					TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
					ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
					ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
					ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
					ofn.lpstrFile[0] = '\0'; // ������
					ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
					ofn.lpstrFilter =filter.c_str(); // �t�B���^�[
					ofn.nFilterIndex = 1; // �����I������t�B���^�[
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
					ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
				}

				// �t�@�C����I������_�C�A���O�̕\��
				if (GetOpenFileName(&ofn) == TRUE) {
					// �t�@�C���p�X���擾
					fileName = ofn.lpstrFile;

					// �J�����g�f�B���N�g������̑��΃p�X���擾
					fileName = FileManager::GetAssetsRelativePath(fileName);

					// ���������"\\"��"/"�ɒu��
					FileManager::ReplaceBackslashes(fileName);

					// �f�B���N�g����߂�
					SetCurrentDirectory(defaultCurrentDir);

					return;
				}
				else {
					return;
				}
			};
		};

	if (ImGui::TreeNode("Radius"))
	{
		ImGui::DragFloat("Radius X", &information_.radiusX, 0.01, 0, 1000.0f);
		ImGui::DragFloat("Radius Y", &information_.radiusY, 0.01, 0, 1000.0f);
		ImGui::DragFloat("Radius Z", &information_.radiusZ, 0.01, 0, 1000.0f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Enemy Information"))
	{
		ImGui::DragInt("Enemy Num", reinterpret_cast<int*>(&information_.enemyNum), 1, 0, 0xffff);
		
		//ImGui::Text("", information_.enemyName.c_str()); ImGui::SameLine();
		static char nameBuffer[256] = "";
		ImGui::InputTextWithHint(": Enemy Name", information_.enemyName.c_str(), nameBuffer, IM_ARRAYSIZE(nameBuffer));
		information_.enemyName = nameBuffer;

		if (ImGui::SmallButton("Model Path"))	SerchInDirectory(information_.modelPath,"fbx"); ImGui::SameLine();
		ImGui::Text((": "+ information_.modelPath).c_str()); 
		
		ImGui::Text("File Name");	ImGui::SameLine();
		ImGui::Text((": " + informationDir_).c_str());
		
		ImGui::TreePop();
	}

	prevRangeType_ = information_.RangeType;

	if (ImGui::TreeNode("Information for Generator"))
	{
		if (ImGui::Button("Save")) information_.Save(); ImGui::SameLine();
		if (ImGui::Button("Load")) information_.Load(informationDir_); ImGui::SameLine();
		if (ImGui::Button("Delete")) KillMe();
		
		if (ImGui::RadioButton("Sphere", reinterpret_cast<int*>(&information_.RangeType), static_cast<int>(RANGE_TYPE::SPHERE))); ImGui::SameLine();
		if (ImGui::RadioButton("Box", reinterpret_cast<int*>(&information_.RangeType), static_cast<int>(RANGE_TYPE::BOX)));

		ImGui::TreePop();
	}
	//����̌`�`�F�b�N
	float coefficient = 1;

	if (prevRangeType_ != information_.RangeType)
	{
		delete collider_;

		switch (information_.RangeType)
		{
		case RANGE_TYPE::BOX: collider_ = new BoxCollider({ 0,0,0 }, { information_.radiusX,information_.radiusY ,information_.radiusZ }); break;
		case RANGE_TYPE::SPHERE: collider_ = new SphereCollider({ 0,0,0 }, 1.0f); break;
		}
	}

	//����̕`��

	switch (information_.RangeType)
	{
	case RANGE_TYPE::BOX: coefficient = 2; break;
	case RANGE_TYPE::SPHERE: coefficient = 1; break;
	}

	collider_->SetSize({ information_.radiusX*coefficient ,information_.radiusY*coefficient ,information_.radiusZ*coefficient });
	collider_->Draw(GetPosition());
}

void EnemyGenerator::Save(json& saveObj, int index)
{
	information_.Save();
}

void EnemyGenerator::Load(json& loadObj, int index)
{
	auto&& jsonName = loadObj[index]["InformationFilePath"].get<string>();
	if (jsonName.empty()) return;

	information_.Load(jsonName);
}

void EnemyGenerator::BoxGenerate()
{
	std::random_device random_device{};
	std::mt19937 engine(random_device());

	auto&& position = GetPosition();

	auto frand = [&](float min, float max)->float
		{
			std::uniform_real_distribution<float> unireal(min, max);
			return unireal(engine);
		};

	for (auto i = 0u; i < information_.enemyNum; ++i) {
		XMFLOAT3 plantPos = {};
		plantPos.x = frand(-information_.radiusX + position.x, information_.radiusX + position.x);
		plantPos.y = frand(-information_.radiusY + position.y, information_.radiusY + position.y);
		plantPos.z = frand(-information_.radiusZ + position.z, information_.radiusZ + position.z);

		auto enemy = CreateStageObject(information_.enemyName + std::to_string(i), information_.modelPath, parent_);
		
		enemy->SetPosition(plantPos);
		enemy->SetRotate({0,frand(0,360),0 });
		enemy->AddComponent(CreateComponent("bossComp", BossBehavior, enemy));

		enemies_.push_back(enemy);
	}
}

void EnemyGenerator::SphereGenerate()
{
	std::random_device random_device{};
	std::mt19937 engine(random_device());

	auto&& position = GetPosition();

	auto frand = [&](float min , float max)->float 
		{
			std::uniform_real_distribution<float> unireal(min, max);
			return unireal(engine);
		};

	for (auto i = 0u; i < information_.enemyNum; ++i) {
		
		XMFLOAT3 plantPos = { frand(-1.0f, 1.0f), frand(-1.0f, 1.0f), frand(-1.0f, 1.0f) };
		float length = frand(.0f, 1.0f);
		{
			auto temporaryVec = XMVector3Normalize(XMLoadFloat3(&plantPos));
			XMStoreFloat3(&plantPos, temporaryVec);//plantPos�̐��K��

			plantPos.x *= length;
			plantPos.y *= length;
			plantPos.z *= length;//length�̒����ɂ���B
		}

		plantPos.x *= information_.radiusX;
		plantPos.y *= information_.radiusY;
		plantPos.z *= information_.radiusZ;

		plantPos.x += position.x;
		plantPos.y += position.y;
		plantPos.z += position.z;

		auto enemy = CreateStageObject(information_.enemyName + std::to_string(i), information_.modelPath, parent_);

		enemy->SetPosition(plantPos);
		enemy->SetRotate({ 0,frand(0,360),0 });
		enemy->AddComponent(CreateComponent("bossComp", BossBehavior, enemy));

		enemies_.push_back(enemy);
	}
}

void EnemyGenerator::Generate(void* parent)
{
	srand(time(NULL));
	parent_ = reinterpret_cast<Stage*>(parent);

	information_.enemyNum = information_.enemyNum >= 0
		? information_.enemyNum: 1u;

	switch (information_.RangeType)
	{
	case Generator::RANGE_TYPE::BOX:BoxGenerate(); break;

	case Generator::RANGE_TYPE::SPHERE:SphereGenerate(); break;

	default:SphereGenerate();
	}
}

void EnemyGenerator::Information::Save()
{
	json saveObj = {};
	saveObj["RANGE_TYPE"] = this->RangeType;
	saveObj["RadiusX"] = this->radiusX;
	saveObj["RadiusY"] = this->radiusY;
	saveObj["RadiusZ"] = this->radiusZ;

	saveObj["EnemyNum"] = this->enemyNum;
	saveObj["EnemyName"] = this->enemyName;
	saveObj["ModelPath"] = this->enemyName;

	JsonReader::Save(holder_->informationDir_, saveObj);
}

void EnemyGenerator::Information::Load(string jsonName)
{
	json file;
	if (!JsonReader::Load(jsonName, file)) return;

	RangeType = file.contains("RANGE_TYPE")
		? static_cast<Generator::RANGE_TYPE>(file["RANGE_TYPE"].get<int>()) : Generator::RANGE_TYPE::SPHERE;

	radiusX = file.contains("RadiusX")
		? file["RadiusX"].get<float>() : 1.0f;

	radiusY = file.contains("RadiusY")
		? file["RadiusY"].get<float>() : 1.0f;

	radiusZ = file.contains("RadiusZ")
		? file["RadiusZ"].get<float>() : 1.0f;

	enemyNum = file.contains("EnemyNum")
		? file["EnemyNum"].get<uint32_t>() : 1u;

	enemyName = file.contains("EnemyName")
		? file["EnemyName"].get<string>() : "";

	modelPath = file.contains("ModelPath")
		? file["ModelPath"].get<string>() : "";
}

EnemyGenerator::Information::Information(Generator* holder):holder_(holder)
{
}
