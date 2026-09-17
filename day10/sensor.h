#ifndef SENSOR_H
#define SENSOR_H

#define HISTORY_SIZE 5

typedef struct 
{
    int channel;

    double rpm_buffer[HISTORY_SIZE];

    double average_rpm;
    double sum_rpm;
    double now_rpm;

    int write_index;
    int valid_count;
} Sensor;

//初始化传感器
void sensor_init(Sensor *sensor, int channel);


//加入转速
void sensor_set_rpm(Sensor *sensor, double rpm);

#endif // !SENSOR_H