#include <stdio.h>
#include "sensor.h"

int main(void)
{
    Sensor sensor;

    sensor_init(&sensor, 1);

    double test_rpm[] = 
    {
        0,
        500,

        1500,
        1500,
        1500,
        1500,
        1500,

        500,
        500,
        500,
        500,
        500,

        -1
    };

    int count = sizeof(test_rpm) /
                sizeof(test_rpm[0]);

    for (int i = 0; i < count; i++)
    {
        sensor_set_rpm(&sensor, test_rpm[i]);

        sensor_print(&sensor);
    }


    return 0;
}
