#ifndef BAR_GRAPH_H
#define BAR_GRAPH_H

#include <stdint.h>
#include <stdbool.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "pin_defs.h"
#include "motor_driver.h"

LOG_MODULE_REGISTER(bar_graph);

/**
 * Initializes the GPIO pins for the bar graph LED based on the mode of the motor driver.
 * 
 * @brief Enables the bar graph LED's GPIO pins
 * 
 * @return int returns 0 if enabled correctly, and a negative error code if it failed to initialize
 */
int enable_bar_graph(void);

/**
 * Sets the value of the bar graph LED based on the data provided.
 * 
 * @brief Set the value of bar graph LED
 * 
 * @param data 8-bit binary in hexadecimal form, i.e., the pattern that is needed to be shown on the bar graph LED
 * @return int returns 0 on success, or a negative error code on failure
 */
int set_bar_graph(uint8_t data);

/**
 * Converts a boolean array of size 8 into an 8-bit unsigned integer
 * 
 * @param input_array boolean input array of size 8
 * @return uint8_t returns an 8-bit number formed after combining the boolean array
 */
uint8_t bool_to_uint8(bool *input_array);

#endif
