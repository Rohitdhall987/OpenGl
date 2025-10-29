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

glm::vec3 cubePositions[10] = {
    { -2.0f,  0.0f, -1.0f },
    {  2.0f,  1.0f,  0.0f },
    {  0.0f, -1.0f,  2.0f },
    { -1.5f,  1.5f, -2.0f },
    {  1.2f, -0.5f, -2.5f },
    { -2.3f, -0.3f,  1.5f },
    {  0.8f,  1.0f, -1.2f },
    {  2.5f,  0.5f,  1.0f },
    { -1.8f, -1.2f,  0.8f },
    {  0.0f,  0.8f, -3.0f }
};

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(0.3f, -1.3f, -4.0f),
    glm::vec3(-1.0f,  2.0f, -1.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
};

glm::vec3 pointLightColor[] = {
    glm::vec3((float)239/255, (float)45/255, (float)86/255),
    glm::vec3((float)237 /255, (float)125 /255, (float)58 /255),
    glm::vec3((float)140 /255,(float)216 /255,(float)103 /255),
    glm::vec3((float)47 /255, (float)191 /255,  (float)113 /255),
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

    glm::vec3 background(0.125f/4, 0.33f/4 , 0.315f/4);
    Material cube_material;
    DirLight dir_light;
    PointLight point_light;

    cube_material.diffuseMap = new Texture("resources/textures/brick.png");
    cube_material.specularMap = new Texture("resources/textures/roughness.png");
    cube_material.emissionMap = new Texture("resources/textures/crying_girl.png");
    cube_material.shininess = 32.0f;
    cube_material.emissionStrength = 0.5f;

    

    dir_light.ambient = glm::vec3(background);
    dir_light.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    dir_light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    dir_light.direction = glm::vec3(-0.3f, -0.8f, -0.6f);

    phong_shader.Use();
    phong_shader.SetMaterial(cube_material);
    phong_shader.SetDirectionLight(dir_light);

    for (int i = 0; i < 4; i++) {
        point_light.constant = 1.0f;
        point_light.linear = 0.09f;
        point_light.quadratic = 0.032f;

        point_light.ambient = glm::vec3(background);
        point_light.diffuse = pointLightColor[i];
        point_light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
        point_light.position = pointLightPositions[i];
        phong_shader.SetPointLight(point_light,i);
    }



    glActiveTexture(GL_TEXTURE0);
    cube_material.diffuseMap->Bind();

    glActiveTexture(GL_TEXTURE1);
    cube_material.specularMap->Bind();

    /*glActiveTexture(GL_TEXTURE2);
    cube_material.emissionMap->Bind();*/

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
        
        glm::mat4 cube_model = glm::mat4(1.0f);
        phong_shader.SetMat4("view", camera.GetView());
        phong_shader.SetMat4("projection", camera.GetProjection());
        phong_shader.SetVec3("viewPos", camera.cameraPos);

        vao.Bind();
        for (int i = 0; i < 10; i++) {
            glm::mat4 cube_model = glm::mat4(1.0f);

            // translate each cube
            cube_model = glm::translate(cube_model, cubePositions[i]);

            float angle = rotation + i * 36.0f; // unique rotation per cube
            cube_model = glm::rotate(cube_model, glm::radians(angle),
                glm::vec3(0.3f * i, 1.0f, 0.5f * i));

            phong_shader.SetMat4("model", cube_model);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        }

        // --- LIGHT RENDER ---
        color_shader.Use();

        for (int i = 0; i < 4; i++) {
            glm::mat4 light_model = glm::translate(glm::mat4(1.0f), pointLightPositions[i]);
            light_model = glm::scale(light_model, glm::vec3(0.2f));
            color_shader.SetMat4("model", light_model);
            color_shader.SetMat4("view", camera.GetView());
            color_shader.SetMat4("projection", camera.GetProjection());
            color_shader.SetVec3("objectColor", pointLightColor[i]);
            light.Bind();
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        }


        lastFrame = currentFrame;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}