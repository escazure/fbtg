#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <cmath>
#define _GLFW_WAYLAND

void parse_config();
void init_subsystems();
void run();
void shutdown();
