#include "DEV_Config.h"
#include <gpiod.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GPIO_CHIP "/dev/gpiochip0"

static struct gpiod_chip *chip;

static struct gpiod_line *line_rst;
static struct gpiod_line *line_dc;

static struct gpiod_line *line_key_up;
static struct gpiod_line *line_key_down;
static struct gpiod_line *line_key_left;
static struct gpiod_line *line_key_right;
static struct gpiod_line *line_key_press;
static struct gpiod_line *line_key1;
static struct gpiod_line *line_key2;
static struct gpiod_line *line_key3;

static int spi_fd = -1;

// GPIO

void DEV_Digital_Write(UWORD Pin, UBYTE Value)
{
    if (Pin == LCD_CS) {
        return;
    }

    if (!chip) {
        printf("DEV_Digital_Write: chip not initialized\n");
        return;
    }

    struct gpiod_line *line = gpiod_chip_get_line(chip, Pin);
    if (!line) {
        printf("DEV_Digital_Write: failed to get line %u\n", Pin);
        return;
    }

    if (gpiod_line_set_value(line, Value) < 0) {
        printf("DEV_Digital_Write: failed to set value on line %u\n", Pin);
    }
}

UBYTE DEV_Digital_Read(UWORD Pin)
{
    if (!chip) {
        printf("DEV_Digital_Read: chip not initialized\n");
        return 0;
    }

    struct gpiod_line *line = gpiod_chip_get_line(chip, Pin);
    if (!line) {
        printf("DEV_Digital_Read: failed to get line %u\n", Pin);
        return 0;
    }

    int v = gpiod_line_get_value(line);
    if (v < 0) {
        printf("DEV_Digital_Read: failed to read value from line %u\n", Pin);
        return 0;
    }

    return (UBYTE)v;
}

void DEV_GPIO_Mode(UWORD Pin, UWORD Mode)
{
    /* Same idea: don't try to request LCD_CS as a GPIO line */
    if (Pin == LCD_CS) {
        return;
    }

    if (!chip) {
        printf("DEV_GPIO_Mode: chip not initialized\n");
        return;
    }

    struct gpiod_line *line = gpiod_chip_get_line(chip, Pin);
    if (!line) {
        printf("DEV_GPIO_Mode: failed to get line %u\n", Pin);
        return;
    }

    if (Mode == 0) {
        if (gpiod_line_request_input(line, "waveshare") < 0) {
            printf("DEV_GPIO_Mode: failed to request input on line %u\n", Pin);
        }
    } else {
        if (gpiod_line_request_output(line, "waveshare", 0) < 0) {
            printf("DEV_GPIO_Mode: failed to request output on line %u\n", Pin);
        }
    }
}

void DEV_Delay_ms(UDOUBLE xms)
{
    usleep(xms * 1000);
}

void DEV_SPI_WriteByte(uint8_t Value)
{
    if (spi_fd < 0) {
        printf("DEV_SPI_WriteByte: SPI not initialized\n");
        return;
    }
    write(spi_fd, &Value, 1);
}

void DEV_SPI_Write_nByte(uint8_t *pData, uint32_t Len)
{
    if (spi_fd < 0) {
        printf("DEV_SPI_Write_nByte: SPI not initialized\n");
        return;
    }
    write(spi_fd, pData, Len);
}

// Init

static void DEV_GPIO_Init(void)
{
    DEV_GPIO_Mode(LCD_RST, 1);
    DEV_GPIO_Mode(LCD_DC, 1);
    DEV_GPIO_Mode(LCD_BL, 1);

    /* Buttons */
    DEV_GPIO_Mode(KEY_UP_PIN, 0);
    DEV_GPIO_Mode(KEY_DOWN_PIN, 0);
    DEV_GPIO_Mode(KEY_LEFT_PIN, 0);
    DEV_GPIO_Mode(KEY_RIGHT_PIN, 0);
    DEV_GPIO_Mode(KEY_PRESS_PIN, 0);
    DEV_GPIO_Mode(KEY1_PIN, 0);
    DEV_GPIO_Mode(KEY2_PIN, 0);
    DEV_GPIO_Mode(KEY3_PIN, 0);

    /* Default states */
    DEV_Digital_Write(LCD_BL, 1);  // backlight on
}

UBYTE DEV_ModuleInit(void)
{
    /* Open GPIO chip */
    chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        printf("Failed to open %s\n", GPIO_CHIP);
        return 1;
    }

    /* Open SPI */
    spi_fd = open("/dev/spidev0.0", O_WRONLY);
    if (spi_fd < 0) {
        printf("Failed to open /dev/spidev0.0\n");
        gpiod_chip_close(chip);
        chip = NULL;
        return 1;
    }

    DEV_GPIO_Init();

    return 0;
}

void DEV_SetBacklight(UWORD Value)
{
    (void)Value;
}

void DEV_ModuleExit(void)
{
    if (spi_fd >= 0) {
        close(spi_fd);
        spi_fd = -1;
    }
    if (chip) {
        gpiod_chip_close(chip);
        chip = NULL;
    }
    exit(0);
}
