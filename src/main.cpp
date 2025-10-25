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
    // positions           // texture coords
    // Back face
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,

    // Front face
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,

    // Left face
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,

    // Right face
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,

     // Bottom face
     -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
     -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,

     // Top face
     -0.5f,  0.5f, -0.5f,   0.0f, 0.0f,
      0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,   0.0f, 1.0f
};


unsigned int indices[] = {
    0, 1, 2, 2, 3, 0,       // back
    4, 5, 6, 6, 7, 4,       // front
    8, 9,10,10,11, 8,       // left
   12,13,14,14,15,12,       // right
   16,17,18,18,19,16,       // bottom
   20,21,22,22,23,20        // top
};


int main(void)
{
    glfwInit();
   
    glfwSetErrorCallback(Callback::errCallBack);

    GLFWwindow* window = glfwCreateWindow(800, 800, "LEARN OPENGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewInit();

    VAO vao;
    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indices, sizeof(indices));

    vao.Bind();
    vbo.Bind();
    ebo.Bind();

    vao.AddAttrib(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);  
    vao.AddAttrib(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    Shader shader("src/shaders/vertex.glsl", "src/shaders/frag.glsl");
    shader.Use();

    Texture cry_girl_t("resources/textures/crying_girl.png");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    unsigned int MVP_loc = shader.GetUniform("MVP");

    

    float deltaTime = 0.0f;	
    float lastFrame = 0.0f;

    Camera camera;

    glfwSetWindowUserPointer(window, &camera);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, Callback::frameSizeCallBack);
    glfwSetCursorPosCallback(window, Callback::MouseCallback);
    glfwSetScrollCallback(window, Callback::ScrollCallback);
    glfwSetKeyCallback(window,Callback::KeyCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        

        camera.ProcessInput(window,deltaTime);

        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 MVP = camera.GetProjection() * camera.GetView() * model;

        glClearColor(0.25f, 0.66f, 0.63f,1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, glm::value_ptr(MVP));
        
        cry_girl_t.Bind();
        vao.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(float), GL_UNSIGNED_INT, 0);
        
        lastFrame = currentFrame;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


