#include "headers/callback.h"
#include <GL/glew.h>
#include <iostream>

#include "headers/camera.h"

void Callback::frameSizeCallBack(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Callback::errCallBack(int error_code, const char* description)
{
    std::cout << error_code << " : " << description << std::endl;
}

void Callback::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (cam)
        cam->MouseMovement(xpos, ypos);
}

void Callback::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (cam)
        cam->ScrollInput(xoffset, yoffset);
}

void Callback::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)glfwSetWindowShouldClose(window,true);
}
