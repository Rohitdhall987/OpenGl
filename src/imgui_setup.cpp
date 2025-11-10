#include "headers/imgui_setup.h"
#include "headers/utils/windoes_utils.h"

#include <codecvt> 

unsigned int MyImgui::ids = 0;
unsigned int MyImgui::active_obj = 0;
bool MyImgui::invert_textures = false;

std::vector<std::string> MyImgui::import_types = { "fbx", "obj","glb", "gltf", "stl", "ply", "3ds", "blend", "dae", "pmx" };
std::string MyImgui::selected_im_t = MyImgui::import_types[0];

std::vector<Object> MyImgui::objects = {};

MyImgui::MyImgui(GLFWwindow* win, Camera& cam) : camera(cam) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init();

    window = win;
    outline_shader = new Shader("resources/shaders/outline_vertex.glsl", "resources/shaders/color.glsl");
}

void MyImgui::ShutDown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void MyImgui::Render_Models()
{

    glfwGetFramebufferSize(window, &width, &height);

    dir_light.ambient = glm::vec3(amb_col[0], amb_col[1], amb_col[2]);
    dir_light.diffuse = glm::vec3(light_col[0], light_col[1], light_col[2]);
    dir_light.direction = glm::vec3(light_dir[0], light_dir[1], light_dir[2]);

    for (Object& obj : objects)
    {

        obj.shader->Use();
        obj.shader->SetMaterial(obj.material_prop);
        obj.shader->SetDirectionLight(dir_light);
        obj.shader->SetMat4("view", camera.GetView());
        obj.shader->SetMat4("projection", camera.GetProjection(width, height));
        obj.shader->SetVec3("viewPos", camera.cameraPos);


        obj.modelMatrix = glm::translate(glm::mat4(1.0f), obj.transform.position);
        obj.modelMatrix = glm::rotate(obj.modelMatrix, obj.transform.rotation.z, glm::vec3(0, 0, 1));
        obj.modelMatrix = glm::rotate(obj.modelMatrix, obj.transform.rotation.y, glm::vec3(0, 1, 0));
        obj.modelMatrix = glm::rotate(obj.modelMatrix, obj.transform.rotation.x, glm::vec3(1, 0, 0));
        obj.modelMatrix = glm::scale(obj.modelMatrix, obj.transform.scale);
                  
        obj.shader->SetMat4("model", obj.modelMatrix);

        obj.model.Draw(*obj.shader);
    }
}

void MyImgui::Render_Selected_Model()
{
    for (const Object& obj : objects)
    {
        if (obj.id == active_obj)
        {
            obj.shader->Use();
            obj.shader->SetMat4("view", camera.GetView());
            obj.shader->SetMat4("projection", camera.GetProjection(width, height));
            obj.shader->SetMat4("model", obj.modelMatrix);
            obj.model.Draw(*obj.shader);
            break;
        }
    }
}

void MyImgui::Render_Outlines()
{
    for (const Object& obj : objects)
    {
        if (obj.id == active_obj)
        {
            outline_shader->Use();
            outline_shader->SetMat4("view", camera.GetView());
            outline_shader->SetMat4("projection", camera.GetProjection(width, height));
            outline_shader->SetMat4("model", obj.modelMatrix);
            outline_shader->SetFloat("outlineThickness", thickness);
            outline_shader->SetVec3("outlineColor", glm::vec3(outline_col[0], outline_col[1], outline_col[2]));
            obj.model.Draw(*outline_shader);
        }
    }
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
        ImGui::OpenPopup("Import Dialog");
        
    

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

        ImGui::Checkbox("Invert textures", &invert_textures);

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
                size_t pos = filepath.find_last_of(L"\\/");
                std::wstring filename;
                if (pos != std::wstring::npos)
                    filename = filepath.substr(pos + 1);
                else
                    filename = filepath;
                Transform trans;
                Material prop;
                Shader* sdr = new Shader("resources/shaders/vertex.glsl", "resources/shaders/frag.glsl");
                Object obj = { ids, converter.to_bytes(filename), Model(path, invert_textures), sdr, trans, prop};

                objects.push_back(obj);
                ids++;
            }
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
    ImGui::SliderFloat("Outline Thickness", &thickness, 0.0f, 0.3f);
    ImGui::ColorPicker3("OutLine Color", outline_col);
    ImGui::Spacing();

    ImGui::ColorPicker3("BackGround Color", bg_col);

    ImGui::Spacing();


    ImGui::ColorPicker3("ambiant", amb_col);

    ImGui::Spacing();

    ImGui::SliderFloat3("Light Direction", light_dir,-1,1);
    ImGui::ColorPicker3("Light Color", light_col);

    
    ImGui::End();


    ImGui::Begin("Objects");

    static unsigned int active_index = 0;

    for (int i = 0; i < objects.size(); i++) {

        Object obj = objects[i];

        if (ImGui::Selectable(obj.name.c_str(), obj.id == active_obj)) {
            active_obj = obj.id;
            active_index = i;
        }
    }

    ImGui::End();

    if (!objects.empty()) {
        ImGui::Begin("Object_Settings");

        ImGui::SliderFloat("Shininess", &(objects[active_index].material_prop.shininess), 1.0f, 256.0f);
        ImGui::SliderFloat("specular", &(objects[active_index].material_prop.specular), 0.0f, 10.0f);
        
        ImGui::Spacing();

        ImGui::Text("Transform xyz");
        
        ImGui::DragFloat3("Position", glm::value_ptr(objects[active_index].transform.position), 0.2f);
        ImGui::DragFloat3("Rotaion", glm::value_ptr(objects[active_index].transform.rotation), 0.2f);
        ImGui::DragFloat3("Scale", glm::value_ptr(objects[active_index].transform.scale), 0.2f);

        ImGui::End();
    }
}