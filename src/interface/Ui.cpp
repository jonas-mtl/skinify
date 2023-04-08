#include "ImGui.hpp"
#include "Ui.hpp"


namespace UI
{
    void LoadFont()
    {
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        io.Fonts->Clear();

        if (std::filesystem::exists("./resources/fonts/Roboto-Medium.ttf"))
        {
            ImFontConfig regularFont;
            regularFont.FontDataOwnedByAtlas = false;
            io.Fonts->AddFontFromFileTTF("./resources/fonts/Roboto-Medium.ttf", 16.0f, &regularFont);

            ImFontConfig bigFont;
            bigFont.FontDataOwnedByAtlas = false;
            UI::bigFont = io.Fonts->AddFontFromFileTTF("./resources/fonts/Roboto-Medium.ttf", 24.0f, &bigFont);
        }
        else
        {
            printf("Required font was not found!");
            io.Fonts->AddFontDefault();
        }
    }

    void LoadTheme()
    {
        LoadFont();

        style = &ImGui::GetStyle();

        style->ChildRounding = 6;
        style->FrameRounding = 5;

        ImVec4 primaryColor(157.0f / 255.0f, 126.0f / 255.0f, 81.0f / 255.0f, 1.0f);
        ImVec4 primaryColorHover(185.0f / 255.0f, 149.0f / 255.0f, 96.0f / 255.0f, 1.0f);
        ImVec4 primaryColorHoverBG(185.0f / 255.0f, 149.0f / 255.0f, 96.0f / 255.0f, 0.15f);

        style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        style->Colors[ImGuiCol_WindowBg] = ImVec4(0.059f, 0.059f, 0.059f, 1.00f);
        style->Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
        style->Colors[ImGuiCol_FrameBgHovered] = primaryColorHoverBG;
        style->Colors[ImGuiCol_FrameBgActive] = primaryColorHoverBG;
        style->Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
        style->Colors[ImGuiCol_SliderGrab] = primaryColorHover;
        style->Colors[ImGuiCol_SliderGrabActive] = primaryColor;
        style->Colors[ImGuiCol_Button] = primaryColor;
        style->Colors[ImGuiCol_ButtonHovered] = primaryColorHover;
        style->Colors[ImGuiCol_ButtonActive] = primaryColor;
    }

    bool LoadImage(Image* imageSrc, GLuint* imageOut, uint16_t* widthOut, uint16_t* heightOut)
    {
        // Create texture identifier
        GLuint imageTexture{};
        glGenTextures(1, &imageTexture);
        glBindTexture(GL_TEXTURE_2D, imageTexture);

        // Filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Turn image data into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSrc->_w, imageSrc->_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageSrc->p_data);

        *imageOut = imageTexture;
        *widthOut = imageSrc->_w;
        *heightOut = imageSrc->_h;

        return true;
    }

    void Render(GLFWwindow* window)
    {
        using ImGui::GetCursorPosY;

        static int headSize{ 1 };
        static float shadowOpacity{ 0.7f };
        static float lightIntensity{ 0.5f };
        static int shadowRadius{ 4 };
        static bool headOverlay{ true };

        static GLuint generatedSkinTexture{};
        static uint16_t generatedSkinTextureWidth{};
        static uint16_t generatedSkinTextureHeight{};

        static GLuint bannerTexture{};
        static uint16_t bannerTextureWidth{};
        static uint16_t bannerTextureHeight{};

        static bool startUp{ true };
        static bool imgLoaded{ false };
        static bool outputCheck{ false };

        static char srcPath[50]{};
        static char srcPathBuffer[50]{};
        static char outPath[50]{ "output.png" };

        ImGuiIO& io = ImGui::GetIO();

        int glfwWinWidth{};
        glfwGetWindowSize(window, &glfwWinWidth, nullptr);

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);

        bool show_window = true;
        ImGui::Begin("##mainWnd", &show_window, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

        if (startUp) LoadImage(Interface::getBannerImage(), &bannerTexture, &bannerTextureWidth, &bannerTextureHeight);
        ImGui::Image((void*)(intptr_t)bannerTexture, ImVec2(bannerTextureWidth, bannerTextureHeight));

        size_t globalLeftMargin{ 23 };

        ImGui::TextCustom("Skin file path:", globalLeftMargin);
        ImGui::SetCursorPosX(globalLeftMargin);
        ImGui::InputText("##srcPath", srcPathBuffer, 50);

        if (strcmp(srcPathBuffer, srcPath) != 0)
        {
            imgLoaded = Skinify::load(srcPathBuffer);
            strcpy(srcPath, srcPathBuffer);
        }

        ImGui::Spacing();

        ImGui::TextCustom("Output file path:", globalLeftMargin);
        ImGui::SetCursorPosX(globalLeftMargin);
        ImGui::InputText("##outPath", outPath, 50);

        outputCheck = std::string(outPath).substr(strlen(outPath) - 4) == ".png";

        ImGui::Spacing();


        // grid for sliders
        ImGui::Columns(2, "sliderColumns", false);
        ImGui::SetColumnWidth(0, (float)glfwWinWidth / 2);

        ImGui::TextCustom("Head Size", globalLeftMargin);
        ImGui::SliderIntCustom("##head", &headSize, 1, 4, globalLeftMargin);

        ImGui::Spacing();
        ImGui::TextCustom("Light Intensity", globalLeftMargin);
        ImGui::SliderFloatCustom("##lightIntensity", &lightIntensity, 0, 1, globalLeftMargin);

        ImGui::NextColumn();
        ImGui::TextCustom("Shadow Opacity", (float)(glfwWinWidth / 2) + globalLeftMargin);
        ImGui::SliderFloatCustom("##sideShadow", &shadowOpacity, 0, 1, (float)(glfwWinWidth / 2) + globalLeftMargin);

        ImGui::Spacing();
        ImGui::TextCustom("Shadow Radius", (float)(glfwWinWidth / 2) + globalLeftMargin);
        ImGui::SliderIntCustom("##shadowRadius", &shadowRadius, 1, 6, (float)(glfwWinWidth / 2) + globalLeftMargin);

        ImGui::Columns(1);

        ImGui::Dummy(ImVec2(5, 5));
        ImGui::SetCursorPosX(globalLeftMargin);
        ImGui::Checkbox("Head overlay ", &headOverlay);

        ImGui::Dummy(ImVec2(20, 20));
        ImGui::SetCursorPosX((glfwWinWidth - ImGui::CalcTextSize(" - Click here to skinify - ").x) / 2 - 9);

        
        if (!imgLoaded || !outputCheck) ImGui::BeginDisabled();

        // generate skinified image
        if (ImGui::Button(" - Click here to Skinify - "))
        {
            Skinify::generate(headSize, shadowOpacity, lightIntensity, shadowRadius, headOverlay, outPath);

            Image& canvasImage = *Skinify::canvas;
            bool imageStatus = LoadImage(&canvasImage, &generatedSkinTexture, &generatedSkinTextureWidth, &generatedSkinTextureHeight);
            IM_ASSERT(imageStatus);
            strcpy(srcPath, "-");

        }
        if (!imgLoaded || !outputCheck) ImGui::EndDisabled();

        // display generated image
        if (generatedSkinTexture != 0)
        {
            ImGui::SetCursorPosX((float)(glfwWinWidth - generatedSkinTextureWidth) / 2 + 8);
            ImGui::Image((void*)(intptr_t)generatedSkinTexture, ImVec2(generatedSkinTextureWidth, generatedSkinTextureHeight));
        }

        startUp = false;
        ImGui::End();
    }
}
