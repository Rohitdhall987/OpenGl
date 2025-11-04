#include "headers/imgui_setup.h"
#include "headers/utils/windoes_utils.h"

#include <codecvt> 

MyImgui::MyImgui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void MyImgui::ShutDown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void MyImgui::Render_Model(const Shader& shader)
{
    m.Draw(shader);
}

void MyImgui::Render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    Frames();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MyImgui::Frames() {
    ImGui::NewFrame();
    ImGui::Begin("Settings");
    if (ImGui::Button("Import pmx")) 
    {
        std::wstring filepath = winUtil::open_file_dialog(L"PMX Model Files (*.pmx)\0*.pmx\0All Files (*.*)\0*.*\0\0");
        if (!filepath.empty()) {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::string path = converter.to_bytes(filepath);

            m = Model(path);
        }
    }
    ImGui::ColorPicker3("OutLine Color", outline_col);
    ImGui::SliderFloat("Outline Thickness", &thickness, 0.0f, 0.3f);
    ImGui::Spacing();

    ImGui::ColorPicker3("BackGround Color", bg_col);

    ImGui::Spacing();

    ImGui::SliderFloat("Shininess",&shine,1.0f,256.0f);
    ImGui::ColorPicker3("ambiant", amb_col);

    ImGui::Spacing();

    ImGui::SliderFloat3("Light Direction", light_dir,-1,1);
    ImGui::ColorPicker3("Light Color", light_col);
    ImGui::SliderFloat("specular", &light_spe, 0.0f, 10.0f);
    
    ImGui::End();
}