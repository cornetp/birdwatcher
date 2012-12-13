#include <stdio.h>
#include <streams.h>

#include "Debug.h"
#include "bwOverlay.h"

static struct CharTable_t CharTable[128] = {
	{0  , 0 , 0 , 0 },		/* 000 - NUL	   */
	{0  , 0 , 0 , 0 },		/* 001 - SOH	   */
	{0  , 0 , 0 , 0 },		/* 002 - STX	   */
	{0  , 0 , 0 , 0 },		/* 003 - ETX	   */
	{0  , 0 , 0 , 0 },		/* 004 - EOT	   */
	{0  , 0 , 0 , 0 },		/* 005 - ENQ	   */
	{0  , 0 , 0 , 0 },		/* 006 - ACK	   */
	{0  , 0 , 0 , 0 },		/* 007 - BEL	   */
	{0  , 0 , 0 , 0 },		/* 008 - BS		   */
	{0  , 0 , 0 , 0 },		/* 009 - HT		   */
	{0  , 0 , 0 , 0 },		/* 010 - LF		   */
	{0  , 0 , 0 , 0 },		/* 011 - VT		   */
	{0  , 0 , 0 , 0 },		/* 012 - FF		   */
	{0  , 0 , 0 , 0 },		/* 013 - CR		   */
	{0  , 0 , 0 , 0 },		/* 014 - SO		   */
	{0  , 0 , 0 , 0 },		/* 015 - SI		   */
	{0  , 0 , 0 , 0 },		/* 016 - DLE	   */
	{0  , 0 , 0 , 0 },		/* 017 - DC1	   */
	{0  , 0 , 0 , 0 },		/* 018 - DC2	   */
	{0  , 0 , 0 , 0 },		/* 019 - DC3	   */
	{0  , 0 , 0 , 0 },		/* 020 - DC4	   */
	{0  , 0 , 0 , 0 },		/* 021 - NAK	   */
	{0  , 0 , 0 , 0 },		/* 022 - SYN	   */
	{0  , 0 , 0 , 0 },		/* 023 - ETB	   */
	{0  , 0 , 0 , 0 },		/* 024 - CAN	   */
	{0  , 0 , 0 , 0 },		/* 025 - EM		   */
	{0  , 0 , 0 , 0 },		/* 026 - SUB	   */
	{0  , 0 , 0 , 0 },		/* 027 - ESC	   */
	{0  , 0 , 0 , 0 },		/* 028 - FS		   */
	{0  , 0 , 0 , 0 },		/* 029 - GS		   */
	{0  , 0 , 0 , 0 },		/* 030 - RS		   */
	{0  , 0 , 0 , 0 },		/* 031 - US		   */
	{81 , 5 , 6 , 11},		/* 032 - (space)   */
	{44 , 59, 3 , 14},		/* 033 - !		   */
	{243, 59, 5 , 14},		/* 034 - "		   */
	{71 , 59, 8 , 14},		/* 035 - #		   */
	{84 , 59, 6 , 14},		/* 036 - $		   */
	{95 , 59, 12, 14},		/* 037 - %		   */
	{125, 59, 11, 14},		/* 038 - &		   */
	{236, 59, 3 , 14},		/* 039 - '		   */
	{0  , 0 , 0 , 0 },		/* 040 - (		   */
	{0  , 0 , 0 , 0 },		/* 041 - )		   */
	{141, 59, 7 , 14},		/* 042 - *		   */
	{162, 59, 9 , 14},		/* 043 - +		   */
	{9  , 59, 4 , 14},		/* 044 - ,		   */
	{152, 59, 6 , 14},		/* 045 - -		   */
	{0  , 59, 3 , 14},		/* 046 - .		   */
	{188, 59, 4 , 14},		/* 047 - /		   */
	{2  , 5 , 7 , 11},		/* 048 - 0		   */
	{10 , 5 , 7 , 11},		/* 049 - 1		   */
	{18 , 5 , 7 , 11},		/* 050 - 2		   */
	{26 , 5 , 7 , 11},		/* 051 - 3		   */
	{33 , 5 , 7 , 11},		/* 052 - 4		   */
	{42 , 5 , 7 , 11},		/* 053 - 5		   */
	{50 , 5 , 7 , 11},		/* 054 - 6		   */
	{57 , 5 , 7 , 11},		/* 055 - 7		   */
	{66 , 5 , 7 , 11},		/* 056 - 8		   */
	{74 , 5 , 7 , 11},		/* 057 - 9		   */
	{25 , 59, 3 , 14},		/* 058 - :		   */
	{17 , 59, 4 , 14},		/* 059 - ;		   */
	{0  , 0 , 0 , 0 },		/* 060 - <		   */
	{175, 59, 9 , 14},		/* 061 - =		   */
	{0  , 0 , 0 , 0 },		/* 062 - >		   */
	{33 , 59, 5 , 14},		/* 063 - ?		   */
	{53 , 59, 14, 15},		/* 064 - @		   */
	{0  , 40, 12, 11},		/* 065 - A		   */
	{16 , 40, 9 , 11},		/* 066 - B		   */
	{31 , 40, 9 , 11},		/* 067 - V		   */
	{45 , 40, 10, 11},		/* 068 - D		   */
	{60 , 40, 9 , 11},		/* 069 - E		   */
	{73 , 40, 8 , 11},		/* 070 - F		   */
	{87 , 40, 10, 11},		/* 071 - G		   */
	{101, 40, 11, 11},		/* 072 - H		   */
	{116, 40, 5 , 11},		/* 073 - I		   */
	{125, 40, 6 , 11},		/* 074 - J		   */
	{135, 40, 11, 11},		/* 075 - K		   */
	{151, 40, 9 , 11},		/* 076 - L		   */
	{164, 40, 14, 11},		/* 077 - M		   */
	{182, 40, 12, 11},		/* 078 - N		   */
	{199, 40, 10, 11},		/* 079 - O		   */
	{214, 40, 8 , 11},		/* 080 - P		   */
	{228, 40, 10, 14},		/* 081 - Q		   */
	{243, 40, 10, 11},		/* 082 - R		   */
	{258, 40, 7 , 11},		/* 083 - S		   */
	{270, 40, 9 , 11},		/* 084 - T		   */
	{283, 40, 11, 11},		/* 085 - U		   */
	{298, 40, 11, 11},		/* 086 - V		   */
	{313, 40, 15, 11},		/* 087 - W		   */
	{332, 40, 11, 11},		/* 088 - X		   */
	{348, 40, 10, 11},		/* 089 - Y		   */
	{362, 40, 9 , 11},		/* 090 - Z		   */
	{0  , 0 , 0 , 0 },		/* 091 - [		   */
	{195, 59, 6 , 14},		/* 092 - \		   */
	{0  , 0 , 0 , 0 },		/* 093 - ]		   */
	{112, 59, 8 , 14},		/* 094 - ^		   */
	{211, 59, 8 , 14},		/* 095 - _		   */
	{0  , 0 , 0 , 0 },		/* 096 - `		   */
	{2  , 21, 6 , 14},		/* 097 - a		   */
	{12 , 21, 7 , 14},		/* 098 - b		   */
	{25 , 21, 5 , 14},		/* 099 - c		   */
	{36 , 21, 7 , 14},		/* 100 - d		   */
	{48 , 21, 5 , 14},		/* 101 - e		   */
	{58 , 21, 6 , 14},		/* 102 - f		   */
	{66 , 21, 7 , 14},		/* 103 - g		   */
	{77 , 21, 7 , 14},		/* 104 - h		   */
	{88 , 21, 3 , 14},		/* 105 - i		   */
	{94 , 21, 4 , 14},		/* 106 - j		   */
	{103, 21, 8 , 14},		/* 107 - k		   */
	{115, 21, 4 , 14},		/* 108 - l		   */
	{122, 21, 11, 14},		/* 109 - m		   */
	{137, 21, 7 , 14},		/* 110 - n		   */
	{149, 21, 6 , 14},		/* 111 - o		   */
	{160, 21, 7 , 14},		/* 112 - p		   */
	{173, 21, 7 , 14},		/* 113 - q		   */
	{184, 21, 5 , 14},		/* 114 - r		   */
	{194, 21, 4 , 14},		/* 115 - s		   */
	{203, 21, 4 , 14},		/* 116 - t		   */
	{211, 21, 7 , 14},		/* 117 - u		   */
	{222, 21, 7 , 14},		/* 118 - v		   */
	{233, 21, 11, 14},		/* 119 - w		   */
	{248, 21, 9 , 14},		/* 120 - x		   */
	{259, 21, 7 , 14},		/* 121 - y		   */
	{270, 21, 6 , 14},		/* 122 - z		   */
	{0  , 0 , 0 , 0 },		/* 123 - {		   */
	{204, 59, 3 , 15},		/* 124 - |		   */
	{0  , 0 , 0 , 0 },		/* 125 - }		   */
	{223, 59, 9 , 14},		/* 126 - ~		   */
	{0  , 0 , 0 , 0 },		/* 127 - ⌂		   */
};

