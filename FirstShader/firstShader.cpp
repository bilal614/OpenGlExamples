#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

constexpr unsigned int NumOfVAOs = 1;

using namespace std;

GLFWwindow* window;

GLuint renderingProgram;
GLuint vao[NumOfVAOs];

GLuint createShaderProgram()
{
    const char* vshaderSource = 
        "#version  330\n"
        "void main(void)\n"
        "{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0);}";

    const char* fshaderSource = 
        "#version  330\n"
        "out vec4 color;\n"
        "void main(void)\n"
        "{ if(gl_FragCoord.x < 300) color = vec4(1.0, 0.0, 0.0, 1.0);"
        "  else color = vec4(0.0, 0.0, 1.0, 1.0);}";

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vshaderSource, NULL);
    glShaderSource(fShader, 1, &fshaderSource, NULL);
    glCompileShader(vShader);
    glCompileShader(fShader);

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);

    return vfProgram;
}

void errorCallback(int, const char* err_str)
{
    std::cout << "GLFW Error: " << err_str << std::endl;
}

void init(GLFWwindow* window)
{
    renderingProgram = createShaderProgram();
    glGenVertexArrays(NumOfVAOs, vao);
    glBindVertexArray(*vao);
}

void display(GLFWwindow* window, double currentTime)
{
    glUseProgram(renderingProgram);
    glPointSize(30.0f);
    glDrawArrays(GL_POINTS, 0, 1);
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
    window = glfwCreateWindow(600, 600, "First OpenGL Shaders", NULL, NULL);
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