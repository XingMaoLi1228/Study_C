#ifndef SENSOR_H
#define SENSOR_H

#define HISTORY_SIZE 5

#define ALARM_RPM 1000.0
#define RECOVERY_RPM 900.0

#define ALARM_CONFIRM 3
#define RECOVERY_CONFIRM 3


typedef enum 
{
    STATE_STOP,
    STATE_RUNNING,
    STATE_ALARM,
    STATE_ERROR
} DeviceState;

typedef struct 
{
    int channel;

    double current_rpm;

    double rpm_buffer[HISTORY_SIZE];

    int valid_count;
    int write_index;

    double rpm_sum;
    double average_rpm;

    int alarm_count;
    int recovery_count;

    DeviceState state;
} Sensor;

//初始化传感器
void sensor_init(Sensor *sensor, int channel);

//加入新的转速值
void sensor_set_rpm(Sensor *sensor, double rpm);

//打印传感器状态
void sensor_print(const Sensor *sensor);

//人工复位
void sensor_reset(Sensor *sensor);


#endif // !SENSOR_H

