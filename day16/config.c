#include <stdio.h>

#include "config.h"

//加载配置文件
ConfigResult config_load(AlarmConfig *config, const char *filename)
{
    if (config == NULL)
    {
        return CONFIG_ERROR_FILE;
    }

    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        return CONFIG_ERROR_FILE;
    }

    char line[128];

    int alarm_rpm_ok = 0;
    int recovery_rpm_ok = 0;
    int alarm_confirm_ok = 0;
    int recovery_confirm_ok = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        double double_value = 0.0;
        int int_value = 0;

        if (sscanf(line, "alarm_rpm=%lf", &double_value) == 1)
        {
            config->alarm_rpm = double_value;
            alarm_rpm_ok = 1;
        }
        else if (sscanf(line, "recovery_rpm=%lf", &double_value) == 1)
        {
            config->recovery_rpm = double_value;
            recovery_rpm_ok = 1;
        }
        else if (sscanf(line, "alarm_confirm=%d", &int_value) == 1)
        {
            config->alarm_confirm = int_value;
            alarm_confirm_ok = 1;
        }
        else if (sscanf(line, "recovery_confirm=%d", &int_value) == 1)
        {
            config->recovery_confirm = int_value;
            recovery_confirm_ok = 1;
        }
    }

    fclose(file);

    if ((alarm_rpm_ok &&
        recovery_rpm_ok &&
        alarm_confirm_ok &&
        recovery_confirm_ok) != 1)
    {
        return CONFIG_ERROR_MISSING;
    }

    //判断合理性
    if (config->alarm_confirm <= 0 ||
        config->recovery_confirm <= 0 ||
        config->alarm_rpm <= config->recovery_rpm ||
        config->alarm_rpm <= 0.0 ||
        config->recovery_rpm <= 0.0)
    {
        return CONFIG_ERROR_RANGE;
    }



    return CONFIG_OK;
}

//配置文件状态转字符串
char *config_state_to_string(ConfigResult config_state)
{
    switch (config_state)
    {
        case CONFIG_OK:
            return "CONFIG_OK";

        case CONFIG_ERROR_FILE:
            return "CONFIG_ERROR_FILE";

        case CONFIG_ERROR_MISSING:
            return "CONFIG_ERROR_MISSING";

        case CONFIG_ERROR_RANGE:
            return "CONFIG_ERROR_RANGE";

        default:
            return "Unknown file state";
    }
}


