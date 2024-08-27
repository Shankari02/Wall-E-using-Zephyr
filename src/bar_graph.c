#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(bar_graph, LOG_LEVEL_INF);

// Define the pins according to your hardware setup
#define BG_LED_1  DT_GPIO_PIN(DT_ALIAS(bg_leds), gpios)
#define BG_LED_2  DT_GPIO_PIN(DT_ALIAS(bg_leds), gpios)
#define BG_LED_3  DT_GPIO_PIN(DT_ALIAS(bg_leds), gpios)
#define BG_LED_4  DT_GPIO_PIN(DT_ALIAS(bg_leds), gpios)
#define BG_LED_5  DT_GPIO_PIN(DT_ALIAS(bg_leds), gpios)
#define BG_LED_6  DT_GPIO_PIN(DT_ALIAS(bg_leds), gpios)
#define BG_LED_7  DT_GPIO_PIN(DT_ALIAS(bg_leds), gpios)
#define BG_LED_8  DT_GPIO_PIN(DT_ALIAS(bg_leds), gpios)

// Define GPIO device for your specific hardware
static const struct device *gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpio0));

// Define pin mask for the two states
static const uint8_t bitmask[2] = {0xFF, 0x0F};

// Define pin output array
static const int pin_out[8] = {BG_LED_1, BG_LED_2, BG_LED_3, BG_LED_4, BG_LED_5, BG_LED_6, BG_LED_7, BG_LED_8};

// Function to get motor driver status
static int get_motor_driver_status(void)
{
    // Replace with actual motor driver status check
    return 0;  // Example: 0 means motor driver A is off
}

int enable_bar_graph(void)
{
    if (!device_is_ready(gpio_dev)) {
        LOG_ERR("GPIO device is not ready");
        return -ENODEV;
    }

    uint64_t bit_mask = 0;
    int status = get_motor_driver_status();

    if (status == 0) {
        bit_mask = (1ULL << BG_LED_1) | (1ULL << BG_LED_2) | (1ULL << BG_LED_3) | (1ULL << BG_LED_4) | (1ULL << BG_LED_5) | (1ULL << BG_LED_6) | (1ULL << BG_LED_7) | (1ULL << BG_LED_8);
        enabled_bar_graph_flag = 0;
    } else if (status == 1) {
        bit_mask = (1ULL << BG_LED_5) | (1ULL << BG_LED_6) | (1ULL << BG_LED_7) | (1ULL << BG_LED_8);
        enabled_bar_graph_flag = 1;
    }

    // Configure the GPIO pins
    for (int i = 0; i < 8; i++) {
        if ((bit_mask & (1ULL << pin_out[i])) != 0) {
            gpio_pin_configure(gpio_dev, pin_out[i], GPIO_OUTPUT);
        } else {
            gpio_pin_configure(gpio_dev, pin_out[i], GPIO_DISCONNECTED);
        }
    }

    LOG_INF("Enabled bar graph LEDs in mode: %d", enabled_bar_graph_flag);
    return 0;
}

int set_bar_graph(uint8_t data)
{
    uint8_t mode_bitmask = bitmask[enabled_bar_graph_flag];

    for (int i = 0; i < 8; i++) {
        if ((mode_bitmask & 0x80) == 0x80) {
            if ((data & 0x80) == 0x80) {
                gpio_pin_set(gpio_dev, pin_out[i], 1);
            } else {
                gpio_pin_set(gpio_dev, pin_out[i], 0);
            }
        }
        mode_bitmask <<= 1;
        data <<= 1;
    }

    return 0;
}

uint8_t bool_to_uint8(bool *input_array)
{
    return ((input_array[0] << 7) | (input_array[1] << 6) | (input_array[2] << 5) | (input_array[3] << 4) | (input_array[4] << 3) | (input_array[5] << 2) | (input_array[6] << 1) | input_array[7]);
}
