#include <stdio.h>

#define SENSOR_COUNT 3
#define HISTORY_SIZE 5

//定义结构体  typedef --> 起别名
// typedef struct 
// {
//     int channel;
//     double rpm;
//     double temperature;
// } Sensor;


// int main(void)
// {

//     //初始化结构体数组
//     Sensor sensors[SENSOR_COUNT] = 
//     {
//         {1, 1500.0, 45.0},
//         {2, 1800.0, 45.1},
//         {3, 1700.0, 44.2}
//     };

//     for (int i = 0; i < SENSOR_COUNT; i++)
//     {
//         printf("Sensor %d rpm = %.2f\n", i + 1, sensors[i].rpm);
//     }

//     // sensor1.channel = 1;
//     // sensor1.rpm = 1500;
//     // sensor1.temperature = 45.2;

//     // Sensor sensor2 = {1, 1800.0, 45.0}; //初始化结构体变量

//     // printf("channel = %d\n", sensor1.channel);
//     // printf("Rpm = %.2f\n", sensor1.rpm);
//     // printf("Temperature = %.2f\n", sensor1.temperature);

//     // printf("channel = %d\n", sensor2.channel);
//     // printf("Rpm = %.2f\n", sensor2.rpm);
//     // printf("Temperature = %.2f\n", sensor2.temperature);

//     return 0;
// }

// void print_sensor(Sensor *sensor)
// {
//     sensor->rpm = 2000.0;

//     printf("Channel = %d\n", sensor->channel);
//     printf("Rpm = %.1f\n", sensor->rpm);
//     printf("Temperature = %.1f\n", sensor->temperature);
// }

// int main(void)
// {
//     Sensor sensor1 = {1, 1500.0, 55.0};

//     print_sensor(&sensor1);
//     printf("Sensor1 rpm = %.1f\n", sensor1.rpm);

//     return 0;
// }

// struct Sensor
// {
//     int channel;
//     double rpm;
// };

// void print_sensors(struct Sensor *sensors, int count)
// {
//     if (sensors == NULL || count <= 0)
//     {
//         return;
//     }

//     for (int i = 0; i < count; i++)
//     {
//         printf("Sensor%d channel = %d, rpm = %.1f\n", 
//                 i + 1,
//                 sensors[i].channel,
//                 sensors[i].rpm);
//     }
// }

// int main(void)
// {
//     struct Sensor sensors[SENSOR_COUNT] =
//     {
//         {1, 1000.0},
//         {2, 1200.0},
//         {3, 1500.0}
//     };

//     print_sensors(sensors, SENSOR_COUNT);

//     return 0;

// }

// struct Sensor
// {
//     int channel;
//     double rpm;
//     int alarm;
// };

// void Is_alarm(struct Sensor *sensors, int sensors_count, int *number)
// {
//     if (sensors == NULL || number == NULL ||sensors_count <= 0)
//     {
//         return;
//     }

//     *number = 0;

//     for (int i = 0; i < sensors_count; i++)
//     {
//         if (sensors[i].rpm > 1800.0)
//         {
//             sensors[i].alarm = 1;
//             *number += 1;
//         }
//         else
//         {
//             sensors[i].alarm = 0;
//         }
        
//     }
// }

// int main(void)
// {
//     struct Sensor sensors[SENSOR_COUNT] =
//     {
//         {1, 1900, 0},
//         {2, 1200, 0},
//         {3, 2000, 0}
//     };
//     int sensor_alarm_count = 0;

//     Is_alarm(sensors, SENSOR_COUNT, &sensor_alarm_count);

//     for (int i = 0; i < SENSOR_COUNT; i++)
//     {
//         if (sensors[i].alarm)
//         {
//             printf("Sensor %d is alarm, rpm = %.1f\n", sensors[i].channel, sensors[i].rpm);
//         }
//     }

//     printf("Sensors has %d alarm\n", sensor_alarm_count);

//     return 0;
// }


// //定义传感器结构体
// typedef struct
// {
//     int channel;
//     double rpm;
//     int alarm;
// } Sensor;


// //判断报警
// void is_alarm(Sensor *sensors, int sensor_count, int *alarm_count)
// {
//     if (sensors == NULL || alarm_count == NULL || sensor_count <= 0)
//     {
//         return;
//     }

//     *alarm_count = 0;

//     for (int i = 0; i < sensor_count; i++)
//     {
//         if (sensors[i].rpm > 1800.0)
//         {
//             sensors[i].alarm = 1;
//             *alarm_count += 1;
//         }
//         else
//         {
//             sensors[i].alarm = 0;
//         }
//     }
// }

// //打印报警传感器
// void print_alarm_sensors(Sensor *sensors, int sensor_count)
// {
//     if (sensors == NULL || sensor_count <= 0)
//     {
//         return;
//     }

//     for (int i = 0; i < sensor_count; i++)
//     {
//         printf("Sensor %d is alarm, rpm = %.1f\n", i + 1, sensors[i].rpm);
//     }
// }

// int main(void)
// {
//     Sensor sensors[SENSOR_COUNT] = 
//     {
//         {1, 1000, 0},
//         {2, 2000, 0},
//         {3, 2100, 0}
//     };
//     int sensor_count;

//     is_alarm(sensors, SENSOR_COUNT, &sensor_count);
//     print_alarm_sensors(sensors, SENSOR_COUNT);

//     printf("Sensors has %d alarm\n", sensor_count);

//     return 0;
// }


//结构体里有数组

typedef struct
{
    int channel;
    double rpm_history[HISTORY_SIZE];
    double average_rpm;
    int alarm;
} Sensor;

//计算一个传感器的平均转速
double average_rpm(const Sensor *sensor)
{
    if (sensor == NULL)
    {
        return 0.0;
    }

    double sum = 0.0;

    for (int i = 0; i < HISTORY_SIZE; i++)
    {
        sum += sensor->rpm_history[i];
    }

    return sum / HISTORY_SIZE;
}

//计算多个传感器的平均转速
void updata_sensors(Sensor *sensor)
{
    if (sensor == NULL || HISTORY_SIZE <= 0)
    {
        return;
    }

    for (int i = 0; i < SENSOR_COUNT; i++)
    {
        //求平均
        sensor[i].average_rpm = average_rpm(&sensor[i]);

        //判断报警
        if (sensor[i].average_rpm > 1800)
        {
            sensor[i].alarm = 1;
        }
        else
        {
            sensor[i].alarm = 0;
        }
    }
}
int main(void)
{
    // //初始化sensor1
    // Sensor sensor1 = 
    // {
    //     1,
    //     {1000, 1200, 1400, 1800, 2000},
    //     0
    // };

    // double avg_rpm = average_rpm(&sensor1);

    // printf("Average rpm: %.2f\n", avg_rpm);

    //初始化sensor[]数组
    Sensor sensors[SENSOR_COUNT] =
    {
        {
            1,
            {1000, 1200, 1400, 1600, 1800},
            0,
            0
        },
        {
            2,
            {1500, 1800, 1900, 2000, 2100},
            0,
            0
        },
        {
            3,
            {900, 1000, 1100, 1200, 1300},
            0,
            0
        }

    };

    updata_sensors(sensors);

    for (int i = 0; i < SENSOR_COUNT; i++)
    {
        if (sensors[i].alarm)
        {
            printf("Sensor %d has alarm, average rpm: %.2f\n", sensors[i].channel, sensors[i].average_rpm);
        }
    }

    return 0;
}