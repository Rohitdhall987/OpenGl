#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/callback.h"
#include "headers/VAO.h"
#include "headers/VBO.h"
#include "headers/EBO.h"
#include "headers/shader.h"
#include "headers/texture.h"
#include "headers/camera.h"

float vertices[] = {
    // positions          // normals           // texture coords
    // Back face
    -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

    // Front face
    -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

    // Left face
    -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,

    // Right face
     0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  1.0f, 1.0f,

     // Bottom face
     -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

     // Top face
    -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};



unsigned int indices[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9,10,10,11, 8,
   12,13,14,14,15,12,
   16,17,18,18,19,16,
   20,21,22,22,23,20
};


int main(void)
{
    glfwInit();

    glfwSetErrorCallback(Callback::errCallBack);

    GLFWwindow* window = glfwCreateWindow(800, 800, "LEARN OPENGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewInit();
    glEnable(GL_DEPTH_TEST);

    // ----------------------------------------------------------------------
    // CUBE (VAO) SETUP 
    // ----------------------------------------------------------------------

    VAO vao;
    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indices, sizeof(indices));

    vao.Bind();
    vbo.Bind();
    ebo.Bind();
 
    vao.AddAttrib(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    vao.AddAttrib(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.AddAttrib(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();


    // ----------------------------------------------------------------------
    // LIGHT (VAO) SETUP 
    // ----------------------------------------------------------------------

    glm::vec3 light_pos = glm::vec3(1.2f, 1.2f, 0.2f);
    glm::vec3 light_clr = glm::vec3(1.0f, 1.0f, 1.0f);

    VAO light;
    light.Bind();
    vbo.Bind();  
    ebo.Bind();  
    light.AddAttrib(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    light.AddAttrib(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    light.AddAttrib(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    light.Unbind();
    vbo.Unbind();
    ebo.Unbind();
    // ----------------------------------------------------------------------

    Texture cry_girl_t("resources/textures/crying_girl.png");

    cry_girl_t.Bind();

    Shader phong_shader("resources/shaders/vertex.glsl", "resources/shaders/frag.glsl");

    Shader color_shader("resources/shaders/vertex.glsl", "resources/shaders/color.glsl");

    

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


    float rotation = 0.0f;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    glm::vec3 background(0.125f, 0.33f , 0.315f);
    Material cube_material;
    Light light_settings;

    cube_material.ambient = glm::vec3(background);
    cube_material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    cube_material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    cube_material.shininess = 32.0f;

    light_settings.ambient = glm::vec3(background);
    light_settings.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    light_settings.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;

        rotation += 50.0f * deltaTime;

        if (rotation >= 360) {
            rotation = 0.0f;
        }

        camera.ProcessInput(window, deltaTime);

        
         //glClearColor(0.25f/2, 0.66f/2, 0.63f/2,1.0f);    
        glClearColor(background.x, background.y, background.z, 1.0f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // --- CUBE RENDER ---
        phong_shader.Use();
        phong_shader.SetMat4("model", glm::mat4(1.0f));
        phong_shader.SetMat4("view", camera.GetView());
        phong_shader.SetMat4("projection", camera.GetProjection());

        glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 rotatedLightPos = glm::vec3(rotMat * glm::vec4(light_pos, 1.0f));
        light_settings.position = rotatedLightPos;

        phong_shader.SetMaterial(cube_material);
        phong_shader.SetLight(light_settings);
        phong_shader.SetVec3("viewPos", camera.cameraPos);

        vao.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        // --- LIGHT RENDER ---
        color_shader.Use();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), rotatedLightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        color_shader.SetMat4("model", model);
        color_shader.SetMat4("view", camera.GetView());
        color_shader.SetMat4("projection", camera.GetProjection());
        color_shader.SetVec3("objectColor", light_clr);
        light.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);


        lastFrame = currentFrame;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}