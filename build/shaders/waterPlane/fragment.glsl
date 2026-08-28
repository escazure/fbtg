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
    p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    return fract(sin(p) * 43758.5453123);
}

float perlinNoise(vec2 uv){
	vec2 i = floor(uv);
	vec2 f = fract(uv);

	vec2 gradA = hash22(i) * 2.0 - 1.0;
	vec2 gradB = hash22(i + vec2(1.0, 0.0)) * 2.0 - 1.0;
	vec2 gradC = hash22(i + vec2(0.0, 1.0)) * 2.0 - 1.0;
	vec2 gradD = hash22(i + vec2(1.0, 1.0)) * 2.0 - 1.0;

	float dotA = dot(gradA, f);
	float dotB = dot(gradB, f - vec2(1.0, 0.0));
	float dotC = dot(gradC, f - vec2(0.0, 1.0));
	float dotD = dot(gradD, f - vec2(1.0, 1.0));

	vec2 w = f * f * f * (f * (f * 6.0 - 15.0) + 10.0);

	float u1 = mix(dotA, dotB, w.x);
	float u2 = mix(dotC, dotD, w.x);

	return mix(u1, u2, w.y);
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
	    float px = perlinNoise(suv) * d;
		float py = perlinNoise(suv + offset) * d;
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
	vec3 N = vec3(0.0, 1.0, 0.0);
	vec3 R = reflect(I, N);
	vec3 skyColor = getSkyColor(R);

	vec2 ndc = (clipSpacePos.xy / clipSpacePos.w) * 0.5 + 0.5;
	vec2 reflectUV = vec2(ndc.x, ndc.y);
	vec4 terrainReflection = texture(uReflectionMap, reflectUV);

	vec3 waterColor = getWaterColor();

	vec3 finalColor = mix(skyColor, terrainReflection.rgb, terrainReflection.a);
	finalColor = mix(finalColor, waterColor, 0.1);
	FragColor = vec4(finalColor, 1.0);
}
