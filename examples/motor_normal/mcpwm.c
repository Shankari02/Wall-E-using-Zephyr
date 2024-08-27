#include <zephyr.h>
#include <device.h>
#include <drivers/motor.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(MOTOR, LOG_LEVEL_INF);

#define MOTOR_SPEED 60
#define MOTOR_DELAY_MS 1000

void motor_thread(void)
{
    struct device *motor1 = device_get_binding(DT_LABEL(DT_NODELABEL(motor1)));
    if (motor1 == NULL) {
        LOG_ERR("Motor device not found");
        return;
    }

    // Assuming the driver provides an API similar to the one in FreeRTOS
    enable_motor_driver(motor1, MOTOR_A_1);

    while (1) {
        set_motor_speed(motor1, MOTOR_FORWARD, MOTOR_SPEED);
        LOG_INF("Forward");
        k_sleep(K_MSEC(MOTOR_DELAY_MS));

        set_motor_speed(motor1, MOTOR_BACKWARD, MOTOR_SPEED);
        LOG_INF("Backward");
        k_sleep(K_MSEC(MOTOR_DELAY_MS));

        set_motor_speed(motor1, MOTOR_STOP, 0);
        LOG_INF("Stop");
        k_sleep(K_MSEC(MOTOR_DELAY_MS));
    }
}

K_THREAD_DEFINE(motor_thread_id, 1024, motor_thread, NULL, NULL, NULL, 7, 0, 0);
