#include <stdio.h>

#include "acquisition.h"


//初始化采集
void acquisition_init(Acquisition *acquisition, int channel)
{
    if (acquisition == NULL)
    {
        return;
    }

    acquisition->channel = channel;
    acquisition->value = 0;
    acquisition->data_index = 0;
}


//采集数据并返回状态
AcquisitionResult acquisition_read(
    Acquisition *acquisition,
    double *rpm
)
{
    if (acquisition == NULL || rpm == NULL)
    {
        return ACQ_ERROR_NULL;
    }

    double rpm_data[4][12] =
    {
        {800, 900, 1100, 1200, 1300, 1400, 1500, 500, 500, 500, 500, 500},
        {700, 750, 800, 850, 900, 950, 1000, 1050, 1100, 1150, 1200, 1250},
        {1200, 1250, 1300, 1350, 1400, 1450, 1500, 1550, 1600, 1650, 1700, 1750},
        {500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600}
    };

    int channel_count = 0;

    channel_count = sizeof(rpm_data) / sizeof(rpm_data[0]);

    if (acquisition->channel < 1 || acquisition->channel > channel_count)
    {
        return ACQ_ERROR_CHANNEL;
    }

    if (acquisition->data_index < 0 || acquisition->data_index >= channel_count)
    {
        acquisition->data_index = 0;
    }

    acquisition->value = 
        rpm_data[acquisition->channel - 1][acquisition->data_index];

    acquisition->data_index = 
        (acquisition->data_index + 1) %
        (sizeof(rpm_data[0]) / sizeof(rpm_data[0][0]));

    *rpm = acquisition->value;

    return ACQ_OK;
}


//采集状态转字符串

const char *acquisition_result_to_string(
    AcquisitionResult acquisition_result
)
{
    switch (acquisition_result)
    {
        case ACQ_OK:
            return "ACQ_OK";

        case ACQ_ERROR_CHANNEL:
            return "ACQ_ERROR_CHANNEL";
        
        case ACQ_ERROR_NULL:
            return "ACQ_ERROR_NULL";

        case ACQ_ERROR_READ:
            return "ACQ_ERROR_READ";

        case ACQ_ERROR_TIMEOUT:
            return "ACQ_ERROR_TIMEOUT";

        default:
            return "Unknown acquisition state";
    }
}

