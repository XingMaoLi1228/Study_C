#include <stdio.h>

#define HISTORY_SIZE 5
#define SENSOR_COUNT 4

typedef enum
{
    STOP,
    RUNNING,
    ALARM,
    ERROR
} DeviceStatus;

typedef struct 
{
    int channel;
    double rpm_buffer[HISTORY_SIZE];
    double average_rpm;
    DeviceStatus state;
} Sensor;


//计算平均转速
void calculate_average_rpm(Sensor *sensor, int history_size)
{
    if (sensor == NULL || history_size <= 0 || history_size > HISTORY_SIZE)
    {
        return;
    }

    double sum = 0.0;

    for (int i = 0; i < history_size; i++)
    {
        sum += sensor->rpm_buffer[i];
    }

    sensor->average_rpm = sum / history_size;
    
}


//更新单个传感器状态
void update_one_state(Sensor *sensor, int history_count)
{
    if (sensor == NULL || history_count <= 0 || history_count > HISTORY_SIZE)
    {
        return;
    }

    int has_error = 0;

    for (int i = 0; i < history_count; i++)
    {
        if (sensor->rpm_buffer[i] < 0.0)
        {
            has_error = 1;
            break;
        }
    }

    if (has_error)
    {
        sensor->state = ERROR;
        sensor->average_rpm = 0.0;
    }
    else if (sensor->average_rpm == 0.0)
    {
        sensor->state = STOP;
    }
    else if (sensor->average_rpm < 1800.0)
    {
        sensor->state = RUNNING;
    }
    else
    {
        sensor->state = ALARM;
    }
}

//打印状态
void print_state(Sensor *sensors, int sensor_count)
{
    if (sensors == NULL || sensor_count <= 0)
    {
        return;
    }

    for (int i = 0; i < sensor_count; i++)
    {
        switch (sensors[i].state)
        {
            case ERROR:
                printf("CH%d Average_RPM: %.2f  ERROR\n", sensors[i].channel, sensors[i].average_rpm);
                break;
            case STOP:
                printf("CH%d Average_RPM: %.2f  STOP\n", sensors[i].channel, sensors[i].average_rpm);
                break;
            case RUNNING:
                printf("CH%d Average_RPM: %.2f  RUNNING\n", sensors[i].channel, sensors[i].average_rpm);
                break;
            case ALARM:
                printf("CH%d Average_RPM: %.2f  ALARM\n", sensors[i].channel, sensors[i].average_rpm);
                break;
            default:
                printf("CH%d Average_RPM: %.2f  Unknown state\n", sensors[i].channel, sensors[i].average_rpm);
                break;               
        }
    }
}

//更新缓存
void push_rpm(Sensor *sensor, double new_rpm, int history_size)
{
    if (sensor == NULL || history_size <= 0)
    {
        return;
    }

    for (int i = 0; i < history_size - 1; i++)
    {
        sensor->rpm_buffer[i] = sensor ->rpm_buffer[i + 1];
    }

    sensor->rpm_buffer[history_size - 1] = new_rpm;

}

//计算新的单通道平均转速
void process_one_new_rpm(Sensor *sensor, double new_rpm, int history_count)
{
    if (sensor == NULL || history_count <= 0 || history_count > HISTORY_SIZE)
    {
        return;
    }

    //放进缓冲区
    push_rpm(sensor, new_rpm, history_count);

    //计算新的平均值
    calculate_average_rpm(sensor, history_count);

    //更新这个传感器的状态
    update_one_state(sensor, history_count);

}

//计算所有通道的平均转速
void process_all_new_rpm(Sensor *sensors, const double *new_rpm, int sensor_count, int history_count)
{
    if (sensors == NULL || 
        new_rpm == NULL ||
        sensor_count <= 0 ||
        history_count <= 0 ||
        history_count > HISTORY_SIZE)
    {
        return;
    }

    for (int i = 0; i < sensor_count; i++)
    {
        process_one_new_rpm(&sensors[i], new_rpm[i], history_count);
    }
}

int main(void)
{
    //初始化传感器
    Sensor sensors[SENSOR_COUNT] =
    {
        {
            1, {-100, -100, -100, 120, -200}, 0, STOP
        },
        {
            2, {1000, 1020, 1000, 1200, 2000}, 0, STOP
        },
        {
            3, {0, 0, 0, 0, 0}, 0, STOP
        },
        {
            4, {2000, 2500, 2200, 1220, 2800}, 0, STOP
        },
    };

    double new_rpm[SENSOR_COUNT] = 
    {
        1500,
        1600,
        0.0,
        2100.0
    };
    //测试通道
    // process_one_new_rpm(&sensors[1], 1500.0, HISTORY_SIZE);
    process_all_new_rpm(sensors, new_rpm, SENSOR_COUNT, HISTORY_SIZE);


    //打印
    // printf("CH%d Average_RPM: %.2f\n", sensors[1].channel, sensors[1].average_rpm);
    print_state(sensors, SENSOR_COUNT);
    return 0;
}