#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <cmath>
#include "ShaderProgram.hpp"
#include "TextureManager.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include "Model.hpp"
#include "MeshAnimation.hpp"
#include <chrono>
#include "ModelList.hpp"
#include "ImGuiSelector.hpp"



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool Anim = false;

float cerceveSuresi = 1.0f / 24.0f;

int main()
{
    ModelList modelList;
    modelList.setModelList(".\\models\\md5");
   

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "MD5 Model", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Pencere olusturulamadi" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed  to initialize GLAD " << std::endl;
        return -1;
    }

    ImGuiSelector Selector(window);

    

    Shader shaderProgram = Shader();

    shaderProgram.AttachShader("./shaders/simplevs2.glsl", GL_VERTEX_SHADER);
    shaderProgram.AttachShader("./shaders/simplefs2.glsl", GL_FRAGMENT_SHADER);
    shaderProgram.link();

    ModelMD5 GenelModel;
    GenelModel.loadModel(modelList.ModelPathList.at(Selector.selectedItem));

    shaderProgram.addUniform("model");
    shaderProgram.addUniform("view");
    shaderProgram.addUniform("projection");
    float angle = 45.0;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);


    glm::vec3 ort = GenelModel.getLookAt();
    glm::vec3 p = glm::vec3(ort.x , ort.y , ort.z );
    float scaleSize = GenelModel.getMaxSize();
    auto start = std::chrono::steady_clock::now();

    float rz = 0.0f;
    float ry = 0.0f;
    float rx = 0.0f;

    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    

    while (!glfwWindowShouldClose(window))
    {
        auto end = std::chrono::steady_clock::now();
        
        
        shaderProgram.use();
        
        glm::mat4 projection = glm::perspective(glm::radians(angle), 1280.0f / 720.0f, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(30.0,30.0,30.0), p, glm::vec3(0.0, 0.0, 1.0));
        glm::mat4 model = glm::scale(glm::mat4(1.0), glm::vec3(20/scaleSize));
        model = glm::rotate(model, glm::radians(rx), glm::vec3(1.0, 0.0, 0.0));
        model = glm::rotate(model, glm::radians(ry), glm::vec3(0.0, 1.0, 0.0));
        model = glm::rotate(model, glm::radians(rz), glm::vec3(0.0, 0.0, 1.0));
        model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
        
        
        shaderProgram.setMat4("model", &model);
        shaderProgram.setMat4("view", &view);
        shaderProgram.setMat4("projection", &projection);
        
        shaderProgram.use();

        
        if (Anim && Selector.selectedAnimItem!=-1)
        {
            GenelModel.setAnimActive();
            Anim = false;
        }
        if (std::chrono::duration<float>(end - start).count() > cerceveSuresi)
        {
            glClearColor(0.514f, 0.6f, 0.694f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            GenelModel.Draw(std::chrono::duration<float>(end - start).count());
            start = end;

            Selector.TablolariCiz(rx, ry, rz, modelList, GenelModel);
            ort = GenelModel.getLookAt();
            p = glm::vec3(ort.x*20/scaleSize, ort.y * 20 / scaleSize, ort.z * 20 / scaleSize);
            scaleSize = GenelModel.getMaxSize();

            glfwSwapBuffers(window);
        }
        glfwPollEvents();
        
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE)
        glfwTerminate();
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_SPACE)
        {
            Anim = true;
        }

    }
}

