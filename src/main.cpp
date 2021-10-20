#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h" // biblioteka do renderowania prostych interfejsów graficznych
#include <stdio.h>

#include <glad/glad.h>  // loader funkcji opengl

#include <GLFW/glfw3.h> // musi byæ zawsze po gladzie, biblioteka do tworzenia okna, zczytuje ruch myszy, klawisze itd 

#include <iostream>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Ustawienie wyœwietlania b³êdów okna
    glfwSetErrorCallback(glfw_error_callback);

    // Inicjalizacja glfw
    if (!glfwInit())
        return 1;

    // Ustawienie minimalnej wspieranej wersji OpenGL (4.3)
    const char* glsl_version = "#version 430";
    // Parametry glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //mówimy ¿e bêdziemy korzyctaæ z kotekstu core (tego nowego, trudniejszego ale szybszego)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Tworzenie okna
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Za*ebiste PAGI z Filipem :)", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync- synchronizacja pionowa- blokuje fps ¿eby zsynchronizowaæ go z czêstotliwoœci¹ odœwie¿ania monitora

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
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // GLuint to samo co unsigned int
    GLuint VBO; // Vertex buffer object ID variable, Przechowuje wszystkie dane geometrii
    glGenBuffers(1, &VBO); //funkcja tworz¹ca bufor na karcie graficznej i zapisuj¹ca jego ID w zmiennej VBO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // GL_ARRAY_BUFFER podpina vertex buffer, to taki bufor co przechowuje werteksy

    //przekazujemy dane do wczesniej stworzonego bufora
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //GL_STATIC_DRAW mówi funkcji ¿e dane bêd¹ u¿ywane tak œrednio czêsto i ona sobie wtedy w dobrym miejsu w pamiêci je u³o¿y
                           
    glBindBuffer(GL_ARRAY_BUFFER, 0); // ustawia ¿aden bufor

    // SZEJDERY! **************************************************

    // ----- VERTEX SHADER -----

    //#version 330 core // powiedzenie z jakiej wersji opengla korzystamy. najni¿sza targetowana wersja
    //layout(location = 0) in vec3 aPos; // [layout(location = 0)]- zaczynaj¹c od pocz¹tku, bex przesuniêcia w buforze
    //                                   // [in]-wczytaj [vec3]-trzy floaty [aPos]-do zmiennej o nazwie aPos i typie Vec3
    //
    //void main()
    //{
    //    // gl_Position- zmienna do której MUSISZ wpisaæ pozycje vertexa. Za pomoc¹ tej zmiennej s¹ przekazywane dane do fragment shadera (rysunek)
    //    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    //}
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); //tworzymy (buffor) vertex shader i zapisujemy jego ID DO ZMIENNEJ vertexShader

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //przekazujemy kod do "kontenera" na vertex shader
    glCompileShader(vertexShader); //kompilujemy vertex shader i tu ju¿ mo¿na powiedzieæ "Stworzyliœmy vertex shader"

    // Sprawdzanie b³êdów kompilacji vertex shadera
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ----- FRAGMENT SHADER -----

    //#version 330 core // To samo co w vertex shaderze
    //out vec4 FragColor; // Kolor piksela który zostanie wpisany na ekran
    //
    //void main()
    //{
    //    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // Wpisujemy kolor do zmiennej FragColor (rgba)
    //}
    const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Sprawdzanie b³êdów kompilacji fragment shadera
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // TWORZYMY SHADER PROGRAM (SPINAMY VERTEX I FRAGMENT SHADER W JEDNOŒÆ)

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();

    // Podepnij wczeœniej skompilowane shadery do shaderProgram
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Dokonaj ³¹czenia shaderów (zlinkuj)
    glLinkProgram(shaderProgram);

    // Usuñ niepotrzebne shadery (bo s¹ ju¿ czêœci¹ shaderProgram)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Zmienne pomocnicze paremetryzuj¹ce rendering
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // ============================================ Main loop =======================================================================

    while (!glfwWindowShouldClose(window)) //sprawdzamy czy zostal klikniêty X (zamkniête okno)
    {
        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents(); //konieczna do dzia³ania klawiatury i myszy, generalnie to zbiera input

        // Boilerplate, inicjalizuje imgui i tyle
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Another Window", &show_another_window);

            //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            // Poni¿ej siê dziej¹ dzikie rzeczy zwi¹zane z rzutowaniem wskaŸnika ImVec4 na float
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            //ImGui::Button("But¹");

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
        glfwGetFramebufferSize(window, &display_w, &display_h); //zwraca rozmiar okna (dok³adnie to framebufora)

        glViewport(0, 0, display_w, display_h); //przekazanie rozmiaru okna do open gl.

        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);   // Ustawia kolor czyszczenia ekranu. 
                                                                                    // Dzia³a to tak, ze za ka¿dym grazem gdy w przysz³oœci wywo³am funkcje 
                                                                                    // glClear to bufor nadpisze sie tym w³aœnie kolorem
        glClear(GL_COLOR_BUFFER_BIT); //nadpisuje wszystko kolorem

        // Tu jest prawdziwy Remdering



        // Tu siê koñczy Remdering

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window); //podwójne buforowanie
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}