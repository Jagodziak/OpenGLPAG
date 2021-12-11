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

float lastX = WINDOW_WIDTH / 2.0f, lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
bool wasEscapePressedLastFrame = false;
bool cursorEnabled = false;

float cameraSpeed = 12.0f;
float cameraMultiplier = 4.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraPitch = 0.0f;
float cameraYaw = 0.0f;
float cameraRoll = 0.0f;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void processInput(GLFWwindow* window) //WYWO£YWANY CO KLATKÊ, zczytuje imput z klawiatrury i myszy
{
    int escape = glfwGetKey(window, GLFW_KEY_ESCAPE); //glfwgetkey przekazuje stan klawisza
    if (escape == GLFW_PRESS)
    {
        if (!wasEscapePressedLastFrame)
        {
            cursorEnabled = !cursorEnabled;
            glfwSetInputMode(window, GLFW_CURSOR, cursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        }
        wasEscapePressedLastFrame = true;
    }
    else if (escape == GLFW_RELEASE)
    {
        wasEscapePressedLastFrame = false; 
    }

    float deltaSpeed = cameraSpeed * deltaTime; // adjust accordingly, delta speed to camera speed ustawiony przez nas (dostosowuje w zaleznosci od trwania klatki)


    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        deltaSpeed *= cameraMultiplier;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += deltaSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= deltaSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaSpeed; //iloczyn wektorowy, normalize zeby wektor mia³ dlugoœæ 1
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos += deltaSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos -= deltaSpeed * cameraUp;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    if (cursorEnabled)
    {
        lastX = xpos;
        lastY = ypos;
    }

    if (!cursorEnabled)
    {
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
        lastX = xpos;
        lastY = ypos;

        const float sensitivity = 0.1f; //czu³oœæ kamery 
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        cameraYaw += xoffset;
        cameraPitch += yoffset;

        if (cameraPitch > 89.0f) //zeby nie dalo sie zrobiæ salta 
            cameraPitch = 89.0f;
        if (cameraPitch < -89.0f)
            cameraPitch = -89.0f;

        //matma, robi wektor który patrzy w przód kamery 
        glm::vec3 direction;
        direction.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        direction.y = sin(glm::radians(cameraPitch));
        direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        cameraFront = glm::normalize(direction);
    }
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
   
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Zadanie 3 - Domki", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window); 
    glfwSwapInterval(1); 
    glfwSetCursorPosCallback(window, mouse_callback); //ustawia ze mouse_callback bêdzie sie wywo³ywaæ gdy poruszasz mysz¹
    
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

    Shader basicShader("res/shaders/basic.vert", "res/shaders/basic.frag"); //
    Shader phongShader("res/shaders/phong.vert", "res/shaders/phong.frag");

    std::vector<Model*> sceneObjects;

    Model skybox("res/models/skybox.fbx");
    skybox.texture.load("res/textures/spiaggia_di_mondello_4k.png");
    skybox.modelTransform.rotate(glm::vec3(glm::radians(180.0f), 0.0f, 0.0f));
    skybox.modelTransform.scale(glm::vec3(12.0f, 12.0f, 12.0f));

    Transform sceneRoot;

    Model ground("res/models/ground.fbx");
    sceneObjects.push_back(&ground);
    ground.texture.load("res/textures/sand_01_diff_2k.png");
    ground.modelTransform.rotate(glm::vec3(glm::radians(180.0f), 0.0f, 0.0f));
    sceneRoot.addChild(&ground.modelTransform);

    Transform houses;
    sceneRoot.addChild(&houses);

    const int gridX = 200, gridY = 200;
    const float gridIncrement = 150.0f; // odstêp miêdzy domkami 
    std::vector<Transform> houseOffsets; //wektor w którym s¹ zapisane pozycje wszystkich domków
    std::vector<Transform> roofOffsets;
    houseOffsets.reserve(gridX * gridY); // rezerwujemy miejsce w wektorze na pozycje domków 
    roofOffsets.reserve(gridX * gridY);

    float yOffset = gridY * -0.5f * gridIncrement; //wpisuje pozycje domków do house offset
    for (int i = 0; i < gridY; i++)
    {
        float xOffset = gridX * -0.5f * gridIncrement;
        for (int j = 0; j < gridX; j++)
        {
            Transform houseTransform;
            houseTransform.move(glm::vec3(xOffset, 0.0f, yOffset));
            houseOffsets.push_back(houseTransform);
            sceneRoot.addChild(&houseOffsets.back());
            
            Transform roofTransform;
            roofOffsets.push_back(roofTransform);
            houseTransform.addChild(&roofOffsets.back());
            
            xOffset += gridIncrement;
        }
        yOffset += gridIncrement;
    }
    
    Model base("res/models/house_base.fbx", false, &houseOffsets);
    sceneObjects.push_back(&base);
    base.texture.load("res/textures/blue_painted_planks_diff_2k.png");
    houses.addChild(&base.modelTransform);

    Model roof("res/models/house_roof.fbx", false, &roofOffsets);
    sceneObjects.push_back(&roof);
    roof.texture.load("res/textures/reed_roof_03_diff_2k.png");
    base.modelTransform.addChild(&roof.modelTransform);


    bool wireframe = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glm::mat4 modelMatrix, viewMatrix, projectionMatrix;

    // Lights
    bool ambientEnabled = true;
    glm::vec3 ambient = glm::vec3(0.3f, 0.3f, 0.3f);

    bool directionalEnabled = true;
    glm::vec3 directionalLight0Color = glm::vec3(0.45f, 0.45f, 0.45f);
    glm::vec3 directionalLight0Direction = glm::vec3(-1.0f, -1.0f, -1.0f);
    Model directionalLight0Gizmo("res/models/arrow.fbx");
    directionalLight0Gizmo.texture.load("res/textures/white.jpg");

    bool pointEnabled = true;
    glm::vec3 pointLight0Color = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 pointLight0Position = glm::vec3(1.0f, 75.0f, 1.0f);
    Model pointLight0Gizmo("res/models/planet.fbx");
    pointLight0Gizmo.texture.load("res/textures/white.jpg");

    bool spot0Enabled = true;
    glm::vec3 spotLight0Color = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 spotLight0Position = glm::vec3(-120.0f, 20.0f, 0.0f);
    glm::vec3 spotLight0Direction = glm::vec3(1.0f, 0.0f, 0.0f);
    float spotLight0Cutoff = 30.0f;
    Model spotLight0Gizmo("res/models/cone.fbx");
    spotLight0Gizmo.texture.load("res/textures/white.jpg");

    bool spot1Enabled = true;
    glm::vec3 spotLight1Color = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 spotLight1Position = glm::vec3(120.0f, 20.0f, 0.0f);
    glm::vec3 spotLight1Direction = glm::vec3(-1.0f, 0.0f, 0.0f);
    float spotLight1Cutoff = 30.0f;
    Model spotLight1Gizmo("res/models/cone.fbx");
    spotLight1Gizmo.texture.load("res/textures/white.jpg");

    glm::vec3 housePosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 housePositionLastFrame = housePosition;
    glm::vec3 roofPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 roofPositionLastFrame = roofPosition;

    glfwSetInputMode(window, GLFW_CURSOR, cursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED); //ma pocz¹tek ustawia czy krsor jest zablokowany czy nie

    //
    // ============================================ Main loop =======================================================================
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame; ///
        lastFrame = currentFrame;

        glfwPollEvents(); //zbiera input,cdzieki temu moge korzystaæ z glfw getkey
        processInput(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Okienko");
            ImGui::Checkbox("Draw wireframe", &wireframe);
            ImGui::ColorEdit3("Clear color", (float*)&clear_color); 

            ImGui::Text("House Controls");
            ImGui::DragFloat3("House 1 pos", glm::value_ptr(housePosition));
            ImGui::DragFloat3("Roof 1 pos", glm::value_ptr(roofPosition));

            ImGui::Text("Light Controls");
            ImGui::Checkbox("Ambient enabled", &ambientEnabled);
            ImGui::ColorEdit3("Ambient color", glm::value_ptr(ambient));

            ImGui::Checkbox("Directional enabled", &directionalEnabled);
            ImGui::ColorEdit3("Directional light 0 color", glm::value_ptr(directionalLight0Color));
            ImGui::DragFloat3("Directional light 0 pos", glm::value_ptr(directionalLight0Direction));

            ImGui::Checkbox("Point enabled", &pointEnabled);
            ImGui::ColorEdit3("Point light 0 color", glm::value_ptr(pointLight0Color));
            ImGui::DragFloat3("Point light 0 pos", glm::value_ptr(pointLight0Position));

            ImGui::Checkbox("Spot light 0 enabled", &spot0Enabled);
            ImGui::ColorEdit3("Spot light 0 color", glm::value_ptr(spotLight0Color));
            ImGui::DragFloat3("Spot light 0 pos", glm::value_ptr(spotLight0Position));
            ImGui::DragFloat3("Spot light 0 dir", glm::value_ptr(spotLight0Direction));
            ImGui::DragFloat("Spot light 0 cutoff", &spotLight0Cutoff);

            ImGui::Checkbox("Spot light 1 enabled", &spot1Enabled);
            ImGui::ColorEdit3("Spot light 1 color", glm::value_ptr(spotLight1Color));
            ImGui::DragFloat3("Spot light 1 pos", glm::value_ptr(spotLight1Position));
            ImGui::DragFloat3("Spot light 1 dir", glm::value_ptr(spotLight1Direction));
            ImGui::DragFloat("Spot light 1 cutoff", &spotLight1Cutoff);

            ImGui::Text("Camera Controls");
            ImGui::SliderFloat("Speed", &cameraSpeed, 0.1f, 25.0f);
            ImGui::SliderFloat("Multiplier", &cameraMultiplier, 0.1f, 10.0f);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        glEnable(GL_DEPTH_TEST);

        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        glEnable(GL_CULL_FACE); // wlacza opcje ni¿ej
        glCullFace(GL_BACK); //nie renderujemy ty³ów œcianek 

        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projectionMatrix = glm::perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.01f, 15000.0f);

        // House translation logic
        if (housePosition != housePositionLastFrame || roofPosition != roofPositionLastFrame)
        {
            roofOffsets[(gridY * gridX) / 2].reset();
            roofOffsets[(gridY * gridX) / 2].move(glm::vec3(roofPosition));

            houseOffsets[(gridY * gridX) / 2].reset();
            houseOffsets[(gridY * gridX) / 2].move(glm::vec3(housePosition));
            
            sceneRoot.recalculate();
            base.updateInstanceMatrices();
            roof.updateInstanceMatrices();
        }

        housePositionLastFrame = housePosition;
        roofPositionLastFrame = roofPosition;

        // Setting ambient lighting, skybox reaguje na kolor ambientu i kierunkowego
        basicShader.use();
        glm::vec3 skyboxColor = glm::vec3(0.0f);
        if (ambientEnabled)
            skyboxColor += ambient;
        if (directionalEnabled)
            skyboxColor += directionalLight0Color;
        basicShader.setVec3("ambient", skyboxColor);
        basicShader.setMat4("view", viewMatrix);
        basicShader.setMat4("projection", projectionMatrix);
        skybox.draw(basicShader);

        //basic shaderem renderujemy skybox i wizualizacje œwiate³ 

        pointLight0Position.x = glm::sin(lastFrame) * 65.0f; ///////////////////// orbitowanie punktowego œwiat³a wokó³ sceny
        pointLight0Position.z = glm::cos(lastFrame) * 65.0f;//////////////////////////////////////
        pointLight0Gizmo.modelTransform.reset(); // renderowanie wuzualizacji œwiat³a -GIZMO
        pointLight0Gizmo.modelTransform.move(pointLight0Position);
        pointLight0Gizmo.modelTransform.scale(glm::vec3(0.25f));
        basicShader.setVec3("ambient", pointLight0Color); //ustawia kolor kulki. uniform o nazwie ambient jest myl¹cy pozdrawiam 
        pointLight0Gizmo.draw(basicShader);

        //aby gizmo patrzy³o siê w stronê w któr¹ œwieci œwiat³o.
        //lookat zwraca macierz transformacji. przekazujesz mu pozycje gdzie ma sie coœ znajdowaæ i punkt na który ma patrzeæ a on robi z tego macierz
        //do pozycji œwiat³a dodajemy kierunek œwiat³a i to nam daje punkt 
        spotLight0Gizmo.modelTransform.worldTransform = glm::inverse(glm::lookAt(spotLight0Position, spotLight0Position + glm::normalize(spotLight0Direction), glm::vec3(0.0f, 1.0f, 0.0f)));
        basicShader.setVec3("ambient", spotLight0Color);
        spotLight0Gizmo.draw(basicShader);

        spotLight1Gizmo.modelTransform.worldTransform = glm::inverse(glm::lookAt(spotLight1Position, spotLight1Position + glm::normalize(spotLight1Direction), glm::vec3(0.0f, 1.0f, 0.0f)));
        basicShader.setVec3("ambient", spotLight1Color);
        spotLight1Gizmo.draw(basicShader);

        //directional nie ma po³o¿enia wiêc jest wpisane arbitralne
        directionalLight0Gizmo.modelTransform.worldTransform = glm::inverse(glm::lookAt(glm::vec3(0.0f, 80.0f, 0.0f), glm::vec3(0.0f, 80.0f, 0.0f) + glm::normalize(directionalLight0Direction), glm::vec3(0.0f, 1.0f, 0.0f)));
        basicShader.setVec3("ambient", directionalLight0Color);
        directionalLight0Gizmo.draw(basicShader);

        //
        phongShader.use();
        // Ambient
        if (ambientEnabled)
            phongShader.setVec3("ambient", ambient); //jak œwiatlo jest wlaczone to przekazuje kolor
        else
            phongShader.setVec3("ambient", glm::vec3(0.0f)); //jak nie to zwracam czarny 

        // Directional
        phongShader.setVec3("directionalLight0Dir", directionalLight0Direction); //przekazuje kierunek directionala
        if (directionalEnabled)
            phongShader.setVec3("directionalLight0Color", directionalLight0Color);
        else
            phongShader.setVec3("directionalLight0Color", glm::vec3(0.0f));

        // Point light
        phongShader.setVec3("pointLight0Pos", pointLight0Position);
        if (pointEnabled)
            phongShader.setVec3("pointLight0Color", pointLight0Color);
        else
            phongShader.setVec3("pointLight0Color", glm::vec3(0.0f));

        // Spot lights
        phongShader.setVec3("spotLight0Pos", spotLight0Position);
        if (spot0Enabled)
            phongShader.setVec3("spotLight0Color", spotLight0Color);
        else
            phongShader.setVec3("spotLight0Color", glm::vec3(0.0f));
        phongShader.setVec3("spotLight0Dir", spotLight0Direction);
        phongShader.setFloat("spotLight0Cutoff", glm::cos(glm::radians(spotLight0Cutoff)));

        phongShader.setVec3("spotLight1Pos", spotLight1Position);
        if (spot1Enabled)
            phongShader.setVec3("spotLight1Color", spotLight1Color);
        else
            phongShader.setVec3("spotLight1Color", glm::vec3(0.0f));
        phongShader.setVec3("spotLight1Dir", spotLight1Direction);
        phongShader.setFloat("spotLight1Cutoff", glm::cos(glm::radians(spotLight1Cutoff)));

        phongShader.setVec3("viewPos", cameraPos);
        phongShader.setMat4("view", viewMatrix);
        phongShader.setMat4("projection", projectionMatrix);

        for (int i = 0; i < sceneObjects.size(); i++)
        {
            sceneObjects[i]->draw(phongShader);
        }
        //

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