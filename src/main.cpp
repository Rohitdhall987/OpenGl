#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main(void)
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800,600,"LEARN OPENGL",NULL,NULL);

    glfwMakeContextCurrent(window);

    glewInit();

    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.3f,0.3f,0.7f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}