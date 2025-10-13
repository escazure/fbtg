#include "fbtg_core.h"
#include "fbtg_callbacks.h"
#include "fbtg_render.h"
#include "escgen.hpp"

std::vector<escgen::Vertex> t_buffer;
std::size_t width = 8, length = 8;

GLFWwindow* init_subsystems(){
    glfwSetErrorCallback(error_callback);

    if(!glfwInit()){
        std::cout << "Failed to init glfw\n";
    }

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(mode -> width, mode -> height,"Test",NULL,NULL);
    if(!window){
        std::cout << "Failed to create window\n";
    }

    glfwMakeContextCurrent(window);

    int w, h;
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
    framebuffer_size_callback(window, w, h);
	return window;	
}

void run(GLFWwindow* _window){
	while(!glfwWindowShouldClose(_window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render_buffer(t_buffer, width, length);
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}	
}

void shutdown(void** _handle, GLFWwindow* _window){
	dlclose(_handle);
    glfwDestroyWindow(_window);
    glfwTerminate();
}

int main(){  
    GLFWwindow* window = init_subsystems();

	std::string test_fun, test_name;
	std::cout << "Provide function: \n";
	std::getline(std::cin, test_fun);

	std::cout << "Provide function name: \n";
	std::getline(std::cin, test_name);

	if(!escgen::write_fun(test_fun, test_name)){
		error_callback(-1, ("Failed to open/create file: " + test_name).c_str());
		return 1;
	}
    
	if(!escgen::compile_fun(test_name)){
		error_callback(-2, ("Failed to compile file: " + test_name).c_str());
		return 1;
	}

	float (*fun)(float,float);
	void* handle;

	if(!escgen::get_fun_pointer(test_name, &fun, &handle)){
		error_callback(-5, "Failed to get function pointer");
		return 1;
	}

	t_buffer = temp_gen(width, length, &fun);
    //parse_config();
    run(window);
    shutdown(&handle, window);
}
