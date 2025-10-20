#include "fbtg_core.h"
#include "fbtg_callbacks.h"
#include "fbtg_render.h"
#include "plugin_system.hpp"
#include "input.h"

std::vector<plugin_system::Vertex> t_buffer;
std::size_t width = 32, length = 32;

std::vector<float> vertices = {
	-0.5f, 0.0f, 0,
	0.0f, 1.0f, 0,
	0.5f, 0.0f, 0
};

GLFWwindow* init_subsystems(){
    glfwSetErrorCallback(error_callback);

    if(!glfwInit()){
        std::cout << "Failed to init glfw\n";
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(mode -> width, mode -> height,"Test",NULL,NULL);
    if(!window){
        std::cout << "Failed to create window\n";
    }

    glfwMakeContextCurrent(window);

	if(gl3wInit() != 0) {
		std::cout << "Failed to init gl3w\n";
		return 0;
	}

    int w, h;
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
    framebuffer_size_callback(window, w, h);

	render_module::init(vertices);

	return window;	
}

void run(GLFWwindow* _window){
	while(!glfwWindowShouldClose(_window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render_module::render();
		glfwSwapBuffers(_window);
		glfwPollEvents();
		Input::update(_window);
		glClearColor(0.3,0.5,0.7,0.7);
		glClear(GL_COLOR_BUFFER_BIT);
	}	
}

void shutdown(void** _handle, GLFWwindow* _window){
	dlclose(_handle);
    glfwDestroyWindow(_window);
    glfwTerminate();
}

int main(){  
    GLFWwindow* window = init_subsystems();
	if(!window){
		std::cout << "Something during initialization gone wrong, see logs\n";
		return -1;
	}

	std::string test_fun, test_name;
	std::cout << "Provide function: \n";
	std::getline(std::cin, test_fun);

	std::cout << "Provide function name: \n";
	std::getline(std::cin, test_name);

	if(!plugin_system::write_fun(test_fun, test_name)){
		error_callback(-1, ("Failed to open/create file: " + test_name).c_str());
		return 1;
	}
    
	if(!plugin_system::compile_fun(test_name)){
		error_callback(-2, ("Failed to compile file: " + test_name).c_str());
		return 1;
	}

	float (*fun)(float,float);
	void* handle;

	if(!plugin_system::get_fun_pointer(test_name, &fun, &handle)){
		error_callback(-5, "Failed to get function pointer");
		return 1;
	}

    //parse_config();
    run(window);
    shutdown(&handle, window);
}
