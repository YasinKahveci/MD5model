#include"ImGuiSelector.hpp"

ImGuiSelector::ImGuiSelector(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();
}

void ImGuiSelector::TablolariCiz(float& rx, float& ry, float& rz, ModelList& modelList, ModelMD5& GenelModel)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    RotateTablo(rx, ry, rz);

    ModelListCiz(modelList, GenelModel);

    AnimListCiz(modelList, GenelModel);

    ImGui::EndFrame();


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiSelector::RotateTablo(float &rx,float &ry,float &rz)
{
    ImGui::Begin("Donusum Penceresi", &ack, corner);
    ImGui::SetWindowPos("Donusum Penceresi", ImVec2(25, 600), 0);


    ImGui::SliderFloat("RotateX", &rx, 0, 360);
    ImGui::SliderFloat("RotateY", &ry, 0, 360);
    ImGui::SliderFloat("RotateZ", &rz, 0, 360);

    ImGui::End();
}
void ImGuiSelector::ModelListCiz(ModelList& modelList, ModelMD5& GenelModel)
{
    ImGui::Begin("Model Listesi", &ack, cornerNoMove);
    ImGui::SetWindowSize({ 450,500 });
    ImGui::SetWindowPos("Model Listesi", ImVec2(25, 25), 0);
    for (int i = 0; i < modelList.ModelPathList.size(); i++)
    {
        if (i == selectedItem)
        {
            std::string item = "SELECTED : " + std::string(modelList.ModelPathList.at(i).c_str());
            ImGui::Selectable(item.c_str());
        }
        else if (ImGui::Selectable(modelList.ModelPathList.at(i).c_str()))
        {
            system("CLS");
            if (i != selectedItem)
            {
                selectedItem = i;
                std::cout << modelList.ModelPathList.at(selectedItem) << std::endl;
                TextureManager::getInstance()->unLoadAllTexture();
                GenelModel.ClearAnim();
                selectedAnimItem = -1;
                GenelModel.loadModel(modelList.ModelPathList.at(selectedItem));
                modelList.setAnimList(selectedItem);
            }
        }

    }
    ImGui::End();
}
void ImGuiSelector::AnimListCiz(ModelList& modelList, ModelMD5& GenelModel)
{
    if (modelList.AnimPathList.size()>0)
    {
        ImGui::Begin("Animasyon Listesi", &ack, cornerNoMove);
        ImGui::SetWindowSize({ 450,100 });
        ImGui::SetWindowPos("Animasyon Listesi", ImVec2(800, 25), 0);
        for (int i = 0; i < modelList.AnimPathList.size(); i++)
        {
            if (i == selectedAnimItem)
            {
                std::string item = "SELECTED : " + std::string(modelList.AnimPathList.at(i).c_str());
                ImGui::Selectable(item.c_str());

            }
            else if (ImGui::Selectable(modelList.AnimPathList.at(i).c_str()))
            {
                system("CLS");
                if (i != selectedAnimItem)
                {
                    selectedAnimItem = i;
                    std::cout << "Anim Yuklendi: " << modelList.AnimPathList.at(selectedAnimItem) << std::endl;
                    GenelModel.ClearAnim();
                    GenelModel.loadModelAnim(modelList.AnimPathList.at(selectedAnimItem));
                }
            }

        }
        ImGui::End();
    }
}