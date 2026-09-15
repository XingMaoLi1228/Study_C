// < >  → 系统/编译器提供的头文件
// " "  → 我自己项目里的头文件
#include <stdio.h>
#include <stdlib.h>

#include "sensor.h"

//生成随机数
double generate_random_rpm(double min, double max)
{
    double ratio = (double)rand() / RAND_MAX;

    return min + ratio * (max - min);
}

//初始化传感器
void sensor_init(Sensor *sensor, int channel)
{
    if (sensor == NULL || channel < 0)
    {
        return;
    }

    sensor->channel = channel;

    for (int i = 0; i < HISTORY_SIZE; i++)
    {
        sensor->rpm_buffer[i] = 0.0;
    }

    sensor->alarm_count = 0;
    sensor->recovery_count = 0;

    sensor->valid_count = 0;
    sensor->write_index = 0;

    sensor->average_rpm = 0;

    sensor->state = STATE_NORMAL;
}


//清除计数器
static void reset_counts(Sensor *sensor)
{
    if (sensor == NULL)
    {
        return;
    }

    sensor->alarm_count = 0;
    sensor->recovery_count = 0;
}

//把RPM写入循环缓冲区
void update_buffer(Sensor *sensor, double new_rpm)
{
    if (sensor == NULL)
    {
        return;
    }

    if (new_rpm < 0.0)
    {
        sensor->state = STATE_ERROR;

        return;
    }

    sensor->rpm_buffer[sensor->write_index] = new_rpm;

    sensor->write_index = 
        (sensor->write_index + 1) % HISTORY_SIZE;

    if (sensor->valid_count < HISTORY_SIZE)
    {
        sensor->valid_count++;
    }
}

//计算平均
double calculate_average(Sensor *sensor)
{
    if (sensor == NULL || sensor->valid_count <= 0)
    {
        return 0.0;
    }

    double sum = 0.0;

    for (int i = 0; i < sensor->valid_count; i++)
    {
        sum += sensor->rpm_buffer[i];
    }

    return sum / sensor->valid_count;
}

//更新报警状态
void update_state(Sensor *sensor)
{
    if (sensor == NULL)
    {
        return;
    }

    switch (sensor->state)
    {
        case STATE_ERROR:

            if (sensor->average_rpm >= ALARM_RPM)
            {
                sensor->state = STATE_ALARM;

                sensor->alarm_count++;

                sensor->recovery_count = 0;
            }
            
            if (sensor->average_rpm <= RECOVERY_RPM)
            {
                sensor->state = STATE_NORMAL;

                sensor->recovery_count++;

                sensor->alarm_count = 0;
            }
            break;
        
        case STATE_NORMAL:
            
            if (sensor->average_rpm < ALARM_RPM)
            {
                sensor->alarm_count = 0;

                break;
            }
            else
            {
                sensor->alarm_count++;

                if (sensor->alarm_count >= ALARM_CONFIRM)
                {
                    sensor->state = STATE_ALARM;
                    
                    reset_counts(sensor);
                }
            }

            break;
        
        case STATE_ALARM:

            if (sensor->average_rpm <= RECOVERY_RPM)
            {
                sensor->recovery_count++;

                if (sensor->recovery_count >= RECOVERY_CONFIRM)
                {
                    sensor->state = STATE_NORMAL;

                    reset_counts(sensor);
                }
            }
            else
            {
                sensor->recovery_count = 0;
            }

            break;

        default:
            break;
    }

}

//状态转字符串
static const char *state_to_string(DeviceState state)
{
    switch (state)
    {
        case STATE_ALARM:
            return "ALARM";
        
        case STATE_ERROR:
            return "ERROR";

        case STATE_NORMAL:
            return "NORMAL";

        default:
            return "Unknown";
    }
}

//打印传感器
void print_sensor(const Sensor *sensor)
{
    if (sensor == NULL)
    {
        return;
    }

    printf("CH%d AVG_RPM: %.2f STATE:%s\n", 
            sensor->channel, 
            sensor->average_rpm, 
            state_to_string(sensor->state));
}