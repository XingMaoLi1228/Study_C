#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define SENSOR_COUNT 4
#define HISTORY_SIZE 5

#define ALARM_RPM 1000.0
#define RECOVERY_RPM 900.0

#define ALARM_CONFIRM 3
#define RECOVERY_CONFIRM 3

typedef enum 
{
    STATE_NORMAL,
    STATE_ALARM
} DeviceState;

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
double generate_random_rpm(double min, double max)
{
    double ratio = (double)rand() / RAND_MAX;

    return min +ratio * (max - min);
}


//初始化传感器
void sensor_init(Sensor *sensor, int channel)
{
    if (sensor == NULL ||
        channel <= 0)
    {
        return;
    }

    sensor->channel = channel;
    
    for (int i = 0; i < HISTORY_SIZE; i++)
    {
        sensor->rpm_buffer[i] = 0.0;
    }

    sensor->alarm_count = 0;
    sensor->average_rpm = 0.0;
    sensor->recovery_count = 0;
    sensor->valid_count = 0;
    sensor->write_index = 0;
    sensor->state = STATE_NORMAL;
}

//把RPM写入循环缓冲区
void update_buffer(Sensor *sensor, double new_rpm)
{
    if (sensor == NULL)
    {
        return;
    }

    sensor->rpm_buffer[sensor->write_index] = new_rpm;

    sensor->write_index = (sensor->write_index + 1) % HISTORY_SIZE;

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

                    sensor->alarm_count = 0;
                    sensor->recovery_count = 0;
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

                    sensor->alarm_count = 0;
                    sensor->recovery_count = 0;
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

//打印传感器
void print_sensor(const Sensor *sensor)
{
    if (sensor == NULL)
    {
        return;
    }

    switch (sensor->state)
    {
        case STATE_NORMAL:
            printf("CH%d RPM_AVG: %.2f STATE: NORMAL\n", sensor->channel, sensor->average_rpm);
            break;
        
        case STATE_ALARM:
            printf("CH%d RPM_AVG: %.2f STATE: ALARM\n", sensor->channel, sensor->average_rpm);
            break;
        default:
            break;
    }
}

int main(void)
{
    srand((unsigned int)time(NULL));

    Sensor sensors[SENSOR_COUNT];

    //初始化传感器
    for (int i = 0; i < SENSOR_COUNT; i++)
    {
        sensor_init(&sensors[i], i + 1);
    }

    while (1)
    {
        for (int i = 0; i < SENSOR_COUNT; i++)
        {
            double new_rpm = generate_random_rpm(-300.0, 3000.0);

            update_buffer(&sensors[i], new_rpm);

            sensors[i].average_rpm = calculate_average(&sensors[i]);

            update_state(&sensors[i]);

            print_sensor(&sensors[i]);
        }

        printf("-------------------------------\n");

        Sleep(1000);
    }

    //生成随机数

    return 0;
}