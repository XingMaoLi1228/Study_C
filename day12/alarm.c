#include <stdio.h>

#include "alarm.h"

//清除报警状态
static void alarm_clean(AlarmRpm *alarm)
{
    if (alarm == NULL)
    {
        return;
    }

    alarm->alarm_count = 0;
    alarm->recovery_count = 0;
}


//初始化报警
void alarm_init(AlarmRpm *alarm,
                double alarm_rpm,
                double recovery_rpm,
                int alarm_confirm,
                int recovery_confirm)
{
    if (alarm == NULL)
    {
        return;
    }

    alarm->alarm_rpm = alarm_rpm;
    alarm->recovery_rpm = recovery_rpm;

    alarm->alarm_confirm = alarm_confirm;
    alarm->recovery_confirm = recovery_confirm;

    alarm->alarm_count = 0;
    alarm->recovery_count = 0;

    alarm->state = STATE_NORMAL;
}

//更新报警状态
void alarm_update(AlarmRpm *alarm, double rpm)
{
    if (alarm == NULL)
    {
        return;
    }

    if (alarm->state == STATE_NORMAL)
    {
        if (rpm >= alarm->alarm_rpm)
        {
            alarm->alarm_count++;

            if (alarm->alarm_count >= 
                    alarm->alarm_confirm)
            {
                alarm->state = STATE_ALARM;

                alarm_clean(alarm);
            }
        }
        else
        {
            alarm_clean(alarm);
        }
    }

    if (alarm->state == STATE_ALARM)
    {
        if (rpm <= alarm->recovery_rpm)
        {
            alarm->recovery_count++;

            if (alarm->recovery_count >= 
                    alarm->recovery_confirm)
            {
                alarm->state = STATE_NORMAL;

                alarm_clean(alarm);
            }
        }
        else
        {
            alarm_clean(alarm);
        }
    }
}


char *alarm_state_to_string(AlarmRpmState state)

{
    switch (state)
    {
        case STATE_NORMAL:
            return "NORMAL";

        case STATE_ALARM:
            return "ALARM";

        default:
            return "Unknown state";
    }
}