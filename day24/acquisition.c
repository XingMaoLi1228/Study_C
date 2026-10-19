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
        {5000, 5100, 800, 5200, 5300, 5400,800, 800, 800, 800, 800, 800},
        {700, 750, 800, 850, 900, 950, 1000, 1050, 1100, 1150, 1200, 1250},
        {1200, 1250, 1300, 1350, 1400, 1450, 1500, 1550, 1600, 1650, 1700, 1750},
        {500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600}
    };

    //通道数
    int channel_count = sizeof(rpm_data) / sizeof(rpm_data[0]);


    //样本数
    int sample_count = sizeof(rpm_data[0]) / sizeof(rpm_data[0][0]);


    //检查通道号，合法范围是 1~4
    if (acquisition->channel < 1 || acquisition->channel > channel_count)
    {
        return ACQ_ERROR_CHANNEL;
    }

    //检查样本索引，合法范围是 0~11
    if (acquisition->data_index < 0 || acquisition->data_index >= sample_count)
    {
        acquisition->data_index = 0;
    }

    //读取当前通道的当前样本
    acquisition->value = 
        rpm_data[acquisition->channel - 1][acquisition->data_index];

    //移动到下一个样本，读完一行后回到开头
    acquisition->data_index = 
        (acquisition->data_index + 1) %
        (sizeof(rpm_data[0]) / sizeof(rpm_data[0][0]));

    //将结果写回调用者
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

        case ACQ_ERROR_LENGTH:
            return "ACQ_ERROR_LENGTH";

        default:
            return "Unknown acquisition state";                           
    }
}

