#include <stdio.h>
#include <ctype.h>

// int main(void)
// {
//     // double rpm;
//     // double rotation_frequency;

//     // printf("Enter axle rotation speed in rpm: ");
//     // scanf("%lf", &rpm);

//     // rotation_frequency = rpm / 60.0;
//     // printf("Axle rotation frequency: %.1f Hz\n", rotation_frequency);

//     float signal_frequency = 0.0;
//     float sample_rate = 0.0;
//     float sample_per_cycle;

//     while (signal_frequency <= 0 || sample_rate <= 0)
//     {
//         printf("Enter signal_frequency (Hz): ");
//         scanf("%f", &signal_frequency);
//         printf("Enter sample_rate(Hz): ");
//         scanf("%f", &sample_rate);
//     }
//     // if (signal_frequency <= 0 || sample_rate <= 0)
//     // {
//     //     printf("Error: frequency must be greater than zero");
//     //     return 1;   //立即退出程序，返回错误码1
//     // }

//     sample_per_cycle = (sample_rate / signal_frequency);

//     if (sample_per_cycle <= 2.0f)
//     {
//         printf("Aliasing risk");
//     }
//     else if (sample_per_cycle < 10.0f)
//     {
//         printf("Too few samples");
//     }
//     else if (sample_per_cycle < 128.0f)
//     {
//         printf("Sampling is acceptable");
//     }
//     else
//     {
//         printf("Sampling quality is good");
//     }

//     return 0;
// }


// int main(void)
// {
//     double rpm = -1.0;
//     double rotation_frequency;

//     while (rpm < 0.0)
//     {
//         printf("Enter axle rotation sppeed in rpm:");
//         scanf("%lf", &rpm);

//         if (rpm < 0.0f) 
//         {
//             printf("Error: rotation speed must be greater than or equal to zero\n");
//         }
//         else if (rpm == 0.0f)
//         {
//             printf("Stopped\n");
//         }
//         else if (rpm < 600.0f)
//         {
//             printf("Low speed\n");
//         }
//         else if (rpm <= 3000.0f)
//         {
//             printf("Normal speed\n");
//         }
//         else
//         {
//             printf("Overspeed\n");
//         }
//     }

//     rotation_frequency = rpm / 60.0;
//     printf("Axle rotation frequency: %.2f Hz\n", rotation_frequency);

//     return 0;
// }


// int main(void)
// {
//     double rpm = 0.0;
//     double rotation_frequency;
//     int valid_count = 0;    //有效转速次数
//     double total_rpm = 0.0; //累计有效转速
//     double avg_rpm = 0.0;   //平均转速
//     double max_rpm = 0.0;   //最大转速
//     double min_rpm = 0.0;   //最小转速
//     bool first_input = true;
//     int ch;
//     int has_extra;

//     while (1)
//     {
//         has_extra = 0;

//         printf("Enter axle rotation speed in rpm (-1 to exit):");
//         int result = scanf("%lf", &rpm);

//         //判断输入是否有效，字母则清空输入缓冲区
//         if (result == EOF)
//         {
//             printf("Input error\n");
//             break;
//         }
//         else if (result != 1)
//         {

//             while ((ch = getchar()) != '\n' &&  ch != EOF)
//             {
//                 //清除输入缓冲区的无效数字
//             }
//             continue;
//         }

//         while ((ch = getchar()) != '\n' &&  ch != EOF)
//         {
//             if (!isspace(ch))  //判断是否为空格
//             {
//                 has_extra = 1;
//             }
//         }

//         if (has_extra)
//         {
//             printf("Please enter only a number\n");
//             continue;
//         }

//         //转速判断
//         if (rpm < 0.0)
//         {
//             if (rpm == -1.0)
//             {
//                 printf("Exiting program\n");
//                 break;
//             }
//             else
//             {
//                 printf("Error: rotation speeed must be greater than or equal to zero\n");
//                 continue;
//             }
//         }
//         else if (rpm == 0.0)
//         {
//             printf("Stopped\n");
//         }
//         else if (rpm < 600.0)
//         {
//             printf("Low speed\n");
//         }
//         else if (rpm <= 3000.0)
//         {
//             printf("normal speed\n");
//         }
//         else
//         {
//             printf("Overspeed\n");
//         }

