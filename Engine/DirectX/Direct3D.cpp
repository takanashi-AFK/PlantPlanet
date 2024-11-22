#include <d3dcompiler.h>
#include "Direct3D.h"
#include "../Global.h"
#include "../GameObject/Transform.h"
#include<fstream>
#include<filesystem>

//画面の描画に関する処理
namespace Direct3D
{
	//【スワップチェーン】
	//画用紙を2枚用紙しておき、片方を画面に映している間にもう一方に描画。
	//1フレーム分の絵が出来上がったら画用紙を交換。これにより画面がちらつかない。
	//その辺を司るのがスワップチェーン
	IDXGISwapChain*         pSwapChain_ = nullptr;

	//【レンダーターゲットビュー】
	//描画したいものと、描画先（上でいう画用紙）の橋渡しをするもの
	ID3D11RenderTargetView* pRenderTargetView_ = nullptr;

	//【デプスステンシル】
	//Zバッファ法を用いて、3D物体の前後関係を正しく表示するためのもの
	ID3D11Texture2D*		pDepthStencil;

	//【デプスステンシルビュー】
	//デプスステンシルの情報をシェーダーに渡すためのもの
	ID3D11DepthStencilView* pDepthStencilView;
	ID3D11DepthStencilState* pDepthStencilState[BLEND_MAX];

	//【ブレンドステート】
	//半透明のものをどのように表現するか
	ID3D11BlendState*	pBlendState[BLEND_MAX];

	bool		isDrawCollision_ = true;	//コリジョンを表示するか
	bool		_isLighting = false;		//ライティングするか



	//extern宣言した変数の初期化
	ID3D11Device*           pDevice_ = nullptr;
	ID3D11DeviceContext*    pContext_ = nullptr;
	SHADER_BUNDLE			shaderBundle[SHADER_MAX] = { 0 };
	int						screenWidth_ = 0;
	int						screenHeight_ = 0;
	bool					isFullScreen_ = false;
	HWND 					hWnd_ = nullptr;

