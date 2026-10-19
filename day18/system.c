#include "system.h"
#include <stdio.h>
#include <conio.h>

//系统状态转字符串
const char *system_state_to_string(SystemState state)
{
    switch (state)
    {
        case SYSTEM_STOP:
            return "SYSTEM_STOP";

        case SYSTEM_RUNNING:
            return "SYSTEM_RUNNING";

        case SYSTEM_ERROR:
            return "SYSTEM_ERROR";

        default:
            return "Unknown system state!";
    }
}

//更新系统中状态
SystemState system_update_state(SystemState now_system_state, 
                                Command command, 
                                int *error_flag)
{
    if (error_flag == NULL)
    {
        return SYSTEM_ERROR;
    }


    if (command == COMMAND_RESET)
    {
        *error_flag = 0;
        
        return SYSTEM_STOP;
    }

    if (*error_flag)
    {
        return SYSTEM_ERROR;
    }

    switch (now_system_state)
    {
        case SYSTEM_STOP:

            if (command == COMMAND_START)
            {
                return SYSTEM_RUNNING;
            }
            break;

        case SYSTEM_RUNNING:
            
            if (command == COMMAND_STOP)
            {
                return SYSTEM_STOP;
            }
            break;

        case SYSTEM_ERROR:
            break;

        default:
            return SYSTEM_ERROR;
    }

    return now_system_state;
}


Command input_command(void)
{
    char input;

    if (!_kbhit())
    {
        return COMMAND_NONE;
    }

    input =(char)_getch();

    switch (input)
    {
        case 's':
        case 'S':
            return COMMAND_START;

        case 'p':
        case 'P':
            return COMMAND_STOP;

        case 'r':
        case 'R':
            return COMMAND_RESET;

        case 'q':
        case 'Q':
            return COMMAND_EXIT;

        case 'e':
        case 'E':
            return COMMAND_SIMULATE_ERROR;

        default:
            return COMMAND_NONE;
    }
}


