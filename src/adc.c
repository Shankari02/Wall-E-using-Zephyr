#include <zephyr.h>
#include <device.h>
#include <drivers/adc.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(adc_example, LOG_LEVEL_INF);

#define ADC_SAMPLING_RATE 100 // Adjust as needed
#define ADC_RESOLUTION ADC_RESOLUTION_12BIT

static const int adc_channels[5] = {0, 1, 2, 3, 4}; // Example ADC channels

typedef struct {
    const struct device *dev;
    struct adc_channel_cfg channel_cfg;
} adc_obj_t;

static int config_adc(adc_obj_t *adc_obj)
{
    if (!adc_obj || !adc_obj->dev) {
        LOG_ERR("ADC device is not initialized");
        return -EINVAL;
    }

    for (size_t i = 0; i < ARRAY_SIZE(adc_channels); i++) {
        adc_obj->channel_cfg.channel_id = adc_channels[i];
        adc_obj->channel_cfg.sampling_delay = K_MSEC(ADC_SAMPLING_RATE);
        adc_obj->channel_cfg.gain = ADC_GAIN_1;
        adc_obj->channel_cfg.reference = ADC_REF_INTERNAL;
        adc_obj->channel_cfg.input_positive = adc_channels[i];

        if (adc_channel_setup(adc_obj->dev, &adc_obj->channel_cfg) < 0) {
            LOG_ERR("Failed to configure ADC channel %d", adc_channels[i]);
            return -EIO;
        }
    }

    return 0;
}

static int calib_init(adc_obj_t *adc_obj)
{
    // Placeholder for calibration if needed
    return 0;
}

int enable_adc(adc_obj_t **adc_obj)
{
    if (!adc_obj) {
        return -EINVAL;
    }

    *adc_obj = k_malloc(sizeof(adc_obj_t));
    if (*adc_obj == NULL) {
        LOG_ERR("Memory allocation failed for ADC object");
        return -ENOMEM;
    }

    (*adc_obj)->dev = device_get_binding(DT_LABEL(DT_NODELABEL(adc)));
    if (!(*adc_obj)->dev) {
        LOG_ERR("ADC device not found");
        k_free(*adc_obj);
        return -ENODEV;
    }

    (*adc_obj)->channel_cfg.gain = ADC_GAIN_1;
    (*adc_obj)->channel_cfg.reference = ADC_REF_INTERNAL;
    (*adc_obj)->channel_cfg.acquisition_time = ADC_ACQ_TIME_DEFAULT;

    int ret = config_adc(*adc_obj);
    if (ret != 0) {
        k_free(*adc_obj);
        return ret;
    }

    ret = calib_init(*adc_obj);
    if (ret != 0) {
        k_free(*adc_obj);
        return ret;
    }

    return 0;
}

int read_adc(adc_obj_t *adc_obj, int channel)
{
    if (!adc_obj || !adc_obj->dev) {
        LOG_ERR("ADC device is not initialized");
        return -EINVAL;
    }

    uint16_t sample_buffer;
    struct adc_sequence sequence = {
        .channels = BIT(channel),
        .buffer = &sample_buffer,
        .buffer_size = sizeof(sample_buffer),
        .resolution = ADC_RESOLUTION,
    };

    int ret = adc_read(adc_obj->dev, &sequence);
    if (ret < 0) {
        LOG_ERR("Failed to read ADC value from channel %d", channel);
        return ret;
    }

    return sample_buffer; // Return the raw ADC value
}
