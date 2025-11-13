#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float pitch = 0.0f;
    float yaw = -90.0f;
    float lastX = 400.0f;
    float lastY = 400.0f;

    float fov = 45.0f;

    bool firstMouse = true;


    glm::mat4 GetView() const;
    glm::mat4 GetProjection(int width, int height) const;
    void ProcessInput(GLFWwindow* window, float deltaTime);

    void MouseMovement(double xpos, double ypos);

    void ScrollInput(double xoffset, double yoffset);

    void ResetMouse(double xpos, double ypos);

};
