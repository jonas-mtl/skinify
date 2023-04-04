#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "stb_image_write.h"
#include "skinify.h"
#include "core/image.h"


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Skinify", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    if (std::filesystem::exists("./res/fonts/Roboto-Medium.ttf"))
    {
        io.Fonts->AddFontFromFileTTF("./res/fonts/Roboto-Medium.ttf", 16.0f);
    }
    else
    {
        io.Fonts->AddFontDefault();
    }

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        // Error handling
        glfwTerminate();
        return -1;
    }


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        {
            static int headSize = 0.0f;
            static int counter = 0;

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(io.DisplaySize);

            bool show_window = true;
            ImGui::Begin("---", &show_window, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Welcome to the skinifier!");               

            ImGui::SliderInt("Head size", &headSize, 1, 3);

            if (ImGui::Button("Click here to skinify"))
            {
                Skinify skin("test.png");
                Image bg("back.png");
                skin.skin_headScaleMultiplier = headSize;
                Image skinImg = skin.generate();
                bg.resize(skinImg.w, skinImg.h);
                bg.overlay(skinImg, 0, 0);


                bg.write("output.png");
                skinImg.destruct = true;
            }
            
            ImGui::End();
        }


        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
