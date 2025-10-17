#include "input.h" 

short Input::keys_current[]={ GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D};
short Input::keys_previous[]={GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D};

void Input::update(GLFWwindow* window){
	memcpy(Input::keys_previous, Input::keys_current, sizeof(Input::keys_current));
	for(short key : keys_current){
		Input::keys_current[key] = glfwGetKey(window, key) == GLFW_PRESS;
	}
	if (Input::keys_current[GLFW_KEY_W]) std::cout << "Currently pressin' W\n";
	if (Input::keys_current[GLFW_KEY_A]) std::cout << "Currently pressin' A\n";
	if (Input::keys_current[GLFW_KEY_S]) std::cout << "Currently pressin' S\n";
	if (Input::keys_current[GLFW_KEY_D]) std::cout << "Currently pressin' D\n";
}


