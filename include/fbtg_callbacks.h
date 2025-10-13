#pragma once

#include <iostream>
#include <GLFW/glfw3.h>

void error_callback(int error, const char* description);
void key_callback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
