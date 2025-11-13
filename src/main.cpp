
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

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

float skyboxVertices[] = {
    -1.0f,  1.0f, -1.0f, // 0
    -1.0f, -1.0f, -1.0f, // 1
     1.0f, -1.0f, -1.0f, // 2
     1.0f,  1.0f, -1.0f, // 3
    -1.0f,  1.0f,  1.0f, // 4
    -1.0f, -1.0f,  1.0f, // 5
     1.0f, -1.0f,  1.0f, // 6
     1.0f,  1.0f,  1.0f  // 7
};

unsigned int skyboxIndices[] = {
    // back
    0, 1, 2, 2, 3, 0,
    // left
    4, 5, 1, 1, 0, 4,
    // right
    3, 2, 6, 6, 7, 3,
    // front
    4, 0, 3, 3, 7, 4,
    // bottom
    1, 5, 6, 6, 2, 1,
    // top
    4, 7, 6, 6, 5, 4
};


std::vector<std::string> faces = {
    "right.jpg",
    "left.jpg",
    "top.jpg",
    "bottom.jpg",
    "back.jpg",
    "front.jpg",
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




    unsigned int SKYVAO, SKYVBO, SKYEBO;
    glGenVertexArrays(1, &SKYVAO);
    glGenBuffers(1, &SKYVBO);
    glGenBuffers(1, &SKYEBO);

    glBindVertexArray(SKYVAO);
    glBindBuffer(GL_ARRAY_BUFFER, SKYVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SKYEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), skyboxIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);

    
    
    unsigned int cubMap;
    glGenTextures(1, &cubMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubMap);
    

    int w, h, nrChannels;
    unsigned char* data;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        data = stbi_load(("resources/textures/skybox/" + faces[i]).c_str(), &w, &h, &nrChannels, 0);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data
        );

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    
    Shader screenShader("resources/shaders/frame_vert.glsl", "resources/shaders/frame_buffer.glsl");
    Shader skyboxShader("resources/shaders/skybox_vert.glsl", "resources/shaders/skybox_frag.glsl");

    skyboxShader.Use();

    skyboxShader.SetInt("skybox", 0);

    float deltaTime = 0.0f, lastFrame = 0.0f;
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


        glDepthFunc(GL_LEQUAL);
        skyboxShader.Use();
        glm::mat4 view(1.0f);

        view = glm::mat4(glm::mat3(camera.GetView()));

        skyboxShader.SetMat4("view", view);
        skyboxShader.SetMat4("projection", camera.GetProjection(width, height));

        glBindVertexArray(SKYVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubMap);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);




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
