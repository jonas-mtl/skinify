#include "skinify.h"
#include "image.h"

int main()
{
	Skinify skin("test.png");

	Image skinImg = skin.generate();
	skinImg.destruct = true;
	
	skinImg.write("output.png");
}