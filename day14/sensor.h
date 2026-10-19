#ifndef SENSOR_H
#define SENSOR_H

#define HISTORY_SIZE 5

typedef struct 
{
    int channel;

    double rpm_buffer[HISTORY_SIZE];

    double sum_rpm;
    double average_rpm;

    int valid_count;
    int write_index;
} Sensor;

//初始化传感器
void sensor_init(Sensor *sensor, int channel);

//设置转速
void sensor_set_rpm(Sensor *sensor, double rpm);

#endif // !SENSOR_H
