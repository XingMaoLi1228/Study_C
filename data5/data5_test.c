#include <stdio.h>

#define SAMPLE_COUNT 5
#define WINDOW_SIZE 3

//求和
double sum_array(const double *data, int count)
{
    
    if (data == NULL || count <= 0)
    {
        return 0.0;
    }

    double sum = 0.0;

    for (int i = 0; i < count; i++)
    {
        sum += *data;
        data++;
    }

    return sum;
}

//最大最小
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

//平均
double average_array(const double *data, int count)
{
    if (data == NULL || count <= 0)
    {
        return 0.0;
    }

    double sum = 0.0;

    for (int i = 0; i < count; i++)
    {
        sum += *data;
        data++;
    }

    return sum / count;
}

//遍历数组
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

//滑动平均
void smoothed_average(const double *input, double *output, int count, int window)
{
    if (input == NULL || output == NULL || count <= 0 || window <= 0 || window > count)
    {
        return;
    }

    double sum;

    for (int i = 0; i < count - window + 1; i++)
    {
        sum = 0.0;

        for (int j = 0; j < window; j++)
        {
            sum += input[i + j];
        }

        output[i] = sum / window;
    }
}

int main(void)
{
    double rpm[SAMPLE_COUNT] = {1000, 1200, 1500, 1800, 2000};
    double smoothed_rpm[SAMPLE_COUNT - WINDOW_SIZE + 1];
    double max_rpm;
    double min_rpm;
    double avg_rpm;
    double sum_rpm;

    sum_rpm = sum_array(rpm, SAMPLE_COUNT);
    avg_rpm = average_array(rpm, SAMPLE_COUNT);
    max_min_array(rpm, SAMPLE_COUNT, &max_rpm, &min_rpm);
    print_array(rpm, SAMPLE_COUNT);
    smoothed_average(rpm, smoothed_rpm, SAMPLE_COUNT, WINDOW_SIZE);

    printf("Sum rpm: %.2f\n", sum_rpm);
    printf("Maximum rpm: %.2f\n", max_rpm);
    printf("Minimum rpm: %.2f\n", min_rpm);
    printf("Average rpm: %.2f\n", avg_rpm);

    for (int i = 0; i < SAMPLE_COUNT - WINDOW_SIZE + 1; i++)
    {
        printf("Smoothed average rpm: %.2f\n", smoothed_rpm[i]);
    }

    return 0;
}