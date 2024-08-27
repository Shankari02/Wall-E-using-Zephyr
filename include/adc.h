#ifndef _ADC_ZEPHYR_H
#define _ADC_ZEPHYR_H

#include <zephyr.h>
#include <drivers/adc.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(adc_zephyr);

// Example resolution and attenuation, adjust according to your ADC specifications
#define ADC_RESOLUTION 12
#define ADC_GAIN ADC_GAIN_1
#define ADC_REFERENCE ADC_REF_INTERNAL

// Define ADC channels based on your hardware setup
#define ADC_CHANNEL_COUNT 5

/**
 * @brief Structure representing an ADC object.
 * 
 * This structure holds the device and channel configuration for an ADC.
 */
typedef struct
{
    const struct device *adc_dev; /**< Device structure for ADC. */
    struct adc_channel_cfg channel_cfg[ADC_CHANNEL_COUNT]; /**< Array of channel configurations. */
    bool do_calib[ADC_CHANNEL_COUNT]; /**< Array indicating whether calibration should be performed for each channel. */
} adc_obj_t;

typedef adc_obj_t *adc_handle_t;

/**
 * @brief Configures the ADC device and channels.
 * 
 * @param adc_obj - Pointer to the ADC object.
 * @return int - returns 0 if successful, or an error code if configuration fails.
 **/
int config_adc(adc_obj_t *adc_obj);

/**
 * @brief Initializes and calibrates the ADC.
 * 
 * @param adc_obj - Pointer to the ADC object.
 * @return int - returns 0 if successful, or an error code if initialization fails.
 **/
int calib_init(adc_obj_t *adc_obj);

/**
 * @brief Enables the ADC and performs initialization.
 * 
 * @param adc_obj - Pointer to a pointer to the ADC object.
 * @return int - returns 0 if successful, or an error code if initialization fails.
 **/
int enable_adc(adc_handle_t *adc_obj);

/**
 * @brief Reads the ADC value from the specified channel.
 * 
 * @param adc_handle - Pointer to the ADC object.
 * @param channel - Channel number to be read.
 * @return int - returns the ADC value read from the channel.
 **/
int read_adc(adc_handle_t adc_handle, int channel);

#endif // _ADC_ZEPHYR_H
