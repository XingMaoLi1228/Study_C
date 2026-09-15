#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "sensor.c"

#define SENSOR_COUNT 4

int main(void)
{
    Sensor sensors[SENSOR_COUNT];

    //初始化传感器
    for (int i = 0; i < SENSOR_COUNT; i++)
    {
        sensor_init(&sensors[i], i + 1);
    }

    //初始化随机数
    srand((unsigned int)time(NULL));

    while (1)
    {
        for (int i = 0; i < SENSOR_COUNT; i++)
        {
            //产生随机数（RPM）
            double new_rpm = generate_random_rpm(-300.0, 3000.0);

            //加入缓冲区
            update_buffer(&sensors[i], new_rpm);

            //计算平均转速
            sensors[i].average_rpm = calculate_average(&sensors[i]);

            //更新报警状态
            update_state(&sensors[i]);

            //打印传感器
            print_sensor(&sensors[i]);
        }

        printf("===================================\n");

        Sleep(1000);
    }

    return 0;
}