#include <stdio.h>

#include "alarm.h"

//初始化报警状态
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

//清除报警状态
static void clean_state(AlarmRpm *alarm)
{
    if (alarm == NULL)
    {
        return;
    }

    alarm->alarm_count = 0;
    alarm->recovery_count = 0;
}

//更新报警状态
void alarm_update_state(AlarmRpm *alarm, double rpm)
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

            if (alarm->alarm_count >= alarm->alarm_confirm)
            {
                alarm->state = STATE_ALARM;
                clean_state(alarm);
            }
        }
        else
        {
            clean_state(alarm);
        }
    }
    else if (alarm->state == STATE_ALARM)
    {
        if (rpm <= alarm->recovery_rpm )
        {
            alarm->recovery_count++;

            if (alarm->recovery_count >= alarm->recovery_confirm)
            {
                alarm->state = STATE_NORMAL;
                clean_state(alarm);
            }
        }
        else
        {
            clean_state(alarm);
        }
    }
}

//报警状态转字符串
const char *alarm_state_to_string(AlarmRpmState state)
{
    switch (state)
    {
        case STATE_NORMAL:
            return "NORMAL";
        
        case STATE_ALARM:
            return "ALARM";

        default:
            return "Unknown alarm state";
    }
}

//获取报警事件
AlarmEvent alarm_get_event(
    AlarmRpmState old_state,
    AlarmRpmState new_state
)
{
    if (old_state == STATE_ALARM &&
        new_state == STATE_NORMAL)
    {
        return ALARM_EVENT_RECOVERED;
    }
    else if (old_state == STATE_NORMAL &&
            new_state == STATE_ALARM)
    {
        return ALARM_EVENT_TRIGGERED;
    }
    else
    {
        return ALARM_EVENT_NONE;
    }

    return ALARM_EVENT_NONE;
}

const char *event_state_to_string(AlarmEvent alarm_event)
{
    switch (alarm_event)
    {
        case ALARM_EVENT_NONE:
            return "ALARM_EVENT_NONE";

        case ALARM_EVENT_TRIGGERED:
            return "ALARM_EVENT_TRIGGERED";

        case ALARM_EVENT_RECOVERED:
            return "ALARM_EVENT_RECOVERED";

        default:
            return "Unknown alarm event state!";
    }
}