CBirdwatcherOverlay::CBirdwatcherOverlay(const char *bmpFileName)
{
  BITMAPFILEHEADER bitmapFileHeader;
  BITMAPINFOHEADER bitmapInfoHeader;
  FILE *fp = NULL;
  fopen_s(&fp, bmpFileName, "rb");
  if(!fp)
  {
    DbgPrint("error: cannot open file \"%s\"\n", bmpFileName);
    return;
  }

  fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
  //verify that this is a bmp file by check bitmap id
  if (bitmapFileHeader.bfType !=0x4D42)
  {
    DbgPrint("error: \"%s\" is not a valid bmp file\n", bmpFileName);
    fclose(fp);
    return;
  }

  //read the bitmap info header
  fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
  m_PixImageSizeX = bitmapInfoHeader.biWidth;
  m_PixImageSizeY = bitmapInfoHeader.biHeight;

  //move file point to the begging of bitmap data
  fseek(fp, bitmapFileHeader.bfOffBits, SEEK_SET);

  //allocate enough memory for the bitmap image data
  m_charPixTable = (unsigned char*)malloc(bitmapInfoHeader.biSizeImage);
  if (!m_charPixTable)
  {
    DbgPrint("error: cannot allocate memory for image data (%lu bytes)\n", bitmapInfoHeader.biSizeImage);
    fclose(fp);
    return;
  }

  DbgPrint("CBirdwatcherOverlay::CBirdwatcherOverlay(%s): size of image = %dx%d, total size = %d, bitcount = %d"
	  , bmpFileName, bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight, bitmapInfoHeader.biSizeImage, bitmapInfoHeader.biBitCount);

  //read in the bitmap image data
  fread(m_charPixTable, bitmapInfoHeader.biSizeImage, 1, fp);
  fclose(fp);
}

