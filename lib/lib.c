#include "lib.h"
#include <gpiod.h> // gpio
#include <fcntl.h> // spi open
#include <errno.h> // errno
#include <string.h> // strerror()

#define GPIO_CHIP "/dev/gpiochip0"
#define CONSUMER  "homelabDisplay"

static struct gpiod_chip *chip;

static struct gpiod_line_request *output_request; // LCD_RST, LCD_DC, LCD_BL
static struct gpiod_line_request *button_request; // KEY_PRESS, KEY1, KEY2, KEY3
static struct gpiod_edge_event_buffer *event_buffer;

static int spi_fd = -1;

static const unsigned int output_offsets[] = { LCD_RST, LCD_DC, LCD_BL };
#define OUTPUT_COUNT ((size_t)(sizeof(output_offsets) / sizeof(output_offsets[0])))

static const unsigned int button_offsets[] = { KEY_PRESS_PIN, KEY1_PIN, KEY2_PIN, KEY3_PIN };
#define BUTTON_COUNT ((size_t)(sizeof(button_offsets) / sizeof(button_offsets[0])))
#define EVENT_BUF_CAP 8

void DEV_Digital_Write(UWORD Pin, UBYTE Value) {
    if (Pin == LCD_CS) {
        return;
    };

    if (!output_request) {
        printf("DEV_Digital_Write: outputs not initialized\n");
        return;
    };

    enum gpiod_line_value value = Value ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
    if (gpiod_line_request_set_value(output_request, Pin, value) < 0) {
        printf("DEV_Digital_Write: failed to set line %u: %s\n", Pin, strerror(errno));
    };
};

int DEV_Button_Read(UWORD Pin) {
    if (!button_request) {
        return 1; // treat as released
    };

    enum gpiod_line_value value = gpiod_line_request_get_value(button_request, Pin);
    return (value == GPIOD_LINE_VALUE_INACTIVE) ? 0 : 1;
};

void DEV_Button_Init() {
    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    struct gpiod_request_config *req_cfg = gpiod_request_config_new();

    if (!chip) {
        printf("DEV_Button_Init: chip not initialized\n");
        goto cleanup;
    };
    if (!settings || !line_cfg || !req_cfg) {
        printf("DEV_Button_Init: allocation failed\n");
        goto cleanup;
    };

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);
    gpiod_line_settings_set_edge_detection(settings, GPIOD_LINE_EDGE_BOTH);
    gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_PULL_UP);
    gpiod_line_settings_set_debounce_period_us(settings, 10000); // 10ms hardware debounce

    if (gpiod_line_config_add_line_settings(line_cfg, button_offsets, BUTTON_COUNT, settings) < 0) {
        printf("DEV_Button_Init: failed to add line settings: %s\n", strerror(errno));
        goto cleanup;
    };

    gpiod_request_config_set_consumer(req_cfg, CONSUMER);

    button_request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
    if (!button_request) {
        printf("DEV_Button_Init: failed to request buttons: %s\n", strerror(errno));
        goto cleanup;
    };

    event_buffer = gpiod_edge_event_buffer_new(EVENT_BUF_CAP);
    if (!event_buffer) {
        printf("DEV_Button_Init: failed to allocate event buffer: %s\n", strerror(errno));
    };

cleanup:
    if (req_cfg) gpiod_request_config_free(req_cfg);
    if (line_cfg) gpiod_line_config_free(line_cfg);
    if (settings) gpiod_line_settings_free(settings);
};

int DEV_Button_WaitPress(int timeout_ms) {
    if (!button_request || !event_buffer) {
        return -1;
    };

    int64_t timeout_ns = (timeout_ms < 0) ? -1 : (int64_t)timeout_ms * 1000000;
    if (gpiod_line_request_wait_edge_events(button_request, timeout_ns) <= 0) {
        return -1; // timeout or error
    };

    int read = gpiod_line_request_read_edge_events(button_request, event_buffer, EVENT_BUF_CAP);
    if (read < 0) {
        return -1;
    };

    for (int i = 0; i < read; i++) {
        struct gpiod_edge_event *event = gpiod_edge_event_buffer_get_event(event_buffer, (unsigned long)i);
        if (!event) {
            continue;
        };

        if (gpiod_edge_event_get_event_type(event) == GPIOD_EDGE_EVENT_FALLING_EDGE) {
            return (int)gpiod_edge_event_get_line_offset(event);
        };
    };

    return -1; // only release edges this time
};

void DEV_Delay_ms(UDOUBLE xms) {
    usleep(xms * 1000);
};

void DEV_SPI_WriteByte(uint8_t Value) {
    if (spi_fd < 0) {
        printf("DEV_SPI_WriteByte: SPI not initialized\n");
        return;
    };

    ssize_t written = write(spi_fd, &Value, 1);
    if (written != 1) {
        printf("DEV_SPI_WriteByte: write failed (ret=%zd)\n", written);
    };
};

void DEV_SPI_Write_nByte(uint8_t *pData, uint32_t Len) {
    if (spi_fd < 0) {
        printf("DEV_SPI_Write_nByte: SPI not initialized\n");
        return;
    };

    ssize_t written = write(spi_fd, pData, Len);
    if (written < 0) {
        printf("DEV_SPI_Write_nByte: write failed\n");
    } else if ((uint32_t)written != Len) {
        printf("DEV_SPI_Write_nByte: partial write (%zd/%u)\n", written, Len);
    };
};

static struct gpiod_line_request *request_output_lines() {
    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    struct gpiod_request_config *req_cfg = gpiod_request_config_new();
    struct gpiod_line_request *request = NULL;

    if (!settings || !line_cfg || !req_cfg) {
        goto cleanup;
    };

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);

    if (gpiod_line_config_add_line_settings(line_cfg, output_offsets, OUTPUT_COUNT, settings) < 0) {
        goto cleanup;
    };

    gpiod_request_config_set_consumer(req_cfg, CONSUMER);

    request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);

cleanup:
    if (req_cfg) gpiod_request_config_free(req_cfg);
    if (line_cfg) gpiod_line_config_free(line_cfg);
    if (settings) gpiod_line_settings_free(settings);
    return request;
};

UBYTE DEV_ModuleInit() {
    // GPIO
    chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        printf("Failed to open %s: %s\n", GPIO_CHIP, strerror(errno));
        return 1;
    };

    output_request = request_output_lines();
    if (!output_request) {
        printf("Failed to request output lines: %s\n", strerror(errno));
        gpiod_chip_close(chip);
        chip = NULL;
        return 1;
    };

    // SPI
    spi_fd = open("/dev/spidev0.0", O_WRONLY);
    if (spi_fd < 0) {
        printf("Failed to open /dev/spidev0.0: %s\n", strerror(errno));
        gpiod_line_request_release(output_request);
        output_request = NULL;
        gpiod_chip_close(chip);
        chip = NULL;
        return 1;
    };

    DEV_Digital_Write(LCD_BL, 1); // Backlight on
    return 0;
};

void DEV_ModuleExit() {
    if (spi_fd >= 0) {
        close(spi_fd);
        spi_fd = -1;
    };
    if (event_buffer) {
        gpiod_edge_event_buffer_free(event_buffer);
        event_buffer = NULL;
    };
    if (button_request) {
        gpiod_line_request_release(button_request);
        button_request = NULL;
    };
    if (output_request) {
        gpiod_line_request_release(output_request);
        output_request = NULL;
    };
    if (chip) {
        gpiod_chip_close(chip);
        chip = NULL;
    };
    exit(0);
};
