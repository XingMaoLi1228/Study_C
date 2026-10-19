#ifndef ACQUISITION_H
#define ACQUISITION_H

typedef struct 
{
    int channel;
    double value;
    int data_index;
} Acquisition;

//初始化采集
void acquisition_init(Acquisition *acquisition, int channel);

//采集数据
double acquisition_read(Acquisition *acquisition);


#endif // !ACQUISITION_H

