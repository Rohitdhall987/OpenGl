#pragma once

struct GLFWwindow;

class Callback {
public:
	static void frameSizeCallBack(GLFWwindow* window, int width, int height);
	static void errCallBack(int error_code, const char* description);
};