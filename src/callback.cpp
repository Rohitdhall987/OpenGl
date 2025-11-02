#include "headers/callback.h"
#include <GL/glew.h>
#include <imgui.h>
#include <iostream>

#include "headers/camera.h"

bool Callback::middleMousePressed = false;


void Callback::frameSizeCallBack(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Callback::errCallBack(int error_code, const char* description)
{
    std::cout << error_code << " : " << description << std::endl;
}

void Callback::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
     ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureMouse)
        return;

    if (!middleMousePressed)
        return;

    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (cam)
        cam->MouseMovement(xpos, ypos);
}

void Callback::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
     ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return;
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (cam)
        cam->ScrollInput(xoffset, yoffset);
}

void Callback::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
     ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureKeyboard)
        return;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)glfwSetWindowShouldClose(window,true);
}

void Callback::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        if (action == GLFW_PRESS)
        {
            middleMousePressed = true;

            // Get the current cursor position before locking
            double xpos, ypos;
            glfwGetCursorPos(window,  &xpos, &ypos);

            // Reset camera’s mouse tracking
            Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
            if (cam) cam->ResetMouse(xpos, ypos);


            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else if (action == GLFW_RELEASE)
        {
            middleMousePressed = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}