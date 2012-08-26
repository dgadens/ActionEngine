//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
matrix xWorld;
matrix xView;
matrix xProjection;
float3 xLightPosition;

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos          : POSITION;
    float3 Normal       : NORMAL;
    float2 Texcoord1    : TEXCOORD;
   // float2 Texcoord2  : TEXCOORD2;
};

struct PS_INPUT
{
    float4 Pos 	     	: SV_POSITION;
    float3 Normal    	: TEXCOORD0;
    float2 Texcoord1 	: TEXCOORD1;
    float3 Pos3D     	: TEXCOORD2;
};

Texture2D txDiffuse;
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

    output.Pos= mul( input.Pos, xWorld );
    output.Pos= mul( output.Pos, xView );
    output.Pos= mul( output.Pos, xProjection );
    output.Pos3D = mul( input.Pos, xWorld ).xyz;
    output.Normal = mul( input.Normal, xWorld );
    output.Texcoord1 = input.Texcoord1;
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
    float4 color = float4(1,1,1,1);

    float3 lightDir = normalize(xLightPosition - input.Pos3D);
    float lightFactor = dot(lightDir, normalize(input.Normal));
    color = color * lightFactor;
    color.a = 1.0f;	
    
    return txDiffuse.Sample( samLinear, input.Texcoord1 ) * color;
}


//--------------------------------------------------------------------------------------
technique10 Technique1
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}


