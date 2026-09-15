#include <stdio.h>

//初始化4个Senesor, 批量更新状态，打印所有状态，统计报警数量，输出系统状态

#define HISTORY_SIZE 5
#define SENSOR_COUNT 4


//定义枚举
typedef enum 
{
    STOP,
    RUNNING,
    ALARM,
    ERROR
} DeviceStatus;

//定义结构体
typedef struct 
{
    int channel;
    double history_rpm[HISTORY_SIZE];
    double average_rpm;
    DeviceStatus state;
} Sensor;

//求均值
void update_average_rpm(Sensor *sensor, int rpm_count)
{
    if (sensor == NULL || rpm_count <= 0)
    {
        return;
    }

    double sum = 0.0;

    for (int i = 0; i < rpm_count; i++)
    {
        sum += sensor->history_rpm[i];
    }

    sensor->average_rpm = sum / rpm_count;
}


//更新状态
void update_status(Sensor *sensors, int sensor_count, int rpm_count)
{
    if (sensors == NULL || sensor_count <= 0 || rpm_count <= 0)
    {
        return;
    }

    for (int i = 0; i < sensor_count; i++)
    {
        update_average_rpm(&sensors[i], rpm_count);

        if (sensors[i].average_rpm < 0.0)
        {
            sensors[i].state = ERROR;
        }
        else if (sensors[i].average_rpm == 0.0)
        {
            sensors[i].state = STOP;
        }
        else if (sensors[i].average_rpm < 1800.0)
        {
            sensors[i].state = RUNNING;
        }
        else
        {
            sensors[i].state = ALARM;
        }

    }  
}

//打印所有状态
void print_status(const Sensor *sensors, int sensor_count)
{
    if (sensors == NULL)
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
                printf("CH%d Average_RPM: %.2f  Unknowe status\n", sensors[i].channel, sensors[i].average_rpm);  
                break;     
        }
    }
}

//统计报警数量
int alarm_count(const Sensor *sensors, int sensor_count)
{
    if (sensors == NULL)
    {
        return 0;
    }

    int count = 0;

    for (int i = 0; i < sensor_count; i++)
    {
        if (sensors[i].state == ALARM)
        {
            count++;
        }
    }

    return count;
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

    int alarm_rpm_count;

    update_status(sensors, SENSOR_COUNT, HISTORY_SIZE);
    alarm_rpm_count = alarm_count(sensors, SENSOR_COUNT);

    printf("---------- SENSOR SYSTEM ----------\n\n");
    
    print_status(sensors, SENSOR_COUNT);

    printf("\nAlarm count: %d\n", alarm_rpm_count);

    return 0;
}