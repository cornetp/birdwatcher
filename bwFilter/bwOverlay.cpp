#include <stdio.h>
#include <streams.h>

#include "Debug.h"
#include "bwOverlay.h"
#include "font_8x8.h"

CBirdwatcherOverlay::CBirdwatcherOverlay(const char *bmpFileName)
{
}

CBirdwatcherOverlay::~CBirdwatcherOverlay()
{
}

int CBirdwatcherOverlay::PrintChar(BYTE* inputImage, int sizex, int sizey, int posx, int posy, char c)
{
	int x, y;
	posx = max(posx, 0);
	posx = min(posx, (int)sizex-1-8);
	posy = max(posy, 0);
	posy = min(posy, (int)sizey-1-8);

	char *font_ = (char*)fontdata_8x8;
#define font(y,x) font_[y*8+x]
	for(y=0; y<8; y++)
	{
		for(x=0; x<8; x++)
		{
			int outx = posx + x;
			int outy = sizey - (posy + y);
			if(font((int)c,y)>>(7-x)&1)
			{
				inputImage[3*(outx+sizex*outy)+0] = 255; //
				inputImage[3*(outx+sizex*outy)+1] = 255; //  -> white overlay
				inputImage[3*(outx+sizex*outy)+2] = 255; //
			}
		}
	}
#undef font
	return 9;
}

void CBirdwatcherOverlay::PrintString(BYTE* inputImage, int sizex, int sizey, int posx, int posy, char *s)
{
	int inc = 0; // width of the previous char
	size_t strsize = strlen(s);
	for(unsigned int i = 0; i < strsize; i++)
	{
		inc += PrintChar(inputImage, sizex, sizey, posx+inc, posy, s[i]);
		inc++;
	}
}
