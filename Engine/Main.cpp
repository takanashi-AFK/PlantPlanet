
//
//　最終更新日：2023/10/20
//

#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "global.h"
#include "GameObject/RootObject.h"
#include "../Game/Objects/UI/UIPanel.h"
#include "../Game/Otheres/GameEditor.h"
#include "ResourceManager/Model.h"
#include "ResourceManager/Image.h"
#include "GameObject/Camera.h"
#include "DirectX/Input.h"
#include "ResourceManager/Audio.h"
#include "../Game/Otheres/AudioController.h"
#include "ResourceManager/VFX.h"
#include "ResourceManager/Transition.h"
#include "../Game/Objects/UI/UICursor.h"

// effekseerのヘッダーをインクルード

#include "../EffekseeLib/EffekseerVFX.h"/*★★★*/


// ImGuiのヘッダーをインクルード
#include "ImGui/imgui.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#pragma comment(lib,"Winmm.lib")

RECT windowRect = {};  // 元のウィンドウサイズを保存
//定数宣言
const char* WIN_CLASS_NAME = "SampleGame";	//ウィンドウクラス名

//プロトタイプ宣言
HWND InitApp(HINSTANCE hInstance, int screenWidth, int screenHeight, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ToggleFullScreen(HWND hWnd, bool& isFullScreen, RECT& windowRect);

// エントリーポイント
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
//#if defined(DEBUG) | defined(_DEBUG)
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#endif

	srand((unsigned)time(NULL));
	SetCurrentDirectory("Assets");

	//初期化ファイル（setup.ini）から必要な情報を取得
	int screenWidth = GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//スクリーンの幅
	int screenHeight = GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//スクリーンの高さ
	int fpsLimit = GetPrivateProfileInt("GAME", "Fps", 60, ".\\setup.ini");				//FPS（画面更新速度）
	int isDrawFps = GetPrivateProfileInt("DEBUG", "ViewFps", 0, ".\\setup.ini");		//キャプションに現在のFPSを表示するかどうか

	//ウィンドウを作成
	HWND hWnd = InitApp(hInstance, screenWidth, screenHeight, nCmdShow);

	//Direct3D準備
	Direct3D::Initialize(hWnd, screenWidth, screenHeight);

	//ImGui準備
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(Direct3D::pDevice_, Direct3D::pContext_);

	// ウィンドウの色を変更する
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.f); // RGBAで指定
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.3f, 0.5f, 1.0f); // RGBAで指定
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.2f, 0.3f, 1.0f); // RGBAで指定
		//style.Colors[ImGuiCol_Button] = ImVec4(0.5f, 0.1f, 0.5f, 1.0f); // RGBAで指定
	}

	//カメラを準備
	Camera::Initialize();

	//入力処理（キーボード、マウス、コントローラー）の準備
	Input::Initialize(hWnd);

	//オーディオ（効果音）の準備
	Audio::Initialize();

	//トランジションの初期化
	Transition::Initialize();

	// effekseerの初期化
	EFFEKSEERLIB::gEfk = new EFFEKSEERLIB::EffekseerManager;/*★★★*/
	EFFEKSEERLIB::gEfk->Initialize(Direct3D::pDevice_,Direct3D::pContext_);/*★★★*/


	//ルートオブジェクト準備
	//すべてのゲームオブジェクトの親となるオブジェクト
	RootObject* pRootObject = new RootObject;
	pRootObject->Initialize();

	//UIPanelのインスタンス取得と初期化
	UIPanel* pUIPanel_ = UIPanel::GetInstance();
	pUIPanel_->Initialize();




	//メッセージループ（何か起きるのを待つ）
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//メッセージあり（こっちが優先）
		if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//メッセージなし（ここでゲームの処理）
		else
		{
			//時間計測
			timeBeginPeriod(1);	//時間計測の制度を上げる
			static int FPS = 0;								//画面更新回数のカウンタ
			static DWORD lastFpsResetTime = timeGetTime();	//最後にキャプションにFPSを表示した
			static DWORD lastUpdateTime = timeGetTime();	//最後に画面を更新した時間
			DWORD nowTime = timeGetTime();					//現在の時間


			//キャプションに現在のFPSを表示する
			if (isDrawFps)
			{
				//1秒（1000ミリ秒）経ったら
				if (nowTime - lastFpsResetTime > 1000)
				{
					//FPSの値を表示
					char string[16];
					wsprintf(string, "FPS:%d", FPS);
					SetWindowText(GetActiveWindow(), string);
					FPS = 0;
					lastFpsResetTime = nowTime;
				}
			}

			// 前回のフレームからの経過時間を計算
			float deltaTime = (nowTime - lastUpdateTime);/*★★★*/

			//指定した時間（FPSを60に設定した場合は60分の1秒）経過していたら更新処理
			if (deltaTime * fpsLimit > 1000.0f)
			{
				//時間計測関連
				lastUpdateTime = nowTime;	//現在の時間（最後に画面を更新した時間）を覚えておく
				FPS++;						//画面更新回数をカウントする


				//入力（キーボード、マウス、コントローラー）情報を更新
				Input::Update();
				// UICursor::Update();

				if (Input::IsKeyDown(DIK_F4)) {
					// 画面表示切替
					ToggleFullScreen(hWnd, Direct3D::isFullScreen_, windowRect);
				}

				//ImGuiの更新
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

#ifdef _DEBUG
				// ImGuiで表示するウィンドウの設定を行う
				ImGui::SetNextWindowPos(ImVec2(0, Direct3D::screenHeight_ * 0.7f));
				ImGui::SetNextWindowSize(ImVec2(Direct3D::screenWidth_ * 0.7f, Direct3D::screenHeight_ * 0.3f));

				// ウィンドウを表示
				ImGui::Begin("Debug", NULL,
					ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
#endif // _DEBUG

				//全オブジェクトの更新処理
				//ルートオブジェクトのUpdateを呼んだあと、自動的に子、孫のUpdateが呼ばれる
				pRootObject->UpdateSub();

				pUIPanel_->UpdateSub();

				// オーディオの更新
				//AudioController::Update(pRootObject);

				//カメラを更新
				Camera::Update();

				// effekseerの更新
				EFFEKSEERLIB::gEfk->Update(deltaTime / 1000.f);/*★★★*/

				// 遠藤先生作成エフェクトの更新
				// VFX::Update();

				//トランジションの更新
				Transition::Update();

				//このフレームの描画開始
				Direct3D::BeginDraw();

				//全オブジェクトを描画
				//ルートオブジェクトのDrawを呼んだあと、自動的に子、孫のUpdateが呼ばれる
				pRootObject->DrawSub();

				pUIPanel_->DrawSub();


				// effekseerの描画
				EFFEKSEERLIB::gEfk->Draw();/*★★★*/
				
				// 遠藤先生作成エフェクトの描画
				// VFX::Draw();

				//トランジションの描画
				Transition::Draw();
#ifdef _DEBUG
				ImGui::End();
#endif // _DEBUG

				//ImGuiの描画
				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				//描画終了
				Direct3D::EndDraw();
				

			
				//ちょっと休ませる
				Sleep(1);
			}
			timeEndPeriod(1);	//時間計測の制度を戻す
		}
	}

	//ImGui終了処理
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

	//いろいろ解放
	Transition::Release();
	VFX::Release();
	Audio::AllRelease();
	Model::AllRelease();
	Image::AllRelease();
	pRootObject->ReleaseSub();
	pUIPanel_->ReleaseSub();
	SAFE_DELETE(pRootObject);
	Direct3D::Release();

	return 0;
}


