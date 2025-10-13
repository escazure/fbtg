#include "fbtg_core.h"

float FSF = 30.0; // Frame Scale Factor
std::size_t width = 8, length = 8;

std::vector<escgen::Vertex> temp_gen(std::size_t _width, std::size_t _length, float(**_fun)(float,float)){
	std::vector<escgen::Vertex> t;
	t.reserve(_width * _length);
	for(std::size_t i = 1; i < _width+1; i++){
		for(std::size_t j = 1; j < _length+1; j++){
			float height = (*_fun)(j,i);
			t.emplace_back(escgen::Vertex(i,j,height,0,0,0));	
		}
	}
	float max = 0.0, min = 0.0;
	for(std::size_t i = 0; i < _width * _length; i++){
		if(max < t[i].z) max = t[i].z;
		if(min > t[i].z) min = t[i].z;
	}	
	for(std::size_t i = 0; i < _width * _length; i++){
		t[i].r = (t[i].z - min) / (max - min);
		t[i].g = t[i].r;
		t[i].b = t[i].r;
	}
	return t;
}

std::vector<escgen::Vertex> t_buffer;

void error_callback(int error, const char* description){
    std::cout << "Error " << error << ": " << description << std::endl;  
}

void render_triangle(escgen::Vertex& _a, escgen::Vertex& _b, escgen::Vertex& _c){
	glBegin(GL_TRIANGLES);
	glColor3f(_a.r, _a.g, _a.b);
	glVertex3f(_a.x, _a.y, _a.z);
	
	glColor3f(_b.r, _b.g, _b.b);
	glVertex3f(_b.x, _b.y, _b.z);

	glColor3f(_c.r, _c.g, _c.b);
	glVertex3f(_c.x, _c.y, _c.z);
	glEnd();
}

void render_buffer(std::vector<escgen::Vertex>& _tbuffer){
	if(_tbuffer.size() < 4){
		error_callback(-4, "Vertex buffer size if less then four");
		return;
	}
	for(std::size_t x = 0; x < width - 1; x++){
		for(std::size_t y = 0; y < length-1; y++){
			int i00 = x*length+y;
			int i10 = (x+1)*length+y;
			int i01 = x*length+(y+1);
			int i11 = (x+1)*length+(y+1);

			render_triangle(_tbuffer[i00], _tbuffer[i10], _tbuffer[i01]);
			render_triangle(_tbuffer[i10], _tbuffer[i11], _tbuffer[i01]);
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

GLFWwindow* setupGLFW(){
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

    return window;
}

void run(GLFWwindow* _window){
	while(!glfwWindowShouldClose(_window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render_buffer(t_buffer);
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
    GLFWwindow* window = setupGLFW();
    
    int w, h;
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    framebuffer_size_callback(window, w, h);

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
    //init_subsystems();
    run(window);
    shutdown(&handle, window);
}
