#pragma once 

#include <iostream>
#include <stdint.h>

#include "Image.hpp"

#define CENTER(canvas, bodyPart) (canvas->_w / 2) - (bodyPart._w / 2)


namespace Skinify 
{
    extern Image* canvas;
    static std::string canvas_srcImagePath{};
    static uint8_t canvas_yOffset{ 2 };
    static uint8_t canvas_upscaleMultiplier{ 4 };

    static float skin_upperShadowOpacityMultiplier{ 1.25f };
    static float skin_shadowOpacity{ 0.5f };


    void init(const char* srcImagePath);

    void generate(uint8_t skin_headScaleMultiplier, float skin_shadowIntensity, float canvas_lightIntesity, uint8_t skin_shadowRadius, bool skin_headOverlay);
};