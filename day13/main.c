#include <stdio.h>

#include "config.h"
#include "sensor.h"
#include "alarm.h"

#define SENSOR_COUNT 4

int main(void)
{
    Sensor sensors[SENSOR_COUNT];

    AlarmRpm rpm_state[SENSOR_COUNT];

    AlarmConfig config;

    const char *filename = "config.ini";

    ConfigResult config_result = 
        config_load(&config, filename);

    printf("Config result: %s\n", 
        config_state_to_string(config_result));

    if (config_result != CONFIG_OK)
    {
        return 1;
    }

    //打印配置参数
    printf("Alarm_rpm: %.2f\n"
            "Recovery_rpm: %.2f\n"
            "Alarm_confirm: %d\n"
            "Recovery_confirm: %d\n",
            config.alarm_rpm,
            config.recovery_rpm,
            config.alarm_confirm,
            config.recovery_confirm);



    //初始化传感器
    for (int i = 0; i < SENSOR_COUNT; i++)
    {
        sensor_init(&sensors[i], i + 1);

        alarm_init(&rpm_state[i],
                    config.alarm_rpm,
                    config.recovery_rpm,
                    config.alarm_confirm,
                    config.recovery_confirm);
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

    for (int sample = 0; sample < sample_count; sample++)
    {
        for (int channel = 0; channel < SENSOR_COUNT; channel++)
        {
            sensor_set_rpm(&sensors[channel], 
                rpm_data[sample][channel]);

            AlarmRpmState old_alarm_state = 
                rpm_state[channel].state;   

            AlarmEvent alarm_event_state;

            alarm_update_state(&rpm_state[channel], 
                sensors[channel].average_rpm);

            AlarmRpmState new_alarm_state =
                rpm_state[channel].state;

            alarm_event_state = 
                alarm_get_event(old_alarm_state, new_alarm_state);
            
            if (alarm_event_state != ALARM_EVENT_NONE)
            {
                printf("CH%d %s\n",sensors[channel].channel,
                    event_state_to_string(alarm_event_state)                );
            }

            printf("CH%d RPM: %.2f Average: %.2f Alarm_count: %d "
                    "Recovery_count: %d State: %s\n",
                    sensors[channel].channel,
                    rpm_data[sample][channel],
                    sensors[channel].average_rpm,
                    rpm_state[channel].alarm_count,
                    rpm_state[channel].recovery_count,
                    alarm_state_to_string(new_alarm_state)
                    );
        }
    }

    return 0;
}