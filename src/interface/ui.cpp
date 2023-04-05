#include "ui.h"


bool UI::LoadImage(Image& imageSrc, GLuint* imageOut, uint16_t* widthOut, uint16_t* heightOut)
{
    // Create texture identifier
    GLuint imageTexture;
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSrc.w, imageSrc.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageSrc.data);

    *imageOut = imageTexture;
    *widthOut = imageSrc.w;
    *heightOut = imageSrc.h;

    return true;
}

void UI::LoadFont()
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

void UI::LoadTheme()
{
    LoadFont();

    UI::style = &ImGui::GetStyle();

    // ROUNDINGS
    style->ChildRounding = 6;
    style->FrameRounding = 5;
    style->GrabRounding = 2;
    style->PopupRounding = 2;
    style->ScrollbarSize = 9;
    style->FramePadding = ImVec2(6, 3);
    style->ItemSpacing = ImVec2(4, 4);

    ImVec4 primaryColor(0.525f, 0.753f, 1.00f, 0.8f);
    ImVec4 primaryColorHover(0.525f, 0.753f, 1.00f, 0.8f - 0.1f);
    ImVec4 primaryColorHoverBG(0.525f, 0.753f, 1.00f, 0.8f - 0.65f);

    style->Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
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

void UI::Render()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    static int headSize = 1.0f;
    static bool headOverlay = true;
    static GLuint generatedSkinTexture = 0;
    static uint16_t generatedSkinTextureWidth = 0;
    static uint16_t generatedSkinTextureHeight = 0;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);

    bool show_window = true;
    ImGui::Begin("---", &show_window, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

    ImGui::PushFont(bigFont);
    ImGui::Text("Welcome to the skinifier!");
    ImGui::PopFont();

    ImGui::SliderInt("Head size", &headSize, 1, 3);
    ImGui::Checkbox("Head overlay", &headOverlay);

    if (ImGui::Button("Click here to skinify"))
    {
        Skinify skin("test.png");

        // update skin settings
        skin.skin_headScaleMultiplier = headSize;
        skin.skin_headOverlay = headOverlay;

        Image generatedSkin(1, 1, 3);
        generatedSkin = skin.generate();

        bool imageStatus = LoadImage(generatedSkin, &generatedSkinTexture, &generatedSkinTextureWidth, &generatedSkinTextureHeight);
        IM_ASSERT(imageStatus);
    }

    if (generatedSkinTexture != 0)
    {
        ImGui::Image((void*)(intptr_t)generatedSkinTexture, ImVec2(generatedSkinTextureWidth, generatedSkinTextureHeight));
    }

    ImGui::End();
}
