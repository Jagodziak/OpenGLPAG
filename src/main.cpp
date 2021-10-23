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

#include <GLFW/glfw3.h> // musi by� zawsze po gladzie, biblioteka do tworzenia okna, zczytuje ruch myszy, klawisze itd 

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Ustawienie wy�wietlania b��d�w okna
    glfwSetErrorCallback(glfw_error_callback);

    // Inicjalizacja glfw
    if (!glfwInit())
        return 1;

    // Ustawienie minimalnej wspieranej wersji OpenGL (4.3)
    const char* glsl_version = "#version 430";
    // Parametry glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //m�wimy �e b�dziemy korzycta� z kotekstu core (tego nowego, trudniejszego ale szybszego)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Tworzenie okna
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Za*ebiste PAGI z Filipem :)", NULL, NULL);
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

    // Sekcja Danych wpisanych z palca +++++++++++++++++++++++++++++++

    float vertices[] = {
        // front vertices
        // vec3 pos, vec2 uv
         0.5f,  0.5f, 0.5f,  1.0f, 1.0f,  // top right
         0.5f, -0.5f, 0.5f,  1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.5f,  0.0f, 1.0f,   // top left 
        // back vertices
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  // top right
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  // bottom right
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  // bottom left
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f  // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3,    // second triangle
        7, 6, 4,
        6, 5, 4,
        4, 5, 0,
        5, 1, 0,
        7, 6, 3,
        6, 3, 2,
        0, 7, 4,
        3, 7, 0,
        1, 6, 5,
        1, 6, 2
    };

    // Wczytywanie tekstury z pliku za pomoc� biblioteki stb_image
    int width, height, nrChannels;
    unsigned char* data = stbi_load("res/textures/stone.jpg", &width, &height, &nrChannels, 0);

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

    //przekazujemy dane do gpu. GL_UNSIGNED_BYTE jest dlatego �e wy�ej definiujemy unsigned char* data  char to jest bit i git
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Usuwamy tekstur� z ramu po przekazaniu do gpu
    stbi_image_free(data);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);//tworzy vertex array object TO NIE JEST BUFFOR

    glBindVertexArray(VAO);

    // Tworzymy bufor do przechowywania naszych indices (Element Buffer Object - EBO)
    GLuint EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // GLuint to samo co unsigned int
    GLuint VBO; // Vertex buffer object ID variable, Przechowuje wszystkie dane geometrii
    glGenBuffers(1, &VBO); //funkcja tworz�ca bufor na karcie graficznej i zapisuj�ca jego ID w zmiennej VBO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // GL_ARRAY_BUFFER podpina vertex buffer, to taki bufor co przechowuje werteksy

    //przekazujemy dane do wczesniej stworzonego bufora
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //GL_STATIC_DRAW m�wi funkcji �e dane b�d� u�ywane tak �rednio cz�sto i ona sobie wtedy w dobrym miejsu w pami�ci je u�o�y
                        
    // Ustawienie wska�nika atrybutu
    // location = 0 w shaderze odwo�uje si� do indexu
    // 3 m�wi �e potem w shaderze b�dzie to zmienna typu vec3
    // argument pointer (ostatni) m�wi o ile bajt�w musimy si� przesun�� �eby znale�� pierwsz� interesuj�c� nas warto��
    // 3 * sizeof(float) - to jest stride i definiuje przeskok mi�dzy kolejnymi vertexami
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // Zacznij u�ywa� attribute pointera o indexie 0

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // ustawia �aden bufor

    // SZEJDERY! **************************************************

    // ----- VERTEX SHADER -----

    // �adujemy shader z pliku
    std::ifstream shaderFile("res/shaders/basic.vert");
    std::stringstream buffer;
    buffer << shaderFile.rdbuf();

    std::string vertexShaderSource = buffer.str();
    const char* vertShader = vertexShaderSource.c_str();

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); //tworzymy (buffor) vertex shader i zapisujemy jego ID DO ZMIENNEJ vertexShader

    glShaderSource(vertexShader, 1, &vertShader, NULL); //przekazujemy kod do "kontenera" na vertex shader
    glCompileShader(vertexShader); //kompilujemy vertex shader i tu ju� mo�na powiedzie� "Stworzyli�my vertex shader"

    // Sprawdzanie b��d�w kompilacji vertex shadera
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ----- FRAGMENT SHA DER -----

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

    // TWORZYMY SHADER PROGRAM (SPINAMY VERTEX I FRAGMENT SHADER W JEDNO��)

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();

    // Podepnij wcze�niej skompilowane shadery do shaderProgram
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Dokonaj ��czenia shader�w (zlinkuj)
    glLinkProgram(shaderProgram);

    // Usu� niepotrzebne shadery (bo s� ju� cz�ci� shaderProgram)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Zmienne pomocnicze paremetryzuj�ce rendering
    bool wireframe = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glm::mat4 modelMatrix = glm::mat4(1.0f); // Macierz transformacji do ustawienia obiektu w �wiecie, zainicjalizowali�my j� macierz� jednostkw�- przez to 1.0f
    float rotationX = 0.0f;
    float rotationY = 0.0f;

    // ============================================ Main loop =======================================================================

    while (!glfwWindowShouldClose(window)) //sprawdzamy czy zostal klikni�ty X (zamkni�te okno)
    {
        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents(); //konieczna do dzia�ania klawiatury i myszy, generalnie to zbiera input

        // Boilerplate, inicjalizuje imgui i tyle
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Draw wireframe", &wireframe);

            ImGui::SliderFloat("Rotation X", &rotationX, 0.0f, 360.0f);
            ImGui::SliderFloat("Rotation Y", &rotationY, 0.0f, 360.0f);
            // Poni�ej si� dziej� dzikie rzeczy zwi�zane z rzutowaniem wska�nika ImVec4 na float
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            //ImGui::Button("But�");

            //ImGui::SameLine();
            //ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering =============================
        
        // Imgui ui render
        ImGui::Render();

        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h); //zwraca rozmiar okna (dok�adnie to framebufora)

        glViewport(0, 0, display_w, display_h); //przekazanie rozmiaru okna do open gl.

        // U�ywaj bufora g��boko�ci (rozwi�zuje problemy z sortowaniem)
        glEnable(GL_DEPTH_TEST);

        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);   // Ustawia kolor czyszczenia ekranu. 
                                                                                    // Dzia�a to tak, ze za ka�dym grazem gdy w przysz�o�ci wywo�am funkcje 
                                                                                    // glClear to bufor nadpisze sie tym w�a�nie kolorem
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //nadpisuje wszystko kolorem i clearuje zbufor
        
        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Przygotowanie macierzy
        modelMatrix = glm::mat4(1.0f); // Reset macierzy
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));

        // Tu jest prawdziwy Remdering

        int modelLocation = glGetUniformLocation(shaderProgram, "model"); // Funkcja szuka uniforma "model" w shaderProgram
        glUseProgram(shaderProgram);
        // Ustawiamy warto�� uniforma "model" na zawarto�� zmiennej modelMatrix
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix)); // Glm value ptr wyci�ga wska�nik potrzebny dla funkcji glUniform

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); //rysujemy tr�jk�ty u�ywaj�c EBO
        //glDrawArrays(GL_TRIANGLES, 0, 3); // Rysujemy tr�jk�ty zaczynaj�c od pocz�tku i bior�c pod uwag� 3 vertexy (bez EBO)
        glBindVertexArray(0);

        // Tu si� ko�czy Remdering

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