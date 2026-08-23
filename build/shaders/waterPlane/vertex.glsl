#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 uv;
out vec4 clipSpacePos;
out vec3 worldPos;

void main(){
	uv = aTexCoords;
	worldPos = vec3(uModel * vec4(aPos, 1.0));
	clipSpacePos = uProjection * uView * uModel * vec4(aPos, 1.0);
	gl_Position = clipSpacePos;
}
