
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

    MyImgui gui(window);
    
    Shader phong_shader("resources/shaders/vertex.glsl", "resources/shaders/frag.glsl");
    Shader outline_shader("resources/shaders/outline_vertex.glsl", "resources/shaders/color.glsl");
   
    Material cube_material;
    DirLight dir_light;

    float deltaTime = 0.0f, lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        glEnable(GL_DEPTH_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.ProcessInput(window, deltaTime);

        glClearColor(gui.bg_col[0], gui.bg_col[1], gui.bg_col[2], gui.bg_col[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        cube_material.shininess = gui.shine;
        cube_material.emissionStrength = 0.5f;

        dir_light.ambient = glm::vec3(gui.amb_col[0], gui.amb_col[1], gui.amb_col[2]);
        dir_light.diffuse = glm::vec3(gui.light_col[0], gui.light_col[1], gui.light_col[2]);
        dir_light.specular = glm::vec3(gui.light_spe, gui.light_spe, gui.light_spe);
        dir_light.direction = glm::vec3(gui.light_dir[0], gui.light_dir[1], gui.light_dir[2]);


        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);

        phong_shader.Use();
        phong_shader.SetMaterial(cube_material);
        phong_shader.SetDirectionLight(dir_light);
        phong_shader.SetMat4("view", camera.GetView());
        phong_shader.SetMat4("projection", camera.GetProjection(mode->width, mode->height));
        phong_shader.SetVec3("viewPos", camera.cameraPos);
        outline_shader.Use();
        outline_shader.SetMat4("view", camera.GetView());
        outline_shader.SetMat4("projection", camera.GetProjection(mode->width, mode->height));

        gui.Render_Models(phong_shader);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        gui.Render_Outlines(outline_shader);

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
