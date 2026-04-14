
#ifndef BACKGROUND_COLOR

#include "program.h"
#include "lcd.h"
#include "fonts.h"

typedef struct {
    UWORD *Image;
    UWORD Width;
    UWORD Height;
    UWORD WidthMemory;
    UWORD HeightMemory;
    UWORD Color;
    UWORD WidthByte;
    UWORD HeightByte;
    UWORD Depth;
    UBYTE Mode;
} PAINT;
extern PAINT Paint;

#define BLACK 0x31A6 // #2E3440
#define BLACK2 0x424A // #3B4252
#define BLACK3 0x4A6B // #434C5E
#define BLACK4 0x52AB // #4C566A

#define WHITE 0xF79E // #ECEFF4
#define WHITE2 0xEF5D // #E5E9F0
#define WHITE3 0xDEFB // #D8DEE9

#define BLUE 0x4E71 // #5E81AC
#define BLUE2 0x6CF3 // #81A1C1
#define BLUE3 0x75FA // #88C0D0
#define BLUE4 0x76F3 // #8FBCBB

#define RED 0xBAEB // #BF616A
#define ORANGE 0xE4EA // #D08770
#define YELLOW 0xF6F1 // #EBCB8B
#define GREEN 0xA6F1 // #A3BE8C
#define PURPLE 0xB4B5 // #B48EAD

#define BACKGROUND_COLOR BLACK
#define FONT_COLOR WHITE3

typedef enum {
    DOT_PIXEL_1X1  = 1,		// 1 x 1
    DOT_PIXEL_2X2  , 		// 2 X 2
    DOT_PIXEL_3X3  ,		// 3 X 3
    DOT_PIXEL_4X4  ,		// 4 X 4
    DOT_PIXEL_5X5  , 		// 5 X 5
    DOT_PIXEL_6X6  , 		// 6 X 6
    DOT_PIXEL_7X7  , 		// 7 X 7
    DOT_PIXEL_8X8  , 		// 8 X 8
} DOT_PIXEL;
#define DOT_PIXEL_DFT  DOT_PIXEL_1X1  //Default dot pilex

typedef enum {
    DOT_FILL_AROUND  = 1,		// dot pixel 1 x 1
    DOT_FILL_RIGHTUP  , 		// dot pixel 2 X 2
} DOT_STYLE;
#define DOT_STYLE_DFT  DOT_FILL_AROUND  //Default dot pilex

typedef enum {
    DRAW_FILL_EMPTY = 0,
    DRAW_FILL_FULL,
} DRAW_FILL;

void Paint_NewImage(UWORD Width, UWORD Height, UWORD Color, UWORD Depth);
void Paint_SetPixel(UWORD Xpoint, UWORD Ypoint, UWORD Color);

void Paint_Clear(UWORD Color);
void Paint_ClearWindow(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color);

void Paint_DrawPoint(UWORD Xpoint, UWORD Ypoint, UWORD Color, DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_FillWay);
void Paint_DrawLine(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color, DOT_PIXEL Line_width);

void Paint_DrawChar(UWORD Xstart, UWORD Ystart, const char Acsii_Char, sFONT* Font, UWORD Color_Foreground, UWORD Color_Background);
void Paint_DrawString(UWORD Xstart, UWORD Ystart, const char * pString, sFONT* Font, UWORD Color_Foreground, UWORD Color_Background);

void Paint_DrawCheck(UWORD X, UWORD Y);
void Paint_DrawX(UWORD X, UWORD Y);

#endif
