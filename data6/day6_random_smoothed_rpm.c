#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>  

#define SENSOR_COUNT 4
#define HISTORY_SIZE 5

#define ALARM_CONFIRM 3
#define RECOVERY_CONFIRM 3
#define ALARM_RPM 1000
#define RECOVERY_RPM 900

//定义命令枚举
typedef enum
{
    CMD_NONE,
    CMD_RESET,
    CMD_QUIT
} Command;

//定义运行状态枚举 
typedef enum
{
    STATE_STOP,
    STATE_RUNNING,
    STATE_ALARM,
    STATE_ERROR
} DeviceState;

//定义传感器结构体
typedef struct 
{
    int channel;

    double rpm_buffer[HISTORY_SIZE];
    int valid_count;
    int write_index;

    double average_rpm;
    double rpm_sum;

    DeviceState state;

    int alarm_count; 
    int recover_count;

    int error_latched;
} Sensor;


//声明函数
double generate_random_rpm(double min_rpm, double max_rpm);

Command read_command(void);

void handle_command(
        Command command,
        Sensor *sensors,
        const double *new_rpm,
        int sensor_count,
        int *running
);



//生成随机范围的数
double generate_random_rpm(double min_rpm, double max_rpm)
{
    double ratio = (double)rand() / RAND_MAX;  //RAND_MAX 最大的随机数

    return min_rpm + ratio * (max_rpm - min_rpm);
}

//读取键盘命令
Command read_command(void)
{
    if (!_kbhit())
    {
        return CMD_NONE;
    }

    char key = _getch();

    if (key == 'r')
    {
        return CMD_RESET;
    }
    else if (key == 'q')
    {
        return CMD_QUIT;
    }

    return CMD_NONE;
}

void push_rpm(Sensor *sensor, double new_rpm, int history_size);

void update_one_state(Sensor *sensor, double new_rpm, double alarm_rpm, double recovery_rpm, int alarm_confirm, int recovery_confirm);

void reset_error(Sensor *sensor, double current_rpm);

void print_state(const Sensor *sensor, int sensor_count);

void process_one_new_state(Sensor *sensor, int history_size, double new_rpm, double alarm_rpm, double recovery_rpm, int alarm_confirm, int recovery_confirm);

void process_all_new_state(Sensor *sensor, double *new_rpm, int sensor_count, int history_size, double alarm_rpm, double recovery_rpm, int alarm_confirm, int recovery_confirm);

void print_ring_buffer(const Sensor *sensor, int history_size);



//处理命令
void handle_command(
        Command command,
        Sensor *sensors,
        const double *new_rpm,
        int sensor_count,
        int *running
)
{
    if (sensors == NULL ||
        new_rpm == NULL ||
        running == NULL ||
        sensor_count <= 0)
    {
        return;
    }

    switch (command)
    {
        case CMD_RESET:

            for (int i = 0; i < sensor_count; i++)
            {
                reset_error(&sensors[i], new_rpm[i]);
            }

            printf("Reset command received.\n");
            break;

        case CMD_QUIT:
            
            *running = 0;
            break;
        
        case CMD_NONE:
        default:
            break;

    }

}

//滑动转速求平均
void push_rpm(Sensor *sensor, double new_rpm, int history_size)
{
    if (sensor == NULL || history_size <= 0 || history_size > HISTORY_SIZE)
    {
        return;
    }

    if (sensor->valid_count == history_size)
    {
        sensor->rpm_sum -= sensor->rpm_buffer[sensor->write_index];
    }

    sensor->rpm_buffer[sensor->write_index] = new_rpm;
    sensor->rpm_sum += new_rpm;

    // sensor->write_index++;

    // if (sensor->write_index >= HISTORY_SIZE)
    // {
    //     sensor->write_index = 0;
    // }
    sensor->write_index = (sensor->write_index + 1) % history_size;

    if (sensor->valid_count < history_size)
    {
        sensor->valid_count++;
    }

    //求平均转速
    sensor->average_rpm = sensor->rpm_sum / sensor->valid_count;
}

