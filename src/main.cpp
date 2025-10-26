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

    Shader shader("resources/shaders/vertex.glsl", "resources/shaders/frag.glsl");

    Shader light_shader("resources/shaders/vertex.glsl", "resources/shaders/color.glsl");

    

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

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;

        rotation += 50.0f * deltaTime;

        if (rotation >= 360) {
            rotation = 0.0f;
        }

        camera.ProcessInput(window, deltaTime);

        
         glClearColor(0.25f/2, 0.66f/2, 0.63f/2,1.0f);    
        //glClearColor(0.035f, 0.035f, 0.035f, 1.0f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // --- CUBE RENDER ---
        shader.Use();
        shader.SetMat4("model", glm::mat4(1.0f));
        shader.SetMat4("view", camera.GetView());
        shader.SetMat4("projection", camera.GetProjection());

        vao.Bind();

        glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 rotatedLightPos = glm::vec3(rotMat * glm::vec4(light_pos, 1.0f));

        shader.SetVec3("objectColor", glm::vec3(0.3f, 1.0f, 0.4f));
        shader.SetVec3("lightPos", rotatedLightPos);
        shader.SetVec3("lightColor", light_clr);
        shader.SetVec3("viewPos", camera.cameraPos);

        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        // --- LIGHT RENDER ---
        light_shader.Use();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), rotatedLightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        light_shader.SetMat4("model", model);
        light_shader.SetMat4("view", camera.GetView());
        light_shader.SetMat4("projection", camera.GetProjection());
        light_shader.SetVec3("objectColor", light_clr);
        light.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);


        lastFrame = currentFrame;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}