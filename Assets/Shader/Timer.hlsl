#include"CB2D.hlsli"
//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUTPUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)
{
    VS_OUTPUT output;
    output.pos = mul(pos, g_matWorld);
    output.uv = mul(uv, g_matTexture);
    output.gpos = pos;
    return output;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUTPUT input) : SV_Target
{
    //return g_vecColor * g_texture.Sample(g_sampler, input.uv);
    
    float2 uvpos = float2(2.0 * input.uv.x - 1, 1.0 - 2 * input.uv.y);
    float ang = atan2(uvpos.y, uvpos.x);
    float dig = degrees(ang);
    if (dig < 0)
        dig = dig + 360;
    if (dig > g_angle.x && dig < g_angle.y)
    {
        return g_vecColor * g_texture.Sample(g_sampler, input.uv);
    }
    else
        return (0, 0, 0, 0);
}