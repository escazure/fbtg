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

void main(){
	vec3 I = normalize(worldPos - uCameraPos);
	vec3 N = vec3(0.0, 1.0, 0.0);
	vec3 R = reflect(I, N);
	vec3 skyColor = getSkyColor(R);

	vec2 ndc = (clipSpacePos.xy / clipSpacePos.w) * 0.5 + 0.5;
	vec2 reflectUV = vec2(ndc.x, ndc.y);
	vec4 terrainReflection = texture(uReflectionMap, reflectUV);

	vec3 finalColor = mix(skyColor, terrainReflection.rgb, terrainReflection.a);
	FragColor = vec4(finalColor, 1.0);
}
