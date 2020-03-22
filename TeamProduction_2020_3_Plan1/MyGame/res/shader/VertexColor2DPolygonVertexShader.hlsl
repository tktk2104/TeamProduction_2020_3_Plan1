cbuffer ConstantBuffer : register(b0)
{
	float3x3 worldMatrix;
	float2 screenSize;
	float2 pad2;
	float4 blendRate;
};

struct VS_INPUT
{
	float4 position : POSITION;
	float4 vertexColor : COLOR;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 vertexColor : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.position = input.position;
	output.position.z = 1.0f;
	output.position.xyz = mul(worldMatrix, output.position.xyz);
	output.vertexColor = input.vertexColor;
	output.position.xy = output.position.xy * float2(2.0 / screenSize.x, 2.0 / -screenSize.y) + float2(-1.0, 1.0);

	return output;
}