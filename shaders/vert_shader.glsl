#version 330 core
layout(location = 0) in vec3 aPos;
uniform vec2 uScale;

out vec3 fragPos;

void main(){
	vec3 scaled = vec3(aPos.xy * uScale, aPos.z);
	gl_Position = vec4(scaled, 1.0);
	fragPos = aPos;
}

