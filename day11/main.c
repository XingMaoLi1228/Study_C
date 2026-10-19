#include <stdio.h>

#include "sensor.h"
#include "alarm.h"
#include "config.h"

#define SAMPLE_COUNT 8

int main(void)
{
    Sensor sensor;
    Alarm alarm;
    AlarmConfig config;



    printf("file state:%s\n", config_state_to_string(
        config_load(&config, ".\\config\\config.txt")
    ));

    double rpm_data[SAMPLE_COUNT] =
    {
        800.0,
        900.0,
        1100.0,
        1200.0,
        1300.0,
        850.0,
        800.0,
        750.0
    };

    sensor_init(&sensor, 1);

    alarm_rpm_init(
        &alarm,
        config.alarm_rpm,
        config.recovery_rpm,
        config.alarm_confirm,
        config.recovery_confirm
    );

    printf("Alarm RPM: %.2f, Recovery RPM: %.2f, Alarm Confirm: %d, Recovery Confirm: %d\n",
            config.alarm_rpm,
            config.recovery_rpm,
            config.alarm_confirm,
            config.recovery_confirm);

    for (int i = 0; i < SAMPLE_COUNT; i++)
    {
        sensor_set_rpm(&sensor, rpm_data[i]);

        alarm_rpm_state(&alarm, sensor.average_rpm);

        printf("RPM: %.2f, Average: %.2f, Alarm_count: %d, STATE: %s\n",
                rpm_data[i],
                sensor.average_rpm,
                alarm.alarm_count,
                alarm_state_to_string(&alarm));
    }

    return 0;
}