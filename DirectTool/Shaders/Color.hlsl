cbuffer VS_Camera : register(b10)
{
    matrix View;
    matrix Projection;
    matrix ViewProjection;
    matrix InvView;
    matrix InvProjection;
    matrix InvViewProjection;

};

cbuffer VS_Camera : register(b9)
{
    matrix World;
};
cbuffer Buffer_Position : register(b1)
{
    float2 Position;
    float2 Padding;
}

static const float2 arrBasePos[4] =
{
    float2(-1.0, 1.0),
	float2(1.0, 1.0),
	float2(-1.0, -1.0),
	float2(1.0, -1.0),


};
static const float2 WinSize = { 1280, 720 };

static const float2 arrUV[4] =
{
    float2(0.0, 0.0),
	float2(1.0, 0.0),
	float2(0.0, 1.0),
	float2(1.0, 1.0),
};


struct PixelInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCROOD0;
};

PixelInput VS(uint VertexID : SV_VertexID)
{
    PixelInput output;

   output.position = float4(arrBasePos[VertexID].xy, 0.0, 1.0);
   output.uv = arrUV[VertexID].xy;
   


    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 color = input.color;

    float2 origin = Position;

    color = float4(0, 0, 0, 0.3f);
    float dx = abs(input.position.x - origin.x);

    float dy = abs(input.position.y - origin.y);

    float distance = sqrt(dx * dx + dy * dy);
    

    if(distance < 200)
    {
        color.r = 1;
        float2 temp = origin - input.position.xy;


        //color.a = (distance - abs(temp)) / distance;
 
        discard;
        //if(color.a <0.05f)
           // discard;

    }

    return color;
}