#include "headers/callback.h"
#include <GL/glew.h>
#include <iostream>

void Callback::frameSizeCallBack(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Callback::errCallBack(int error_code, const char* description)
{
    std::cout << error_code << " : " << description << std::endl;
}