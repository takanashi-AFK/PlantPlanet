#include "../../../Engine/DirectX/Input.h"
#include "../../../Engine/GameObject/Camera.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../Stage/Stage.h"
#include "../Stage/StageObject.h"
#include "TPSCamera.h"
#include"../../../Engine/ResourceManager/Model.h"
#include"../../../Engine/Global.h"

namespace {
    const float DEFAULT_SENSITIVITY = 15.f;     // 規定感度
    const float DEFAULT_TARGET_HEIGHT = 4.f;    // 規定カメラの高さ
    const float DEFAULT_TARGET_DISTANCE = 7.f;  // 規定カメラの距離
    const float SENSITIVITY_MAX = 1.f;         // 感度の最大値
    const float SENSITIVITY_MIN = 0.f;          // 感度の最小値
    const float ROTATE_UPPER_LIMIT = -80.f;     // 回転の上限
    const float ROTATE_LOWER_LIMIT = 80.f;      // 回転の下限
	const float ROTATE_LITTLE_ANGLE = 30.f;     // 回転の少しの角度
	const float ROTATE_Y = 0.5f;                // 回転の感度
	const int   CONTROLLER_MAGUNIFICATION = 10;   // コントローラーの倍率
	const XMVECTOR INITIALIZE_AXIS_Z = { 0,0,1,0 };         // 初期軸

}

TPSCamera::TPSCamera(GameObject* parent)
	:GameObject(parent, "TPSCamera"), 
    angle_(),                                   /* 回転角度 */
    sensitivity_(DEFAULT_SENSITIVITY),          /* 感度 */
    pTarget_(),                                 /* ターゲット */
    isActive_(true),                            /* 有効無効 */
    targetHeight_(DEFAULT_TARGET_HEIGHT),       /* カメラの高さ */
    targetDistance_(DEFAULT_TARGET_DISTANCE),   /* カメラの距離 */
    prevAxis_()                                 /* 前回の軸 */
{
}

void TPSCamera::Initialize()
{
}

void TPSCamera::Update()
{
    // カメラが有効でない場合は処理を行わない
    if (IsActive() == false)return;

    // ターゲットが存在しない場合は探す
    if (IsTarget() == false) pTarget_ = FindObject(targetName_);
    
    // 探してもターゲットが存在しない場合は処理を行わない
    if (IsTarget() == false) return;

    // 回転角度を計算 ※マウスとパッドの移動量を元に行う
    CalcRotateAngle(Input::GetMouseMove(), Input::GetPadStickR());

    // カメラの焦点・位置を格納する変数を用意
    XMFLOAT3 camTarget{};
    XMFLOAT3 camPosition{};

    // カメラの位置と焦点を計算
    CalcCameraPositionAndTarget(camPosition, camTarget);
    
    // カメラの位置と焦点を設定
    Camera::SetTarget(camTarget);
    Camera::SetPosition(camPosition);
}

void TPSCamera::Draw()
{
}

void TPSCamera::Release()
{
}

