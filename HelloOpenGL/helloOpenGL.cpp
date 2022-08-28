#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


using namespace std;

GLFWwindow* window;

void errorCallback(int, const char* err_str)
{
    std::cout << "GLFW Error: " << err_str << std::endl;
}

void init(GLFWwindow* window)
{

}

void display(GLFWwindow* window, double currentTime)
{
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main()
{
    glfwSetErrorCallback(errorCallback);
    if(!glfwInit())
    {
        std::cout << "GLFW init failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(600, 600, "Hello OpenGL", NULL, NULL);
    if( window == NULL )
    {
        std::cout << "GLFW window creation failed" << std::endl;
    }
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
    {
        std::cout << "GLEW init failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);
    
    init(window);

    while(!glfwWindowShouldClose(window))
    {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);

}