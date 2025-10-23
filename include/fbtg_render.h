#pragma once

#include "plugin_system.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "../external/gl3w.h"
#include <GLFW/glfw3.h>

namespace render_module {
	extern GLuint vao;
	extern GLuint vbo;
	extern GLuint vert_shader, frag_shader;
	extern GLuint shader_program;
	extern std::size_t vertex_count, data_width, data_length;
	
	void load_from_file(const std::string& path, GLenum type);	
	void create_program(const std::string& vert_path, const std::string& frag_path);
	void use();

	void init(const std::vector<float>& vertices, std::size_t widht, std::size_t length);
	void render();
	void clean();
}
