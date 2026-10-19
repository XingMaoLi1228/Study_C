#include <stdio.h>
#include <windows.h>
#include <conio.h>

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

    //测试，故意给CH3赋0
    acquisition[2].channel = 0;

    int error_flag = 0;
    system_state = SYSTEM_STOP;
    command = COMMAND_NONE;
    int sample_count = 0;
    AcquisitionResult last_acquisition_error = ACQ_OK;

    ULONGLONG last_sample_time = GetTickCount64(); //GetTickCount64

    //循环采集处理
    while (1)
    {
        ULONGLONG now_time = GetTickCount64();


        command = input_command();

        if (command != COMMAND_NONE)
        {
            if (command == COMMAND_EXIT)
            {
                printf("Program exit\n");
                break;
            }

            if (command == COMMAND_SIMULATE_ERROR)
            {
                error_flag = 1;
            }

            if (command == COMMAND_RESET)
            {
                last_acquisition_error = ACQ_OK;
            }

            system_state = system_update_state(
                system_state,
                command,
                &error_flag
            );

            printf(system_state_to_string(system_state));
            printf("\n");

        }

        if (now_time - last_sample_time >= 1000)
        {

            if (system_state == SYSTEM_RUNNING)
            {
                    //循环采集
                    for (int channel = 0; channel < SENSOR_COUNT; channel++)
                    {
                        double rpm;
                        AcquisitionResult acquisition_result;

                        //采集数据
                        acquisition_result = acquisition_read(&acquisition[channel], &rpm);

                        if (acquisition_result != ACQ_OK)
                        {
                            error_flag = 1;

                            if (last_acquisition_error == ACQ_OK)
                            {
                                last_acquisition_error = acquisition_result;
                            }

                            printf("CH%d %s\n",
                                sensors[channel].channel,
                                acquisition_result_to_string(acquisition_result));
                        }
                        else
                        {
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

                    //更新系统状态
                    if (error_flag)
                    {
                        SystemState old_state = system_state;
                        
                        system_state = system_update_state(
                            old_state,
                            COMMAND_NONE,
                            &error_flag
                        );

                        printf("Acquisition failed: %s -> %s\n",
                                system_state_to_string(old_state),
                                system_state_to_string(system_state)
                        );
                    }

                    last_sample_time = now_time;
            }
        }
        
        Sleep(10);
    }



    return 0;
}

