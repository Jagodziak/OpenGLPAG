#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h" // biblioteka do renderowania prostych interfejs�w graficznych
#include <stdio.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>  // loader funkcji opengl

#include <GLFW/glfw3.h> //biblioteka do tworzenia okna, zczytuje ruch myszy, klawisze itd 

#include "Shader.hpp"
#include "Model.hpp"

const int WINDOW_WIDTH = 1920, WINDOW_HEIGHT = 1080;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main() 
{
    // Ustawienie wy�wietlania b��d�w okna
    glfwSetErrorCallback(glfw_error_callback);

    // Inicjalizacja glfw
    if (!glfwInit())
        return 1;

    // Ustawienie minimalnej wspieranej wersji OpenGL (4.3)
    const char* glsl_version = "#version 430";
    // Parametry glfw
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Tworzenie okna
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Zadanie 1- Kostka", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window); 
    glfwSwapInterval(1); // Enable vsync- synchronizacja pionowa- blokuje fps �eby zsynchronizowa� go z cz�stotliwo�ci� od�wie�ania monitora

    // Initialize OpenGL loader
    bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();

    // Shader
    Shader basicShader("res/shaders/basic.vert", "res/shaders/basic.frag");

    // Model
    std::vector<Model*> sceneObjects;

    Model skybox("res/models/skybox.fbx");
    sceneObjects.push_back(&skybox);
    skybox.texture.load("res/textures/stars_milky_way.jpg");

    Transform sceneRoot;
    // For the solar system to be the right way up
    sceneRoot.rotate(glm::vec3(glm::radians(180.0f), 0.0f, 0.0f));

    Model sun("res/models/planet.fbx");
    sceneObjects.push_back(&sun);
    sun.texture.load("res/textures/sun.jpg");
    sun.modelTransform.rotationSpeed = 0.01f;
    sceneRoot.addChild(&sun.modelTransform);

    float orbitRadius, orbitAngle, orbitSpeed, planetRadius, planetSpeed;

    // ===== PLANET =================================================
    orbitRadius = 40.0f;
    orbitAngle = 5.0f;
    orbitSpeed = -0.004f;
    planetRadius = 0.08f;
    planetSpeed = 0.05f;

    Transform mercuryAngle;
    mercuryAngle.rotate(glm::vec3(0.0f, 0.0f, glm::radians(orbitAngle)));
    sceneRoot.addChild(&mercuryAngle);

    Model mercuryOrbit("res/models/orbit.fbx", true);
    sceneObjects.push_back(&mercuryOrbit);
    mercuryOrbit.texture.load("res/textures/white.jpg");
    mercuryOrbit.modelTransform.scale(glm::vec3(orbitRadius * 0.1f));
    mercuryAngle.addChild(&mercuryOrbit.modelTransform);

    Transform mercuryPivot;
    mercuryPivot.rotationSpeed = orbitSpeed;
    mercuryAngle.addChild(&mercuryPivot);

    Model mercury("res/models/planet.fbx");
    sceneObjects.push_back(&mercury);
    mercury.texture.load("res/textures/mercury.jpg");
    mercury.modelTransform.move(glm::vec3(orbitRadius, 0.0f, 0.0f));
    mercury.modelTransform.scale(glm::vec3(planetRadius));
    mercury.modelTransform.rotationSpeed = planetSpeed;
    mercuryPivot.addChild(&mercury.modelTransform);
    // ===============================================================

    // ===== PLANET =================================================
    orbitRadius = 60.0f;
    orbitAngle = 0.0f;
    orbitSpeed = 0.001f;
    planetRadius = 0.25f;
    planetSpeed = 0.01f;

    Transform venusAngle;
    venusAngle.rotate(glm::vec3(0.0f, 0.0f, glm::radians(orbitAngle)));
    sceneRoot.addChild(&venusAngle);

    Model venusOrbit("res/models/orbit.fbx", true);
    sceneObjects.push_back(&venusOrbit);
    venusOrbit.texture.load("res/textures/white.jpg");
    venusOrbit.modelTransform.scale(glm::vec3(orbitRadius * 0.1f));
    venusAngle.addChild(&venusOrbit.modelTransform);

    Transform venusPivot;
    venusPivot.rotationSpeed = orbitSpeed;
    venusAngle.addChild(&venusPivot);

    Model venus("res/models/planet.fbx");
    sceneObjects.push_back(&venus);
    venus.texture.load("res/textures/venus.jpg");
    venus.modelTransform.move(glm::vec3(orbitRadius, 0.0f, 0.0f));
    venus.modelTransform.scale(glm::vec3(planetRadius));
    venus.modelTransform.rotationSpeed = planetSpeed;
    venusPivot.addChild(&venus.modelTransform);
    // ===============================================================

    // ===== PLANET =================================================
    orbitRadius = 80.0f;
    orbitAngle = 10.0f;
    orbitSpeed = 0.0005f;
    planetRadius = 0.35f;
    planetSpeed = -0.01f;

    Transform earthAngle;
    earthAngle.rotate(glm::vec3(0.0f, 0.0f, glm::radians(orbitAngle)));
    sceneRoot.addChild(&earthAngle);

    Model earthOrbit("res/models/orbit.fbx", true);
    sceneObjects.push_back(&earthOrbit);
    earthOrbit.texture.load("res/textures/white.jpg");
    earthOrbit.modelTransform.scale(glm::vec3(orbitRadius * 0.1f));
    earthAngle.addChild(&earthOrbit.modelTransform);

    Transform earthPivot;
    earthPivot.rotationSpeed = orbitSpeed;
    earthAngle.addChild(&earthPivot);

    Transform earthCenter;
    earthCenter.move(glm::vec3(orbitRadius, 0.0f, 0.0f));
    earthPivot.addChild(&earthCenter);

    Model earth("res/models/planet.fbx");
    sceneObjects.push_back(&earth);
    earth.texture.load("res/textures/earth.jpg");
    earth.modelTransform.scale(glm::vec3(planetRadius));
    earth.modelTransform.rotationSpeed = planetSpeed;
    earthCenter.addChild(&earth.modelTransform);
    // ===============================================================

    // ===== MOON =================================================
    orbitRadius = 10.0f;
    orbitAngle = 0.0f;
    orbitSpeed = 0.0005f;
    planetRadius = 0.05f;
    planetSpeed = 0.01f;

    Transform moonAngle;
    moonAngle.rotate(glm::vec3(0.0f, 0.0f, glm::radians(orbitAngle)));
    earthCenter.addChild(&moonAngle);

    Model moonOrbit("res/models/orbit.fbx", true);
    sceneObjects.push_back(&moonOrbit);
    moonOrbit.texture.load("res/textures/white.jpg");
    moonOrbit.modelTransform.scale(glm::vec3(orbitRadius * 0.1f));
    moonAngle.addChild(&moonOrbit.modelTransform);

    Transform moonPivot;
    moonPivot.rotationSpeed = orbitSpeed;
    moonAngle.addChild(&moonPivot);

    Model moon("res/models/planet.fbx");
    sceneObjects.push_back(&moon);
    moon.texture.load("res/textures/moon.jpg");
    moon.modelTransform.move(glm::vec3(orbitRadius, 0.0f, 0.0f));
    moon.modelTransform.scale(glm::vec3(planetRadius));
    moon.modelTransform.rotationSpeed = planetSpeed;
    moonPivot.addChild(&moon.modelTransform);
    // ===============================================================
    
    // ===== PLANET =================================================
    orbitRadius = 100.0f;
    orbitAngle = 0.0f;
    orbitSpeed = 0.0005f;
    planetRadius = 0.25f;
    planetSpeed = 0.02f;

    Transform marsAngle;
    marsAngle.rotate(glm::vec3(0.0f, 0.0f, glm::radians(orbitAngle)));
    sceneRoot.addChild(&marsAngle);

    Model marsOrbit("res/models/orbit.fbx", true);
    sceneObjects.push_back(&marsOrbit);
    marsOrbit.texture.load("res/textures/white.jpg");
    marsOrbit.modelTransform.scale(glm::vec3(orbitRadius * 0.1f));
    marsAngle.addChild(&marsOrbit.modelTransform);

    Transform marsPivot;
    marsPivot.rotationSpeed = orbitSpeed;
    marsAngle.addChild(&marsPivot);

    Transform marsCenter;
    marsCenter.move(glm::vec3(orbitRadius, 0.0f, 0.0f));
    marsPivot.addChild(&marsCenter);

    Model mars("res/models/planet.fbx");
    sceneObjects.push_back(&mars);
    mars.texture.load("res/textures/mars.jpg");
    mars.modelTransform.scale(glm::vec3(planetRadius));
    mars.modelTransform.rotationSpeed = planetSpeed;
    marsCenter.addChild(&mars.modelTransform);
    // ===============================================================

    // ===== PLANET =================================================
    orbitRadius = 150.0f;
    orbitAngle = 0.0f;
    orbitSpeed = 0.0004f;
    planetRadius = 1.0f;
    planetSpeed = 0.02f;

    Transform jupiterAngle;
    jupiterAngle.rotate(glm::vec3(0.0f, 0.0f, glm::radians(orbitAngle)));
    sceneRoot.addChild(&jupiterAngle);

    Model jupiterOrbit("res/models/orbit.fbx", true);
    sceneObjects.push_back(&jupiterOrbit);
    jupiterOrbit.texture.load("res/textures/white.jpg");
    jupiterOrbit.modelTransform.scale(glm::vec3(orbitRadius * 0.1f));
    jupiterAngle.addChild(&jupiterOrbit.modelTransform);

    Transform jupiterPivot;
    jupiterPivot.rotationSpeed = orbitSpeed;
    jupiterAngle.addChild(&jupiterPivot);

    Transform jupiterCenter;
    jupiterCenter.move(glm::vec3(orbitRadius, 0.0f, 0.0f));
    jupiterPivot.addChild(&jupiterCenter);

    Model jupiter("res/models/planet.fbx");
    sceneObjects.push_back(&jupiter);
    jupiter.texture.load("res/textures/jupiter.jpg");
    jupiter.modelTransform.scale(glm::vec3(planetRadius));
    jupiter.modelTransform.rotationSpeed = planetSpeed;
    jupiterCenter.addChild(&jupiter.modelTransform);
    // ===============================================================

    // ===== PLANET =================================================
    orbitRadius = 200.0f;
    orbitAngle = 0.0f;
    orbitSpeed = 0.0003f;
    planetRadius = 0.7f;
    planetSpeed = 0.02f;

    Transform saturnAngle;
    saturnAngle.rotate(glm::vec3(0.0f, 0.0f, glm::radians(orbitAngle)));
    sceneRoot.addChild(&saturnAngle);

    Model saturnOrbit("res/models/orbit.fbx", true);
    sceneObjects.push_back(&saturnOrbit);
    saturnOrbit.texture.load("res/textures/white.jpg");
    saturnOrbit.modelTransform.scale(glm::vec3(orbitRadius * 0.1f));
    saturnAngle.addChild(&saturnOrbit.modelTransform);

    Transform saturnPivot;
    saturnPivot.rotationSpeed = orbitSpeed;
    saturnAngle.addChild(&saturnPivot);

    Transform saturnCenter;
    saturnCenter.move(glm::vec3(orbitRadius, 0.0f, 0.0f));
    saturnPivot.addChild(&saturnCenter);

    Model saturn("res/models/planet.fbx");
    sceneObjects.push_back(&saturn);
    saturn.texture.load("res/textures/saturn.jpg");
    saturn.modelTransform.scale(glm::vec3(planetRadius));
    saturn.modelTransform.rotationSpeed = planetSpeed;
    saturnCenter.addChild(&saturn.modelTransform);
    // ===============================================================

    // ===== PLANET =================================================
    orbitRadius = 290.0f;
    orbitAngle = 3.0f;
    orbitSpeed = 0.0007f;
    planetRadius = 0.6f;
    planetSpeed = 0.04f;

    Transform uranusAngle;
    uranusAngle.rotate(glm::vec3(0.0f, 0.0f, glm::radians(orbitAngle)));
    sceneRoot.addChild(&uranusAngle);

    Model uranusOrbit("res/models/orbit.fbx", true);
    sceneObjects.push_back(&uranusOrbit);
    uranusOrbit.texture.load("res/textures/white.jpg");
    uranusOrbit.modelTransform.scale(glm::vec3(orbitRadius * 0.1f));
    uranusAngle.addChild(&uranusOrbit.modelTransform);

    Transform uranusPivot;
    uranusPivot.rotationSpeed = orbitSpeed;
    uranusAngle.addChild(&uranusPivot);

    Transform uranusCenter;
    uranusCenter.move(glm::vec3(orbitRadius, 0.0f, 0.0f));
    uranusPivot.addChild(&uranusCenter);

    Model uranus("res/models/planet.fbx");
    sceneObjects.push_back(&uranus);
    uranus.texture.load("res/textures/uranus.jpg");
    uranus.modelTransform.scale(glm::vec3(planetRadius));
    uranus.modelTransform.rotationSpeed = planetSpeed;
    uranusCenter.addChild(&uranus.modelTransform);
    // ===============================================================

    // ===== PLANET =================================================
    orbitRadius = 350.0f;
    orbitAngle = 5.0f;
    orbitSpeed = 0.0002f;
    planetRadius = 0.55f;
    planetSpeed = 0.08f;

    Transform neptuneAngle;
    neptuneAngle.rotate(glm::vec3(0.0f, 0.0f, glm::radians(orbitAngle)));
    sceneRoot.addChild(&neptuneAngle);

    Model neptuneOrbit("res/models/orbit.fbx", true);
    sceneObjects.push_back(&neptuneOrbit);
    neptuneOrbit.texture.load("res/textures/white.jpg");
    neptuneOrbit.modelTransform.scale(glm::vec3(orbitRadius * 0.1f));
    neptuneAngle.addChild(&neptuneOrbit.modelTransform);

    Transform neptunePivot;
    neptunePivot.rotationSpeed = orbitSpeed;
    neptuneAngle.addChild(&neptunePivot);

    Transform neptuneCenter;
    neptuneCenter.move(glm::vec3(orbitRadius, 0.0f, 0.0f));
    neptunePivot.addChild(&neptuneCenter);

    Model neptune("res/models/planet.fbx");
    sceneObjects.push_back(&neptune);
    neptune.texture.load("res/textures/neptune.jpg");
    neptune.modelTransform.scale(glm::vec3(planetRadius));
    neptune.modelTransform.rotationSpeed = planetSpeed;
    neptuneCenter.addChild(&neptune.modelTransform);
    // ===============================================================

    // Zmienne pomocnicze paremetryzuj�ce rendering
    bool wireframe = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 color = ImVec4(1.0f, 1.0f, 1.00f, 1.00f);
    float cameraPosition[3] = { 0.0f, 0.0f, 0.0f };
    float cameraPitch = 30.0f;
    float cameraYaw = 0.0f;
    float cameraZoom = 100.0f;
    glm::mat4 modelMatrix, viewMatrix, projectionMatrix;
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float modelScale = 1.0f;
    int iterations = 1;

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

            ImGui::SliderFloat("Model scale", &modelScale, 0.01f, 2.0f);
            ImGui::Text("Camera Controls");
            ImGui::SliderFloat3("Camera Pos", cameraPosition, -50.0f, 50.0f);
            ImGui::SliderFloat("Camera Pitch", &cameraPitch, -180.0f, 180.0f);
            ImGui::SliderFloat("Camera Yaw", &cameraYaw, -180.0f, 180.0f);
            ImGui::SliderFloat("Camera Zoom", &cameraZoom, -180.0f, 180.0f);
            ImGui::Text("Color Controls");
            ImGui::ColorEdit3("texture color", (float*)&color);
            ImGui::ColorEdit3("clear color", (float*)&clear_color); 


            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        // U�ywaj bufora g��boko�ci (rozwi�zuje problemy z sortowaniem)
        glEnable(GL_DEPTH_TEST);

        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        
        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        /*testModel.modelTransform.reset();
        testModel.modelTransform.rotate(glm::vec3(rotationX, rotationY, 0.0f));
        testModel.modelTransform.scale(glm::vec3(modelScale, modelScale, modelScale));*/

        // Przygotowanie macierzy
        viewMatrix = glm::lookAt(glm::vec3(cameraYaw, cameraPitch, cameraZoom), glm::vec3(cameraPosition[0], cameraPosition[1], cameraPosition[2]), glm::vec3(0.0f, 1.0f, 0.0f));

        projectionMatrix = glm::mat4(1.0f);
        projectionMatrix = glm::perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.01f, 3000.0f);

        basicShader.setVec4("color", glm::vec4(color.x, color.y, color.z, color.w));
        basicShader.setMat4("view", viewMatrix);
        basicShader.setMat4("projection", projectionMatrix);

        sceneRoot.recalculate(glm::mat4(1.0f));

        for (int i = 0; i < sceneObjects.size(); i++)
        {
            sceneObjects[i]->draw(basicShader);
        }

        glBindVertexArray(0);

        // Imgui ui render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window); //podw�jne buforowanie
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown(); 
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}