#include "lcd.h"
#include "lib.h"
#include "menus.h" // global screen var

LCD_DIS sLCD_DIS;

static void LCD_Reset() {
    LCD_RST_1;
    DEV_Delay_ms(100);
    LCD_RST_0;
    DEV_Delay_ms(100);
    LCD_RST_1;
    DEV_Delay_ms(100);
};

static void LCD_SendCommand(UBYTE Reg) {
    LCD_DC_0;
    DEV_SPI_WriteByte(Reg);
};

static void LCD_SendData_8Bit(UBYTE Data) {
    LCD_DC_1;
    DEV_SPI_WriteByte(Data);
};

void LCD_SetGramScanWay(LCD_SCAN_DIR Scan_dir) {
    sLCD_DIS.LCD_Scan_Dir = Scan_dir;
    if (Scan_dir == L2R_U2D || Scan_dir == L2R_D2U || Scan_dir == R2L_U2D || Scan_dir == R2L_D2U) {
        sLCD_DIS.LCD_Dis_Column	= LCD_HEIGHT ;
        sLCD_DIS.LCD_Dis_Page = LCD_WIDTH ;
    } else {
        sLCD_DIS.LCD_Dis_Column	= LCD_WIDTH ;
        sLCD_DIS.LCD_Dis_Page = LCD_HEIGHT ;
    };

    uint16_t MemoryAccessReg_Data=0; // 0x36
    switch (Scan_dir) {
    case L2R_U2D:
        MemoryAccessReg_Data = 0X00 | 0x00; // x scan dir | y scan dir
        break;
    case L2R_D2U:
        MemoryAccessReg_Data = 0x00 | 0x80;
        break;
    case R2L_U2D: // 0X4
        MemoryAccessReg_Data = 0x40 | 0x00;
        break;
    case R2L_D2U: // 0XC
        MemoryAccessReg_Data = 0x40 | 0x80;
        break;
    case U2D_L2R: // 0X2
        MemoryAccessReg_Data = 0X00 | 0X00 | 0x20;
        break;
    case U2D_R2L: // 0X6
        MemoryAccessReg_Data = 0x00 | 0X40 | 0x20;
        break;
    case D2U_L2R: // 0XA
        MemoryAccessReg_Data = 0x80 | 0x00 | 0x20;
        break;
    case D2U_R2L: // 0XE
        MemoryAccessReg_Data = 0x40 | 0x80 | 0x20;
        break;
    };

    if ((MemoryAccessReg_Data && 0x20) != 1) {
        sLCD_DIS.LCD_X_Adjust = LCD_X;
        sLCD_DIS.LCD_Y_Adjust = LCD_Y;
    } else {
        sLCD_DIS.LCD_X_Adjust = LCD_Y;
        sLCD_DIS.LCD_Y_Adjust = LCD_X;
    }

    // Set the read / write scan direction of the frame memory
    LCD_SendCommand(0x36); // MX, MY, RGB mode
    LCD_SendData_8Bit(MemoryAccessReg_Data | 0x08);	// RGB
};

static void LCD_InitReg() {
    // ST7735R Framerate
    LCD_SendCommand(0xB1);
    LCD_SendData_8Bit(0x01);
    LCD_SendData_8Bit(0x2C);
    LCD_SendData_8Bit(0x2D);

    LCD_SendCommand(0xB2);
    LCD_SendData_8Bit(0x01);
    LCD_SendData_8Bit(0x2C);
    LCD_SendData_8Bit(0x2D);

    LCD_SendCommand(0xB3);
    LCD_SendData_8Bit(0x01);
    LCD_SendData_8Bit(0x2C);
    LCD_SendData_8Bit(0x2D);
    LCD_SendData_8Bit(0x01);
    LCD_SendData_8Bit(0x2C);
    LCD_SendData_8Bit(0x2D);

    LCD_SendCommand(0xB4); // Column inversion
    LCD_SendData_8Bit(0x07);

    // ST7735R Power sequence
    LCD_SendCommand(0xC0);
    LCD_SendData_8Bit(0xA2);
    LCD_SendData_8Bit(0x02);
    LCD_SendData_8Bit(0x84);
    LCD_SendCommand(0xC1);
    LCD_SendData_8Bit(0xC5);

    LCD_SendCommand(0xC2);
    LCD_SendData_8Bit(0x0A);
    LCD_SendData_8Bit(0x00);

    LCD_SendCommand(0xC3);
    LCD_SendData_8Bit(0x8A);
    LCD_SendData_8Bit(0x2A);
    LCD_SendCommand(0xC4);
    LCD_SendData_8Bit(0x8A);
    LCD_SendData_8Bit(0xEE);

    LCD_SendCommand(0xC5); // VCOM
    LCD_SendData_8Bit(0x0E);

    // ST7735R Gamma sequence
    LCD_SendCommand(0xe0);
    LCD_SendData_8Bit(0x0f);
    LCD_SendData_8Bit(0x1a);
    LCD_SendData_8Bit(0x0f);
    LCD_SendData_8Bit(0x18);
    LCD_SendData_8Bit(0x2f);
    LCD_SendData_8Bit(0x28);
    LCD_SendData_8Bit(0x20);
    LCD_SendData_8Bit(0x22);
    LCD_SendData_8Bit(0x1f);
    LCD_SendData_8Bit(0x1b);
    LCD_SendData_8Bit(0x23);
    LCD_SendData_8Bit(0x37);
    LCD_SendData_8Bit(0x00);
    LCD_SendData_8Bit(0x07);
    LCD_SendData_8Bit(0x02);
    LCD_SendData_8Bit(0x10);

    LCD_SendCommand(0xe1);
    LCD_SendData_8Bit(0x0f);
    LCD_SendData_8Bit(0x1b);
    LCD_SendData_8Bit(0x0f);
    LCD_SendData_8Bit(0x17);
    LCD_SendData_8Bit(0x33);
    LCD_SendData_8Bit(0x2c);
    LCD_SendData_8Bit(0x29);
    LCD_SendData_8Bit(0x2e);
    LCD_SendData_8Bit(0x30);
    LCD_SendData_8Bit(0x30);
    LCD_SendData_8Bit(0x39);
    LCD_SendData_8Bit(0x3f);
    LCD_SendData_8Bit(0x00);
    LCD_SendData_8Bit(0x07);
    LCD_SendData_8Bit(0x03);
    LCD_SendData_8Bit(0x10);

    LCD_SendCommand(0xF0); // Enable test command
    LCD_SendData_8Bit(0x01);

    LCD_SendCommand(0xF6); // Disable ram power save mode
    LCD_SendData_8Bit(0x00);

    LCD_SendCommand(0x3A); // 65k mode
    LCD_SendData_8Bit(0x05);  
};

