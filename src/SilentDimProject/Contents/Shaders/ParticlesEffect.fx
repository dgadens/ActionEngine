//------- XNA interface --------
float4x4 xView;
float4x4 xProjection;
float4x4 xWorld;
float3 xCamPos;
float3 xAllowedRotDir;
float3 xCamUp;
float xTime;
float4 xGeneric; //velocity, maxSize, 0, 0

//------- Texture Samplers --------
Texture2D txTexture;
SamplerState samLinear 
{ 
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = CLAMP; 
    AddressV = CLAMP; 
};

struct VS_INPUT
{
    float4 Position  	  : POSITION;
    float4 TexCoord  	  : TEXCOORD0;
    float4 AditionalInfo  : TEXCOORD1;
};

struct PS_INPUT
{
    float4 Position  : SV_POSITION;
    float2 TexCoord  : TEXCOORD0;
    float4 Color     : COLOR0;
};

//------- Technique: Explosion --------
float3 BillboardVertex(float3 billboardCenter, float2 cornerID, float size)
{
	float3 eyeVector = billboardCenter - xCamPos;		
	
	float3 sideVector = cross(eyeVector,xCamUp);
	sideVector = normalize(sideVector);
	float3 upVector = cross(sideVector,eyeVector);
	upVector = normalize(upVector);
	
	float3 finalPosition = billboardCenter;
	finalPosition += (cornerID.x-0.5f)*sideVector*size;
	finalPosition += (0.5f-cornerID.y)*upVector*size;	
	
	return finalPosition;
}

PS_INPUT ExplosionVS(VS_INPUT input)
{
	PS_INPUT Output = (PS_INPUT)0;	

	float3 startingPosition = float3(0,0,0);
	
	float2 texCoords = input.TexCoord.xy;
	float birthTime = input.TexCoord.z;
	float maxAge = input.TexCoord.w;
	float3 moveDirection = input.AditionalInfo.xyz;
	float random = input.AditionalInfo.w;	
	
	float age = xTime - birthTime;	
	float relAge = age/maxAge;	
	
	float sizer = saturate(1-relAge*relAge/2.0f);
  	float size = xGeneric.y*random*sizer;
    
	float totalDisplacement = sin(relAge*6.28f/4.0f)*xGeneric.x*random;
	float3 billboardCenter = startingPosition + totalDisplacement*moveDirection;	
	billboardCenter += age*float3(0,-1,0)/2000.0f;		
	
	float3 finalPosition = BillboardVertex(billboardCenter, texCoords, size);
	float4 finalPosition4 = float4(finalPosition, 1);
		
    	Output.Position= mul( finalPosition4, xWorld );
	Output.Position= mul( Output.Position, xView );
	Output.Position= mul( Output.Position, xProjection );
	
	float alpha = 1-relAge*relAge;
	Output.Color = float4(0.5f,0.5f,0.5f,alpha);	

	Output.TexCoord = texCoords;	

	return Output;
}

float4 ExplosionPS(PS_INPUT input) : SV_Target
{
	float4 ret = txTexture.Sample( samLinear, input.TexCoord) * input.Color;
	return ret;
}


technique10 TechniqueExplosion
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, ExplosionVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, ExplosionPS() ) );
    }
}

PS_INPUT SmokeVS(VS_INPUT input)
{
	PS_INPUT Output = (PS_INPUT)0;	

	float3 startingPosition = float3(0,0,0);
	
	float2 texCoords = input.TexCoord.xy;
	float birthTime = input.TexCoord.z;
	float maxAge = input.TexCoord.w;
	float3 moveDirection = input.AditionalInfo.xyz;
	float random = input.AditionalInfo.w;	

	float age = xTime - birthTime;	
	float relAge = age/maxAge;
	
  	float size = relAge * xGeneric.y *random;
    
	float totalDisplacement = sin((age/maxAge)*6.28f/4.0f)*xGeneric.x*random;
	float3 billboardCenter = startingPosition + totalDisplacement*moveDirection;	
	billboardCenter += age*float3(0,-1,0)/1000.0f;		
	
	float3 finalPosition = BillboardVertex(billboardCenter, texCoords, size);
	float4 finalPosition4 = float4(finalPosition, 1);

    	Output.Position= mul( finalPosition4, xWorld );
	Output.Position= mul( Output.Position, xView );
	Output.Position= mul( Output.Position, xProjection );
			
	float alpha = 1-relAge*relAge;
	Output.Color = float4(1,1,1,alpha);	

	Output.TexCoord = texCoords;	

	return Output;
}


technique10 TechniqueSmoke
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, SmokeVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, ExplosionPS() ) );
    }
}

PS_INPUT TrailVS(VS_INPUT input)
{
	PS_INPUT Output = (PS_INPUT)0;	

	float2 texCoords = input.TexCoord.xy;
	float birthTime = input.TexCoord.z;
	float maxAge = input.TexCoord.w;

	float age = fmod(xTime-birthTime, maxAge);	
	float relAge= age/maxAge;
   
	float3 finalPosition = BillboardVertex(input.Position.xyz, texCoords, relAge);
	float4 finalPosition4 = float4(finalPosition, 1);
	
	//coloca o centro no meio denovo para aplicar a matriz world, depois volta para a posicao
	float4 basePosition = float4(finalPosition - input.Position.xyz,1);
	basePosition = mul( basePosition, xWorld );	
	finalPosition4 = basePosition + float4(input.Position.xyz,0);

    	Output.Position= finalPosition4;
	Output.Position= mul( Output.Position, xView );
	Output.Position= mul( Output.Position, xProjection );
			
	float alpha = 1-relAge*relAge;
	Output.Color = float4(1,1,1,alpha);	

	Output.TexCoord = texCoords;	

	return Output;
}


technique10 TechniqueTrail
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, TrailVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, ExplosionPS() ) );
    }
}
