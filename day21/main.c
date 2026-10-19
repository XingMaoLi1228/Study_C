#include <stdio.h>
#include <windows.h>

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
    ERROR_SIMULATED,
    ERROR_PROCESSING
} ErrorReason;

//一帧数据处理结果
typedef enum 
{
    PROCESS_OK,
    PROCESS_ERROR_NULL
} ProcessResult;

//故障原因转字符串
static const char *error_reason_to_string(ErrorReason error_reason)
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

        case ERROR_PROCESSING:
            return "ERROR_PROCESSing";

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

//根据数组下标安全读取一个通道
static AcquisitionResult read_channel_by_index(
    Acquisition acquisition[],
    int acquisition_length,
    int channel_index,
    double *rpm
)
{
    //检查指针
    if (acquisition == NULL || rpm == NULL)
    {
        return ACQ_ERROR_NULL;
    }

    //检查数组长度
    if (acquisition_length < SENSOR_COUNT)
    {
        return ACQ_ERROR_LENGTH;
    }

    //检查数组下标
    if (channel_index < 0 ||
        channel_index >= SENSOR_COUNT ||
        channel_index >= acquisition_length
    )
    {
        return ACQ_ERROR_CHANNEL;
    }

    return acquisition_read(
            &acquisition[channel_index],
            rpm
    );
}

//采集一帧数据
static AcquisitionResult read_frame(
    Acquisition acquisition[],
    int acquisition_length, //采集对象数组的元素数量
    double rpm_frame[],
    int rpm_frame_length,  //转速数据数组的长度
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

    if (
        acquisition_length < SENSOR_COUNT ||
        rpm_frame_length < SENSOR_COUNT
    )
    {
        return ACQ_ERROR_LENGTH;
    }

    *failed_channel = 0;

    for (int channel = 0; channel < SENSOR_COUNT; channel++)
    {
        AcquisitionResult result = 
            read_channel_by_index(
                acquisition,
                acquisition_length,
                channel,
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

//处理结果转字符串
static const char *process_result_to_string(
    ProcessResult result
)
{
    switch (result)
    {
        case PROCESS_OK:
            return "PROCESS_OK";

        case PROCESS_ERROR_NULL:
            return "PROCESS_ERROR_NULL";

        default:
            return "Unknown process result";
    }
}

//处理一帧数据
static ProcessResult process_frame(
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
        return PROCESS_ERROR_NULL;
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

    return PROCESS_OK;

}


int main(void)

{
    //模拟采集变量
    Acquisition acquisition[SENSOR_COUNT];

    //传感器变量
    Sensor sensors[SENSOR_COUNT];

    //转速报警变量
    AlarmRpm rpm_alarm[SENSOR_COUNT];

    //系统状态变量
    SystemState system_state;

    //配置变量
    AlarmConfig config;

    //文件名变量
    const char *filename = "config.ini";

    //配置结果变量
    ConfigResult config_result;

    //命令变量
    Command command;

    //错误结果变量
    ErrorReason error_reason = ERROR_NONE; 

    //无效转速计数
    int invalid_count[SENSOR_COUNT] = {0};

    //读取配置文件
    config_result = config_load(&config, filename);

    //打印配置状态
    printf("Config state: %s\n", 
                config_state_to_string(config_result));

    if (config_result != CONFIG_OK)
    {
        return 1;
    }

    //初始化通道
    channel_init(
        acquisition,
        sensors,
        rpm_alarm,
        config
    );

    //测试，故意给CH3赋0
    // acquisition[2].channel = 0;

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
                        (int)(sizeof(acquisition) / sizeof(acquisition[0])),
                        rpm_frame,
                        (int)(sizeof(rpm_frame) / sizeof(rpm_frame[0])),
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
                    ProcessResult process_result = process_frame(
                        sensors,
                        rpm_alarm,
                        rpm_frame,
                        invalid_count,
                        &error_flag,
                        &error_reason
                    );

                    //检查返回状态
                    if (process_result != PROCESS_OK)
                    {
                        error_flag = 1;
                        error_reason = ERROR_PROCESSING;

                        //打印处理失败结果
                        printf(
                            "Process faild: %s\n",
                            process_result_to_string(
                                process_result
                            )
                        );
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
 
                    printf("Acquisition failed: %s -> %s, reason: %s\n",
                            system_state_to_string(old_state),
                            system_state_to_string(system_state),
                            error_reason_to_string(error_reason)
                    );
                }

                last_sample_time = now_time;
            }
        }
        
        Sleep(10);
    }

    return 0;
}


