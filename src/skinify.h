#pragma once 

#include <iostream>
#include <stdint.h>

#include "core/image.h"

#define CENTER(canvas, bodyPart) (canvas.w / 2) - (bodyPart.w / 2)

struct Skinify {
    uint8_t       skin_headScaleMultiplier{ 3 };
    uint8_t       skin_shadowRadius{ 4 };
    float         skin_upperShadowOpacityMultiplier{ 1.25f };
    float         skin_shadowIntensity{ 0.7f };
    float         skin_shadowOpacity{ 0.5f };
    bool          skin_headOverlay{ true };

    std::string   canvas_srcImagePath{};
    float         canvas_lightIntesity{ 0.5f };
    uint8_t       canvas_yOffset{ 2 };
    uint8_t       canvas_upscaleMultiplier{ 4 };


    Skinify(const char* srcImagePath);

    Image& generate();
};