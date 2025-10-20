#version 330 core
in vec3 fragPos;
out vec4 FragColor;

void main(){
	vec3 color = abs(fragPos);
	color = clamp(color, 0.0, 1.0);
	FragColor = vec4(color, 1.0);
}