//更新单个传感器状态
void update_one_state(Sensor *sensor, double new_rpm, double alarm_rpm, double recovery_rpm, int alarm_confirm, int recovery_confirm)
{
    if (sensor == NULL || 
        alarm_rpm <= 0 || 
        alarm_confirm <= 0 || 
        recovery_confirm <= 0 ||
        recovery_rpm <= 0 ||
        recovery_rpm >= alarm_rpm)
    {
        return;
    }

    if (new_rpm < 0.0)
    {
        sensor->state = STATE_ERROR;
        sensor->error_latched = 1;

        sensor->alarm_count = 0;
        sensor->recover_count = 0;
        return;
    }

    switch (sensor->state)
    {
        case STATE_ERROR:

            // //数据恢复正常，自动恢复
            // if (new_rpm >= 0.0)
            // {
            //     if (new_rpm == 0.0)
            //     {
            //         sensor->state = STATE_STOP;
            //     }
            //     else 
            //     {
            //         sensor->state = STATE_RUNNING;
            //     }
            // }

            break;

        case STATE_STOP:
            if (sensor->average_rpm > 0.0)
            {
                sensor->state = STATE_RUNNING;
            }

            break;

        case STATE_RUNNING:
            if (sensor->average_rpm <= 0.0)
            {
                sensor->state = STATE_STOP;
                break;
            }

            if (sensor->average_rpm >= alarm_rpm)
            {
                sensor->alarm_count++;
            }
            else
            {
                sensor->alarm_count = 0;
            }

            if (sensor->alarm_count >= alarm_confirm)
            {
                sensor->state = STATE_ALARM;

                sensor->alarm_count = 0;
                sensor->recover_count = 0;
            }

            break;

        case STATE_ALARM:
            if (sensor->average_rpm < recovery_rpm)
            {
                sensor->recover_count++;
            }
            else
            {
                sensor->recover_count = 0;
            }

            if (sensor->recover_count >= recovery_confirm)
            {
                sensor->state = STATE_RUNNING;

                sensor->alarm_count = 0;
                sensor->recover_count = 0;
            }

            break;

        default:
            break;
            
    }

}

//错误复位函数
void reset_error(Sensor *sensor, double current_rpm)
{
    if (sensor == NULL)
    {
        return;
    }

    if (sensor->state != STATE_ERROR)
    {
        return;
    }

    if (current_rpm < 0.0)
    {
        return;
    }

    sensor->error_latched = 0;

    if (current_rpm == 0)
    {
        sensor->state = STATE_STOP;
    }
    else
    {
        sensor->state = STATE_RUNNING;
    }
}

//打印通道传感器状态
void print_state(const Sensor *sensor, int sensor_count)
{
    if (sensor == NULL)
    {
        return;
    }

    for (int i = 0; i < sensor_count; i++)
    {
        switch (sensor[i].state)
        {
            case STATE_ERROR:
                printf("CH%d Average_RPM: %.2f  STATE_ERROR Latched:%d\n", sensor[i].channel, sensor[i].average_rpm, sensor[i].error_latched);
                break;
            case STATE_STOP:
                printf("CH%d Average_RPM: %.2f  STATE_STOP Latched:%d\n", sensor[i].channel, sensor[i].average_rpm, sensor[i].error_latched);
                break;
            case STATE_RUNNING:
                printf("CH%d Average_RPM: %.2f  STATE_RUNNING Latched:%d\n", sensor[i].channel, sensor[i].average_rpm, sensor[i].error_latched);
                break;
            case STATE_ALARM:
                printf("CH%d Average_RPM: %.2f  STATE_ALARM Latched:%d\n", sensor[i].channel, sensor[i].average_rpm, sensor[i].error_latched);
                break;
            default:
                printf("CH%d Average_RPM: %.2f  Unknown Latched:%d\n", sensor[i].channel, sensor[i].average_rpm, sensor[i].error_latched);
                break;
        }
    }
};

