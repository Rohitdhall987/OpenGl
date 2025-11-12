
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/callback.h"
#include "headers/shader.h"
#include "headers/camera.h"

#include "headers/imgui_setup.h"



float quadVertices[] = {
    // Positions   // Texture Coords
    -1.0f,  1.0f,   0.0f, 1.0f,   // Top-left
    -1.0f, -1.0f,   0.0f, 0.0f,   // Bottom-left
     1.0f, -1.0f,   1.0f, 0.0f,   // Bottom-right

    -1.0f,  1.0f,   0.0f, 1.0f,   // Top-left
     1.0f, -1.0f,   1.0f, 0.0f,   // Bottom-right
     1.0f,  1.0f,   1.0f, 1.0f    // Top-right
};

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_STENCIL_TEST);

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

    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    unsigned int frameBufferTexture;
    glGenTextures(1, &frameBufferTexture);
    glBindTexture(GL_TEXTURE_2D, frameBufferTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mode->width, mode->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);


    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mode->width, mode->height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        exit(2);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);




    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    Shader screenShader("resources/shaders/frame_vert.glsl", "resources/shaders/frame_buffer.glsl");

    int width, height;

    while (!glfwWindowShouldClose(window))
    {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        camera.ProcessInput(window, deltaTime);

        glfwGetFramebufferSize(window, &width, &height);

        glBindTexture(GL_TEXTURE_2D, frameBufferTexture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        glClearColor(gui.bg_col[0], gui.bg_col[1], gui.bg_col[2], gui.bg_col[3]);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glEnable(GL_DEPTH_TEST);

        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glStencilMask(0x00);  
        gui.Render_Models();

        
        //glDisable(GL_DEPTH_TEST);
        //glStencilFunc(GL_ALWAYS, 1, 0xFF);
        //glStencilMask(0xFF);  
        //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); 
        //gui.Render_Selected_Model();  
        //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);  


        //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        //glStencilMask(0x00);
        //gui.Render_Outlines();

        //glStencilMask(0xFF);
        //glEnable(GL_DEPTH_TEST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        screenShader.Use();
        screenShader.SetFloat("pixelSize", 6.0f); // adjust to taste
        screenShader.SetVec2("resolution", glm::vec2(width, height));
        glBindVertexArray(quadVAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        gui.Render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    gui.ShutDown();

    glfwTerminate();
    return 0;
}
