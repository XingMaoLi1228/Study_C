#include <stdio.h>

// #define SENSOR_COUNT 3
// #define HISTORY_COUNT 5

// typedef struct 
// {
//     int channle;
//     double history_rpm[HISTORY_COUNT];
//     double average_rpm;
//     int alarm;
// } Sensor;


// //求单个转速平均
// double average_array(const Sensor *sensor)
// {
//     if (sensor == NULL)
//     {
//         return 0.0;
//     }

//     double sum = 0.0;

//     for (int i = 0; i < HISTORY_COUNT; i++)
//     {
//         sum += sensor->history_rpm[i];
//     }

//     return sum / HISTORY_COUNT;
// }

// void is_alarm_average_rpm(Sensor *sensors)
// {
//     if (sensors == NULL)
//     {
//         return;
//     }

//     for (int i = 0; i < SENSOR_COUNT; i++)
//     {
//         sensors[i].average_rpm = average_array(&sensors[i]);

//         if (sensors[i].average_rpm > 1800.0)
//         {
//             sensors[i].alarm = 1;
//         }
//         else
//         {
//             sensors[i].alarm = 0;
//         }
//     }
// }

// int main(void)
// {
//     Sensor sensors[SENSOR_COUNT] =
//     {
//         {
//             1,
//             {1000, 1200, 1400, 2500, 3000},
//             0,
//             0
//         },
//         {
//             2,
//             {1500, 0, 2500, 2600, 1200},
//             0,
//             0
//         },
//         {
//             3,
//             {2000, 1450, 2600, 2400, 3600},
//             0,
//             0
//         }
//     };

//     is_alarm_average_rpm(sensors);

//     for (int i = 0; i < SENSOR_COUNT; i++)
//     {
//         if(sensors[i].alarm)
//         {
//             printf("Sensor %d is alarm, average rpm: %.2f\n", sensors[i].channle, sensors[i].average_rpm);
//         }
//     }

//     return 0;
// }

// #include <stdio.h>

// #define SENSOR_COUNT 3
// #define HISTORY_SIZE 5


// //定义结构体
// typedef struct 
// {
//     int channel;
//     double history_rpm[HISTORY_SIZE];
//     double average_rpm;
//     int alarm;
// } Sensor;

// double average_array(const Sensor *sensor)
// {
//     if (sensor == NULL)
//     {
//         return 0.0;
//     }

//     double sum = 0.0;

//     for (int i = 0; i < HISTORY_SIZE; i++)
//     {
//         sum += sensor->history_rpm[i];
//     }

//     return sum / HISTORY_SIZE;
// }

// //判断求多个传感器是否报警
// void is_alarm_average_rpm(Sensor *sensors)
// {
//     if (sensors == NULL)
//     {
//         return;
//     }

//     for (int i = 0; i < SENSOR_COUNT; i++)
//     {
//         sensors[i].average_rpm = average_array(&sensors[i]);

//         if (sensors[i].average_rpm > 1800.0)
//         {
//             sensors[i].alarm = 1;
//         }
//         else
//         {
//             sensors[i].alarm = 0;
//         }
//     }
// }

// int  main(void)
// {
//     Sensor sensors[SENSOR_COUNT] =
//     {
//         {
//             1,
//             {1000, 1500, 2000, 2500, 2800},
//             0,
//             0
//         },
//         {
//             2,
//             {2000, 2500, 2310, 1810, 1200},
//             0,
//             0
//         },
//         {
//             3,
//             {1520, 1625, 2300, 1478, 2600},
//             0,
//             0
//         }
//     };

//     is_alarm_average_rpm(sensors);

//     for (int i = 0; i < SENSOR_COUNT; i++)
//     {
//         if (sensors[i].alarm)
//         {
//             printf("Sensor %d is alarm, average rpm is %.2f\n", sensors[i].channel, sensors[i].average_rpm);
//         }
//     }

//     return 0;
// }

//定义枚举



typedef enum 
{
    STOP,
    RUNNING,
    ALARM
} DeviceStaus;

//定义结构体
typedef struct
{
    int channel;
    double rpm;
    DeviceStaus state;
} Sensor;

//更新状态
void updata_state(Sensor *sensor)
{
    if (sensor == NULL)
    {
        return;
    }

    if (sensor->rpm <= 0.0)
    {
        sensor->state = STOP;
    }
    else if (sensor->rpm > 1800.0)
    {
        sensor->state = ALARM;
    }
    else
    {
        sensor->state = RUNNING;
    }
}

//打印状态
void print_state(const Sensor *sensor)
{
    if (sensor == NULL)
    {
        return;
    }

    switch (sensor->state)
    {
        case STOP:
            printf("STOP\n");
            break;
        case RUNNING:
            printf("RUNNING\n");
            break;
        case ALARM:
            printf("ALARM\n");
            break;
    }
}


int mian(void)
{
    Sensor sensor = 
    {
        1,
        1200,
        RUNNING
    };

    updata_state(&sensor);
    print_state(&sensor);

    return 0;
} 