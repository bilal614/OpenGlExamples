#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "asset.hpp"

constexpr unsigned int NumOfVAOs = 1;

using namespace std;

GLFWwindow* window;

GLuint renderingProgram;
GLuint vao[NumOfVAOs];

std::string readShaderSource(const std::filesystem::path& path)
{
    std::stringstream content;
    std::ifstream fileStream(path, std::ios::in);

    std::string line;
    while(!fileStream.eof())
    {
        std::getline(fileStream, line);
        content << line << std::endl;
    }
    fileStream.close();
    return content.str();
}

void printShaderLog(GLuint shader)
{
    int len = 0;
    int chWritten = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if(len > 0)
    {
        std::vector<char> log;
        log.reserve(len);
        glGetShaderInfoLog(shader, len, &chWritten, log.data());
        std::cout << "Shader Info Log: " << log.data() << std::endl;
    }
}

void printProgramLog(GLuint shader)
{
    int len = 0;
    int chWritten = 0;
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &len);
    if(len > 0)
    {
        std::vector<char> log;
        log.reserve(len);
        glGetProgramInfoLog(shader, len, &chWritten, log.data());
        std::cout << "Shader Info Log: " << log.data() << std::endl;
    }
}

bool checkOpenGLError()
{
    bool foundError = false;
    int glErr = glGetError();
    while(glErr != GL_NO_ERROR){
        std::cout << "glError: " << glErr << std::endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

GLuint createShaderProgram()
{
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;

    std::filesystem::path vertexShaderPath{Asset::ShaderDir}, fragmentShaderPath{Asset::ShaderDir};
    vertexShaderPath /= "firstShader.vert";
    fragmentShaderPath /= "firstShader.frag";

    auto vshaderSourceStr = readShaderSource(vertexShaderPath);
    auto fshaderSourceStr = readShaderSource(fragmentShaderPath);
    const char* vshaderSource = vshaderSourceStr.c_str();
    const char* fshaderSource = fshaderSourceStr.c_str();

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vshaderSource, NULL);
    glCompileShader(vShader);
    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if(vertCompiled != 1)
    {
        std::cout << "vertex compilation failed" << std::endl;
        printShaderLog(vShader);
    }

    glShaderSource(fShader, 1, &fshaderSource, NULL);
    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if(fragCompiled != 1)
    {
        std::cout << "fragment compilation failed" << std::endl;
        printShaderLog(fShader);
    }

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    checkOpenGLError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if(linked != 1)
    {
        std::cout << "linking failed" << std::endl;
        printProgramLog(vfProgram);
    }
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