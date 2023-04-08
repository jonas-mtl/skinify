#include "Skinify.hpp"


namespace Skinify 
{
    Image* canvas = nullptr;

    bool load(const char* srcImageName)
    {
        canvas = new Image(srcImageName);
	    canvas_srcImagePath = srcImageName;

        return canvas->_status == false ? false : true;
    }

    void generate(uint8_t skin_headScaleMultiplier, float skin_shadowIntensity, float canvas_lightIntesity, uint8_t skin_shadowRadius, bool skin_headOverlay)
    {
        // head
        Image head = *canvas;
        head.crop(8, 8, 8, 8);
        head.resize(head._w * skin_headScaleMultiplier, head._h * skin_headScaleMultiplier);
    
        // head overlay
        Image headOverlay = *canvas;
        headOverlay.crop(40, 8, 8, 8);
        headOverlay.resize(headOverlay._w * skin_headScaleMultiplier, headOverlay._h * skin_headScaleMultiplier);

        // head side
        Image headSide = *canvas;
        headSide.crop(6, 8, 2, 8);
        headSide.colorMask(canvas_lightIntesity, canvas_lightIntesity, canvas_lightIntesity);
        headSide.resize(headSide._w * skin_headScaleMultiplier, headSide._h * skin_headScaleMultiplier);

        // head side overlay
        Image headSideOverlay = *canvas;
        headSideOverlay.crop(38, 8, 2, 8);
        headSideOverlay.colorMask(canvas_lightIntesity, canvas_lightIntesity, canvas_lightIntesity);
        headSideOverlay.resize(headSideOverlay._w * skin_headScaleMultiplier, headSideOverlay._h * skin_headScaleMultiplier);

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

        canvas->overlay(head, (canvas->_w / 2) - (head._w / 2) + skin_headScaleMultiplier - 1, 0 + canvas_yOffset);
        canvas->overlay(headSide, (canvas->_w / 2) - (head._w / 2) - skin_headScaleMultiplier - 1, 0 + canvas_yOffset);

        if (skin_headOverlay) {
            canvas->overlay(headOverlay, (canvas->_w / 2) - (headOverlay._w / 2) + skin_headScaleMultiplier - 1, 0 + canvas_yOffset);
            canvas->overlay(headSideOverlay, (canvas->_w / 2) - (head._w / 2) - skin_headScaleMultiplier - 1, 0 + canvas_yOffset);
        }
    
        canvas->overlay(body, CENTER(canvas, body), head._h + canvas_yOffset);

        canvas->overlay(armR, CENTER(canvas, body) + body._w, head._h + canvas_yOffset);
        canvas->overlay(armL, CENTER(canvas, body) - (body._w / 2), head._h + canvas_yOffset);
        canvas->overlay(armLSide, CENTER(canvas, body) - armL._w * 1.5, head._h + canvas_yOffset);

        canvas->overlay(legR, CENTER(canvas, body) + legR._w, head._h + body._h + canvas_yOffset);
        canvas->overlay(legL, CENTER(canvas, body), head._h + body._h + canvas_yOffset);
        canvas->overlay(legLSide, CENTER(canvas, body) - legL._w / 2, head._h + body._h + canvas_yOffset);

        uint16_t maxDimension = (head._w + headSide._w) > (head._h + (12 * 2)) ? (head._w + headSide._w) : (head._h + (12 * 2));

        canvas->crop((canvas->_w - maxDimension) / 2, 0, maxDimension + (canvas_yOffset * 2), maxDimension + (canvas_yOffset * 2));

        canvas->resize(canvas->_w * canvas_upscaleMultiplier, canvas->_h * canvas_upscaleMultiplier);

        canvas->write("output.png");
    }
}