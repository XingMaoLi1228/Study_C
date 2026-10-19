#include <stdio.h>

#include "config.h"
#include "sensor.h"
#include "alarm.h"
#include "acquisition.h"

#define SENSOR_COUNT 4

int main(void)
{
    Acquisition acquisition[SENSOR_COUNT];
    Sensor sensors[SENSOR_COUNT];
    AlarmRpm rpm_alarm[SENSOR_COUNT];

    double rpm;

    for (int channel = 0; channel < SENSOR_COUNT; channel++)
    {
        acquisition_init(&acquisition[channel], channel + 1);
        sensor_init(&sensors[channel], channel + 1);
        alarm_init(&rpm_alarm[channel], 
                1000.0,
                900.0,
                3,
                3);
    }

    for (int i = 0; i < 12; i++)
    {
        for (int channel = 0; channel < SENSOR_COUNT; channel++)
        {
            rpm = acquisition_read(&acquisition[channel]);

            sensor_set_rpm(&sensors[channel], rpm);

            alarm_update_state(&rpm_alarm[channel], 
                sensors[channel].average_rpm);

            printf("CH%d RPM:%.2f Average: %.2f "
                "Alarm_count:%d "
                "Recovery_count:%d "
                "State:%s\n",
                acquisition[channel].channel,
                rpm,
                sensors[channel].average_rpm,
                rpm_alarm[channel].alarm_count,
                rpm_alarm[channel].recovery_count,
                alarm_state_to_string(rpm_alarm[channel].state));
        }
    }
    
    return 0;
}