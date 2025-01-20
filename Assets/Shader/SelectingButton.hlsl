#include"CB2D.hlsli"
//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUTPUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)
{
    VS_OUTPUT output;
    output.pos.xy *= 0.8;
    output.pos = mul(pos, g_matWorld);
    output.uv = mul(uv, g_matTexture);
    return output;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 col= g_vecColor * g_texture.Sample(g_sampler, input.uv);
    //col.rgb *= 0.7;
    int p = 2;

    float edge = pow(abs(0.5 - input.uv.x),p);
    edge += pow(abs(0.5 - input.uv.y), p);
    
    edge = pow((0.5-input.uv.x), 2) + pow((0.5-input.uv.y), 2);
    edge = pow(edge, 2);
    edge *= 3;
    
    col.rg += edge;
    col.b -= edge;

    return col;
}