#include <stdio.h>

#include "sensor.h"
#include "alarm.h"


int main(void)
{
    Sensor sensor;
    Alarm rpm_alarm;

    sensor_init(&sensor, 1);

    alarm_init(&rpm_alarm, 1000.0, 900.0, 3, 3);

    double rpm_data[] = 
    {
        800,
        1100,
        1200,
        1300,
        1200,
        850,
        800,
        700,

        700,
        700,
        700,
        700
    };

    int count = sizeof(rpm_data) / 
                sizeof(rpm_data[0]);

    for (int i = 0; i < count; i++)
    {
        sensor_set_rpm(&sensor, rpm_data[i]);

        alarm_update(&rpm_alarm, sensor.average_rpm);

        printf("Now_RPM: %.2f, Average_RPM:%.2f, Alarm_count:%d, Recovery_count:%d, Is_alarm:%d\n",
            sensor.now_rpm,
            sensor.average_rpm,
            rpm_alarm.alarm_count,
            rpm_alarm.recovery_count,
            rpm_alarm.rpm_alarm_state
            );
    }

    return 0;
}