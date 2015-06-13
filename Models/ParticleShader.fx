cbuffer cbChangesEveryFrame : register( b0 )
{
    matrix worldMatrix;
};

cbuffer cbNeverChanges : register( b1 )
{
    matrix viewMatrix;
};

cbuffer cbChangeOnResize : register( b2 )
{
    matrix projMatrix;
};

cbuffer cbCameraData : register( b3 )
{
    float3 cameraPos;
};

struct VS_Input
{
    float4 pos  : POSITION;
    float3 col : COLOUR;
    float3 norm : NORMAL;
};

struct PS_Input
{
    float4 pos  : SV_POSITION;
    float3 col : COLOUR;
    float3 norm : NORMAL;
    float3 lightVec : TEXCOORD1;
	float3 viewVec : TEXCOORD2;
};


PS_Input VS_Main( VS_Input vertex )
{
    PS_Input vsOut = ( PS_Input )0;
    
	//vsOut.pos = mul( vertex.pos, worldMatrix );
	float4 worldPos = mul( vertex.pos, worldMatrix );

    //vsOut.pos = mul( vsOut.pos, viewMatrix );
	vsOut.pos = mul( worldPos, viewMatrix );

    vsOut.pos = mul( vsOut.pos, projMatrix );
    vsOut.col = vertex.col;

	vsOut.norm = mul( vertex.norm, (float3x3)worldMatrix );
    vsOut.norm = normalize( vsOut.norm );

    //float3 lightPos = float3( 0.0f, 5000.0f, 5000.0f );
	float3 lightPos = float3( 0.0f, 5000.0f, 0.0f );
    vsOut.lightVec = normalize( lightPos - worldPos );

    vsOut.viewVec = normalize( cameraPos - worldPos );
	
    return vsOut;
}

float4 PS_Main( PS_Input frag ) : SV_TARGET
{
	float3 textureColor = frag.col;

	float3 ambientColor = float3( 0.2f, 0.2f, 0.2f );
    float3 lightColor = float3( 0.7f, 0.7f, 0.7f );

    float3 lightVec = normalize( frag.lightVec );
    float3 normal = normalize( frag.norm );

    float diffuseTerm = clamp( dot( normal, lightVec ), 0.0f, 1.0f );
    float specularTerm = 0;

    if( diffuseTerm > 0.0f )
    {
        float3 viewVec = normalize( frag.viewVec );
        float3 halfVec = normalize( lightVec + viewVec );

        specularTerm = pow( saturate( dot( normal, halfVec ) ), 25 );
    }

    float3 finalLightColor = ambientColor + lightColor * diffuseTerm + lightColor * specularTerm;

	float3 finalColor = finalLightColor*textureColor;

    return float4( finalColor, 1.0f );
}
