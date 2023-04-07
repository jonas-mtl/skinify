#include "skinify.hpp"


namespace Skinify 
{
    Image* canvas = nullptr;

    void init(const char* srcImageName)
    {
        canvas = new Image(srcImageName);
	    canvas_srcImagePath = srcImageName;
    }

    void generate(uint8_t skin_headScaleMultiplier, float skin_shadowIntensity, float canvas_lightIntesity, uint8_t skin_shadowRadius, bool skin_headOverlay)
    {
        if (skin_headScaleMultiplier == 3) skin_headScaleMultiplier = 4;

        // head
        Image head = *canvas;
        head.crop(8, 8, 8, 8);
        head.resize(head.w * skin_headScaleMultiplier, head.h * skin_headScaleMultiplier);
    
        // head overlay
        Image headOverlay = *canvas;
        headOverlay.crop(40, 8, 8, 8);
        headOverlay.resize(headOverlay.w * skin_headScaleMultiplier, headOverlay.h * skin_headScaleMultiplier);

        // head side
        Image headSide = *canvas;
        headSide.crop(6, 8, 2, 8);
        headSide.colorMask(canvas_lightIntesity, canvas_lightIntesity, canvas_lightIntesity);
        headSide.resize(headSide.w * skin_headScaleMultiplier, headSide.h * skin_headScaleMultiplier);

        // body
        Image body = *canvas;
        body.crop(20, 20, 8, 12);
        body.addShadow(skin_shadowIntensity * skin_upperShadowOpacityMultiplier, skin_shadowRadius, skin_shadowOpacity);


        // left arm
        Image armL = *canvas;
        armL.crop(44, 20, 4, 12);
        armL.addShadow(skin_shadowIntensity * skin_upperShadowOpacityMultiplier, skin_shadowRadius, skin_shadowOpacity);


        // left arm side
        Image armLSide = *canvas;
        armLSide.crop(43, 20, 2, 12);
        armLSide.colorMask(canvas_lightIntesity, canvas_lightIntesity, canvas_lightIntesity);
        armLSide.addShadow(skin_shadowIntensity * skin_upperShadowOpacityMultiplier, skin_shadowRadius, skin_shadowOpacity);

        // right arm
        Image armR = *canvas;
        armR.crop(35, 52, 4, 12);
        armR.addShadow(skin_shadowIntensity * skin_upperShadowOpacityMultiplier, skin_shadowRadius, skin_shadowOpacity);

        // left leg
        Image legL = *canvas;
        legL.crop(4, 20, 4, 12);
        legL.addShadow(skin_shadowIntensity, skin_shadowRadius, skin_shadowOpacity);

        // left leg side
        Image legLSide = *canvas;
        legLSide.crop(2, 20, 2, 12);
        legLSide.colorMask(canvas_lightIntesity, canvas_lightIntesity, canvas_lightIntesity);
        legLSide.addShadow(skin_shadowIntensity, skin_shadowRadius, skin_shadowOpacity);

        // right leg
        Image legR = *canvas;
        legR.crop(20, 52, 4, 12);
        legR.addShadow(skin_shadowIntensity, skin_shadowRadius, skin_shadowOpacity);

        canvas->colorMask(0.f, 0.f, 0, 0.f);

        canvas->overlay(head, (canvas->w / 2) - (head.w / 2) + skin_headScaleMultiplier - 1, 0 + canvas_yOffset);

        if (skin_headOverlay) canvas->overlay(headOverlay, (canvas->w / 2) - (headOverlay.w / 2) + skin_headScaleMultiplier - 1, 0 + canvas_yOffset);
    
        canvas->overlay(headSide, (canvas->w / 2) - (head.w / 2) - skin_headScaleMultiplier - 1, 0 + canvas_yOffset);

        canvas->overlay(body, CENTER(canvas, body), head.h + canvas_yOffset);

        canvas->overlay(armR, CENTER(canvas, body) + body.w, head.h + canvas_yOffset);
        canvas->overlay(armL, CENTER(canvas, body) - (body.w / 2), head.h + canvas_yOffset);
        canvas->overlay(armLSide, CENTER(canvas, body) - armL.w * 1.5, head.h + canvas_yOffset);

        canvas->overlay(legR, CENTER(canvas, body) + legR.w, head.h + body.h + canvas_yOffset);
        canvas->overlay(legL, CENTER(canvas, body), head.h + body.h + canvas_yOffset);
        canvas->overlay(legLSide, CENTER(canvas, body) - legL.w / 2, head.h + body.h + canvas_yOffset);

        uint16_t maxDimension = (head.w + headSide.w) > (head.h + (12 * 2)) ? (head.w + headSide.w) : (head.h + (12 * 2));

        canvas->crop((canvas->w - maxDimension) / 2, 0, maxDimension + (canvas_yOffset * 2), maxDimension + (canvas_yOffset * 2));

        canvas->resize(canvas->w * canvas_upscaleMultiplier, canvas->h * canvas_upscaleMultiplier);

        canvas->write("output.png");
    }
}