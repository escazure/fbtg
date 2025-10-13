#pragma once

#include <iostream>
#include <dlfcn.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include "escgen.hpp"
#define _GLFW_WAYLAND

void parse_config();
GLFWwindow* init_subsystems();
void run(GLFWwindow* _window);
void shutdown(void** _handle, GLFWwindow* _window);
