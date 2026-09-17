#ifndef ALARM_H
#define ALARM_H


typedef enum
{
    STATE_NORMAL,
    STATE_ACTIVE
} RpmAlarmState;

typedef struct 
{
    double alarm_threshold;
    double recovery_threshold;

    int alarm_confirm;
    int recovery_confirm;

    int alarm_count;
    int recovery_count;

    RpmAlarmState rpm_alarm_state;
} Alarm;

//报警初始化
void alarm_init(Alarm *alarm,
                double alarm_threshold,
                double recovery_threshold,
                int alarm_confirm,
                int recovery_confirm                
);

//更新报警
void alarm_update(Alarm *alarm, double value);

#endif // 