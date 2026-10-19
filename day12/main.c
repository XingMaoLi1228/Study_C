#include <stdio.h>

#include "alarm.h"
#include "sensor.h"
#include "config.h"

#define SENSOR_COUNT 4

int main(void)
{
    Sensor sensor[SENSOR_COUNT];

    AlarmRpm rpm_alarm[SENSOR_COUNT];

    AlarmConfig alarm_config;

    ConfigResult config_result = 
        config_load(&alarm_config, "config.txt");


    printf("File State:%s\n",
                config_state_to_string(config_result));

    if (config_result != CONFIG_OK)
    {
        return 1;
    }


    double rpm_data[][SENSOR_COUNT] =
    {
        {800, 900, 1100, 1500},
        {820, 960, 1200, 1600},
        {850, 970, 1300, 1700},
        {870, 980, 1400, 1800},
        {850, 950, 700, 700},
        {830, 930, 700, 700},
        {810, 910, 700, 700},
        {800, 890, 700, 700},
        {790, 880, 700, 700},
        {780, 870, 700, 700},
        {770, 860, 700, 700}
    };

    int sample_count = sizeof(rpm_data) / sizeof(rpm_data[0]);

    for (int i = 0; i < SENSOR_COUNT; i++)
    {
        //初始化多个传感器
        sensor_init(&sensor[i], i + 1);
        alarm_init(&rpm_alarm[i],
                    alarm_config.alarm_rpm,
                    alarm_config.recovery_rpm,
                    alarm_config.alarm_confirm,
                    alarm_config.recovery_confirm);
    }

    for (int sample = 0; sample < sample_count; sample++)
    {
        for (int channel = 0; channel < SENSOR_COUNT; channel++)
        {
            sensor_set_rpm(&sensor[channel], rpm_data[sample][channel]);

            alarm_update(&rpm_alarm[channel], 
                sensor[channel].average_rpm);
            
            printf("CH%d RPM: %.2f Average: %.2f ALARM_count: %d Recovery_count: %d ALARM_state: %s\n",
                sensor[channel].channel,
                rpm_data[sample][channel],
                sensor[channel].average_rpm,
                rpm_alarm[channel].alarm_count,
                rpm_alarm[channel].recovery_count,
                alarm_state_to_string(rpm_alarm[channel].state)
        );

        }
    }

    return 0;
}
