#ifndef ACQUISITION_H
#define ACQUISITION_H




//采集状态
typedef struct 
{
    int channel;
    double value;
    int data_index;
} Acquisition;

//采集状态枚举
typedef enum
{
    ACQ_OK,
    ACQ_ERROR_CHANNEL,
    ACQ_ERROR_NULL,
    ACQ_ERROR_READ,
    ACQ_ERROR_TIMEOUT,
    ACQ_ERROR_LENGTH
} AcquisitionResult;

//初始化采集
void acquisition_init(Acquisition *acquisition, int channel);

//采集数据
AcquisitionResult acquisition_read(Acquisition *Acquisition,
    double *rpm);

//采集状态装字符串
const char *acquisition_result_to_string(
    AcquisitionResult acquisition_result
);

#endif // !ACQUISITION_H



