#include "ImGui.hpp"

#include "stb_image.h"


namespace Interface
{
    void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }

    bool Render()
    {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return false;

        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        // Create window with graphics context
        int windowWidth{ 300 };
        GLFWwindow* window = glfwCreateWindow(windowWidth, 665, "Skinify", NULL, NULL);
        if (window == NULL)
            return false;

        // Set window icon
        GLFWimage images[1];
        images[0].pixels = stbi_load("resources/images/icon.png", &images[0].width, &images[0].height, 0, 4);
        glfwSetWindowIcon(window, 1, images);
        stbi_image_free(images[0].pixels);

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);


        // Load banner image
        bannerImage = nullptr;
        bannerImage = new Image("./resources/images/banner.png");

        bannerImage->resize(windowWidth, (bannerImage->_h / (bannerImage->_w / (windowWidth))));


        UI::LoadTheme();

        // Initialize GLEW
        if (glewInit() != GLEW_OK) {
            glfwTerminate();
            return false;
        }

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            UI::Render(window);

            // Rendering
            ImGui::Render();
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

        return true;
    }

    Image* getBannerImage()
    {
        return bannerImage;
    }
}