CBirdwatcherOverlay::~CBirdwatcherOverlay()
{
	if(m_charPixTable)
		free(m_charPixTable);
}

int CBirdwatcherOverlay::PrintChar(BYTE* inputImage, int sizex, int sizey, int posx, int posy, char c)
{
	ASSERT(m_charPixTable!=NULL);

	//DbgPrint("PrintChar %d at %dx%d", c, posx, posy);
	int h = CharTable[c].height;
	int w = CharTable[c].width;

	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < w; j++)
		{
			int inx = CharTable[c].xpos - 1 + j;
			int iny = m_PixImageSizeY - (CharTable[c].ypos + i);
			int outx = posx + j;
			int outy = sizey - (posy + i);
			
			if(m_charPixTable[inx+m_PixImageSizeX*iny] < 255)
			{
				inputImage[3*(outx+sizex*outy)+0] = 255; //
				inputImage[3*(outx+sizex*outy)+1] = 255; //  -> white overlay
				inputImage[3*(outx+sizex*outy)+2] = 255; //
			}
		}
	}
	return w;
}

void CBirdwatcherOverlay::PrintString(BYTE* inputImage, int sizex, int sizey, int posx, int posy, char *s, size_t strsize)
{
	int inc = 0; // width of the previous char
	for(unsigned int i = 0; i < strsize; i++)
	{
		inc += PrintChar(inputImage, sizex, sizey, posx+inc, posy, s[i]);
		inc++;
	}
}
