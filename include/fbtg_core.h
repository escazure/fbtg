#pragma once

#include "../libs/gl3w.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <dlfcn.h>
#include <cmath>
#include <vector>
#define _GLFW_WAYLAND

void parse_config();
GLFWwindow* init_subsystems();
void run(GLFWwindow* _window);
void shutdown(void** _handle, GLFWwindow* _window);
