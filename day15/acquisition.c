#include <stdio.h>

#include "acquisition.h"

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

double acquisition_read(Acquisition *acquisition)
{
    if (acquisition == NULL)
    {
        return 0.0;
    }

    double rpm_data[4][12] =
    {
        {800, 900, 1100, 1200, 1300, 1400, 1500, 500, 500, 500, 500, 500},
        {700, 750, 800, 850, 900, 950, 1000, 1050, 1100, 1150, 1200, 1250},
        {1200, 1250, 1300, 1350, 1400, 1450, 1500, 1550, 1600, 1650, 1700, 1750},
        {500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600}
    };

    acquisition->value = 
        rpm_data[acquisition->channel - 1][acquisition->data_index];

    acquisition->data_index = 
        (acquisition->data_index + 1) %
        (sizeof(rpm_data[0]) / sizeof(rpm_data[0][0]));

    return acquisition->value;
}

