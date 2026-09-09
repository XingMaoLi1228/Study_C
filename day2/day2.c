#include <stdio.h>

// int main(void)
// {
//     double rpm;
//     double rotation_frequency;
//     double tooth_frequency;
//     int teeth;
//     double teeth_time;
//     double tooth_interval_us;

//     printf("Enter axle rotation speed in rpm: ");
//     scanf("%lf", &rpm);

//     printf("Enter number of teeth on the gear: ");
//     scanf("%d", &teeth);

//     rotation_frequency = rpm / 60.0;
//     tooth_frequency = rotation_frequency * teeth;
//     teeth_time = 1.0 / tooth_frequency;
//     tooth_interval_us = teeth_time * 1000000.0;

//     printf("Axle rotation frequency: %.1f Hz\n", rotation_frequency);
//     printf("Tooth frequency: %.1f Hz\n", tooth_frequency);
//     printf("Time between teeth: %.6f seconds\n", teeth_time);
//     printf("Time between teeth: %.2f microseconds\n", tooth_interval_us);

//     return 0;
// }

int main(void)
{
    double fs;
    int sample_count;

    double duration;      //采样时长
    double sample_interval;    //采样间隔
    double frequency_resolution; //频率分辨率

    printf("Enter sample rate (Hz): ");
    scanf("%lf", &fs);

    printf("Enter sample count: ");
    scanf("%d", &sample_count);

    duration = sample_count / fs;
    sample_interval = 1.0 / fs;
    frequency_resolution = 1.0 / duration;

    printf("Duration: %.6f seconds\n", duration);
    printf("sample interval: %.6f seconds\n", sample_interval);
    printf("frequency resolution: %.6f Hz\n", frequency_resolution);
    
    return 0;
}