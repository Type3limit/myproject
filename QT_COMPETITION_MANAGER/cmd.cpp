#include "cmd.h"

command::command()
{
    __cmd=CMD_DEFAULT;
    is_cmd_success=false;
}

void command::cmd_new_status(CMD new_status)
{
    __cmd=new_status;
}

void command::cmd_exec_false()
{
    is_cmd_success=false;
}

void command::cmd_exec_success()
{
    is_cmd_success=true;
}

CMD command::get_cur_status()
{
    return __cmd;
}

bool command::cmd_success()
{
    return is_cmd_success;
}
