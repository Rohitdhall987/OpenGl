#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "headers/callback.h"
#include "headers/VAO.h"
#include "headers/VBO.h"
#include "headers/EBO.h"
#include "headers/shader.h"
#include "headers/texture.h"

float vertices[] = {
     0.7f,  0.7f, 0.0f,         1.0f, 1.0f, // top right
     0.7f, -0.7f, 0.0f,         1.0f, 0.0f, // bottom right
    -0.7f, -0.7f, 0.0f,         0.0f, 0.0f, // bottom left
    -0.7f,  0.7f, 0.0f,         0.0f, 1.0f, // top left 

};
unsigned int indices[] = { 
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

int main(void)
{
    glfwInit();
   
    glfwSetErrorCallback(Callback::errCallBack);

    GLFWwindow* window = glfwCreateWindow(800, 800, "LEARN OPENGL", NULL, NULL);
    glfwMakeContextCurrent(window);

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

    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.25f, 0.66f, 0.63f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        cry_girl_t.Bind();
        vao.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(float), GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


