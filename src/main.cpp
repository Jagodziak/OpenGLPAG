#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h" 
#include <stdio.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>  

#include <GLFW/glfw3.h> 

#include "Shader.hpp"
#include "Model.hpp"

const int WINDOW_WIDTH = 1920, WINDOW_HEIGHT = 1080;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main() 
{   
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
        return 1;
    
    const char* glsl_version = "#version 430";
   
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Zadanie 3 - Uklad planetarny", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window); 
    glfwSwapInterval(1); 
    
    bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }
  
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    ImGui::StyleColorsDark();

    Shader basicShader("res/shaders/basic.vert", "res/shaders/basic.frag");
    Shader basicInstancedShader("res/shaders/basic_instanced.vert", "res/shaders/basic_instanced.frag");

    std::vector<Model*> sceneObjects;

    Model skybox("res/models/skybox.fbx");
    sceneObjects.push_back(&skybox);
    skybox.texture.load("res/textures/spiaggia_di_mondello_4k.png");
    skybox.modelTransform.rotate(glm::vec3(glm::radians(180.0f), 0.0f, 0.0f));
    skybox.modelTransform.scale(glm::vec3(12.0f, 12.0f, 12.0f));

    Transform sceneRoot;
    //sceneRoot.rotate(glm::vec3(glm::radians(180.0f), 0.0f, 0.0f));

    Model ground("res/models/ground.fbx");
    sceneObjects.push_back(&ground);
    ground.texture.load("res/textures/sand_01_diff_2k.png");
    sceneRoot.addChild(&ground.modelTransform);

    Transform houses;
    sceneRoot.addChild(&houses);

    const int gridX = 200, gridY = 200;
    const float gridIncrement = 150.0f;
    std::vector<glm::vec3> houseOffsets;
    houseOffsets.reserve(gridX * gridY);

    float yOffset = gridY * -0.5f * gridIncrement;
    for (int i = 0; i < gridY; i++)
    {
        float xOffset = gridX * -0.5f * gridIncrement;
        for (int j = 0; j < gridX; j++)
        {
            houseOffsets.push_back(glm::vec3(xOffset, 0.0f, yOffset));
            xOffset += gridIncrement;
        }
        yOffset += gridIncrement;
    }

    Model base("res/models/house_base.fbx", false, &houseOffsets);
    sceneObjects.push_back(&base);
    base.texture.load("res/textures/blue_painted_planks_diff_2k.png");
    houses.addChild(&base.modelTransform);

    Model roof("res/models/house_roof.fbx", false, &houseOffsets);
    sceneObjects.push_back(&roof);
    roof.texture.load("res/textures/reed_roof_03_diff_2k.png");
    base.modelTransform.addChild(&roof.modelTransform);


    bool wireframe = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 color = ImVec4(1.0f, 1.0f, 1.00f, 1.00f);
    float cameraTargetPosition[3] = { 0.0f, 0.0f, 0.0f };
    float cameraPitch = 30.0f;
    float cameraYaw = 0.0f;
    float cameraZoom = 100.0f;
    glm::mat4 modelMatrix, viewMatrix, projectionMatrix;
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float modelScale = 1.0f;
    int iterations = 1;

    float coneWidth = 1.0f, coneHeight = 2.0f;
    int coneSides = 16;
    // ============================================ Main loop =======================================================================

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents(); //zbiera input

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Okienko");
            ImGui::Checkbox("Draw wireframe", &wireframe);
            ImGui::Text("Camera Controls");
            ImGui::SliderFloat3("Camera Pos", cameraTargetPosition, -50.0f, 50.0f);
            ImGui::SliderFloat("Camera Pitch", &cameraPitch, -180.0f, 180.0f);
            ImGui::SliderFloat("Camera Yaw", &cameraYaw, -180.0f, 180.0f);
            ImGui::SliderFloat("Camera Zoom", &cameraZoom, 1.0f, 400.0f);
            ImGui::Text("Color Controls");
            ImGui::ColorEdit3("texture color", (float*)&color);
            ImGui::ColorEdit3("clear color", (float*)&clear_color); 
            ImGui::Text("Cone Controls");
            ImGui::SliderFloat("Cone Width", &coneWidth, 0.1f, 10.0f);
            ImGui::SliderFloat("Cone Height", &coneHeight, 0.1f, 10.0f);
            ImGui::SliderInt("Cone Sides", &coneSides, 3, 64);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        
        glEnable(GL_DEPTH_TEST);

        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        
        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        float cameraX = glm::sin(glm::radians(cameraYaw)) * cameraZoom;
        float cameraZ = glm::cos(glm::radians(cameraYaw)) * cameraZoom;
        float cameraY = glm::sin(glm::radians(cameraPitch * 0.5f)) * cameraZoom;
        
        viewMatrix = glm::lookAt(glm::vec3(cameraX, cameraY, cameraZ), glm::vec3(cameraTargetPosition[0], cameraTargetPosition[1], cameraTargetPosition[2]), glm::vec3(0.0f, 1.0f, 0.0f));

        
        projectionMatrix = glm::perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.01f, 15000.0f);

        basicShader.setVec4("color", glm::vec4(color.x, color.y, color.z, color.w));
        basicShader.setMat4("view", viewMatrix);
        basicShader.setMat4("projection", projectionMatrix);

        sceneRoot.simulate();
        sceneRoot.recalculate();

        for (int i = 0; i < sceneObjects.size(); i++)
        {
            sceneObjects[i]->draw(basicInstancedShader);
        }

        // Imgui ui render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window); 
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown(); 
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}