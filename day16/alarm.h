#ifndef ALARM_H
#define ALARM_H

//枚举-> 报警状态
typedef enum 
{
    STATE_NORMAL,
    STATE_ALARM
} AlarmRpmState;

//枚举->事件
typedef enum 
{
    ALARM_EVENT_NONE,
    ALARM_EVENT_TRIGGERED,
    ALARM_EVENT_RECOVERED
} AlarmEvent;


typedef struct 
{
    double alarm_rpm;
    double recovery_rpm;

    int alarm_confirm;
    int recovery_confirm;

    int alarm_count;
    int recovery_count;

    AlarmRpmState state;
} AlarmRpm;

//获取状态变化事件
AlarmEvent alarm_get_event(
    AlarmRpmState old_state,
    AlarmRpmState new_state
);

//事件状态转字符串
const char *event_state_to_string(AlarmEvent alarm_event);

//初始化报警状态
void alarm_init(AlarmRpm *alarm,
                double alarm_rpm,
                double recovery_rpm,
                int alarm_confirm,
                int recovery_confirm);

//更新报警状态
void alarm_update_state(AlarmRpm *alarm, double rpm);

//报警状态转字符串
const char *alarm_state_to_string(AlarmRpmState state);

#endif // !ALARM_H



