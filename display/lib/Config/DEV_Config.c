
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
static struct gpiod_line *line_cs;
static struct gpiod_line *line_rst;
static struct gpiod_line *line_dc;
static struct gpiod_line *line_bl;

static struct gpiod_line *line_key_up;
static struct gpiod_line *line_key_down;
static struct gpiod_line *line_key_left;
static struct gpiod_line *line_key_right;
static struct gpiod_line *line_key_press;
static struct gpiod_line *line_key1;
static struct gpiod_line *line_key2;
static struct gpiod_line *line_key3;

static int spi_fd = -1;

static pthread_t bl_thread;
static volatile int bl_value = 100;

static void *bl_pwm_thread(void *arg)
{
    while (1) {
        int duty = bl_value;
        gpiod_line_set_value(line_bl, 1);
        usleep(duty * 50);
        gpiod_line_set_value(line_bl, 0);
        usleep((100 - duty) * 50);
    }
    return NULL;
}

// GPIO

void DEV_Digital_Write(UWORD Pin, UBYTE Value)
{
    struct gpiod_line *line = gpiod_chip_get_line(chip, Pin);
    gpiod_line_set_value(line, Value);
}

UBYTE DEV_Digital_Read(UWORD Pin)
{
    struct gpiod_line *line = gpiod_chip_get_line(chip, Pin);
    return gpiod_line_get_value(line);
}

void DEV_GPIO_Mode(UWORD Pin, UWORD Mode)
{
    struct gpiod_line *line = gpiod_chip_get_line(chip, Pin);

    if (Mode == 0) {
        gpiod_line_request_input(line, "waveshare");
    } else {
        gpiod_line_request_output(line, "waveshare", 0);
    }
}

void DEV_Delay_ms(UDOUBLE xms)
{
    usleep(xms * 1000);
}


void DEV_SPI_WriteByte(uint8_t Value)
{
    write(spi_fd, &Value, 1);
}

void DEV_SPI_Write_nByte(uint8_t *pData, uint32_t Len)
{
    write(spi_fd, pData, Len);
}

// Init

static void DEV_GPIO_Init(void)
{
    /* Display control pins */
    DEV_GPIO_Mode(LCD_CS, 1);
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
    DEV_Digital_Write(LCD_CS, 1);
    DEV_Digital_Write(LCD_BL, 1);
}

UBYTE DEV_ModuleInit(void)
{
    /* Open GPIO chip */
    chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        printf("Failed to open gpiochip0\n");
        return 1;
    }

    /* Open SPI */
    spi_fd = open("/dev/spidev0.0", O_WRONLY);
    if (spi_fd < 0) {
        printf("Failed to open /dev/spidev0.0\n");
        return 1;
    }

    DEV_GPIO_Init();

    /* Start backlight */
    pthread_create(&bl_thread, NULL, bl_pwm_thread, NULL);

    return 0;
}

void DEV_SetBacklight(UWORD Value)
{
    if (Value > 100) Value = 100;
    bl_value = Value;
}

void DEV_ModuleExit(void)
{
    close(spi_fd);
    gpiod_chip_close(chip);
    exit(0);
}
