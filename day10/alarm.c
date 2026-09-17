#include <stdio.h>
#include "alarm.h"



static void clean_alarm_or_recovery_count(Alarm *alarm)
{
    if (alarm == NULL)
    {
        return;
    }

    alarm->alarm_count = 0;
    alarm->recovery_count = 0;
}

//报警初始化
void alarm_init(Alarm *alarm,
                double alarm_threshold,
                double recovery_threshold,
                int alarm_confirm,
                int recovery_confirm                
)
{
    if (alarm == NULL)
    {
        return;
    }

    alarm->alarm_threshold = alarm_threshold;
    alarm->recovery_threshold = recovery_threshold;

    alarm->alarm_confirm = alarm_confirm;
    alarm->recovery_confirm = recovery_confirm;

    alarm->alarm_count = 0;
    alarm->recovery_count = 0;

    alarm->rpm_alarm_state = STATE_NORMAL;
}


void alarm_update(Alarm *alarm, double value)
{
    if (alarm == NULL)
    {
        return;
    }

    if (alarm->rpm_alarm_state == STATE_NORMAL)
    {
        if (value >= alarm->alarm_threshold)
        {
            alarm->alarm_count++;

            if (alarm->alarm_count >= alarm->alarm_confirm)
            {
                alarm->rpm_alarm_state = STATE_ACTIVE;

                clean_alarm_or_recovery_count(alarm);
            }
        }
        else
        {
            alarm->alarm_count = 0;
        }
    }
    else
    {
        if (value <= alarm->recovery_threshold)
        {
            alarm->recovery_count++;

            if (alarm->recovery_count >= alarm->recovery_confirm)
            {
                alarm->rpm_alarm_state = STATE_NORMAL;

                clean_alarm_or_recovery_count(alarm);
            }
        }
        else
        {
            alarm->recovery_count = 0;
        }
    }
}



