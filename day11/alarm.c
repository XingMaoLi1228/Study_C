#include <stdio.h>

#include "alarm.h"

//初始化报警状态
void alarm_rpm_init(Alarm *alarm, 
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

    alarm->alarm_rpm_state = STATE_NORMAL;

    alarm->alarm_count = 0;
    alarm->recovery_count = 0;
}

//清除报警计数
static void alarm_clearn_count(Alarm *alarm)
{
    if (alarm == NULL)
    {
        return;
    }

    alarm->alarm_count = 0;
    alarm->recovery_count = 0;
}

//更新报警状态
void alarm_rpm_state(Alarm *alarm, double rpm)
{
    if (alarm == NULL)
    {
        return;
    }

    if (alarm->alarm_rpm_state == STATE_NORMAL)
    {
        if (rpm >= alarm->alarm_rpm)
        {
            alarm->alarm_count++;

            if(alarm->alarm_count >= alarm->alarm_confirm)
            {
                alarm->alarm_rpm_state = STATE_ALARM;

                alarm_clearn_count(alarm);
            }
        }
        else
        {
            alarm_clearn_count(alarm);
        }
    }
    else
    {
        if (rpm <= alarm->recovery_confirm)
        {
            alarm->recovery_count++;

            if (alarm->recovery_count >= alarm->alarm_confirm)
            {
                alarm->alarm_rpm_state = STATE_ALARM;

                alarm_clearn_count(alarm);
            }
        }
        else
        {
            alarm_clearn_count(alarm);
        }
    }
}


//报警状态转字符串
const char*alarm_state_to_string(const Alarm *alarm)
{
    if (alarm == NULL)
    {
        return "Unknown";
    }

    switch (alarm->alarm_rpm_state)
    {
        case STATE_NORMAL:
            return "NORMAL";

        case STATE_ALARM:
            return "ALARM";

        default:
            return "Unknown state";
    }
}