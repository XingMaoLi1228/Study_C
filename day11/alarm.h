#ifndef ALARM_H
#define ALARM_H

typedef enum
{
    STATE_NORMAL,
    STATE_ALARM
} AlarmRpmState;

typedef struct 
{
    double alarm_rpm;
    double recovery_rpm;

    int alarm_confirm;
    int recovery_confirm;

    int alarm_count;
    int recovery_count;

    AlarmRpmState alarm_rpm_state;
} Alarm;

//初始化报警状态
void alarm_rpm_init(Alarm *alarm,
                double alarm_rpm,
                double recocery_rpm,
                int alarm_confirm,
                int recovery_confirm);

//更新转速报警状态
void alarm_rpm_state(Alarm *alarm, double rpm);

//报警状态转字符串
const char*alarm_state_to_string(const Alarm *alarm);

#endif // !1