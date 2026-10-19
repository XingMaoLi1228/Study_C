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
    
    sensor->average_rpm = 0.0;
    sensor->sum_rpm = 0.0;

    sensor->valid_count = 0;
    sensor->write_index = 0;

    for (int i = 0; i < HISTORY_SIZE; i++)
    {
        sensor->rpm_buffer[i] = 0.0;
    }
}

//更新buffer
static void update_buffer(Sensor *sensor, double rpm)
{
    if (sensor == NULL ||
        sensor->valid_count < 0)
    {
        return;
    }

    if (sensor->valid_count == 
            HISTORY_SIZE)
    {
        sensor->sum_rpm -= 
            sensor->rpm_buffer
                [sensor->write_index];
    }
    else
    {
        sensor->valid_count++;
    }

    sensor->rpm_buffer[sensor->write_index]
        = rpm;

    sensor->sum_rpm += rpm;

    sensor->write_index = 
        (sensor->write_index + 1) 
            % HISTORY_SIZE;

    sensor->average_rpm 
        = sensor->sum_rpm / sensor->valid_count;
}

//加入新转速
void sensor_set_rpm(Sensor *sensor, double rpm)
{
    if (sensor == NULL)
    {
        return;
    }

    if (rpm < 0.0)
    {
        return;
    }

    update_buffer(sensor, rpm);
}   


