#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/callback.h"
#include "headers/shader.h"
#include "headers/camera.h"
#include "headers/mesh.h"
#include "headers/model.h"

int main(void)
{
    glfwInit();

    glfwSetErrorCallback(Callback::errCallBack);

    GLFWwindow* window = glfwCreateWindow(800, 800, "LEARN OPENGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
    Camera camera;
    glfwSetWindowUserPointer(window, &camera);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, Callback::frameSizeCallBack);
    glfwSetCursorPosCallback(window, Callback::MouseCallback);
    glfwSetScrollCallback(window, Callback::ScrollCallback);
    glfwSetKeyCallback(window, Callback::KeyCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    
    Shader phong_shader("resources/shaders/vertex.glsl", "resources/shaders/frag.glsl");
    Shader color_shader("resources/shaders/vertex.glsl", "resources/shaders/color.glsl");

   
    glm::vec3 background(0.53f, 0.81f, 0.92f);
    Material cube_material;
    DirLight dir_light;


    cube_material.shininess = 32.0f;
    cube_material.emissionStrength = 0.5f;

    dir_light.ambient = glm::vec3(background.x/2,background.y/2,background.z/2);
    dir_light.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    dir_light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    dir_light.direction = glm::vec3(-0.3f, -0.8f, -0.6f);

    phong_shader.Use();
    phong_shader.SetMaterial(cube_material);
    phong_shader.SetDirectionLight(dir_light);




    Model loaded_model("resources/model/genshin/childe.pmx");


    float deltaTime = 0.0f, lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;



        camera.ProcessInput(window, deltaTime);

        glClearColor(background.x, background.y, background.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        phong_shader.Use();
        phong_shader.SetMat4("view", camera.GetView());
        phong_shader.SetMat4("projection", camera.GetProjection());
        phong_shader.SetVec3("viewPos", camera.cameraPos);


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model,glm::vec3(0.1f));
        phong_shader.SetMat4("model", model);


        loaded_model.Draw(phong_shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
