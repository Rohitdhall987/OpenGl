#include "headers/imgui_setup.h"
#include "headers/utils/windoes_utils.h"

#include <codecvt> 

std::vector<std::string> MyImgui::import_types = { "fbx", "obj","glb", "gltf", "stl", "ply", "3ds", "blend", "dae", "pmx" };

std::string MyImgui::selected_im_t = MyImgui::import_types[0];

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
    if (ImGui::Button("Import...")) 
    {
        ImGui::OpenPopup("Import Dialog");
        
    }

    if (ImGui::BeginPopup("Import Dialog"))
    {
        ImGui::Text("Import Settings");

        if (ImGui::BeginCombo("Type", selected_im_t.c_str()))
        {
            for (auto& type : import_types)
            {
                bool selected = (selected_im_t == type);
                if (ImGui::Selectable(type.c_str(), selected))
                    selected_im_t = type;
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }


        if (ImGui::Button("import")) {

            std::wstring selected_ext = std::wstring(selected_im_t.begin(), selected_im_t.end());

            std::wstring filter;
            filter += L"3D Files (*." + selected_ext + L")";
            filter.push_back(L'\0');  // first null terminator
            filter += L"*." + selected_ext;
            filter.push_back(L'\0');  // second null terminator
            filter += L"All Files (*.*)";
            filter.push_back(L'\0');  // third null terminator
            filter += L"*.*";
            filter.push_back(L'\0');  // final null terminator


            std::wstring filepath = winUtil::open_file_dialog(filter.c_str());
            if (!filepath.empty()) {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::string path = converter.to_bytes(filepath);

                m = Model(path);
            }
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
    //ImGui::ColorPicker3("OutLine Color", outline_col);
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