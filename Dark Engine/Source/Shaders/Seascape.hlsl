#include "Common.hlsl"


static const int NUM_STEPS = 8;
static const float PI	 	= 3.1415;
static const float EPSILON	= 1e-3;

// sea
static const int ITER_GEOMETRY = 3;
static const int ITER_FRAGMENT = 5;
static const float SEA_HEIGHT = 0.6;
static const float SEA_CHOPPY = 4.0;
static const float SEA_SPEED = 0.8;
static const float SEA_FREQ = 0.16;
static const float3 SEA_BASE = float3(0.1,0.19,0.22);
static const float3 SEA_WATER_COLOR = float3(0.8,0.9,0.6);
static float SEA_TIME;

static const float2x2 octave_m = float2x2(1.6,1.2,-1.2,1.6);

struct Parameters
{
	float2 Resolution;
	float Time;
};


ConstantBuffer<Parameters> Params : register(b0);

float hash( float2 p ) {
    float h = dot(p,float2(127.1,311.7));  
    return frac(sin(h)*43758.5453123);
}

float noise( float2 p ) {
    float2 i = floor( p );
    float2 f = frac( p );	
	float2 u = f*f*(3.0-2.0*f);
    return -1.0+2.0*lerp( lerp( hash( i + float2(0.0,0.0) ), 
                     hash( i + float2(1.0,0.0) ), u.x),
                lerp( hash( i + float2(0.0,1.0) ), 
                     hash( i + float2(1.0,1.0) ), u.x), u.y);
}

float diffuse(float3 n,float3 l,float p) {
    return pow(dot(n,l) * 0.4 + 0.6,p);
}
float specular(float3 n,float3 l,float3 e,float s) {    
    float nrm = (s + 8.0) / (PI * 8.0);
    return pow(max(dot(reflect(e,n),l),0.0),s) * nrm;
}


float3 getSkyColor(float3 e) {
    e.y = max(e.y,0.0);
    float3 ret;
    ret.x = pow(1.0-e.y,2.0);
    ret.y = 1.0-e.y;
    ret.z = 0.6+(1.0-e.y)*0.4;
    return ret;
}

float3 getSeaColor(float3 p, float3 n, float3 l, float3 eye, float3 dist) {
    float fresnel = clamp(1.0 - dot(n,-eye), 0.0, 1.0);
    fresnel = pow(fresnel,3.0) * 0.65;
       
    float3 reflected = getSkyColor(reflect(eye,n));  
    float3 refracted = SEA_BASE + diffuse(n,l,80.0) * SEA_WATER_COLOR * 0.12;
   
    float3 color = lerp(refracted,reflected,fresnel);
   
    float atten = max(1.0 - dot(dist,dist) * 0.001, 0.0);
    color += SEA_WATER_COLOR * (p.y - SEA_HEIGHT) * 0.18 * atten;
   
    color += float3(specular(n,l,eye,60.0),specular(n,l,eye,60.0),specular(n,l,eye,60.0));
   
    return color;
}
 


float3x3 fromEuler(float3 ang) {
    float2 a1 = float2(sin(ang.x),cos(ang.x));
    float2 a2 = float2(sin(ang.y),cos(ang.y));
    float2 a3 = float2(sin(ang.z),cos(ang.z));
    float3x3 m;
    m[0] = float3(a1.y*a3.y+a1.x*a2.x*a3.x,a1.y*a2.x*a3.x+a3.y*a1.x,-a2.y*a3.x);
    m[1] = float3(-a2.y*a1.x,a1.y*a2.y,a2.x);
    m[2] = float3(a3.y*a1.x*a2.x+a1.y*a3.x,a1.x*a3.x-a1.y*a3.y*a2.x,a2.y*a3.y);
    return m;
}

float sea_octave(float2 uv, float choppy) {
	uv += noise(uv);      
	float2 wv = 1.0-abs(sin(uv));
	float2 swv = abs(cos(uv));  
	wv = lerp(wv,swv,wv);
	return pow(1.0-pow(wv.x * wv.y,0.65),choppy);
}


