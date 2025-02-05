#include<Alpha.hlsli>
#include"CB3D.hlsli"
//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

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
    Normal.w = 0; //4次元目は使わないので0
    Normal = mul(Normal, g_matNormalTrans); //オブジェクトが変形すれば法線も変形
    outData.normal = Normal; //これをピクセルシェーダーへ

	//視線ベクトル（ハイライトの計算に必要
    float4 worldPos = mul(pos, g_matWorld); //ローカル座標にワールド行列をかけてワールド座標へ
    outData.eye = normalize(g_vecCameraPosition - worldPos); //視点から頂点位置を引き算し視線を求めてピクセルシェーダーへ

	//UV「座標
    outData.uv = Uv; //そのままピクセルシェーダーへ

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
	
  //  Dithering(inData.pos.xy, diffuse.a);
	
	//ライトの向き
    float4 lightDir = g_vecLightDir; //グルーバル変数は変更できないので、いったんローカル変数へ
    lightDir = normalize(lightDir); //向きだけが必要なので正規化

	//法線はピクセルシェーダーに持ってきた時点で補完され長さが変わっている
	//正規化しておかないと面の明るさがおかしくなる
    inData.normal = normalize(inData.normal);
	
	//拡散反射光（ディフューズ）
	//法線と光のベクトルの内積が、そこの明るさになる
    float4 shade = saturate(dot(inData.normal, -lightDir)) * (g_light);
    shade.rgb *= g_light.a;
    shade.a = 1; //暗いところが透明になるので、強制的にアルファは1

	//環境光（アンビエント）
	//これはMaya側で指定し、グローバル変数で受け取ったものをそのまま
    float4 ambient = g_vecAmbient;
    ambient = 0.3;

	//鏡面反射光（スペキュラー）
    float4 speculer = float4(0, 0, 0, 0); //とりあえずハイライトは無しにしておいて…
    if (g_vecSpeculer.a != 0)	//スペキュラーの情報があれば
    {
        float4 R = reflect(lightDir, inData.normal); //正反射ベクトル
        speculer = pow(saturate(dot(R, inData.eye)), g_shuniness) * g_vecSpeculer; //ハイライトを求める
    }

	//最終的な色
	
    float grad = 4.0f;
    float base = g_ambinetLight.a;
	
    int3 temp = int3((shade.rgb - floor(shade.rgb)) * grad);
    float3 th = float3((temp.rgb / grad) + base);
	
    th.r = min(th.r, 1);
    th.g = min(th.g, 1);
    th.b = min(th.b, 1);
	
    diffuse.rgb += (g_ambinetLight.rgb);
    shade.rgb *= .5;
    
    float4 col = float4(diffuse.rgb * th + shade.rgb, 1);//beautiful shader
    //float4 col = float4(diffuse.rgb * th.rgb, 1);
    float depth = pow(inData.pos.z ,400);
    float depth_chop = pow(inData.pos.z, 250);
    
    float4 far_col = (depth * float4(depth * .5, depth * .7, depth, 1)) + (col * (1 - depth));
    float4 near_col = float4(col.r, col.g * (.3 * depth_chop + 1), col.b * (sin(depth*1.3)  + 1), 1);
    
    col = depth > 0.6 ? far_col : near_col;
    
    col.a = 1;
    clip(depth < 0.0000001 ? -1 : 1);
    
    return float4(col);
}