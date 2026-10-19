#ifndef SYSTEM_H
#define SYSTEM_H

typedef enum 
{
    SYSTEM_STOP,
    SYSTEM_RUNNING,
    SYSTEM_ERROR
} SystemState;

typedef enum 
{
    COMMAND_NONE,
    COMMAND_STOP,
    COMMAND_START,
    COMMAND_RESET,
    COMMAND_EXIT,
    COMMAND_SIMULATE_ERROR
} Command;

Command input_command(void);


//系统状态转字符串
const char *system_state_to_string(SystemState state);

//更新系统状态
SystemState system_update_state(SystemState now_system_state,
                                Command command,
                                int *error_flag);




#endif // !SYSTEM_H