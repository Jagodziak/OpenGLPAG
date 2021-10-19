#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h" // biblioteka do renderowania prostych interfejsów graficznych
#include <stdio.h>

#include <glad/glad.h>  // loader funkcji opengl

#include <GLFW/glfw3.h> // musi byæ zawsze po gladzie, biblioteka do tworzenia okna, zczytuje ruch myszy, klawisze itd 

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
            // Zmienne pomocnicze dla imgui
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            // Poni¿ej siê dziej¹ dzikie rzeczy zwi¹zane z rzutowaniem wskaŸnika ImVec4 na float
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            ImGui::Button("But¹");

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
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