#pragma once

struct GLFWwindow;

class Callback {
	static bool middleMousePressed;
public:
	static void frameSizeCallBack(GLFWwindow* window, int width, int height);
	static void errCallBack(int error_code, const char* description);
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};