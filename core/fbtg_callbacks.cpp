#include "fbtg_callbacks.h"

float FSF = 30.0;


void error_callback(int error, const char* description){
    std::cout << "Error " << error << ": " << description << std::endl;  
}

void key_callback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods){
	if(_action == GLFW_RELEASE && _key == GLFW_KEY_ESCAPE){
		std::cout << "Pressed ESC\n";
	}
	if(_action == GLFW_RELEASE && _key == GLFW_KEY_P){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if(_action == GLFW_RELEASE && _key == GLFW_KEY_R){
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)width / (float)height;
    if(aspect >= 1.0){
        glOrtho(-aspect*FSF, aspect*FSF, -FSF, FSF, -FSF*10, FSF*10);
    }
    else{
        glOrtho(-FSF, FSF, -FSF/aspect, FSF/aspect, -FSF*10, FSF*10);
    }

    glMatrixMode(GL_MODELVIEW);
}
