#include <stdio.h>
#include <windows.h>

#include "config.h"
#include "sensor.h"
#include "alarm.h"
#include "acquisition.h"
#include "system.h"

#define SENSOR_COUNT 4

int main(void)

{
    Acquisition acquisition[SENSOR_COUNT];
    Sensor sensors[SENSOR_COUNT];
    AlarmRpm rpm_alarm[SENSOR_COUNT];
    SystemState system_state;

    AlarmConfig config;
    const char *filename = "config.ini";
    ConfigResult config_result;
    Command command;

    //读取配置文件
    config_result = config_load(&config, filename);

    printf("Config state: %s\n", 
                config_state_to_string(config_result));

    if (config_result != CONFIG_OK)
    {
        return 1;
    }

    //初始化参数
    for (int channel = 0; channel < SENSOR_COUNT; channel++)
    {
        //初始化采集
        acquisition_init(&acquisition[channel], channel + 1);

        //初始化传感器
        sensor_init(&sensors[channel], channel + 1);

        //初始化报警状态
        alarm_init(&rpm_alarm[channel], 
                    config.alarm_rpm,
                    config.recovery_rpm,
                    config.alarm_confirm,
                    config.recovery_confirm);
    }

    int running = 1;
    int error_flag = 0;
    system_state = SYSTEM_STOP;
    command = COMMAND_NONE;

    //循环采集处理
    while (!error_flag)
    {
        command = input_command();

        system_state = system_update_state(
            system_state,
            command,
            error_flag
        );

        printf(system_state_to_string(system_state));
        printf("\n");

        if (system_state == SYSTEM_RUNNING)
        {
            for (int channel = 0; channel < SENSOR_COUNT; channel++)
            {
                double rpm;

                //采集数据
                rpm = acquisition_read(&acquisition[channel]);

                sensor_set_rpm(&sensors[channel], rpm);

                alarm_update_state(&rpm_alarm[channel], 
                    sensors[channel].average_rpm);

                printf("CH%d RPM:%.2f "
                        "Average:%.2f "
                        "Alarm_count: %d "
                        "Recovery_count: %d "
                        "State: %s\n",
                        sensors[channel].channel,
                        rpm,
                        sensors[channel].average_rpm,
                        rpm_alarm[channel].alarm_count,
                        rpm_alarm[channel].recovery_count,
                        alarm_state_to_string(rpm_alarm[channel].state));
            }
        }

        Sleep(1000);
    }

    return 0;
}