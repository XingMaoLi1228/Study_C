#include <stdio.h>


// int main(void)
// {
//     double rpm[5];
//     double total_rpm = 0.0;
//     double avg_rpm;
//     double max_rpm;
//     double min_rpm;
//     double delta;
//     double max_delta;
//     int max_delta_index = 1;

//     for (int i = 0; i < 5; i++)
//     {
//         printf("Enter rpm %d: ", i + 1);
//         scanf("%lf", &rpm[i]);

        
//         if (i == 0)
//         {
//             max_rpm = rpm[i];
//             min_rpm = rpm[i];
//         }
//         else
//         {
//             if (rpm[i] > max_rpm)
//             {
//                 max_rpm = rpm[i];
//             }
//             if (rpm[i] < min_rpm)
//             {
//                 min_rpm = rpm[i];
//             }
//         }
//     }

//     for (int i = 0; i < 5; i++)
//     {
//         total_rpm += rpm[i];

//         printf("rpm[%d] = %.2f\n", i, rpm[i]);
//     }

//     for (int i = 1; i < 5; i++)
//     {
//         delta = rpm[i] - rpm[i - 1];

//         if (delta < 0)
//         {
//             delta = -delta;
//         }

//         if (delta > max_delta)
//         {
//             max_delta = delta;
//             max_delta_index = i;
//         }
//     }
//     avg_rpm = total_rpm / 5.0;

//     printf("Average alxe rotation speed: %.2f rpm\n", avg_rpm);
//     printf("Maximum alxe rotation speed: %.2f rpm\n", max_rpm);
//     printf("Minimum alxe rotation speed: %.2f rpm\n", min_rpm);
//     printf("Maximum adjacent speed: %.2f rpm\n", max_delta);
//     printf("Maximum adjacent speed change: %.2f rpm\n", rpm[max_delta_index]);
//     printf("Between point %d and point %d\n", max_delta_index, max_delta_index + 1);
//     printf("Speed: %.2f -> %.2f\n", rpm[max_delta_index - 1], rpm[max_delta_index]);

//     return 0;
// }

//定义宏
#define SAMPLE_COUNT  8  
#define WINDOW_SIZE  6


//void 函数不能写 return 0;
//这里的 const 表示：函数可以读取 input 中的数据，但不能通过 input 修改它们。
void moving_average(const double input[], double output[],
                    int count, int window)
{
    for (int i = 0; i < count - window + 1; i++)
    {
        double sum = 0.0;

        for (int j = 0; j < window; j++)
        {
            sum += input[i + j];
        }
        
        output[i] = sum / window;
    }

}

int main(void)
{
    double rpm[SAMPLE_COUNT] = {1000, 1000, 1600, 1000, 1000, 1400, 1500, 1400};
    double changes[SAMPLE_COUNT - 1];
    double smoothed[SAMPLE_COUNT - WINDOW_SIZE + 1];

    for (int i = 0; i < SAMPLE_COUNT - 1; i++)
    {
        changes[i] = rpm[i + 1] - rpm[i];
    }

    moving_average(rpm, smoothed, SAMPLE_COUNT, WINDOW_SIZE);

    for (int i = 0; i < SAMPLE_COUNT - WINDOW_SIZE + 1; i++)
    {
        printf("Smoothed[%d] = %.2f\n", i, smoothed[i]);
    }

    for (int i = 0; i < SAMPLE_COUNT - 1; i++)
    {
        printf("Point %d -> %d: %.2f rpm\n", i + 1, i + 2, changes[i]);
    }

    return 0;
}