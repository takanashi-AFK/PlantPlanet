#include"CB3D.hlsli"

//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D		g_texture: register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー


//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 Normal : NORMAL, float2 Uv : TEXCOORD)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, g_matWVP);		

	//法線の変形
	Normal.w = 0;					//4次元目は使わないので0
	Normal = mul(Normal, g_matNormalTrans);		//オブジェクトが変形すれば法線も変形
	outData.normal = Normal;		//これをピクセルシェーダーへ

	//視線ベクトル（ハイライトの計算に必要
	float4 worldPos = mul(pos, g_matWorld);					//ローカル座標にワールド行列をかけてワールド座標へ
	outData.eye = normalize(g_vecCameraPosition - worldPos);	//視点から頂点位置を引き算し視線を求めてピクセルシェーダーへ

	//UV「座標
	outData.uv = Uv;	//そのままピクセルシェーダーへ
	
    outData.globalPos = worldPos;
    outData.localPos = pos;

   // outData.pos = mul(outData.pos,g_matShadow); //影行列を使って影の位置を計算
	//まとめて出力
	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
	//ライトの向き
	float4 lightDir = g_vecLightDir;	//グルーバル変数は変更できないので、いったんローカル変数へ
	lightDir = normalize(lightDir);	//向きだけが必要なので正規化

	//法線はピクセルシェーダーに持ってきた時点で補完され長さが変わっている
	//正規化しておかないと面の明るさがおかしくなる
	inData.normal = normalize(inData.normal);
	
	//拡散反射光（ディフューズ）
	//法線と光のベクトルの内積が、そこの明るさになる
	float4 shade = saturate(dot(inData.normal, -lightDir));
	shade.a = 1;	//暗いところが透明になるので、強制的にアルファは1

	float4 diffuse;
	//テクスチャ有無
	if (g_isTexture == true)
	{
		//テクスチャの色
		diffuse = g_texture.Sample(g_sampler, inData.uv);
	}
	else
	{
		//マテリアルの色
		diffuse = g_vecDiffuse;
	}

	//環境光（アンビエント）
	//これはMaya側で指定し、グローバル変数で受け取ったものをそのまま
	float4 ambient = g_vecAmbient;

	//鏡面反射光（スペキュラー）
	float4 speculer = float4(0, 0, 0, 0);	//とりあえずハイライトは無しにしておいて…
	if (g_vecSpeculer.a != 0)	//スペキュラーの情報があれば
	{
		float4 R = reflect(lightDir, inData.normal);			//正反射ベクトル
		speculer = pow(saturate(dot(R, inData.eye)), g_shuniness) * g_vecSpeculer;	//ハイライトを求める
	}

    float4 dmgcol = diffuse * float4(0.8f, 0, 0, 1);
	
	//最終的な色
    dmgcol = dmgcol.r < 0.2f ? float4(0.7f, 0, 0, 1) : dmgcol;
	
    return dmgcol;
    
}