//ウィンドウの作成
HWND InitApp(HINSTANCE hInstance, int screenWidth, int screenHeight, int nCmdShow)
{
	//ウィンドウクラス（設計図）を作成
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);					//この構造体のサイズ
	wc.hInstance = hInstance;						//インスタンスハンドル
	wc.lpszClassName = WIN_CLASS_NAME;				//ウィンドウクラス名
	wc.lpfnWndProc = WndProc;						//ウィンドウプロシージャ
	wc.style = CS_VREDRAW | CS_HREDRAW;				//スタイル（デフォルト）
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);	//アイコン
	wc.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);	//小さいアイコン
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);	//マウスカーソル
	wc.lpszMenuName = nullptr;						//メニュー（なし）
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//背景（白）
	RegisterClassEx(&wc);

	//ウィンドウサイズの計算
	RECT winRect = { 0, 0, screenWidth, screenHeight };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);

	//タイトルバーに表示する内容
	char caption[64];
	GetPrivateProfileString("SCREEN", "Caption", "***", caption, 64, ".\\setup.ini");

	//ウィンドウを作成
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,					//ウィンドウクラス名
		caption,						//タイトルバーに表示する内容
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,		//スタイル（普通のウィンドウ）
		CW_USEDEFAULT,					//表示位置左（おまかせ）
		CW_USEDEFAULT,					//表示位置上（おまかせ）
		winRect.right - winRect.left,	//ウィンドウ幅
		winRect.bottom - winRect.top,	//ウィンドウ高さ
		nullptr,						//親ウインドウ（なし）
		nullptr,						//メニュー（なし）
		hInstance,						//インスタンス
		nullptr							//パラメータ（なし）
	);

	//ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);

	return hWnd;
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);


//ウィンドウプロシージャ（何かあった時によばれる関数）
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	//ウィンドウを閉じた
	case WM_DESTROY:
		PostQuitMessage(0);	//プログラム終了
		return 0;

	//マウスが動いた
	case WM_MOUSEMOVE:
		Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}

	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// フルスクリーンモードとウィンドウモードを切り替える関数
void ToggleFullScreen(HWND hWnd, bool& isFullScreen, RECT& windowRect) {
	if (isFullScreen) {
		// ウィンドウモードに戻す
		SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW); 
		SetWindowPos(hWnd, HWND_NOTOPMOST, windowRect.left, windowRect.top,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			SWP_FRAMECHANGED | SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	else {
		// フルスクリーンに切り替え
		GetWindowRect(hWnd, &windowRect);  // 元のウィンドウサイズを保存
		SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
		MONITORINFO mi = { sizeof(mi) };
		if (GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &mi)) {
			SetWindowPos(hWnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
				mi.rcMonitor.right - mi.rcMonitor.left,
				mi.rcMonitor.bottom - mi.rcMonitor.top,
				SWP_FRAMECHANGED | SWP_NOZORDER | SWP_SHOWWINDOW);
		}
	}
	isFullScreen = !isFullScreen;  // フルスクリーン状態を反転
}