void TPSCamera::DrawData()
{
	// カメラの有効無効 //////////////////////////////////////////////
	ImGui::Checkbox("isActive", &isActive_);

	// カメラの感度 //////////////////////////////////////////////
	ImGui::DragFloat("sensitivity", &sensitivity_, 0.1f,SENSITIVITY_MIN, SENSITIVITY_MAX);

	// カメラの高さ //////////////////////////////////////////////
	ImGui::DragFloat("targetHeight", &targetHeight_,0.1f);

	// カメラの距離 //////////////////////////////////////////////
	ImGui::DragFloat("targetDistance", &targetDistance_, 0.1f);

	// 対象の選択 //////////////////////////////////////////////
    std::vector<string> objNames;
    objNames.push_back("null");

    for (auto& obj : (((Stage*)FindObject("Stage"))->GetStageObjects()))
        objNames.push_back(obj->GetObjectName());

    static int select = 0;
    if (select >= objNames.size())return;

    if (ImGui::BeginCombo("target_", objNames[select].c_str())) {
        for (int i = 0; i < objNames.size(); i++) {
            bool is_selected = (select == i);
            if (ImGui::Selectable(objNames[i].c_str(), is_selected))select = i;
            if (is_selected)ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    targetName_ = objNames[select];
}

void TPSCamera::SetRayCastList(std::list<int> list)
{
    this->hModels_ = list;
}

std::list<int> TPSCamera::GetRayCastList()
{
    return this->hModels_;
}

void TPSCamera::CalcRotateAngle(XMFLOAT3 _mouseMove, XMFLOAT3 _padMove)
{
    // マウスの移動量を元に回転角度を加算
    angle_.x += _mouseMove.y * sensitivity_;
    angle_.y += _mouseMove.x * sensitivity_;

    // パッドの移動量を元に回転角度を加算
    angle_.x -= _padMove.y * (sensitivity_ * CONTROLLER_MAGUNIFICATION);
    angle_.y += _padMove.x * (sensitivity_ * CONTROLLER_MAGUNIFICATION);

    // ｘ軸回転の上限・下限を設定し回転を制限
	if (angle_.x < ROTATE_UPPER_LIMIT)angle_.x -= _mouseMove.y * sensitivity_;
	if (angle_.x > ROTATE_LOWER_LIMIT)angle_.x -= _mouseMove.y * sensitivity_;
}

void TPSCamera::CalcCameraPositionAndTarget(XMFLOAT3& _cameraPosition, XMFLOAT3& _cameraTarget)
{
    // 回転の中心を設定
    XMFLOAT3 center = pTarget_->GetPosition();
    center.y += targetHeight_;

    // y軸の回転を行う
    {
        // 回転行列を作成
        XMMATRIX rotateY = XMMatrixRotationY(XMConvertToRadians(angle_.y));

        // ↑の行列を元に回転
        XMVECTOR center_To_camTarget = INITIALIZE_AXIS_Z;
        center_To_camTarget = XMVector3Transform(center_To_camTarget, rotateY);

        // 長さを加える
        float center_To_camTargetDistance = targetDistance_;
        center_To_camTarget *= center_To_camTargetDistance;

        // 原点からの位置を求めて、カメラの焦点を設定
        XMVECTOR origin_To_camTarget = XMLoadFloat3(&center) + center_To_camTarget;
        XMStoreFloat3(&_cameraTarget, origin_To_camTarget);

        // center_To_camTargetの逆ベクトルを用意
        XMVECTOR center_To_camPosition = -center_To_camTarget;

        // ちょっと回転させる
        center_To_camPosition = XMVector3Transform(center_To_camPosition, XMMatrixRotationY(XMConvertToRadians(-ROTATE_LITTLE_ANGLE)));

        // 原点からの位置を求めて、カメラの位置を設定
        XMVECTOR origin_To_camPosition = XMLoadFloat3(&center) + center_To_camPosition;
        XMStoreFloat3(&_cameraPosition, origin_To_camPosition);
    }

    // ｘ軸の回転を行う
    {
        // 中心を移動
        XMVECTOR newCenter = (XMLoadFloat3(&_cameraPosition) + XMLoadFloat3(&_cameraTarget)) * ROTATE_Y;
        XMFLOAT3 prevCenter = center;
        XMStoreFloat3(&center, newCenter);

        // 縦回転の軸を作成
        XMVECTOR axis = XMLoadFloat3(&center) - XMLoadFloat3(&prevCenter);

        if (XMVector3Equal(axis, XMVectorZero())) axis = prevAxis_;
        
        //// 回転行列を作成
        XMMATRIX rotateAxis = XMMatrixRotationAxis(axis, XMConvertToRadians(angle_.x));

        //　焦点を設定 
        XMVECTOR newCenter_To_camTarget = XMLoadFloat3(&_cameraTarget) - XMLoadFloat3(&center);
        newCenter_To_camTarget = XMVector3Transform(newCenter_To_camTarget, rotateAxis);
        XMVECTOR origin_To_camTarget = XMLoadFloat3(&center) + newCenter_To_camTarget;
        XMStoreFloat3(&_cameraTarget, origin_To_camTarget);

        // 位置を設定
        XMVECTOR newCenter_To_camPosition = -newCenter_To_camTarget;
        XMVECTOR origin_To_camPosition = XMLoadFloat3(&center) + newCenter_To_camPosition;
        XMStoreFloat3(&_cameraPosition, origin_To_camPosition);

    }

    float rayCastDir = targetDistance_;;

    for (auto itr : hModels_) {

        RayCastData rayCast = {};
        auto tTOp = _cameraTarget - _cameraPosition;
        auto tTOpvec = XMLoadFloat3(&tTOp);
        tTOpvec = XMVector3Normalize(tTOpvec);
        XMStoreFloat3(&tTOp, -tTOpvec);
        rayCast.start = _cameraPosition;
        rayCast.dir = tTOp ;

        Model::RayCast(itr, &rayCast);
        rayCastDir = rayCast.hit && rayCast.dist > .0f && rayCast.dist < rayCastDir ?
            rayCastDir = rayCast.dist : rayCastDir;

        
    }

    center = pTarget_->GetPosition();
    center.y += targetHeight_;

    // y軸の回転を行う
    {
        // 回転行列を作成
        XMMATRIX rotateY = XMMatrixRotationY(XMConvertToRadians(angle_.y));

        // ↑の行列を元に回転
        XMVECTOR center_To_camTarget = INITIALIZE_AXIS_Z;
        center_To_camTarget = XMVector3Transform(center_To_camTarget, rotateY);

        // 長さを加える
        float center_To_camTargetDistance = rayCastDir;
        center_To_camTarget *= center_To_camTargetDistance;

        // 原点からの位置を求めて、カメラの焦点を設定
        XMVECTOR origin_To_camTarget = XMLoadFloat3(&center) + center_To_camTarget;
        XMStoreFloat3(&_cameraTarget, origin_To_camTarget);

        // center_To_camTargetの逆ベクトルを用意
        XMVECTOR center_To_camPosition = -center_To_camTarget;

        // ちょっと回転させる
        center_To_camPosition = XMVector3Transform(center_To_camPosition, XMMatrixRotationY(XMConvertToRadians(-ROTATE_LITTLE_ANGLE)));

        // 原点からの位置を求めて、カメラの位置を設定
        XMVECTOR origin_To_camPosition = XMLoadFloat3(&center) + center_To_camPosition;
        XMStoreFloat3(&_cameraPosition, origin_To_camPosition);
    }

    // ｘ軸の回転を行う
    {
        // 中心を移動
        XMVECTOR newCenter = (XMLoadFloat3(&_cameraPosition) + XMLoadFloat3(&_cameraTarget)) * ROTATE_Y;
        XMFLOAT3 prevCenter = center;
        XMStoreFloat3(&center, newCenter);

        // 縦回転の軸を作成
        XMVECTOR axis = XMLoadFloat3(&center) - XMLoadFloat3(&prevCenter);

        if (XMVector3Equal(axis, XMVectorZero())) axis = prevAxis_;

        //// 回転行列を作成
        XMMATRIX rotateAxis = XMMatrixRotationAxis(axis, XMConvertToRadians(angle_.x));

        //　焦点を設定 
        XMVECTOR newCenter_To_camTarget = XMLoadFloat3(&_cameraTarget) - XMLoadFloat3(&center);
        newCenter_To_camTarget = XMVector3Transform(newCenter_To_camTarget, rotateAxis);
        XMVECTOR origin_To_camTarget = XMLoadFloat3(&center) + newCenter_To_camTarget;
        XMStoreFloat3(&_cameraTarget, origin_To_camTarget);

        // 位置を設定
        XMVECTOR newCenter_To_camPosition = -newCenter_To_camTarget;
        XMVECTOR origin_To_camPosition = XMLoadFloat3(&center) + newCenter_To_camPosition;
        XMStoreFloat3(&_cameraPosition, origin_To_camPosition);

        if (!XMVector3Equal(axis, XMVectorZero()))prevAxis_ = axis;
    }
}

void TPSCamera::Save(json& saveObj)
{
	saveObj["isActive_"] = isActive_;
	saveObj["sensitivity_"] = sensitivity_;
	saveObj["targetHeight_"] = targetHeight_;
	saveObj["targetDistance_"] = targetDistance_;
	saveObj["targetName_"] = targetName_;
}

void TPSCamera::Load(json& loadObj)
{
	if(loadObj.contains("isActive_"))isActive_ = loadObj["isActive_"];
	if (loadObj.contains("sensitivity_"))sensitivity_ = loadObj["sensitivity_"];
	if (loadObj.contains("targetHeight_"))targetHeight_ = loadObj["targetHeight_"];
	if (loadObj.contains("targetDistance_"))targetDistance_ = loadObj["targetDistance_"];
	if (loadObj.contains("targetName_"))targetName_ = loadObj["targetName_"];
}
