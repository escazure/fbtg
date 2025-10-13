#include "fbtg_callbacks.h"

float FSF = 30.0;


void error_callback(int error, const char* description){
    std::cout << "Error " << error << ": " << description << std::endl;  
}

void key_callback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods){
	if(_action == GLFW_PRESS){
		if(_key == GLFW_KEY_W){
			std::cout << "Pressed W\n";
		}
		if(_key == GLFW_KEY_A){
			std::cout << "Pressed A\n";
		}
		if(_key == GLFW_KEY_S){
			std::cout << "Pressed S\n";
		}
		if(_key == GLFW_KEY_D){
			std::cout << "Pressed D\n";
		}
	}

	if(_action == GLFW_RELEASE){
		if(_key == GLFW_KEY_W){
			std::cout << "Pressed W\n";
		}
		if(_key == GLFW_KEY_A){
			std::cout << "Pressed A\n";
		}
		if(_key == GLFW_KEY_S){
			std::cout << "Pressed S\n";
		}
		if(_key == GLFW_KEY_D){
			std::cout << "Pressed D\n";
		}

	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)width / (float)height;
    if(aspect >= 1.0){
        glOrtho(-aspect*FSF, aspect*FSF, -FSF, FSF, -FSF, FSF);
    }
    else{
        glOrtho(-FSF, FSF, -FSF/aspect, FSF/aspect, -FSF, FSF);
    }

    glMatrixMode(GL_MODELVIEW);
}
