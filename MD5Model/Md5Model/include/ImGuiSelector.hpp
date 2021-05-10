#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <vector>
#include <string>
#include <iostream>
#include "TextureManager.hpp"
#include "Model.hpp"
#include "ModelList.hpp"
class ImGuiSelector
{
public:
	ImGuiSelector(GLFWwindow*);
	void TablolariCiz(float& rx, float& ry, float& rz, ModelList& modelList, ModelMD5& GenelModel);
	int selectedItem = 0;
	int selectedAnimItem = -1;
private:
	void ModelListCiz(ModelList&,ModelMD5&);
	void AnimListCiz(ModelList&, ModelMD5&);
	void RotateTablo(float&, float&, float&);
	bool ack = true;
	ImGuiWindowFlags corner = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;
	ImGuiWindowFlags cornerNoMove = ImGuiWindowFlags_NoMove;
};