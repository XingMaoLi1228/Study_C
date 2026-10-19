#ifndef CONFIG_H
#define CONFIG_H

typedef enum
{
    CONFIG_OK,
    CONFIG_ERROR_FILE,
    CONFIG_ERROR_MISSING,
    CONFIG_ERROR_RANGE
} ConfigResult;

typedef struct 
{
    double alarm_rpm;
    double recovery_rpm;

    int alarm_confirm;
    int recovery_confirm;
} AlarmConfig;

//加载配置文件
ConfigResult config_load(AlarmConfig *config, const char *filename);

//配置文件状态转字符串
char *config_state_to_string(ConfigResult config_state);
#endif // !CONFIG_H