	//イージング関数
	map<string, function<double(double)>> EaseFunc{
	{"InSine",    [](double t) { return (sin(XM_PIDIV2 * t));        }},
	{"OutSine",   [](double t) { return (1 + sin(XM_PIDIV2 * (--t))); }},
	{"InOutSine", [](double t) {  return 0.5 * (1 + sin(XM_PI * (t - 0.5))); }},
	{"InQuad",    [](double t) {  return t * t; }},
	{"OutQuad",    [](double t) { return t * (2 - t); }},
	{"InOutQuad",   [](double t) { return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1; }},
	{"InCubic",     [](double t) { return t * t * t; }},
	{"OutCubic",    [](double t) { return 1 + (--t) * t * t; }},
	{"InOutCubic",  [](double t) { return t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t); }},
	{"InQuart",     [](double t) {   t *= t; return t * t; }},
	{"OutQuart",    [](double t) {   t = (--t) * t; return 1 - t * t; }},
	{"InOutQuart",  [](double t) { if (t < 0.5) { t *= t; return 8 * t * t; }
								   else { t = (--t) * t; return 1 - 8 * t * t; }}},
	{"InQuint",     [](double t) { double t2 = t * t; return t * t2 * t2; }},
	{"InOutQuint",  [](double t) { double t2; if (t < 0.5) { t2 = t * t; return 16 * t * t2 * t2; }
									 else { t2 = (--t) * t; return 1 + 16 * t * t2 * t2; }}},
	{"InExpo",      [](double t) { return (pow(2, 8 * t) - 1) / 255; }},
	{"OutExpo",     [](double t) { return 1 - pow(2, -8 * t); }},
	{"InOutExpo",   [](double t) {if (t < 0.5) { return (pow(2, 16 * t) - 1) / 510; }
								  else { return 1 - 0.5 * pow(2, -16 * (t - 0.5)); }}},
	{"InCirc",      [](double t) { return 1 - sqrt(1 - t); }},
	{"OutCirc",     [](double t) { return sqrt(t); }},
	{"InOutCirc",   [](double t) { if (t < 0.5) { return (1 - sqrt(1 - 2 * t)) * 0.5; }
								   else { return (1 + sqrt(2 * t - 1)) * 0.5; }}},
	{"InBack",      [](double t) { return t * t * (2.70158 * t - 1.70158); }},
	{"OutBack",     [](double t) { return 1 + (--t) * t * (2.70158 * t + 1.70158); }},
	{"InOutBack",   [](double t) { if (t < 0.5) { return t * t * (7 * t - 2.5) * 2; }
								   else { return 1 + (--t) * t * 2 * (7 * t + 2.5); }}},
	{"InElastic",   [](double t) { double t2 = t * t; return t2 * t2 * sin(t * XM_PI * 4.5); }},
	{"OutElastic",  [](double t) { double t2 = (t - 1) * (t - 1); return 1 - t2 * t2 * cos(t * XM_PI * 4.5); }},
	{"InOutElastic",[](double t) { double t2; if (t < 0.45) { t2 = t * t; return 8 * t2 * t2 * sin(t * XM_PI * 9); }
								   else if (t < 0.55) { return 0.5 + 0.75 * sin(t * XM_PI * 4); }
								   else { t2 = (t - 1) * (t - 1); return 1 - 8 * t2 * t2 * sin(t * XM_PI * 9); }}},
	{"InBounce",    [](double t) { return pow(2, 6 * (t - 1)) * abs(sin(t * XM_PI * 3.5)); }},
	{"OutBounce",   [](double t) { return 1 - pow(2, -6 * t) * abs(cos(t * XM_PI * 3.5)); }},
	{"InOutBounce", [](double t) { if (t < 0.5) { return 8 * pow(2, 8 * (t - 1)) * abs(sin(t * XM_PI * 7)); } else { return 1 - 8 * pow(2, -8 * t) * abs(sin(t * XM_PI * 7)); }}}
	};

	//初期化処理
	HRESULT Direct3D::Initialize(HWND hWnd, int screenWidth, int screenHeight)
	{
		// ウィンドウハンドルを保存
		hWnd_ = hWnd;

		///////////////////////////いろいろ準備するための設定///////////////////////////////
		//いろいろな設定項目をまとめた構造体
		DXGI_SWAP_CHAIN_DESC scDesc;

		//とりあえず全部0
		ZeroMemory(&scDesc, sizeof(scDesc));

		//描画先のフォーマット
		scDesc.BufferDesc.Width = screenWidth;		//画面幅
		scDesc.BufferDesc.Height = screenHeight;		//画面高さ
		scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 何色使えるか

		//FPS（1/60秒に1回）
		scDesc.BufferDesc.RefreshRate.Numerator = 60;
		scDesc.BufferDesc.RefreshRate.Denominator = 1;

		//その他
		scDesc.Windowed = TRUE;				//ウィンドウモードかフルスクリーンか
		scDesc.OutputWindow = hWnd;			//ウィンドウハンドル
		scDesc.BufferCount = 1;				//裏画面の枚数
		scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//画面に描画するために使う
		scDesc.SampleDesc.Count = 1;		//MSAA（アンチエイリアス）の設定
		scDesc.SampleDesc.Quality = 0;		//　〃



		///////////////////////////上記設定をもとにデバイス、コンテキスト、スワップチェインを作成///////////////////////////////

		D3D_FEATURE_LEVEL level;
		HRESULT  hr;
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,					// どのビデオアダプタを使用するか？既定ならばnullptrで
			D3D_DRIVER_TYPE_HARDWARE,	// ドライバのタイプを渡す。これ以外は基本的にソフトウェア実装で、どうしてもという時やデバグ用に用いるべし.
			nullptr,					// 上記をD3D_DRIVER_TYPE_SOFTWAREに設定した際に、その処理を行うDLLのハンドルを渡す。それ以外を指定している際には必ずnullptrを渡す.
			0,							// 何らかのフラグを指定する。（デバッグ時はD3D11_CREATE_DEVICE_DEBUG？）
			nullptr,					// デバイス、コンテキストのレベルを設定。nullptrにしとけば可能な限り高いレベルにしてくれる
			0,							// 上の引数でレベルを何個指定したか
			D3D11_SDK_VERSION,			// SDKのバージョン。必ずこの値
			&scDesc,					// 上でいろいろ設定した構造体
			&pSwapChain_,				// 無事完成したSwapChainのアドレスが返ってくる
			&pDevice_,					// 無事完成したDeviceアドレスが返ってくる
			&level,						// 無事完成したDevice、Contextのレベルが返ってくる
			&pContext_);				// 無事完成したContextのアドレスが返ってくる

		//失敗したら終了
		if (FAILED(hr))	return hr;


		///////////////////////////描画のための準備///////////////////////////////
		//スワップチェーンからバックバッファを取得（バックバッファ ＝ 裏画面 ＝ 描画先）
		ID3D11Texture2D* pBackBuffer;
		hr = pSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		//失敗したら終了
		if (FAILED(hr))	return hr;

		//レンダーターゲットビューを作成
		hr = pDevice_->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView_);

		//失敗したら終了
		if (FAILED(hr))	return hr;

		//一時的にバックバッファを取得しただけなので、解放
		pBackBuffer->Release();




		/////////////////////////////////////////////////////////////////////////////////////////////


		// ビューポートの設定
		//レンダリング結果を表示する範囲
		float viewPortMagnification = 1.f;
