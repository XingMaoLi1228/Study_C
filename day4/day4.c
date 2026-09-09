#include <stdio.h>


//&取地址，  *解引用

// int main(void)
// {
//     int a = 10;

//     //定义一个指针
//     int *p = &a;
//     printf("a = %d\n &a = %d", *p, p);
// }


// void change(int *x)
// {
//     *x = 2000;
// }

// int main(void)
// {
//     // int rpm = 1500;

//     // int *p = &rpm;

//     // *p = 100;

//     // printf("rpm = %d\n", rpm);
//     // printf("address = %p\n", (void *)&rpm);
//     // printf("p = %p\n", (void *)p);
//     // printf("*p = %d\n", *p);

//     int rpm = 1500;
//     change(&rpm);

//     printf("rpm = %d\n", rpm);

//     return 0;
// }


//test，定义一个函数，可以修改转速


// void add_rpm(double *rpm, double delta)
// {
//     *rpm += delta;
// }

// int main(void)
// {
//     double rpm = 1500;

//     add_rpm(&rpm, 100);

//     printf("rpm = %.2f\n", rpm);

//     return 0;
// }


//test 一个函数通过多个指针修改多个变量

// void update_status(double *rpm, double *temperature)
// {
//     *rpm += 10;
//     *temperature += 5;
// }

// int main(void)
// {
//     double rpm = 1500;
//     double temperature = 60;

//     update_status(&rpm, &temperature);

//     printf("rpm = %.2f\ntemperature = %0.2f\n", rpm, temperature);
//     return 0;
// }


//数组与指针

// int main(void)
// {
//     double rpm[4] = {1000, 1200, 1500, 1800};

//     printf("rpm[0] = %.2f\n", rpm[0]);
//     printf("*rpm = %.2f\n", *rpm);

//     printf("rpm[1] = %.2f\n", rpm[1]);
//     printf("*(rpm + 1) = %.2f\n", *(rpm + 1));


//     return 0;
// }

//数组中的每个值都增加100

// void add_all(double *data, int count)
// {
//     for(int i = 0; i < count; i++)
//     {
//         *(data + i) += 100;
//     }
// }

// int main(void)
// {
//     double rpm[4] = {1000, 1200, 1500, 1800};

//     add_all(rpm, 4);

//     for (int i = 0; i < 4; i++)
//     {
//         printf("rpm[%d] = %.2f\n", i, *(rpm + i));
//     }
//     return 0;
// }


//返回数组中最大的值

// double find_max(double *data, int count)
// {
//     double max_data = *data;
    
//     for(int i = 0; i < count; i++)
//     {
//         if (*(data + i) > max_data)
//         {
//             max_data = *(data + i);
//         }
//     }

//     return max_data;
// }


// int main(void)
// {
//     double rpm[4] = {1000, 1200, 1500, 1800};

//     double max_rpm;

//     max_rpm = find_max(rpm, 4);

//     printf("max_rpm = %.2f\n", max_rpm);

//     return 0;
// }

// double find_max(double *data, int count)
// {
//     double max_data = data[0];

//     for (int i = 0; i < count; i++)
//     {
//         if (data[i] > max_data)
//         {
//             max_data = data[i];
//         }
//     }

//     return max_data;
// }

// int main(void)
// {
//     double rpm[4] = {1000, 2000, 3000, 4000};

//     double max_rpm = find_max(rpm, 4);

//     printf("max_rpm = %.2f\n", max_rpm);

//     return 0;
// }


//通过指针同时输出最大最小值

// void find_min_max(double *data, int count, double *min, double *max)
// {
//     *min = *data;
//     *max = *data;

//     for (int i = 1; i < count; i++)
//     {
//         if (data[i] > *max)
//         {
//             *max = data[i];
//         }
//         if (data[i] < *min)
//         {
//             *min = data[i];
//         }
//     }
// }

// int main(void)
// {
//     double rpm[4] = {1000, 1200, 1400, 1600};
//     double max_rpm;
//     double min_rpm;

//     find_min_max(rpm, 4, &min_rpm, &max_rpm);

//     printf("max_rpm = %.2f\n", max_rpm);
//     printf("min_rpm = %.2f\n", min_rpm);

//     return 0;
// }


//一次计算三个结果

// void rpm_analysis(const double *data,
//                 int count,
//                 double *average,
//                 double *max,
//                 double *min)
// {
//     double sum = data[0];

//     *max = data[0];
//     *min = data[0];

//     for (int i = 1; i < count; i++)
//     {
//         sum += data[i];

//         if (data[i] > *max)
//         {
//             *max = data[i];
//         }
//         if (data[i] < *min)
//         {
//            *min = data[i];
//         }
//     }

//     *average = sum / count;

//     data[0] = 10;
// }

// int main(void)
// {
//     double rpm[6] = {1000, 1100, 1300, 1250, 1500, 1600};
//     double avg_rpm;
//     double max_rpm;
//     double min_rpm;

//     rpm_analysis(rpm, 6, &avg_rpm, &max_rpm, &min_rpm);

//     printf("Average rpm: %.2f\n", avg_rpm);
//     printf("Maximum rpm: %.2f\n", max_rpm);
//     printf("Minimum rpm: %.2f\n", min_rpm);

//     return 0;
// }


// int main(void)
// {

//     double a = 10;
//     double b = 20;

//     //可以改变指向的位置，不可以改变指向位置的值
//     const double *p;

//     //可以改变指向位置的值，不可以改变指向的位置
//     double *const p;

//     // *p = 20; //不允许
//     p = &b;

//     //定义空指针，“这个指针目前没指向有效对象”
//     double *p = NULL;

//     printf("*p = %.2f\n", *p);
//     printf("p = %p\n", p);
//     return 0;
// }

// void set_rpm(double *rpm, int count)
// {
//     if (rpm == NULL || count <= 0)
//     {
//         return;
//     }
// }

// int main(void)
// {
//     double rpm = 0;

//     set_rpm(NULL, 1);

//     printf("%.2f\n", rpm);

//     return 0;
// }


// int main(void)
// {
//     int size_double = sizeof(double);

//     printf("size_double = %d\n", size_double);
//     return 0;
// }

// void print_array(const double *data, int count)
// {
//     for (int i = 0; i < count; i++)
//     {
//         printf("data[%d] = %.2f\n", i, *data);

//         data++;

//     }
// }

// int main(void)
// {
//     double rpm[5] = {100, 200, 300, 400, 500};

//     print_array(rpm, 5);

//     return 0;
// }

//使用指针遍历数组并求和

// double sum_array(const double *data, int count)
// {
//     double sum = 0.0;

//     //安全检查
//     if (data == NULL || count <= 0)
//     {
//         return 0.0;
//     }

//     for (int i = 0; i < count; i++)
//     {
//         printf("data[%d] = %.2f\n", i, *data);

//         sum += *data;

//         data++; //移动指针
//     }

//     return sum;

// }

// int main(void)
// {
//     double rpm[5] = {100, 200, 300, 400, 500};

//     double sum_rpm = sum_array(rpm, 5);

//     printf("Sum rpm: %.2f\n", sum_rpm);

//     return 0;
// }

double average_array(const double *data, int count)
{
    double sum = 0.0;

    if (data == NULL || count <= 0)
    {
        return 0.0;
    }

    for (int i = 0; i < count; i++)
    {
        sum += *data;
        data++;
    }

    return sum / count;

}

int main(void)
{
    double rpm[5] = {1000, 1200, 1400, 1600, 1800};

    double avg_rpm = average_array(rpm, 5);

    printf("Average rpm = %.2f\n", avg_rpm);

    return 0;
}