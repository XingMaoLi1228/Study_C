#include <stdio.h>
#include <windows.h>
#include <conio.h>

#include "config.h"
#include "sensor.h"
#include "alarm.h"
#include "acquisition.h"
#include "system.h"

#define SENSOR_COUNT 4


//定义具体故障枚举
typedef enum 
{
    ERROR_NONE,
    ERROR_ACQUISITION,
    ERROR_INVALID_RPM,
    ERROR_SIMULATED
} ErrorReason;

//故障原因转字符串
static char *error_reason_to_string(ErrorReason error_reason)
{
    switch (error_reason)
    {
        case ERROR_NONE:
            return "ERROR_NONE";

        case ERROR_ACQUISITION:
            return "ERROR_ACQUISITION";

        case ERROR_INVALID_RPM:
            return "ERROR_INVALID_RPM";

        case ERROR_SIMULATED:
            return "ERROR_SIMULATED";

        default:
            return "Unknown error";
    }
}

//通道初始化
static void channel_init(
    Acquisition *acquisition,
    Sensor *sensors, 
    AlarmRpm *rpm_alarm,
    AlarmConfig config
)
{
    if (acquisition == NULL ||
        sensors == NULL ||
        rpm_alarm == NULL
    )
    {
        return;
    }

    for (int channel = 0; channel < SENSOR_COUNT; channel++)
    {
        acquisition_init(&acquisition[channel], channel + 1);

        sensor_init(&sensors[channel], channel + 1);

        alarm_init(
            &rpm_alarm[channel],
            config.alarm_rpm,
            config.recovery_rpm,
            config.alarm_confirm,
            config.recovery_confirm
        );

    }
}

//采集一帧数据
static AcquisitionResult read_frame(
    Acquisition acquisition[],
    double rpm_frame[],
    int *failed_channel
)
{
    if (acquisition == NULL ||
        rpm_frame == NULL ||
        failed_channel == NULL
        )
    {
        return ACQ_ERROR_NULL;
    }

    *failed_channel = 0;

    for (int channel = 0; channel < SENSOR_COUNT; channel++)
    {
        AcquisitionResult result = 
            acquisition_read(
                &acquisition[channel],
                &rpm_frame[channel]
            );

        if (result != ACQ_OK)
        {
            *failed_channel = channel + 1;
            
            return result;
        }
    }

    return ACQ_OK;
}

//判断采集数据的有效性
static int rpm_is_valid(double rpm)
{
    if (
        rpm < 0.0 || rpm > 2600
    )
    {
        return 0;
    }

    return 1;
}


//处理一帧数据
static void process_frame(
    Sensor sensors[],
    AlarmRpm rpm_alarm[],
    const double rpm_frame[],
    int invalid_count[],
    int *error_flag,
    ErrorReason *error_reason
)
{
    if (sensors == NULL ||
        rpm_alarm == NULL ||
        rpm_frame == NULL ||
        invalid_count == NULL ||
        error_flag == NULL ||
        error_reason == NULL
        )
    {
        return;
    }

    for (int channel = 0; channel < SENSOR_COUNT; channel++)
    {

        if (!rpm_is_valid(rpm_frame[channel]))
        {
            invalid_count[channel]++;

            if (invalid_count[channel] >= 3)
            {
                *error_flag = 1;
                *error_reason = ERROR_INVALID_RPM;
            }

            printf(
                "CH%d invalid rpm:%.2f count:%d\n",
                channel + 1,
                rpm_frame[channel],
                invalid_count[channel]
            );

            continue;
        }

        invalid_count[channel] = 0;

        sensor_set_rpm(
            &sensors[channel],
            rpm_frame[channel]
        );

        alarm_update_state(
            &rpm_alarm[channel],
            sensors[channel].average_rpm
        );

        //打印
        printf(
            "CH%d RPM:%.2f Average:%.2f Alarm_count:%d Recovery_count:%d"
            "State:%s\n",
            sensors[channel].channel,
            rpm_frame[channel],
            sensors[channel].average_rpm,
            rpm_alarm[channel].alarm_count,
            rpm_alarm[channel].recovery_count,
            alarm_state_to_string(rpm_alarm[channel].state)
        );
    }

}


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

    ErrorReason error_reason = ERROR_NONE; 

    //无效转速计数
    int invalid_count[SENSOR_COUNT] = {0};

    //读取配置文件
    config_result = config_load(&config, filename);

    printf("Config state: %s\n", 
                config_state_to_string(config_result));

    if (config_result != CONFIG_OK)
    {
        return 1;
    }

    channel_init(
        acquisition,
        sensors,
        rpm_alarm,
        config
    );

    //测试，故意给CH3赋0
    acquisition[2].channel = 0;

    int error_flag = 0;
    system_state = SYSTEM_STOP;
    command = COMMAND_NONE;
    AcquisitionResult last_acquisition_error = ACQ_OK;

    ULONGLONG last_sample_time = GetTickCount64(); //GetTickCount64

    //循环采集处理
    while (1)
    {
        ULONGLONG now_time = GetTickCount64();


        command = input_command();

        if (command != COMMAND_NONE)
        {
            //退出
            if (command == COMMAND_EXIT)
            {
                printf("Program exit\n");
                break;
            }

            //模拟错误
            if (command == COMMAND_SIMULATE_ERROR)
            {
                if (error_flag == 0)
                {
                    error_flag = 1;
                    error_reason = ERROR_SIMULATED;
                }
            }

            //复位
            if (command == COMMAND_RESET)
            {
                //仅用于模拟，恢复CH3有效通道号
                //重新初始化所有状态
                channel_init(
                    acquisition,
                    sensors,
                    rpm_alarm,
                    config
                );

                //清空无效转速
                for (
                    int channel = 0; channel < SENSOR_COUNT; channel ++
                )
                {
                    invalid_count[channel] = 0;
                }

                //恢复错误信息
                error_reason = ERROR_NONE;

                last_acquisition_error = ACQ_OK;               
            }

            //保存旧的系统状态
            SystemState old_state1 = system_state;

            //更新系统状态
            system_state = system_update_state(
                system_state,
                command,
                &error_flag
            );

            if (old_state1 == SYSTEM_STOP &&
                system_state == SYSTEM_RUNNING)
            {
                last_sample_time = now_time;
            }

            printf(
                "System state: %s, reason: %s\n",
                system_state_to_string(system_state),
                error_reason_to_string(error_reason)
            );
        }

        if (now_time - last_sample_time >= 1000)
        {

            if (system_state == SYSTEM_RUNNING)
            {
                double rpm_frame[SENSOR_COUNT];
                int frame_ok = 1;
                int failed_channel = 0;
                
                //第一阶段，先集齐四个通道
                AcquisitionResult result = 
                    read_frame(
                        acquisition,
                        rpm_frame,
                        &failed_channel
                    );

                frame_ok = (result == ACQ_OK);

                if (!frame_ok)
                {
                    error_flag = 1;
                    error_reason = ERROR_ACQUISITION;

                    if (last_acquisition_error == ACQ_OK)
                    {
                        last_acquisition_error = result;
                    }

                    printf(
                        "CH%d %s\n",
                        failed_channel,
                        acquisition_result_to_string(result)
                    );
                }



                //第二阶段，四个通道全部成功，更新状态
                if (frame_ok)
                {
                    process_frame(
                        sensors,
                        rpm_alarm,
                        rpm_frame,
                        invalid_count,
                        &error_flag,
                        &error_reason
                    );
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



