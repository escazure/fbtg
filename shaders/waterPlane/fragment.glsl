#version 460 core
layout (binding = 0) uniform sampler2D uReflectionMap;
layout (location = 0) out vec4 FragColor;

in vec2 uv;
in vec4 clipSpacePos;
in vec3 worldPos;

uniform vec3 uCameraPos;
uniform vec3 uLightDir;

const vec3 belowHorizon = vec3(150.0, 170.0, 225.0) / 255.0;
const vec3 lightSkyBlue = vec3(210.0, 235.0, 255.0) / 255.0;
const vec3 deepSkyBlue = vec3(25.0, 105.0, 220.0) / 255.0;
const vec3 sunColor = vec3(255.0, 250.0, 240.0) / 255.0;

const vec3 shallowWater = vec3(10.0, 77.0, 104.0) / 255.0;
const vec3 waterFoam = vec3(232.0, 249.0, 253.0) / 255.0;

vec2 hash22(vec2 p){
	uvec2 v = floatBitsToUint(p);
	v.x += 1664525u;
	v ^= v.yx * 1664525u;
	v *= 1664525u;
	v ^= v >> 16u;
	v ^ v.yx >> 5u;

	uvec2 floatBits = (v & 0x007FFFFFu) | 0x3F800000u;
	vec2 res = uintBitsToFloat(floatBits) - 1.0;
	return res;
}

vec3 perlinNoiseDerivatives(vec2 uv){
	vec2 i = floor(uv);
	vec2 f = fract(uv);

	vec2 gradA = normalize(hash22(i) * 2.0 - 1.0);
	vec2 gradB = normalize(hash22(i + vec2(1.0, 0.0)) * 2.0 - 1.0);
	vec2 gradC = normalize(hash22(i + vec2(0.0, 1.0)) * 2.0 - 1.0);
	vec2 gradD = normalize(hash22(i + vec2(1.0, 1.0)) * 2.0 - 1.0);

	float dotA = dot(gradA, f);
	float dotB = dot(gradB, f - vec2(1.0, 0.0));
	float dotC = dot(gradC, f - vec2(0.0, 1.0));
	float dotD = dot(gradD, f - vec2(1.0, 1.0));

	vec2 w = f * f * f * (f * (f * 6.0 - 15.0) + 10.0);
	vec2 dw = f * f * (f * (f * 30.0 - 60.0) + 30.0);

	float u1 = mix(dotA, dotB, w.x);
	float u2 = mix(dotC, dotD, w.x);
	float val = mix(u1, u2, w.y);

	vec2 g1 = mix(gradA, gradB, w.x);
	vec2 g2 = mix(gradC, gradD, w.x);
	vec2 gradInterpolation = mix(g1, g2, w.y);

	float dx = mix(dotB - dotA, dotD - dotC, w.y) * dw.x;
	float dy = mix(dotC - dotA, dotD - dotB, w.x) * dw.y;
	vec2 d = gradInterpolation + vec2(dx, dy);
	
	return vec3(val, d);
}

float voronoiNoise(vec2 uv, float cellSize){
	vec2 scaledUV = uv / cellSize;
	vec2 baseCell = floor(scaledUV);
	vec2 fracUV = fract(scaledUV);

    float minDist = 10.0;

	for(int x = -1; x <= 1; x++){
		for(int y = -1; y <= 1; y++){
			vec2 neighbor = vec2(float(x), float(y));
			vec2 point = hash22(baseCell + neighbor);
			vec2 diff = neighbor + point - fracUV;
			float dist = length(diff);

			minDist = min(minDist, dist);
		}
	}

	return minDist;
} 

float waterNoise(vec2 uv, int octaves, float baseDistortion, float baseFrequency, float baseExponent){
	float res = 0.0;
	const vec2 offset = vec2(-3.5, 3.5);

	float lacunarity = 2.15;
	float gain = 0.51;
	float exp_gain = 1.1;

	float d = baseDistortion;
	float f = baseFrequency;
	float e = baseExponent;
	for(int i = 0; i < octaves; i++){
		vec2 suv = uv * f;
	    float px = perlinNoiseDerivatives(suv).x * d;
		float py = perlinNoiseDerivatives(suv + offset).x * d;
	    float v = voronoiNoise(suv + vec2(px, py), 1.0);
	    res += pow(v, e);
	    f *= lacunarity;
	    d *= gain;
	    e *= exp_gain;
	}

	return res; 
}

float getSunMask(float angularDist, float outerEdgeDeg, float innerEdgeDeg){
	float outerEdge = cos(radians(outerEdgeDeg));
	float innerEdge = cos(radians(innerEdgeDeg));
	return smoothstep(outerEdge, innerEdge, angularDist);
}

vec3 getSkyColor(vec3 viewDir){
	vec3 fragToLight = normalize(-uLightDir);

	float angularDist = dot(viewDir, fragToLight);

	float heightFactor = clamp(viewDir.y, 0.0, 1.0);

	vec3 skyColor = mix(lightSkyBlue, deepSkyBlue, heightFactor);
	float horizonFade = smoothstep(-0.05, 0.05, viewDir.y);
	skyColor = mix(belowHorizon, skyColor, horizonFade);

	float sunGlow = pow(max(angularDist, 0.0), 16.0);
	vec3 skyWithGlow = skyColor + (skyColor * sunGlow * 0.25);

	float sunMask = getSunMask(angularDist, 1.0, 0.6);

	return mix(skyWithGlow, sunColor, sunMask);
}

vec3 getWaterColor(){
	float waterPattern = waterNoise(uv, 3, 2.0, 30.0, 3.5); 
	return mix(shallowWater, waterFoam, waterPattern);
}

void main(){
	vec3 I = normalize(worldPos - uCameraPos);

	vec2 noiseUV = worldPos.xz * 0.15;
	vec2 waveSlope = perlinNoiseDerivatives(noiseUV).yz;

	vec2 uvDeriv = fwidth(noiseUV);
    float pixelFootprint = max(uvDeriv.x, uvDeriv.y);
    float noiseFilter = 1.0 - smoothstep(0.6, 0.8, pixelFootprint);
    waveSlope *= noiseFilter;

	vec3 N = vec3(0.0, 1.0, 0.0);
	vec3 R = reflect(I, N);
	vec3 skyColor = getSkyColor(R);

	vec2 ndc = (clipSpacePos.xy / clipSpacePos.w) * 0.5 + 0.5;
	vec2 reflectUV = ndc;
	vec2 offsetStrength = vec2(0.002);

	vec2 warpedUV = clamp(reflectUV + (waveSlope * offsetStrength), 0.001, 0.999);
	vec4 terrainReflection = texture(uReflectionMap, warpedUV);

	vec3 waterColor = getWaterColor();

	vec3 finalColor = mix(skyColor, terrainReflection.rgb, terrainReflection.a);
	finalColor = mix(finalColor, waterColor, 0.1);
	FragColor = vec4(finalColor, 1.0);
}
