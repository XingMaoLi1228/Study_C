#include <stdio.h>

#define SAMPLE_COUNT 5
#define WINDOW_SIZE 3

//求和
void sum_array(const double *data, int count, double *sum)
{
    if (data == NULL || sum == NULL || count <= 0)
    {
        return;
    }

    for (int i = 0; i < count; i++)
    {
        *sum += *data;
        data++;
    }
}

//遍历数组并打印
void print_array(const double *data, int count)
{
    if (data == NULL || count <= 0)
    {
        return;
    }

    for (int i = 0; i < count; i++)
    {
        printf("data[%d] = %.2f\n", i, *data);
        data++;
    }
}

//求均值
void average_array(const double *data, int count, double *avg_data)
{
    if (data == NULL || avg_data == NULL || count <= 0)
    {
        return;
    }

    double sum = 0.0;

    for (int i = 0; i < count; i++)
    {
        sum += *data;
        data++;
    }

    *avg_data = sum / count;
}

//求最大最小值
void max_min_array(const double *data, int count, double *max, double *min)
{
    if (data == NULL || max == NULL || min == NULL || count <= 0)
    {
        return;
    }

    *max = *data;
    *min = *data;
    data++;

    for (int i = 1; i < count; i++)
    {
        if (*data > *max)
        {
            *max = *data;
        }
        if (*data < *min)
        {
            *min = *data;
        }

        data++;
    }
}

//滑动平均
void smoothed_average(const double *input, double *output, int count, int window)
{
    if (input == NULL || output == NULL || count <= 0 || window <= 0 || window > count)
    {
        return;
    }

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
    double rpm[5] = {1000, 2000, 3000, 4000, 5000};
    double smoothed_rpm[SAMPLE_COUNT - WINDOW_SIZE + 1];
    double avg_rpm;
    double max_rpm;
    double min_rpm;

    print_array(rpm, SAMPLE_COUNT);
    average_array(rpm, SAMPLE_COUNT, &avg_rpm);
    max_min_array(rpm, SAMPLE_COUNT, &max_rpm, &min_rpm);
    smoothed_average(rpm, smoothed_rpm, SAMPLE_COUNT, WINDOW_SIZE);

    printf("Average rpm: %.2f\n", avg_rpm);
    printf("Maximum rpm: %.2f\nMinimum rpm: %.2f\n", max_rpm, min_rpm);

    for (int i = 0; i < SAMPLE_COUNT - WINDOW_SIZE + 1; i++)
    {
        printf("smoothed_avg_rpm[%d] = %.2f\n", i, smoothed_rpm[i]);
    }

    return 0;
}