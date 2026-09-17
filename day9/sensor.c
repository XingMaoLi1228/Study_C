#include <stdio.h>
#include "sensor.h"

//初始化传感器
void sensor_init(Sensor *sensor, int channel)
{
    if (sensor == NULL)
    {
        return;
    }

    sensor->channel = channel;
    
    for (int i = 0; i < HISTORY_SIZE; i++)
    {
        sensor->rpm_buffer[i] = 0.0;
    }

    sensor->current_rpm = 0.0;

    sensor->alarm_count = 0;
    sensor->recovery_count = 0;

    sensor->average_rpm = 0.0;
    sensor->rpm_sum = 0.0;

    sensor->valid_count = 0;
    sensor->write_index = 0;

    sensor->state = STATE_STOP;
}


//清空计数
static void clean_state_count(Sensor *sensor)
{
    if (sensor == NULL)
    {
        return;
    }

    sensor->alarm_count = 0;
    sensor->recovery_count = 0;
}

//更新传感器状态
static void sensor_update_state(Sensor *sensor)
{
    if (sensor == NULL)
    {
        return;
    }

    if (sensor->state == STATE_ERROR)
    {
        return;
    }

    if (sensor->average_rpm == 0)
    {
        sensor->state = STATE_STOP;

        clean_state_count(sensor);

        return;
    }

    switch (sensor->state)
    {
        case STATE_STOP:

            sensor->state = STATE_RUNNING;

            clean_state_count(sensor);

            break;

        case STATE_ALARM:

            if (sensor->average_rpm <= 
                RECOVERY_RPM)
            {
                sensor->recovery_count++;
                sensor->alarm_count = 0;

                if (sensor->recovery_count >=
                     RECOVERY_CONFIRM)
                {
                    sensor->state = STATE_RUNNING;

                    clean_state_count(sensor);
                }
            }
            else
            {
                clean_state_count(sensor);
            }

            break;
        
        case STATE_RUNNING:
            
            if (sensor->average_rpm >= ALARM_RPM)
            {
                sensor->alarm_count++;
                sensor->recovery_count = 0;

                if (sensor->alarm_count >=
                     ALARM_CONFIRM)
                {
                    sensor->state = STATE_ALARM;

                    clean_state_count(sensor);                 
                }
            }
            else
            {
                clean_state_count(sensor);
            }

            break;

        default:
            break;
    }
    
}

//更新传感器buffer
static void sensor_update_buffer(Sensor *sensor, double rpm)
{
    if (sensor == NULL)
    {
        return;
    }

    if (sensor->valid_count < HISTORY_SIZE)
    {
        sensor->rpm_buffer[sensor->write_index] =
            rpm;

        sensor->write_index++;

        sensor->rpm_sum += rpm;
    }
    else
    {
        sensor->rpm_sum =
            sensor->rpm_buffer[sensor->write_index];

        sensor->rpm_buffer[sensor->write_index] =
            rpm;

        sensor->rpm_sum += rpm;
    }

    sensor->write_index++;

    if (sensor->write_index >= HISTORY_SIZE)
    {
        sensor->write_index = 0;
    }

    sensor->average_rpm =
        sensor->rpm_sum / sensor->valid_count;

}

//加入新的转速值
void sensor_set_rpm(Sensor *sensor, double rpm)
{
    if (sensor == NULL)
    {
        return;
    }

    sensor->current_rpm = rpm;

    if (rpm < 0.0)
    {
        sensor->state = STATE_ERROR;
        return;
    }
    
    //更新转速buffer
    sensor_update_buffer(sensor, rpm);

    //更新状态
    sensor_update_state(sensor);
}


//枚举状态转成字符串
static const char *state_to_string(DeviceState state)
{
    switch (state)
    {
        case STATE_ERROR:
            return "ERROR";

        case STATE_RUNNING:
            return "RUNNING";

        case STATE_ALARM:
            return "ALARM";

        case STATE_STOP:
            return "STOP";

        default:
            return "Unknown";
    }
}

//打印传感器状态
void sensor_print(const Sensor *sensor)
{
    if (sensor == NULL)
    {
        return;
    }

    if (sensor->valid_count == 0)
    {
        printf(
            "CH%d no valid data, State:%s\n",
            sensor->channel,
            sensor->state
        );
    }


    printf(
        "CH%d Now_RPM: %.2f, Average_RPM: %.2f, Alarm_count: %d. Recovery_count: %d, State: %s\n",
        sensor->channel,
        sensor->current_rpm,
        sensor->average_rpm,
        sensor->alarm_count,
        sensor->recovery_count,
        state_to_string(sensor->state)
    );

}

