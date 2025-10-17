#pragma once
#include <iostream>
#include <cstring>
#include <GLFW/glfw3.h>

namespace Input{
	extern short keys_current[];
	extern short keys_previous[];

	void update(GLFWwindow* window);
}
