#ifndef __BWCHARTABLE_H
#define __BWCHARTABLE_H

class CBirdwatcherOverlay
{
public:
	CBirdwatcherOverlay(const char *bmpFileName);
	~CBirdwatcherOverlay();
	void PrintString(BYTE* inputImage, int sizex, int sizey, int posx, int posy, char *s);

private:
	int PrintChar(BYTE* inputImage, int sizex, int sizey, int posx, int posy, char c);

};

#endif
