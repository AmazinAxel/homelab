
#ifndef LFLAGS

#define LFLAGS 0
#define NUM_MAXBUF  4

#include <unistd.h> // usleep() close()

#include <stdio.h>
#include <stdint.h> // spi
#include <stdlib.h> //exit()

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

#define LCD_CS   8
#define LCD_RST  27
#define LCD_DC   25
#define LCD_BL   24


// LCD
#define LCD_CS_0		DEV_Digital_Write(LCD_CS,0)
#define LCD_CS_1		DEV_Digital_Write(LCD_CS,1)

#define LCD_RST_0		DEV_Digital_Write(LCD_RST,0)
#define LCD_RST_1		DEV_Digital_Write(LCD_RST,1)

#define LCD_DC_0		DEV_Digital_Write(LCD_DC,0)
#define LCD_DC_1		DEV_Digital_Write(LCD_DC,1)

#define LCD_BL_0		DEV_Digital_Write(LCD_BL,0)
#define LCD_BL_1		DEV_Digital_Write(LCD_BL,1)

#define KEY_UP_PIN      6
#define KEY_DOWN_PIN    19
#define KEY_LEFT_PIN    5
#define KEY_RIGHT_PIN   26
#define KEY_PRESS_PIN   13
#define KEY1_PIN        21
#define KEY2_PIN        20
#define KEY3_PIN        16

#define GET_KEY_UP       		DEV_Digital_Read(KEY_UP_PIN)
#define GET_KEY_DOWN     		DEV_Digital_Read(KEY_DOWN_PIN)
#define GET_KEY_LEFT     		DEV_Digital_Read(KEY_LEFT_PIN)
#define GET_KEY_RIGHT    		DEV_Digital_Read(KEY_RIGHT_PIN)

UBYTE DEV_ModuleInit(void);
void DEV_ModuleExit(void);

void DEV_GPIO_Mode(UWORD Pin, UWORD Mode);
void DEV_Digital_Write(UWORD Pin, UBYTE Value);
UBYTE DEV_Digital_Read(UWORD Pin);
void DEV_Delay_ms(UDOUBLE xms);

void DEV_SPI_WriteByte(uint8_t Value);
void DEV_SPI_Write_nByte(uint8_t *pData, uint32_t Len);
#endif
