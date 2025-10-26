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
#include "headers/camera.h"

float vertices[] = {
    // positions        // texture coords
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

    // Left face
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

    // Right face
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

     // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    // Top face
   -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f
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

 
    vao.AddAttrib(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);



    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();


    // ----------------------------------------------------------------------
    // LIGHT (VAO) SETUP 
    // ----------------------------------------------------------------------

    VAO light;
    light.Bind();
    vbo.Bind();  
    ebo.Bind();  
    light.AddAttrib(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

    light.Unbind();
    vbo.Unbind();
    ebo.Unbind();
    // ----------------------------------------------------------------------


    Shader shader("resources/shaders/vertex.glsl", "resources/shaders/frag.glsl");

    Shader light_shader("resources/shaders/vertex.glsl", "resources/shaders/light_frag.glsl");

    

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    unsigned int model_loc = shader.GetUniform("model");
    unsigned int view_loc = shader.GetUniform("view");
    unsigned int proj_loc = shader.GetUniform("projection");
    unsigned int object_color = shader.GetUniform("object_color");


    unsigned int light_model_loc = light_shader.GetUniform("model");
    unsigned int light_view_loc = light_shader.GetUniform("view");
    unsigned int light_proj_loc = light_shader.GetUniform("projection");
    unsigned int light_object_color = light_shader.GetUniform("object_color");

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    Camera camera;

    glfwSetWindowUserPointer(window, &camera);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, Callback::frameSizeCallBack);
    glfwSetCursorPosCallback(window, Callback::MouseCallback);
    glfwSetScrollCallback(window, Callback::ScrollCallback);
    glfwSetKeyCallback(window, Callback::KeyCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;


        camera.ProcessInput(window, deltaTime);

        glm::mat4 model = glm::mat4(1.0f);
        // glClearColor(0.25f, 0.66f, 0.63f,1.0f);     //nice color to use later
        glClearColor(0.035f, 0.035f, 0.035f, 1.0f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- CUBE RENDER ---
        shader.Use();
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(camera.GetView()));
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(camera.GetProjection()));

        glUniform3fv(object_color, 1, glm::value_ptr(glm::vec3(0.3f, 1.0f, 0.4f)));

        vao.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        // --- LIGHT RENDER ---
        light_shader.Use();
        model = glm::translate(model, glm::vec3(1.2f, 1.2f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(light_model_loc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(light_view_loc, 1, GL_FALSE, glm::value_ptr(camera.GetView()));
        glUniformMatrix4fv(light_proj_loc, 1, GL_FALSE, glm::value_ptr(camera.GetProjection()));
        glUniform3fv(light_object_color, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        light.Bind(); 
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);


        lastFrame = currentFrame;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}