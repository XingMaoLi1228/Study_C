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

    int valid_count;
    int write_index;
} Sensor;


void sensor_init(Sensor *sensor, int channel);

void sensor_set_rpm(Sensor *sensor, double rpm);




#endif // !SENSOR_H