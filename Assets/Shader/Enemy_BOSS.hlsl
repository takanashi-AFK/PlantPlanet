#include"CB3D.hlsli"
#include"Alpha.hlsli"

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
    
    //Dithering(inData.pos.xy, diffuse.a);
	
	//ライトの向き
    float4 lightDir = g_vecLightDir; //グルーバル変数は変更できないので、いったんローカル変数へ
    lightDir = normalize(lightDir); //向きだけが必要なので正規化

	//法線はピクセルシェーダーに持ってきた時点で補完され長さが変わっている
	//正規化しておかないと面の明るさがおかしくなる
    inData.normal = normalize(inData.normal);
	
	//拡散反射光（ディフューズ）
	//法線と光のベクトルの内積が、そこの明るさになる
    float4 shade = saturate(dot(inData.normal, -lightDir));
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
    
    //独特のボスカラー
    
    float wv = frac(sin(inData.localPos.y) * 4);
    
    float m = frac(pow(frac((wv + (frac(val1 * 0.05)))), 18));
   // m += frac(pow(frac(inData.localPos.y + frac(val1 * 0.2)), 18));
    //m += frac(pow(frac(inData.localPos.z + frac(val1 * 0.1)), 18));
    m /= 3;

    float4 bcol = float4(abs(inData.localPos.xyz), 1);

	//最終的な色
	
    float grad = 4.0f;
    float base = 0.6f;
	
    int temp = (shade - floor(shade)) * grad;
    float th = (temp / grad) + base;
	
    th = min(th, 1);
    
    float cr = sin(frac(bcol.r));
    float cg = sin(frac(bcol.g));
    float cb = sin(frac(bcol.b));
    
    bcol = float4(cr,cg,cb, 1);
    bcol = pow(bcol, 0.8)*1.2;
    
    return (float4(diffuse.rgb * th, 1) *0.65) + (float4(m * 0.6, 0, 0, 0) * (1 * (0.7 + sin(val1)*0.5)) + bcol*0.25);
    //return diffuse * (shade + ambient + speculer);
    
}