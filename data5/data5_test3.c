#include <stdio.h>

#define SENSOR_COUNT 4


// //定义枚举
// typedef enum 
// {
//     STOP,
//     RUNNING,
//     ALARM
// } DeviceStatus;

// //定义结构体
// typedef struct
// {
//     int channel;
//     double rpm;
//     DeviceStatus state;
// } Sensor;

// //更新状态
// void update_status(Sensor *sensor)
// {
//     if (sensor == NULL)
//     {
//         return;
//     }

//     if (sensor->rpm <= 0.0)
//     {
//         sensor->state = STOP;
//     }
//     else if (sensor->rpm > 1800.0)
//     {
//         sensor->state = ALARM;
//     }
//     else
//     {
//         sensor->state = RUNNING;
//     }
// }

// //打印状态
// void print_state(const Sensor *sensor)
// {
//     if (sensor == NULL)
//     {
//         return;
//     }

//     switch (sensor->state)
//     {
//         case STOP:
//             printf("STOP\n");
//             break;
//         case RUNNING:
//             printf("RUNNING\n");
//             break;
//         case ALARM:
//             printf("ALARM\n");
//             break;
//         default:
//             printf("Unknow status\n");
//             break;
//     }
// }


// int main(void)
// {
//     Sensor sensor = 
//     {
//         1,
//         1200,
//         RUNNING
//     };

//     update_status(&sensor);
//     print_state(&sensor);

//     return 0;
// } 

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
    double rpm;
    DeviceStatus status;
    int reset;
} Sensor;

void update_state(Sensor *sensor)
{
    if (sensor == NULL)
    {
        return;
    }

    if (sensor->status == ALARM)
    {
        if (sensor->reset == 0)
        {
            return;
        }

        if (sensor->rpm > 1800.0)
        {
            return;
        }
    }

    if (sensor->rpm < 0.0)
    {
        sensor->status = ERROR;
    }
    else if (sensor->rpm == 0.0)
    {
        sensor->status = STOP;
    }
    else if (sensor->rpm <= 1800.0)
    {
        sensor->status = RUNNING;
    }
    else
    {
        sensor->status = ALARM;
    }

    //reset 使用一次后清零
    sensor->reset = 0;
}

void print_status(const Sensor *sensor)
{
    if (sensor == NULL)
    {
        return;
    }

    switch (sensor->status)
    {
        case STOP:
            printf("Sensor %d STOP\n", sensor->channel);
            break;
        case RUNNING:
            printf("Sensor %d RUNNING\n", sensor->channel);
            break;
        case ALARM:
            printf("Sensor %d ALARM\n", sensor->channel);
            break;
        case ERROR:
            printf("Sensor %d ERROR\n", sensor->channel);
            break;
        default:
            printf("Sensor %d Unknown status\n", sensor->channel);
            break;
    }
}


int main(void)
{
    Sensor sensor[SENSOR_COUNT] = 
    {
        {1, 0.0, STOP, 0},
        {2, 1500.0, STOP, 0},
        {3, 2200.0, STOP, 0},
        {4, -100.0, STOP, 0}
    };

    for (int i = 0; i < SENSOR_COUNT; i++)
    {
        update_state(&sensor[i]);
        print_status(&sensor[i]);
    }

    return 0;
}