float map(float3 p) {
    float freq = SEA_FREQ;
    float amp = SEA_HEIGHT;
    float choppy = SEA_CHOPPY;
    float2 uv = p.xz; uv.x *= 0.75;
   
    float d, h = 0.0;  
    for(int i = 0; i < ITER_GEOMETRY; i++) {      
        d = sea_octave((uv+SEA_TIME)*freq,choppy);
        d += sea_octave((uv-SEA_TIME)*freq,choppy);
        h += d * amp;      
        uv =mul( octave_m,uv);
        freq *= 1.9;
        amp *= 0.22;
        choppy = lerp(choppy,1.0,0.2);
    }
    return p.y - h;
}
float heightMapTracing(float3 ori, float3 dir, out float3 p)
{
	p = float3(0.f, 0.f, 0.f);
	float tm = 0.f;
	float tx = 1000.f;
	float hx = map(ori + dir * tx);
	if(hx > 0.f)
	{
	//	p = ori + dir * tx;
		return tx;
	}

	float hm = map(ori + dir * tm);
	float tmid = 0.f;
	for(int i = 0; i < NUM_STEPS; i++)
	{
		tmid = lerp(tm, tx, hm / (hm - hx));
		p = ori + dir * tmid;
		float hmid = map(p);
		if(hmid < 0.f)
		{
			tx = tmid;
			hx = hmid;
		}
		else
		{
			tm = tmid;
			hm = hmid;
		}
	}
	return tmid;
}

float map_detailed(float3 p) {
    float freq = SEA_FREQ;
    float amp = SEA_HEIGHT;
    float choppy = SEA_CHOPPY;
    float2 uv = p.xz; uv.x *= 0.75;
   
    float d, h = 0.0;  
    for(int i = 0; i < ITER_FRAGMENT; i++) {      
        d = sea_octave((uv+SEA_TIME)*freq,choppy);
        d += sea_octave((uv-SEA_TIME)*freq,choppy);
        h += d * amp;      
        uv =mul( octave_m,uv); freq *= 1.9; amp *= 0.22;
        choppy = lerp(choppy,1.0,0.2);
    }
    return p.y - h;
}

float3 getNormal(float3 p, float eps) {
    float3 n = 0.f;
    n.y = map_detailed(p);    
    n.x = map_detailed(float3(p.x+eps,p.y,p.z)) - n.y;
    n.z = map_detailed(float3(p.x,p.y,p.z+eps)) - n.y;
    n.y = eps;
    return normalize(n);
}

float3 getPixel(float2 coord, float time, float2 NewUV)
{
	float EPSILON_NRM = 0.1f / Params.Resolution.x;
	SEA_TIME = Params.Time * SEA_SPEED;
	//float2 newUv = (InPosition.xy - (Params.Resolution.xy / 2.f)) / Params.Resolution.x;
	float2 uv = coord / Params.Resolution.xy;
	uv = uv * 2.f - 1.f;
	uv.x *= Params.Resolution.x / Params.Resolution.y;
	uv = NewUV;
	//uv = (coord.xy - (Params.Resolution.xy / 2.f)) / Params.Resolution.x;
	
	float3 ang = float3(sin(time * 3.f) * 0.1f, sin(time) * 0.2f + 0.3f, time);
	float3 ori = float3(0.f, 3.5f, time * 5.f);
	float3 dir = normalize(float3(uv.xy, -2.f));
	dir.z += length(uv) * 0.15f;
	dir = mul(normalize(dir), fromEuler(ang));
	
	float3 p;
	heightMapTracing(ori, dir, p);
	float3 dist = p - ori;
	float3 n = getNormal(p, dot(dist, dist) * EPSILON_NRM);
	float3 light = normalize(float3(0.f, 1.f, 0.8f));
	
	return lerp(
				getSkyColor(dir),
				getSeaColor(p, n, light, dir, dist),
				pow(smoothstep(0.f, -0.05f, dir.y), 0.3f));
}


void VSMain(in float4 Position : Attribute0, in float2 UV : Attribute1,
				out float2 uv : TEXCOORD, out float4 InPosition : SV_POSITION)
{
	InPosition = Position;
	uv = UV;
}

void PSMain(in float2 uv : TEXCOORD, in float4 InPosition : SV_POSITION, out float4 Color : SV_TARGET)
{
	float Time = Params.Time * 0.3f;
	Time = 23926.1816 * 0.3f;
	float2 Pos = normalize(InPosition.xy + float2(1, 1));
	Pos *= Params.Resolution;
	float2 newUv = uv;
	newUv.y = 1 - newUv.y;
	float3 col = getPixel(Pos, Time, newUv);
	Color = float4(col, 1);
}
