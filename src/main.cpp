#include <iostream>
#include <fstream>
#include <sstream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h" // biblioteka do renderowania prostych interfejs�w graficznych
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>  // loader funkcji opengl

#include <GLFW/glfw3.h> //biblioteka do tworzenia okna, zczytuje ruch myszy, klawisze itd 

const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 800;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void drawMengerCube(GLuint shaderProgram, int maxIterations, glm::mat4& model, glm::vec3 cubePositions[], int cubePositionsCount, int currentIteration = 0);

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


    float vertices[] = { 
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f, //front
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,

         0.5f, -0.5f, -0.5f,    0.0f, 0.0f, //back
        -0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,    0.0f, 0.0f, //top
        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f, //bottom
        -0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,    0.0f, 0.0f, //left
        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,

         0.5f,  0.5f, -0.5f,    0.0f, 0.0f, //right
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
    };

    unsigned int indices[] = {  
        0,1,3, 
        1,2,3, 
        4,5,7, 
        5,6,7,
        8,9,11,
        9,10,11,
        12,13,15,
        13,14,15,
        16,17,19,
        17,18,19,
        20,21,23,
        21,22,23
    };

    // Wczytywanie tekstury z pliku za pomoc� biblioteki stb_image
    int width, height, nrChannels;
    unsigned char* data = stbi_load("res/textures/Opalka.jpg", &width, &height, &nrChannels, 0);

    // Przekazujemy tekstur� do karty graficznej
    GLuint texture; 
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    // Ustawienie filtrowania
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Ustawienie powtarzania
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //przekazujemy dane do gpu
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Usuwamy tekstur� z ramu
    stbi_image_free(data);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    GLuint EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GLuint VBO; // Vertex buffer object ID variable, Przechowuje wszystkie dane geometrii
    glGenBuffers(1, &VBO); //funkcja tworz�ca bufor na karcie graficznej i zapisuj�ca jego ID w zmiennej VBO

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //przekazujemy dane do wczesniej stworzonego bufora
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
                        
    // Ustawienie wska�nika atrybutu
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // aktywowanie attribute pointera o indexie 0

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // ----- VERTEX SHADER -----

    // �adujemy shader z pliku
    std::ifstream shaderFile("res/shaders/basic.vert");
    std::stringstream buffer;
    buffer << shaderFile.rdbuf();

    std::string vertexShaderSource = buffer.str();
    const char* vertShader = vertexShaderSource.c_str();

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertShader, NULL); 
    glCompileShader(vertexShader); 

    // Sprawdzanie b��d�w kompilacji vertex shadera
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ----- FRAGMENT SHADER -----

    std::ifstream shaderFile2("res/shaders/basic.frag");
    std::stringstream buffer2;
    buffer2 << shaderFile2.rdbuf();

    std::string fragmentShaderSource = buffer2.str();
    const char* fragShader = fragmentShaderSource.c_str();

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragShader, NULL);
    glCompileShader(fragmentShader);

    // Sprawdzanie b��d�w kompilacji fragment shadera
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //SHADER PROGRAM 

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Zmienne pomocnicze paremetryzuj�ce rendering
    bool wireframe = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 color = ImVec4(1.0f, 1.0f, 1.00f, 1.00f);
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    int iterations = 1;
    // Przechowuje pozycje sze�cian�w dla renderowania kostki mengera
    glm::vec3 cubePositions[] = {
        // bottom
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 1.0f),
        glm::vec3(0.0f, -1.0f, 1.0f),
        glm::vec3(-1.0f, -1.0f, 1.0f),
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3(0.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
        // mid
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(-1.0f, 0.0f, 1.0f),
        glm::vec3(-1.0f, 0.0f, -1.0f),
        glm::vec3(1.0f, 0.0f, -1.0f),
        // top
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 1.0f),
        glm::vec3(-1.0f, 1.0f, 1.0f),
        glm::vec3(-1.0f, 1.0f, 0.0f),
        glm::vec3(-1.0f, 1.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, -1.0f),
        glm::vec3(1.0f, 1.0f, -1.0f),
    };

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

            ImGui::SliderFloat("Rotation X", &rotationX, 0.0f, 360.0f);
            ImGui::SliderFloat("Rotation Y", &rotationY, 0.0f, 360.0f);
            ImGui::SliderInt("Iterations", &iterations, 0, 5);
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

        // Przygotowanie macierzy
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));


        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);

        glUseProgram(shaderProgram);

        int colorLocation = glGetUniformLocation(shaderProgram, "color");
        glUniform4f(colorLocation, color.x, color.y, color.z, color.w);

        drawMengerCube(shaderProgram, iterations, modelMatrix, cubePositions, 20);

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

void drawMengerCube(GLuint shaderProgram, int maxIterations, glm::mat4& model, glm::vec3 cubePositions[], int cubePositionsCount, int currentIteration)
{
    if (currentIteration == maxIterations)
    {
        int modelLocation = glGetUniformLocation(shaderProgram, "model"); 
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model)); 
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    else
    {
        for (int i = 0; i < cubePositionsCount; i++)
        {
            glm::mat4 tempModel = glm::mat4(1.0f);
            tempModel = glm::scale(tempModel, glm::vec3(1.0f / 3.0f));
            tempModel = glm::translate(tempModel, cubePositions[i]);
            tempModel = model * tempModel;
            drawMengerCube(shaderProgram, maxIterations, tempModel, cubePositions, cubePositionsCount, currentIteration + 1);
        }
    }
}