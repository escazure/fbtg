#pragma once

#include "escgen.hpp"
#include <vector>
#include <GLFW/glfw3.h>

std::vector<escgen::Vertex> temp_gen(std::size_t _width, std::size_t _length, float(** _fun)(float,float));
void render_triangle(escgen::Vertex& _a, escgen::Vertex& _b, escgen::Vertex& _c);
void render_buffer(std::vector<escgen::Vertex>& _tbuffer, std::size_t width, std::size_t length);
