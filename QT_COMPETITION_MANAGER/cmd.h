#ifndef CMD_H
#define CMD_H

typedef int CMD;
typedef unsigned int TYPE;

const CMD CMD_DEFAULT(-1);
const CMD CMD_STUDENT_REGIST(0);
const CMD CMD_STUDENT_LOGIN(1);
const CMD CMD_TEACHER_LOGIN(2);
const CMD CMD_ADMIN_LOGIN(3);
const CMD CMD_SCORE_SELECT(4);
const CMD CMD_TAKE_MATCH(5);
const CMD CMD_GIVE_SCORE(6);
const CMD CMD_ATTENPT_PROJECT(7);
const CMD CMD_SELECT_ALL_STUDENT(8);
const CMD CMD_STUDENT_DEL(9);
const CMD CMD_STUDENT_UPDATE(10);
//const CMD CMD_UPLOAD_FILE(12);
const CMD CMD_SELECT_ALL_PROJECT(11);

const TYPE student_type(0);
const TYPE teacher_type(1);
const TYPE classes_type(2);
const TYPE department_type(3);
const TYPE admin_type(4);
const TYPE project_type(5);
const TYPE st_project_type(6);
const TYPE tc_project_type(7);
const TYPE performance_type(8);

class command
{
public:
    command();
    void cmd_new_status(CMD);
    void cmd_exec_false();
    void cmd_exec_success();
    CMD get_cur_status();
    bool cmd_success();
private:
    CMD __cmd;
    bool is_cmd_success;
};

#endif // CMD_H
