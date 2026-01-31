/*
* Create library
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documnetation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to  whom the Software is
* furished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*/

#include "draw.h"
#include "menus.h" // screen global var

PAINT Paint;

void Paint_NewImage(UWORD Width, UWORD Height, UWORD Color, UWORD Depth) {
    Paint.Image = screen;

    Paint.WidthMemory = Width;
    Paint.HeightMemory = Height;
    Paint.Color = Color;    
    Paint.WidthByte = Width;
    Paint.HeightByte = Height;    
    Paint.Depth = Depth;    
    
    Paint.Width = Width;
    Paint.Height = Height;
};

void Paint_SetPixel(UWORD Xpoint, UWORD Ypoint, UWORD Color) {
    if (Xpoint > Paint.Width || Ypoint > Paint.Height){
        printf("Exceeding display boundaries\r\n");
        return;
    };

    UWORD X = Xpoint;
    UWORD Y = Ypoint;

    if(X > Paint.WidthMemory || Y > Paint.HeightMemory){
        printf("SetPixel: Exceeding display boundaries\r\n");
        return;
    };

    if (Paint.Depth == 1){
        UDOUBLE Addr = X / 8 + Y * Paint.WidthByte;
        UBYTE Rdata = Paint.Image[Addr];
        if(Color == BLACK)
            Paint.Image[Addr] = Rdata & ~(0x80 >> (X % 8));
        else
            Paint.Image[Addr] = Rdata | (0x80 >> (X % 8));
    } else {
        Color = ((Color<<8)&0xff00) | (Color>>8);
        UDOUBLE Addr = X  + Y * Paint.WidthByte;
        Paint.Image[Addr] = Color;
    };
};

void Paint_Clear(UWORD Color) {
    for (UWORD Y = 0; Y < Paint.HeightByte; Y++) {
        for (UWORD X = 0; X < Paint.WidthByte; X++ ) {
            UDOUBLE Addr = X + Y*Paint.WidthByte;
            Paint.Image[Addr] = Color;
        };
    };
};

void Paint_ClearWindow(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color) {
    UWORD X, Y;
    for (Y = Ystart; Y < Yend; Y++) {
        for (X = Xstart; X < Xend; X++) {
            Paint_SetPixel(X, Y, Color);
        };
    };
};

void Paint_DrawPoint(UWORD Xpoint, UWORD Ypoint, UWORD Color, DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_Style) {
    if (Xpoint > Paint.Width || Ypoint > Paint.Height) {
        printf("Paint_DrawPoint Input exceeds the normal display range\r\n");
        return;
    };

    int16_t XDir_Num , YDir_Num;
    if (Dot_Style == DOT_FILL_AROUND) {
        for (XDir_Num = 0; XDir_Num < 2 * Dot_Pixel - 1; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num < 2 * Dot_Pixel - 1; YDir_Num++) {
                if (Xpoint + XDir_Num - Dot_Pixel < 0 || Ypoint + YDir_Num - Dot_Pixel < 0)
                    break;
                Paint_SetPixel(Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel, Color);
            };
        };
    } else {
        for (XDir_Num = 0; XDir_Num <  Dot_Pixel; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num <  Dot_Pixel; YDir_Num++) {
                Paint_SetPixel(Xpoint + XDir_Num - 1, Ypoint + YDir_Num - 1, Color);
            };
        };
    };
};

void Paint_DrawLine(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color, DOT_PIXEL Line_width) {
    UWORD Xpoint = Xstart;
    UWORD Ypoint = Ystart;
    int dx = (int)Xend - (int)Xstart >= 0 ? Xend - Xstart : Xstart - Xend;
    int dy = (int)Yend - (int)Ystart <= 0 ? Yend - Ystart : Ystart - Yend;

    int XAddway = Xstart < Xend ? 1 : -1;
    int YAddway = Ystart < Yend ? 1 : -1;
    int Esp = dx + dy;
    char Dotted_Len = 0;

    for (;;) {
        Dotted_Len++;
        Paint_DrawPoint(Xpoint, Ypoint, Color, Line_width, DOT_STYLE_DFT);
        if (2 * Esp >= dy) {
            if (Xpoint == Xend)
                break;
            Esp += dy;
            Xpoint += XAddway;
        };
        if (2 * Esp <= dx) {
            if (Ypoint == Yend)
                break;
            Esp += dx;
            Ypoint += YAddway;
        };
    };
};

void Paint_DrawChar(UWORD Xpoint, UWORD Ypoint, const char Acsii_Char, sFONT* Font, UWORD Color_Foreground, UWORD Color_Background) {
    UWORD Page, Column;

    uint32_t Char_Offset = (Acsii_Char - ' ') * Font -> Height * (Font -> Width / 8 + (Font -> Width % 8 ? 1 : 0));
    const unsigned char *ptr = &Font->table[Char_Offset];

    for (Page = 0; Page < Font->Height; Page ++ ) {
        for (Column = 0; Column < Font->Width; Column ++ ) {
            if (*ptr & (0x80 >> (Column % 8))) {
                Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
            } else {
                Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Background);
            }
            if (Column % 8 == 7)
                ptr++;
        };
        if (Font->Width % 8 != 0)
            ptr++;
    };
};

void Paint_DrawString(UWORD Xstart, UWORD Ystart, const char * pString, sFONT* Font, UWORD Color_Foreground, UWORD Color_Background) {
    UWORD Xpoint = Xstart;
    UWORD Ypoint = Ystart;

    while (*pString != '\0') {
        if ((Xpoint + Font -> Width) > Paint.Width) {
            Xpoint = Xstart;
            Ypoint += Font -> Height;
        };
        if ((Ypoint + Font -> Height ) > Paint.Height ) {
            Xpoint = Xstart;
            Ypoint = Ystart;
        };
        Paint_DrawChar(Xpoint, Ypoint, *pString, Font, Color_Background, Color_Foreground);

        pString ++; // Next character
        Xpoint += Font -> Width;
    };
};