//         //判断最大最小转速
//         if (first_input)
//         {
//             max_rpm = rpm;
//             min_rpm = rpm;
//             first_input = false;
//         }
//         else
//         {
//             if (rpm > max_rpm)
//             {
//                 max_rpm = rpm;
//             }
//             if (rpm < min_rpm)
//             {
//                 min_rpm = rpm;
//             }
//         }

//         //统计有效转速和次数
//         valid_count++;
//         total_rpm += rpm;

//         rotation_frequency = rpm / 60.0;
//         printf("Axle rotation frequency: %.2f Hz\n", rotation_frequency);
//     }

//     //判断有效转速次数是否大于0，计算平均转速
//     if (valid_count > 0)
//     {
//         avg_rpm = total_rpm / valid_count;
//         printf("Total valid rotation speed count: %d\n", valid_count);
//         printf("Average rotation speed: %.2f rpm\n", avg_rpm);
//         printf("Maximum rotation speed: %.2f rpm\n", max_rpm);
//         printf("Minimum rotation speed: %.2f rpm\n", min_rpm);
//     }
//     else
//     {
//         printf("No valid data\n");
//     }

//     return 0;
// }




//函数
void print_speed_status(double rpm)
{
    if (rpm == 0.0)
    {
        printf("Stopped\n");
    }
    else if (rpm < 600.0)
    {
        printf("Low speed\n");
    }
    else if (rpm <= 3000.0)
    {
        printf("Normal speed\n");
    }
    else
    {
        printf("Overspeed\n");
    }
}

double calculate_rotation_frequency(double rpm)
{
    double rotation_frequency = rpm / 60.0;
    return rotation_frequency;
}


int main(void)
{
    double rpm = 0.0;
    double rotation_frequency;
    int valid_count = 0;
    double total_rpm = 0.0;
    double avg_rpm = 0.0;
    double max_rpm = 0.0;
    double min_rpm = 0.0;
    bool first_input = true;
    int ch;
    int has_extra;

    while (1)
    {
        has_extra = 0;

        printf("Enter axle rotation speed in rpm (-1 to exit):");
        int result = scanf("%lf", &rpm);

        if (result == EOF)
        {
            printf("Input errror\n");
            break;
        }
        else if (result != 1)
        {
            printf("Please enter a number");

            while((ch = getchar()) != '\n' && ch != EOF)
            {
                //清空缓冲区
            }
            continue;
        }

        while ((ch = getchar()) != '\n' && ch != EOF)
        {
            if (!isspace(ch))
            {
                has_extra = 1;
            }
        }
        if (has_extra)
        {
            printf("Please enter only a number\n");
            continue;
        }

        if (rpm < 0.0)
        {
            if (rpm == -1.0)
            {
                printf("Exiting program\n");
                break;
            }
            else
            {
                printf("Error: rotation speed must be greater than or equal to zero\n");
                continue;
            }
        }

        print_speed_status(rpm);  //调用函数

        if (first_input)
        {
            max_rpm = rpm;
            min_rpm = rpm;
            first_input = false;
        }
        else
        {
            if (rpm > max_rpm)
            {
                max_rpm = rpm;
            }
            if (rpm < min_rpm)
            {
                min_rpm = rpm;
            }
        }
        
        valid_count++;
        total_rpm += rpm;
        rotation_frequency = calculate_rotation_frequency(rpm);
        printf("Axle rotation frequency: %.2f Hz\n", rotation_frequency);
    }

    if (valid_count > 0)
    {
        avg_rpm = total_rpm / valid_count;

        printf("Total valid rotation speed count: %d\n", valid_count);
        printf("Average rotation speed: %.2f rpm\n", avg_rpm);
        printf("Maximum rotation speed: %.2f rpm\n", max_rpm);
        printf("Minimum rotation speed: %.2f rpm\n", min_rpm);
    }
    else
    {
        printf("No valid data\n");
    }

    return 0;
}


