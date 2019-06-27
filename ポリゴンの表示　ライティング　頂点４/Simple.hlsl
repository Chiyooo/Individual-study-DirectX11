//グローバル
cbuffer global
{
	matrix g_mW;//ワールド変換
	matrix g_mWVP; //ワールドから射影までの変換行列
	float4 g_vLightDir;//ライトの方向ベクトル
	float4 g_Diffuse; //= float4 (1, 0, 0, 1);//拡散反射（色）

};

//
//バーテックスシェーダー
//
struct VSOut {
	float4	pos : SV_POSITION;
	float4 Color : COLOR0;
};
struct GSOut {
	float4	pos : SV_POSITION;
	float4  Color : COLOR0;
};

VSOut VS( float3 Pos : POSITION , float3 Normal : NORMAL)
{   
	VSOut r = (VSOut)0;

	r.pos = mul(float4(Pos.xyz,1), g_mWVP);
	Normal = mul(Normal, g_mW);
	Normal = normalize(Normal);
	r.Color = 1.0 * g_Diffuse * dot(float4(Normal.xyz,0), -g_vLightDir);//ランバートの余弦則

    return r;  
}

//GS
[maxvertexcount(12)]
void GS(triangle VSOut ip_[3], inout TriangleStream<GSOut> TriStream)
{
	GSOut  output[3];
	output[0].pos = ip_[0].pos;
	output[1].pos = ip_[1].pos;
	output[2].pos = ip_[2].pos;
	
	output[0].Color = ip_[0].Color;
	output[1].Color = ip_[1].Color;
	output[2].Color = ip_[2].Color;
	
	//出力（６頂点分）
	TriStream.Append(output[0]);
	TriStream.Append(output[1]);
	TriStream.Append(output[2]);
	
	TriStream.RestartStrip();//プリミティブ１個分毎に
	output[0].pos += float4(1, 0, 0, 0);
	output[1].pos += float4(1, 0, 0, 0);
	output[2].pos += float4(1, 0, 0, 0);
	
	
	//出力（６頂点分）
	TriStream.Append(output[0]);
	TriStream.Append(output[1]);
	TriStream.Append(output[2]);
	
	TriStream.RestartStrip();//プリミティブ１個分毎に
	output[0].pos += float4(-2, 0, 0, 0);
	output[1].pos += float4(-2, 0, 0, 0);
	output[2].pos += float4(-2, 0, 0, 0);
	
	//出力（６頂点分）
	TriStream.Append(output[0]);
	TriStream.Append(output[1]);
	TriStream.Append(output[2]);

	TriStream.RestartStrip();//プリミティブ１個分毎に
}

//
//ピクセルシェーダー
//
float4 PS(GSOut i) : SV_Target
{
	return i.Color;
}