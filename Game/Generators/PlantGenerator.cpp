#include "PlantGenerator.h"
#include<algorithm>
#include<random>
#include"../Objects/Stage/StageObject.h"
#include"../../Engine/ImGui/imgui.h"
#include"../../Engine/Global.h"

PlantGenerator::PlantGenerator(XMFLOAT3 pos):Generator(pos),parent_(nullptr)
{
}

PlantGenerator::~PlantGenerator()
{
}

void PlantGenerator::Generate(void* parent)
{
    srand(time(NULL));

}

void* PlantGenerator::Pop()
{
    return nullptr;
}

bool PlantGenerator::isEmpty()
{
    return plants_.empty();
}

void PlantGenerator::Draw()
{
}

void PlantGenerator::Save(json& saveObj, int index)
{
}

void PlantGenerator::Load(json& loadObj, int index)
{
}

void PlantGenerator::BoxGenerate()
{
}

void PlantGenerator::SphereGenerate()
{
}