#ifdef _DEBUG
		viewPortMagnification = 0.7f;

#endif // _DEBUG

		D3D11_VIEWPORT vp;
		vp.Height = (float)screenHeight * viewPortMagnification;		//高さ
		vp.Width = (float)screenWidth * viewPortMagnification;			//幅
		vp.MinDepth = 0.0f;		//手前
		vp.MaxDepth = 1.0f;		//奥
		vp.TopLeftX = 0;		//左
		vp.TopLeftY = 0;		//上

		//各パターンのシェーダーセット準備
		InitShaderBundle();
		Direct3D::SetShader(Direct3D::SHADER_3D);

		//深度ステンシルビューの作成
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = screenWidth;
		descDepth.Height = screenHeight;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		pDevice_->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
		pDevice_->CreateDepthStencilView(pDepthStencil, NULL, &pDepthStencilView);

		//深度テストを行う深度ステンシルステートの作成
		{
			//デフォルト
			D3D11_DEPTH_STENCIL_DESC desc = {};
			desc.DepthEnable = true;
			desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			desc.StencilEnable = true;
			pDevice_->CreateDepthStencilState(&desc, &pDepthStencilState[BLEND_DEFAULT]);
			pContext_->OMSetDepthStencilState(pDepthStencilState[BLEND_DEFAULT], 0);

			//加算合成用（書き込みなし）
			desc.StencilEnable = false;
			pDevice_->CreateDepthStencilState(&desc, &pDepthStencilState[BLEND_ADD]);
		}

		//ブレンドステート
		{
			//通常
			D3D11_BLEND_DESC BlendDesc;
			ZeroMemory(&BlendDesc, sizeof(BlendDesc));
			BlendDesc.AlphaToCoverageEnable = FALSE;
			BlendDesc.IndependentBlendEnable = FALSE;
			BlendDesc.RenderTarget[0].BlendEnable = TRUE;

			BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

			BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			pDevice_->CreateBlendState(&BlendDesc, &pBlendState[BLEND_DEFAULT]);
			float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
			pContext_->OMSetBlendState(pBlendState[BLEND_DEFAULT], blendFactor, 0xffffffff);

			//加算合成（重なるほど光って見える効果）
			BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			pDevice_->CreateBlendState(&BlendDesc, &pBlendState[BLEND_ADD]);
		}

		//パイプラインの構築
		//データを画面に描画するための一通りの設定
		pContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // データの入力種類を指定
		pContext_->OMSetRenderTargets(1, &pRenderTargetView_, pDepthStencilView);            // 描画先を設定（今後はレンダーターゲットビューを介して描画してね）
		pContext_->RSSetViewports(1, &vp);                                      // ビューポートのセット

		//コリジョン表示するか
		isDrawCollision_ = GetPrivateProfileInt("DEBUG", "ViewCollider", 0, ".\\setup.ini") != 0;


		screenWidth_ = screenWidth;
		screenHeight_ = screenHeight;

		return S_OK;
	}


	//シェーダー関連で必要なセット準備
	void InitShaderBundle()
	{
		DWORD vectorSize = sizeof(XMFLOAT3);

		D3D11_INPUT_ELEMENT_DESC* hlsl_layout = nullptr;
		D3D11_RASTERIZER_DESC r_desc = {};

		auto compile_fxc = [&](std::wstring file,SHADER_TYPE index , uint8_t s_layout)
			{
				HLSLInclude::DefaultInclude hlsl_header = {};

				// 頂点シェーダの作成（コンパイル）
				ID3DBlob* pCompileVS = NULL;
				D3DCompileFromFile(file.c_str(), nullptr, &hlsl_header, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);

				pDevice_->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaderBundle[index].pVertexShader);

				// ピクセルシェーダの作成（コンパイル）
				ID3DBlob* pCompilePS = NULL;
				D3DCompileFromFile(file.c_str(), nullptr, &hlsl_header, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
				pDevice_->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderBundle[index].pPixelShader);


				// 頂点レイアウトの作成（1頂点の情報が何のデータをどんな順番で持っているか）
			
				pDevice_->CreateInputLayout(hlsl_layout, s_layout, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderBundle[index].pVertexLayout);


				//シェーダーが無事作成できたので、コンパイルしたやつはいらない
				pCompileVS->Release();
				pCompilePS->Release();

				pDevice_->CreateRasterizerState(&r_desc, &shaderBundle[index].pRasterizerState);
			};
		
		//3D
		{
			r_desc.CullMode = D3D11_CULL_BACK;
			r_desc.FillMode = D3D11_FILL_SOLID;
			r_desc.FrontCounterClockwise = FALSE;	//反時計回りは表面じゃない

			D3D11_INPUT_ELEMENT_DESC temp[] = 
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, vectorSize * 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//頂点位置
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, vectorSize * 1, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, vectorSize * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//テクスチャ（UV）座標
			};
			
			hlsl_layout = temp;
			compile_fxc(L"Shader/Simple3D.hlsl", Direct3D::SHADER_3D, sizeof(temp) / sizeof(D3D11_INPUT_ELEMENT_DESC));
		}

		//2D
		{
			r_desc.CullMode = D3D11_CULL_BACK;
			r_desc.FillMode = D3D11_FILL_SOLID;
			r_desc.FrontCounterClockwise = TRUE;

			D3D11_INPUT_ELEMENT_DESC temp[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, vectorSize * 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, vectorSize * 1, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			hlsl_layout = temp;
			compile_fxc(L"Shader/Simple2D.hlsl", Direct3D::SHADER_2D, sizeof(temp) / sizeof(D3D11_INPUT_ELEMENT_DESC));
		}

		//DEBUG用
		{
			r_desc.CullMode = D3D11_CULL_NONE;
			r_desc.FillMode = D3D11_FILL_WIREFRAME;
			r_desc.FrontCounterClockwise = TRUE;

			D3D11_INPUT_ELEMENT_DESC temp[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			hlsl_layout = temp;
			compile_fxc(L"Shader/Debug3D.hlsl", Direct3D::SHADER_UNLIT, sizeof(temp) / sizeof(D3D11_INPUT_ELEMENT_DESC));
		}

		//BillBoard
		{
			r_desc.CullMode = D3D11_CULL_NONE;
			r_desc.FillMode = D3D11_FILL_SOLID;
			r_desc.FrontCounterClockwise = TRUE;

			D3D11_INPUT_ELEMENT_DESC temp[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, vectorSize * 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, vectorSize * 1, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			hlsl_layout = temp;
			compile_fxc(L"Shader/BillBoard.hlsl", Direct3D::SHADER_BILLBOARD, sizeof(temp) / sizeof(D3D11_INPUT_ELEMENT_DESC));
		}

		//SKY
		{
			r_desc.CullMode = D3D11_CULL_BACK;
			r_desc.FillMode = D3D11_FILL_SOLID;
			r_desc.FrontCounterClockwise = FALSE;	//反時計回りは表面じゃない

			D3D11_INPUT_ELEMENT_DESC temp[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, vectorSize * 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//頂点位置
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, vectorSize * 1, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, vectorSize * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//テクスチャ（UV）座標
			};

			hlsl_layout = temp;
			compile_fxc(L"Shader/Sky.hlsl", Direct3D::SHADER_SKY, sizeof(temp) / sizeof(D3D11_INPUT_ELEMENT_DESC));
		}

		//BAR
		{
			r_desc.CullMode = D3D11_CULL_BACK;
			r_desc.FillMode = D3D11_FILL_SOLID;
			r_desc.FrontCounterClockwise = TRUE;

			D3D11_INPUT_ELEMENT_DESC temp[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, vectorSize * 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, vectorSize * 1, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			hlsl_layout = temp;
			compile_fxc(L"Shader/Progress.hlsl", Direct3D::SHADER_BAR, sizeof(temp) / sizeof(D3D11_INPUT_ELEMENT_DESC));
		}

		//DAMAGE
		{
			r_desc.CullMode = D3D11_CULL_BACK;
			r_desc.FillMode = D3D11_FILL_SOLID;
			r_desc.FrontCounterClockwise = FALSE;

			D3D11_INPUT_ELEMENT_DESC temp[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, vectorSize * 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//頂点位置
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, vectorSize * 1, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, vectorSize * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//テクスチャ（UV）座標
			};

			hlsl_layout = temp;
			compile_fxc(L"Shader/Damage.hlsl", Direct3D::SHADER_DAMAGE, sizeof(temp) / sizeof(D3D11_INPUT_ELEMENT_DESC));
		}

		// タイマー
		{
			r_desc.CullMode = D3D11_CULL_BACK;
			r_desc.FillMode = D3D11_FILL_SOLID;
			r_desc.FrontCounterClockwise = TRUE;

			D3D11_INPUT_ELEMENT_DESC temp[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, vectorSize * 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, vectorSize * 1, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			hlsl_layout = temp;
			compile_fxc(L"Shader/Timer.hlsl", Direct3D::SHADER_TIMER, sizeof(temp) / sizeof(D3D11_INPUT_ELEMENT_DESC));
		}

		//敵ボス
		{
			r_desc.CullMode = D3D11_CULL_BACK;
			r_desc.FillMode = D3D11_FILL_SOLID;
			r_desc.FrontCounterClockwise = FALSE;	//反時計回りは表面じゃない

			D3D11_INPUT_ELEMENT_DESC temp[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, vectorSize * 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//頂点位置
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, vectorSize * 1, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, vectorSize * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//テクスチャ（UV）座標
			};

			hlsl_layout = temp;
			compile_fxc(L"Shader/Enemy_BOSS.hlsl", Direct3D::SHADER_BOSS, sizeof(temp) / sizeof(D3D11_INPUT_ELEMENT_DESC));
		}

		//レアフラワー
		{
			r_desc.CullMode = D3D11_CULL_BACK;
			r_desc.FillMode = D3D11_FILL_SOLID;
			r_desc.FrontCounterClockwise = FALSE;	//反時計回りは表面じゃない

			D3D11_INPUT_ELEMENT_DESC temp[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, vectorSize * 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//頂点位置
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, vectorSize * 1, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, vectorSize * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//テクスチャ（UV）座標
			};

			hlsl_layout = temp;
			compile_fxc(L"Shader/RareFlower.hlsl", Direct3D::SHADER_RAREFLOWER, sizeof(temp) / sizeof(D3D11_INPUT_ELEMENT_DESC));
		}
	}


	//今から描画するShaderBundleを設定
	void SetShader(SHADER_TYPE type)
	{
		pContext_->RSSetState(shaderBundle[type].pRasterizerState);
		pContext_->VSSetShader(shaderBundle[type].pVertexShader, NULL, 0);                         // 頂点シェーダをセット
		pContext_->PSSetShader(shaderBundle[type].pPixelShader, NULL, 0);                          // ピクセルシェーダをセット
		pContext_->IASetInputLayout(shaderBundle[type].pVertexLayout);
	}

	//ブレンドモードの変更
	void SetBlendMode(BLEND_MODE blendMode)
	{
		//加算合成
		float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
		pContext_->OMSetBlendState(pBlendState[blendMode], blendFactor, 0xffffffff);
		
		//Zバッファへの書き込み
		pContext_->OMSetDepthStencilState(pDepthStencilState[blendMode], 0);
	}

	//描画開始
	void BeginDraw()
	{
		//何か準備できてないものがあったら諦める
		if (NULL == pDevice_) return;
		if (NULL == pContext_) return;
		if (NULL == pRenderTargetView_) return;
		if (NULL == pSwapChain_) return;

		//背景の色
		float clearColor[4] = { 0.0f, 1.f, 0.0f, 1.0f };//R,G,B,A

		//画面をクリア
		pContext_->ClearRenderTargetView(pRenderTargetView_, clearColor);

		//深度バッファクリア
		pContext_->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);	
	}


	//描画終了
	void EndDraw()
	{
		//スワップ（バックバッファを表に表示する）
		pSwapChain_->Present(0, 0);
	}


	//開放処理
	void Release()
	{
		SAFE_RELEASE(pDepthStencil);
		SAFE_RELEASE(pDepthStencilView);
		SAFE_RELEASE(pRenderTargetView_);
		SAFE_RELEASE(pSwapChain_);

		for (int i = 0; i < BLEND_MAX; i++)
		{
			SAFE_RELEASE(pBlendState[i]);
			SAFE_RELEASE(pDepthStencilState[i]);
		}

		for (int i = 0; i < SHADER_MAX; i++)
		{
			SAFE_RELEASE(shaderBundle[i].pRasterizerState);
			SAFE_RELEASE(shaderBundle[i].pVertexLayout);
			SAFE_RELEASE(shaderBundle[i].pVertexShader);
			SAFE_RELEASE(shaderBundle[i].pPixelShader);
		}

		if (pContext_)
		{
			pContext_->ClearState();
		}

		SAFE_RELEASE(pRenderTargetView_);
		SAFE_RELEASE(pSwapChain_);
		SAFE_RELEASE(pContext_);
		SAFE_RELEASE(pDevice_);
	}


	//三角形と線分の衝突判定（衝突判定に使用）
	//https://pheema.hatenablog.jp/entry/ray-triangle-intersection
	bool Intersect(XMFLOAT3& start, XMFLOAT3& direction, XMFLOAT3& v0, XMFLOAT3& v1, XMFLOAT3& v2, float* distance, XMVECTOR* pos)
	{
		// 微小な定数([M?ller97] での値)
		constexpr float kEpsilon = 1e-6f;

		//三角形の２辺
		XMVECTOR edge1 = XMVectorSet(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z, 0.0f);
		XMVECTOR edge2 = XMVectorSet(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z, 0.0f);

		XMVECTOR alpha = XMVector3Cross(XMLoadFloat3(&direction), edge2);
		float det = XMVector3Dot(edge1, alpha).m128_f32[0];

		// 三角形に対して、レイが平行に入射するような場合 det = 0 となる。
		// det が小さすぎると 1/det が大きくなりすぎて数値的に不安定になるので
		// det ? 0 の場合は交差しないこととする。
		if (-kEpsilon < det && det < kEpsilon)
		{
			return false;
		}

		float invDet = 1.0f / det;
		XMFLOAT3 r = XMFLOAT3(start.x - v0.x, start.y - v0.y, start.z - v0.z);

		// u が 0 <= u <= 1 を満たしているかを調べる。
		float u = XMVector3Dot(alpha, XMLoadFloat3(&r)).m128_f32[0] * invDet;
		if (u < 0.0f || u > 1.0f)
		{
			return false;
		}

		XMVECTOR beta = XMVector3Cross(XMLoadFloat3(&r), edge1);

		// v が 0 <= v <= 1 かつ u + v <= 1 を満たすことを調べる。
		// すなわち、v が 0 <= v <= 1 - u をみたしているかを調べればOK。
		float v = XMVector3Dot(XMLoadFloat3(&direction), beta).m128_f32[0] * invDet;
		if (v < 0.0f || u + v > 1.0f)
		{
			return false;
		}

		// t が 0 <= t を満たすことを調べる。
		float t = XMVector3Dot(edge2, beta).m128_f32[0] * invDet;
		if (t < 0.0f)
		{
			return false;
		}

		*distance = t;
		*pos = XMLoadFloat3(&v0) + (edge1 * u) + (edge2 * v);

		return true;
	}

	//Zバッファへの書き込みON/OFF
	void SetDepthBafferWriteEnable(bool isWrite)
	{
		//ON
		if (isWrite)
		{
			//Zバッファ（デプスステンシルを指定する）
			pContext_->OMSetRenderTargets(1, &pRenderTargetView_, pDepthStencilView);
		}

		//OFF
		else
		{
			pContext_->OMSetRenderTargets(1, &pRenderTargetView_, nullptr);
		}
	}

	void SetViewport(int width, int height)
	{
		// ビューポートの設定
		//レンダリング結果を表示する範囲
		float viewPortMagnification = 1.f;

		//ビューポートの設定
		D3D11_VIEWPORT vp; {
			vp.Height = (float)height * viewPortMagnification;	//高さ
			vp.Width = (float)width * viewPortMagnification;	//幅
			vp.MinDepth = 0.0f;									//手前
			vp.MaxDepth = 1.0f;									//奥
			vp.TopLeftX = 0;									//左
			vp.TopLeftY = 0;									//上
		}
		
		// ビューポートをセット
		pContext_->RSSetViewports(1, &vp);                  

		//スクリーンサイズを保存
		screenWidth_ = width;
		screenHeight_ = height;
	}

	void GetFullScreenSize(int& width, int& height) {

		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
	}

}

HRESULT HLSLInclude::DefaultInclude::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes)
{
	std::string file = pFileName;
	std::string dir = "../Assets/Shader/"; //exeファイルからhlsliまでのディレクトリ

#ifdef _DEBUG
	auto temp = (dir + file);			   //テスト用。ちゃんと正しいファイル名になっているか確認してね
#endif

	std::ifstream istr((dir+file),std::ios::binary);//バイナリじゃないと大変なことになる。具体的には領域外を参照する
	istr.seekg(0,std::ios::end);
	*pBytes = istr.tellg();	

	*ppData = new char[*pBytes];
	
	istr.seekg(0,std::ios::beg);			//読み込みのために場所を最初に戻す

	char* buf = const_cast<char*>(static_cast<const char*>(*ppData));//はじめてコンストキャスト使った
	istr.read(buf, *pBytes);				//バッファへコピー

	return S_OK;
}

HRESULT HLSLInclude::DefaultInclude::Close(LPCVOID pData)
{
	delete[] static_cast<const char*>(pData);

	return S_OK;
}