//计算单个传感器的新平均转速
void process_one_new_state(Sensor *sensor, int history_size, const double new_rpm, double alarm_rpm, double recovery_rpm, int alarm_confirm, int recovery_confirm)
{
    if (sensor == NULL ||
        history_size <= 0 ||
        history_size > HISTORY_SIZE ||
        alarm_rpm <= 0 ||
        recovery_rpm <= 0 ||
        alarm_rpm <= recovery_rpm ||
        alarm_confirm <= 0 ||
        recovery_confirm <= 0)
    {
        return;
    }

    if (new_rpm < 0.0)
    {
        update_one_state(sensor, new_rpm, alarm_rpm, recovery_rpm, alarm_confirm, recovery_confirm);   

        return;
    }

    push_rpm(sensor, new_rpm, history_size);
    update_one_state(sensor, new_rpm, alarm_rpm, recovery_rpm, alarm_confirm, recovery_confirm);

}

//计算所有传感器的新平均转速
void process_all_new_state(Sensor *sensor, double *new_rpm, int sensor_count, int history_size, double alarm_rpm, double recovery_rpm, int alarm_confirm, int recovery_confirm)
{
    if (sensor == NULL ||
        new_rpm == NULL ||
        sensor_count <= 0 ||
        history_size <= 0 ||
        history_size > HISTORY_SIZE ||
        alarm_rpm <= 0 ||
        recovery_rpm <= 0 ||
        recovery_rpm >= alarm_rpm ||
        alarm_confirm <= 0 ||
        recovery_confirm <= 0)
    {
        return;
    }

    for (int i = 0; i < sensor_count; i++)
    {
        process_one_new_state(&sensor[i], history_size, new_rpm[i], alarm_rpm, recovery_rpm, alarm_confirm, recovery_confirm);
    }
}

//打印环形缓冲区
void print_ring_buffer(const Sensor *sensor, int history_size)
{
    if (sensor == NULL || history_size <= 0|| history_size > HISTORY_SIZE)
    {
        return;
    }

    printf("CH%d Buffer: ", sensor->channel);

    int start;

    if (sensor->valid_count < history_size)
    {
        start = 0;
    }
    else
    {
        start = sensor->write_index;
    }

    for (int i = 0; i < sensor->valid_count; i++)
    {
        int index = (start + i) % history_size;

        printf("%.2f ", sensor->rpm_buffer[index]);
    }

    printf("\n");
}

//主函数
int main(void)
{
    Sensor sensors[SENSOR_COUNT] = 
    {
        {1, {0, 0, 0, 0, 0}, 0, 0, 0, 0, STATE_STOP, 0, 0, 0},
        {2, {0, 0, 0, 0, 0}, 0, 0, 0, 0, STATE_STOP, 0, 0, 0},
        {3, {0, 0, 0, 0, 0}, 0, 0, 0, 0, STATE_STOP, 0, 0, 0},
        {4, {0, 0, 0, 0, 0}, 0, 0, 0, 0, STATE_STOP, 0, 0, 0}
    };

    double new_rpm[SENSOR_COUNT];
    int running = 1;


    srand((unsigned int)time(NULL));

    while (running)
    {

        for (int i = 0; i < SENSOR_COUNT; i++)
        {
            new_rpm[i] = generate_random_rpm(-300.0, 2500.0);
        }

        process_all_new_state(
            sensors,
            new_rpm,
            SENSOR_COUNT,
            HISTORY_SIZE,
            ALARM_RPM,
            RECOVERY_RPM,
            ALARM_CONFIRM,
            RECOVERY_CONFIRM
        );

        printf("\n-------  SENSOR SYSTEM  -------\n");

        print_state(sensors, SENSOR_COUNT);

        for (int i = 0; i < SENSOR_COUNT; i++)
        {
            print_ring_buffer(&sensors[i], HISTORY_SIZE);
        }

        Command command = read_command();

        handle_command(command, sensors, new_rpm, SENSOR_COUNT, &running);

        // //获取键盘是否有值
        // if (_kbhit())
        // {
        //     char command = _getch();  //_getch(),获取键盘的字符

        //     if (command == 'r')
        //     {
        //         for (int i = 0; i < SENSOR_COUNT; i++)
        //         {
        //             reset_error(&sensors[i], new_rpm[i]);
        //         }

        //         printf("\nReset command recivide\n");
        //     }
        //     else if (command == 'q')
        //     {
        //         running = 0;
        //     }
        // }

        Sleep(1000);
    }

    return 0;
}


