cbuffer ConstantBuffer : register(b0)
{
	float3x3 worldMatrix;
	float2 screenSize;
	float2 pad2;
	float4 blendRate;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 vertexColor : COLOR;
};

Texture2D TextureMapTexture : register(t0);
SamplerState TextureMapSampler : register(s0);

float4 main(PS_INPUT input) : SV_Target
{
	return input.vertexColor * blendRate;
}