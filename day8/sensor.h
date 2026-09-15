#ifndef SENSOR_H   //防止sensor.h重复包含
#define SENSOR_H

#define HISTORY_SIZE 5

#define ALARM_RPM 1600.0
#define RECOVERY_RPM 1500.0

#define ALARM_CONFIRM 3
#define RECOVERY_CONFIRM 3

//定义枚举
typedef enum
{
    STATE_NORMAL,
    STATE_ALARM,
    STATE_ERROR
} DeviceState;


//定义传感器结构体
typedef struct 
{
    int channel;

    double rpm_buffer[HISTORY_SIZE];

    int write_index;
    int valid_count;

    double average_rpm;

    int alarm_count;
    int recovery_count;

    DeviceState state;
} Sensor;

//生成随机数
double generate_random_rpm(double min, double max);

//初始化传感器
void sensor_init(Sensor *sensor, int channel);

//把RPM写入循环缓冲区
void update_buffer(Sensor *sensor, double new_rpm);

//计算平均
double calculate_average(Sensor *sensor);

//更新报警状态
void update_state(Sensor *sensor);

//打印传感器
void print_sensor(const Sensor *sensor);

#endif