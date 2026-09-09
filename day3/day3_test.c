#include <stdio.h>

#define SAMPLE_COUNT 8
#define WINDOW_SIZE 3

// void smoothed_average(const double input[], double output[], int sample_count, int window_size)
// {
//     for (int i = 0; i < sample_count; i++)
//     {
//         double sum = 0.0;

//         for (int j = 0; j < window_size; j++)
//         {
//             sum += input[i + j];
//         }

//         output[i] = sum / window_size;
//     }
// }

// int main(void)
// {
//     double arr[SAMPLE_COUNT] = {1000, 2000, 3000, 5000, 2000, 1000, 5000, 10100};
//     double smoothed[SAMPLE_COUNT - WINDOW_SIZE + 1];

//     smoothed_average(arr, smoothed, SAMPLE_COUNT, WINDOW_SIZE);

//     for (int i = 0; i < SAMPLE_COUNT - WINDOW_SIZE + 1; i++)
//     {
//         printf("smoothed[%d] = %.2f\n", i, smoothed[i]);
//     }

//     return 0;
// }

void moving_average (const double input[], double output[], int count, int window)
{
    for (int i = 0; i < count; i++)
    {
        double sum = 0.0;

        for (int j = 0; j < window; j++)
        {
            sum += input[i + j];
        }

        output[i] = sum / 3.0;
    }
}

int main(void)
{
    double rpm[SAMPLE_COUNT] = {1000, 2000, 3000, 4000, 2000, 2000};
    double smoothed[SAMPLE_COUNT - WINDOW_SIZE + 1];

    //调用滑动平均函数
    moving_average(rpm, smoothed, SAMPLE_COUNT, WINDOW_SIZE);

    for (int i = 0; i < SAMPLE_COUNT - WINDOW_SIZE + 1; i++)
    {
        printf("Smoothed[%d] = %.2f\n", i, smoothed[i]);
    }

    return 0;
}