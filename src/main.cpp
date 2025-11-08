
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/callback.h"
#include "headers/shader.h"
#include "headers/camera.h"

#include "headers/imgui_setup.h"

int main(void)
{
    glfwInit();

    glfwSetErrorCallback(Callback::errCallBack);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "LEARN OPENGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    Camera camera;
    glfwSetWindowUserPointer(window, &camera);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, Callback::frameSizeCallBack);
    glfwSetCursorPosCallback(window, Callback::MouseCallback);
    glfwSetMouseButtonCallback(window, Callback::MouseButtonCallback);
    glfwSetScrollCallback(window, Callback::ScrollCallback);
    glfwSetKeyCallback(window, Callback::KeyCallback);

    MyImgui gui(window, camera);
    

   


    float deltaTime = 0.0f, lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        glEnable(GL_DEPTH_TEST);
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        camera.ProcessInput(window, deltaTime);
        glClearColor(gui.bg_col[0], gui.bg_col[1], gui.bg_col[2], gui.bg_col[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glStencilMask(0x00);  
        gui.Render_Models();

        
        glDisable(GL_DEPTH_TEST);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);  
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); 
        gui.Render_Selected_Model();  
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);  


        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        gui.Render_Outlines();

        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);
        gui.Render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    gui.ShutDown();

    glfwTerminate();
    return 0;
}
