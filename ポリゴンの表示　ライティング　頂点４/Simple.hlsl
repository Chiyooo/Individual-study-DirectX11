//�O���[�o��
cbuffer global
{
	matrix g_mW;//���[���h�ϊ�
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_vLightDir;//���C�g�̕����x�N�g��
	float4 g_Diffuse; //= float4 (1, 0, 0, 1);//�g�U���ˁi�F�j

};

//
//�o�[�e�b�N�X�V�F�[�_�[
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
	r.Color = 1.0 * g_Diffuse * dot(float4(Normal.xyz,0), -g_vLightDir);//�����o�[�g�̗]����

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
	
	//�o�́i�U���_���j
	TriStream.Append(output[0]);
	TriStream.Append(output[1]);
	TriStream.Append(output[2]);
	
	TriStream.RestartStrip();//�v���~�e�B�u�P������
	output[0].pos += float4(1, 0, 0, 0);
	output[1].pos += float4(1, 0, 0, 0);
	output[2].pos += float4(1, 0, 0, 0);
	
	
	//�o�́i�U���_���j
	TriStream.Append(output[0]);
	TriStream.Append(output[1]);
	TriStream.Append(output[2]);
	
	TriStream.RestartStrip();//�v���~�e�B�u�P������
	output[0].pos += float4(-2, 0, 0, 0);
	output[1].pos += float4(-2, 0, 0, 0);
	output[2].pos += float4(-2, 0, 0, 0);
	
	//�o�́i�U���_���j
	TriStream.Append(output[0]);
	TriStream.Append(output[1]);
	TriStream.Append(output[2]);

	TriStream.RestartStrip();//�v���~�e�B�u�P������
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS(GSOut i) : SV_Target
{
	return i.Color;
}