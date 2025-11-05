#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "model.h"

class MyImgui {
	void Frames();

	static std::vector<std::string> import_types;
	static std::string selected_im_t;
	Model m;
public:
	MyImgui(GLFWwindow* window);
	void Render();
	void ShutDown();
	void Render_Model(const Shader& shader);

	float bg_col[4] = { 0.15625f, 0.15625f, 0.15625f, 1.0f };
	float amb_col[3] = { 0.15625f, 0.15625f, 0.15625f };
	float light_dir[3] = { -0.3f, -0.8f, -0.6f };
	float light_col[3] = { 1.0f, 1.0f, 1.0f };
	float outline_col[3] = { 1.0f, 1.0f, 1.0f };
	float light_spe = 1.0f;
	float shine = 32.0f;
	float thickness = 0.02f;
};