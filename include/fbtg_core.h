#pragma once

#include "../external/gl3w.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <dlfcn.h>
#include <cmath>
#include <vector>

void parse_config();
GLFWwindow* init_subsystems();
void run(GLFWwindow* _window);
void shutdown(void** _handle, GLFWwindow* _window);
