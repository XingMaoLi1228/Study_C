#ifndef ALARM_H
#define ALARM_H

typedef enum
{
    STATE_NORMAL,
    STATE_ALARM,
} AlarmRpmState;

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


//初始化报警
void alarm_init(AlarmRpm *alarm,
                double alarm_rpm,
                double recovery_rpm,
                int alarm_confirm,
                int recovery_confirm);

//更新报警状态
void alarm_update(AlarmRpm *alarm, double rpm);
         
//报警状态转字符串
char *alarm_state_to_string(AlarmRpmState state);

#endif // !1
