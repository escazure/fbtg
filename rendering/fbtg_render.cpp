#include "fbtg_render.h"

GLuint render_module::vao , render_module::vbo, render_module::vert_shader, render_module::frag_shader, render_module::shader_program;

void render_module::init(const std::vector<float>& vertices){
	glGenVertexArrays(1, &render_module::vao);
	glBindVertexArray(render_module::vao);
	
	glGenBuffers(1, &render_module::vbo);
	glBindBuffer(GL_ARRAY_BUFFER, render_module::vbo);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	render_module::create_program("../shaders/vert_shader.glsl", "../shaders/frag_shader.glsl");
}

void render_module::render(){
	render_module::use();
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void render_module::clean(){
	glDeleteShader(render_module::vert_shader);
	glDeleteShader(render_module::frag_shader);
}

void render_module::load_from_file(const std::string& path, GLenum type){
	std::ifstream file(path.c_str());	
	std::stringstream ss;
	ss << file.rdbuf();
	std::string temp = ss.str();
	const char* shader = temp.c_str();

	int success;
	char info_log[512];

	if(type == GL_VERTEX_SHADER){
		render_module::vert_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(render_module::vert_shader, 1, &shader, NULL);
		glCompileShader(render_module::vert_shader);

		glGetShaderiv(render_module::vert_shader, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(render_module::vert_shader, 512, NULL, info_log);
			std::cout << "ERROR: vertex shader compilation failed:\n" << info_log << std::endl;
		}
	}
	
	if(type == GL_FRAGMENT_SHADER){
		render_module::frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(render_module::frag_shader, 1, &shader, NULL);
		glCompileShader(render_module::frag_shader);

		glGetShaderiv(render_module::frag_shader, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(render_module::frag_shader, 512, NULL, info_log);
			std::cout << "ERROR: fragment shader compilation failed:\n" << info_log<< std::endl;
		}
	}
}

void render_module::create_program(const std::string& vert_path, const std::string& frag_path){
	load_from_file(vert_path, GL_VERTEX_SHADER);
	load_from_file(frag_path, GL_FRAGMENT_SHADER);
	render_module::shader_program = glCreateProgram();

	glAttachShader(render_module::shader_program, render_module::vert_shader);
	glAttachShader(render_module::shader_program, render_module::frag_shader);
	glLinkProgram(render_module::shader_program);

	int success;
	char info_log[512];
	glGetProgramiv(render_module::shader_program, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(render_module::shader_program, 512, NULL, info_log);
		std::cout << "ERROR: failed to link shaders:\n" << info_log << std::endl;
	}

	render_module::clean();
}

void render_module::use(){
	glUseProgram(render_module::shader_program);
	glBindVertexArray(render_module::vao);
}
