//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
	float4x4	g_matWVP;			// ワールド・ビュー・プロジェクションの合成行列
	float4x4	g_matNormalTrans;	// 法線の変換行列（回転行列と拡大の逆行列）
	float4x4	g_matWorld;			// ワールド変換行列
	float4		g_vecLightDir;		// ライトの方向ベクトル
    float4      g_light;            // Color of light and Intensity
    float4      g_ambinetLight;     // Color of light and Intensity
	float4		g_vecDiffuse;		// ディフューズカラー（マテリアルの色）
	float4		g_vecAmbient;		// アンビエントカラー（影の色）
	float4		g_vecSpeculer;		// スペキュラーカラー（ハイライトの色）
	float4		g_vecCameraPosition;// 視点（カメラの位置）
	float		g_shuniness;		// ハイライトの強さ（テカリ具合）
	int			g_isTexture;		// テクスチャ貼ってあるかどうか
    float		val0;				//シェーダーの時々に応じて使う汎用数値
    float		val1;				//シェーダーの時々に応じて使う汎用数値
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float4 normal : TEXCOORD2; //法線
    float2 uv : TEXCOORD0; //UV座標
    float4 eye : TEXCOORD1; //視線
    float4 globalPos : TEXCOORD3; //ワールド座標
    float4 localPos : TEXCOORD4;
    //float4 shadowPos : TEXCOORD3;	//影の位置	
};
