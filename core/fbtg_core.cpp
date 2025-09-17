#include "fbtg_core.h"

float FSF = 1000.0; // Frame Scale Factor

void error_callback(int error, const char* description){
    std::cout << "Error " << error << ": " << description << std::endl;  
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

int main(){  
    GLFWwindow* window = setupGLFW();
    
    int w, h;
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    framebuffer_size_callback(window, w, h);
    
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(0.0,0.5,1.0);
        glLineWidth(3);
        float x = 0.0;
        float y = 0.0;
        glBegin(GL_LINES);
            for(int i = 0; i < 100; i++){
                glVertex2f(x,y);
                x=i*10;
                y=std::sin(x*10)*10;
                glVertex2f(x,y);
            }
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //parse_config();
    //init_subsystems();
    //run();
    //shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();
}