void LCD_1in44_Init(LCD_SCAN_DIR Scan_dir) {
    LCD_BL_1; // Backlight on
    LCD_Reset(); // Hardware reset
    LCD_InitReg(); // Initalization register

    // Resolution & scanning method
    LCD_SetGramScanWay(Scan_dir);
    DEV_Delay_ms(100);

    LCD_SendCommand(0x11);
    DEV_Delay_ms(120);

    LCD_SendCommand(0x29); // Turn on
};

void turnOffLCD() {
    LCD_SendCommand(0x28); // Display turn off refresh
    DEV_Delay_ms(10);

    LCD_SendCommand(0x10); // Deep sleep
    DEV_Delay_ms(120);

    LCD_BL_0; // Backlight off
};

void turnOnLCD() {
    LCD_BL_1; // Backlight on
    DEV_Delay_ms(10);

    LCD_SendCommand(0x11); // Wake up from sleep
    DEV_Delay_ms(120);

    LCD_SendCommand(0x29); // Display turn on refresh
    DEV_Delay_ms(10);

    LCD_SetGramScanWay(sLCD_DIS.LCD_Scan_Dir);
    LCD_1in44_SetWindows(0, 0, LCD_WIDTH, LCD_HEIGHT);
};


void LCD_1in44_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend) {
    // X
    LCD_SendCommand ( 0x2A );
    LCD_SendData_8Bit ( 0x00 );
    LCD_SendData_8Bit ( (Xstart & 0xff) + sLCD_DIS.LCD_X_Adjust);
    LCD_SendData_8Bit ( 0x00 );
    LCD_SendData_8Bit ( (( Xend - 1 ) & 0xff) + sLCD_DIS.LCD_X_Adjust);

    // Y
    LCD_SendCommand ( 0x2B );
    LCD_SendData_8Bit ( 0x00 );
    LCD_SendData_8Bit ( (Ystart & 0xff) + sLCD_DIS.LCD_Y_Adjust);
    LCD_SendData_8Bit ( 0x00 );
    LCD_SendData_8Bit ( ( (Yend - 1) & 0xff )+ sLCD_DIS.LCD_Y_Adjust);

    LCD_SendCommand(0x2C);
};

void LCD_1in44_Display() {
    UWORD j;
    LCD_1in44_SetWindows(0, 0, LCD_WIDTH, LCD_HEIGHT);
    LCD_DC_1;
    for (j = 0; j < LCD_HEIGHT; j++) {
        DEV_SPI_Write_nByte((uint8_t *)&screen[j*LCD_WIDTH], LCD_WIDTH*2);
    };
};

void LCD_1in44_DisplayWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD *Image) {
    UDOUBLE Addr = 0;

    UWORD j;
    LCD_1in44_SetWindows(Xstart, Ystart, Xend-1 , Yend-1);
    LCD_DC_1;
    for (j = Ystart; j < Yend - 1; j++) {
        Addr = Xstart + j * LCD_WIDTH ;
        DEV_SPI_Write_nByte((uint8_t *)&Image[Addr], (Xend-Xstart-1)*2);
    };
};

void Handler_1in44_LCD(int signo) {
    DEV_ModuleExit();
    exit(0);
};
