#ifndef __BWCHARTABLE_H
#define __BWCHARTABLE_H

struct CharTable_t
{
	int xpos;
	int ypos;
	int width;
	int height;
};


int PrintChar(BYTE* inputImage, int sizex, int sizey, int posx, int posy, char c);
void PrintString(BYTE* inputImage, int sizex, int sizey, int posx, int posy, char *s, size_t strsize);
void OpenPixTable();


class CBirdwatcherOverlay
{
public:
	CBirdwatcherOverlay(const char *bmpFileName);
	~CBirdwatcherOverlay();
	void PrintString(BYTE* inputImage, int sizex, int sizey, int posx, int posy, char *s, size_t strsize);

private:
	int PrintChar(BYTE* inputImage, int sizex, int sizey, int posx, int posy, char c);

private:
	unsigned char *m_charPixTable;
	int m_PixImageSizeX;
	int m_PixImageSizeY;
};

#endif