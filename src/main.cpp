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

    glfwSwapInterval(1);

    glewInit();

    glfwSetFramebufferSizeCallback(window, Callback::frameSizeCallBack);

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
    float rotation = 0.0f;

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f,-4.0f));

    while (!glfwWindowShouldClose(window)) {

        if (rotation >= 360.0f) {
            rotation = 0.0f;
        }
        else {
            rotation += 0.75f;
        }

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 1.0f));

        glm::mat4 MVP = proj * view * model;

        glClearColor(0.25f, 0.66f, 0.63f,1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, glm::value_ptr(MVP));
        
        cry_girl_t.Bind();
        vao.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(float), GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


