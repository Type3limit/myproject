#ifndef INFO_H
#define INFO_H
#include "cmd.h"
#include <QString>

struct contain_info
{
    command cmd;
    QString sendbuf;
    QString recvbuf;
};

struct info
{
};

struct student:public info
{
    student(QString c_id="",QString c_no="",QString c_passwd="",QString c_name="",QString c_sex="男",QString c_class_id="",QString c_dp_id="")
        :id(c_id),no(c_no),passwd(c_passwd),name(c_name),sex(c_sex),class_id(c_class_id),dp_id(c_dp_id){}
    QString id;
    QString no;
    QString passwd;
    QString name;
    QString sex;
    QString class_id;
    QString dp_id;
};

struct teacher:public info
{
    teacher(QString c_id="",QString c_no="",QString c_passwd="",QString c_name="",QString c_sex="男",QString c_dp_id="",QString c_tc_info="")
        :id(c_id),no(c_no),passwd(c_passwd),name(c_name),sex(c_sex),dp_id(c_dp_id),tc_info(c_tc_info){}
    QString id;
    QString no;
    QString passwd;
    QString name;
    QString sex;
    QString dp_id;
    QString tc_info;
};

struct classes:public info
{
    classes(QString c_class_id="",QString c_class_no="",QString c_class_name="",QString c_class_grade="",QString c_dp_id="")
        :class_id(c_class_id),class_no(c_class_no),class_name(c_class_name),class_grade(c_class_grade),dp_id(c_dp_id){}
    QString class_id;
    QString class_no;
    QString class_name;
    QString class_grade;
    QString dp_id;
};

struct department:public info
{
    department(QString c_dp_id="",QString c_dp_name="",QString c_dp_phone="",QString c_dp_info="")
        :dp_id(c_dp_id),dp_name(c_dp_name),dp_phone(c_dp_phone),dp_info(c_dp_info){}
    QString dp_id;
    QString dp_name;
    QString dp_phone;
    QString dp_info;
};

struct admin:public info
{
    admin(QString c_ad_id="",QString c_ad_name="",QString c_ad_passwd="",QString c_ad_type="")
        :ad_id(c_ad_id),ad_name(c_ad_name),ad_passwd(c_ad_passwd),ad_type(c_ad_type){}
    QString ad_id;
    QString ad_name;
    QString ad_passwd;
    QString ad_type;
};

struct project:public info
{
    project(QString c_pr_id="",QString c_pr_name="",QString c_dp_id="",QString c_dp_address="",QString c_pr_time="",QString c_pr_trainaddress="",
             QString c_pr_start="",QString c_pr_end="",QString c_pr_days="",QString c_pr_info="",QString c_pr_active="")
        :pr_id(c_pr_id),pr_name(c_pr_name),dp_id(c_dp_id),dp_address(c_dp_address),pr_time(c_pr_time),pr_trainaddress(c_pr_trainaddress),
    pr_start(c_pr_start),pr_end(c_pr_end),pr_days(c_pr_days),pr_info(c_pr_info),pr_active(c_pr_active){}
    QString pr_id;
    QString pr_name;
    QString dp_id;
    QString dp_address;
    QString pr_time;
    QString pr_trainaddress;
    QString pr_start;
    QString pr_end;
    QString pr_days;
    QString pr_info;
    QString pr_active;
};

struct st_project:public info
{
    st_project(QString c_st_pid="",QString c_st_id="",QString c_pr_id="",QString c_tc_id="")
        :st_pid(c_st_pid),st_id(c_st_id),pr_id(c_pr_id),tc_id(c_tc_id){}
    QString st_pid;
    QString st_id;
    QString pr_id;
    QString tc_id;
};

struct tc_project:public info
{
    tc_project(QString c_tc_pid="",QString c_tc_id="",QString c_pr_id="")
        :tc_pid(c_tc_pid),tc_id(c_tc_id),pr_id(c_pr_id){}
    QString tc_pid;
    QString tc_id;
    QString pr_id;
};

#endif // INFO_H
