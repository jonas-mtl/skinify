#include "skinify.h"
#include "image.h"


int main()
{
	Skinify skin("test.png");
	Image bg("back.png");

	Image skinImg = skin.generate();
	bg.resize(skinImg.w, skinImg.h);
	bg.overlay(skinImg, 0, 0);


	bg.write("output.png");
	skinImg.destruct = true;

    return 0;
}