#pragma once

#include "plugin_system.hpp"
#include <vector>
#include <GLFW/glfw3.h>

std::vector<plugin_system::Vertex> temp_gen(std::size_t _width, std::size_t _length, float(** _fun)(float,float));
void render_triangle(plugin_system::Vertex& _a, plugin_system::Vertex& _b, plugin_system::Vertex& _c);
void render_buffer(std::vector<plugin_system::Vertex>& _tbuffer, std::size_t width, std::size_t length);
