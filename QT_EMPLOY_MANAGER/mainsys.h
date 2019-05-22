#ifndef MAINSYS_H
#define MAINSYS_H

#include <iostream>
#include <QMainWindow>
#include <QString>
#include <QWidget>
#include <QtWidgets>
#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QComboBox>
#include <QStyle>
#include <QStyleFactory>
#include <QTcpSocket>
#include <QtNetwork>
#include <QTextCodec>
#include <QByteArray>

typedef unsigned int CMD;

const CMD CMD_LOGIN= 0;//登录
const CMD CMD_ADD =1;//添加
const CMD CMD_DEL =2;//删除
const CMD CMD_UPDATE =3;//更新
const CMD CMD_SELECT =4;//查询
const CMD CMD_ATTENDANCE= 5;//考勤
const CMD CMD_SELECT_ALL=6;//查询所有信息


const unsigned int TRANS_SIZE= 4096;//最大传输量
const QString host_address="192.168.43.157";
const unsigned int port=6666;

struct Employ
{
    Employ(QString c_sex="男",QString c_num="",QString c_name="",QString c_contact=""
            ,QString c_address="",QString c_department="",QString c_job="",QString c_password="",QString c_status="缺勤");
    QString sex;
    QString number;//员工编号
    QString name;//姓名
    QString contact;//联系方式
    QString address;//地址
    QString department;//部门
    QString job;//职位
    QString password;//密码
    QString status;//考勤状态
};

struct info
{
    info(CMD c_command,bool cur_success=FALSE,QString c_sex="男",QString c_num="",QString c_name="",QString c_contact=""
            ,QString c_address="",QString c_department="",QString c_job="",QString c_password="",QString c_status="缺勤");
    Employ* data;
    bool cmd_success;
    CMD command;
};



namespace Ui {
class MainSys;
}

class MainSys : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainSys(QWidget *parent = nullptr);
    ~MainSys();
    void Loggin();
    void Show();
    void send();
    QString print_info();
signals:
    void root_loggin();
private slots:
    void recv();
    void recvall();
    void on_root_loggin();
    void change_passwd();
    void on_passwd_change_clicked();
    void on_makesure_button_clicked();
    void on_pushButton_clicked();
    void on_loggin_button_clicked();
    void on_addLineEdit_cursorPositionChanged();
    void add_employ();
    void on_add_button_clicked();
    void del_employ();
    void on_del_button_clicked();
    void update_employ();
    void on_update_LineEdit_EditingFinished();
    void on_update_button_clicked();
    void select_employ();
    void on_select_button_clicked();
    void on_pushButton_2_clicked();

private:
    //通用数据
    info * server_info;
    QTcpSocket * server;

    //用户类型判断
    bool IS_ROOT ;
    bool IS_LOGGIN ;

    //窗口界面
    Ui::MainSys *ui;
    QMainWindow * loggin_window;
    QMainWindow * attendance_window;
    QDialog* passwd_dialog;
    QDialog* add_dialog;
    QDialog* del_dialog;
    QDialog* update_dialog;
    QDialog* select_dialog;
    QTableWidget* table;
    //考勤窗口部件
    QComboBox* box;
    QLabel * welcom_label;
    QLineEdit* name_edit;
    QLineEdit* passwd_edit;
    QPushButton* passwd_change;
    //密码更改部件
    QLineEdit* input_passwd;
    QLabel * passwd_c_tip;
    QLineEdit* new_passwd;
    QLabel * passwd_new_tip;
    QLineEdit* re_passwd;
    QLabel * passwd_re_tip;
    QPushButton* passwd_sure;
    bool changed_able;
    //增加员工窗口部件
    QLineEdit * add_sex;
    QLabel *tip_sex_right;
    QLineEdit * add_number;
    QLabel *tip_number_right;
    QLineEdit * add_name;
    QLabel *tip_name_right;
    QLineEdit * add_con;
    QLabel* tip_con_right;
    QLineEdit * add_address;
    QLabel *tip_add_right;
    QLineEdit * add_depart;
    QLabel *tip_dep_right;
    QLineEdit * add_job;
    QLabel *tip_job_right;
    QLineEdit * add_passwd;
    QLabel *tip_pass_right;
    QLineEdit * repeat_pass;
    QLabel *tip_repass_right;
    QPushButton* add_register;
    bool regis_able;
    //删除员工窗口部件
    QLineEdit * del_number;
    QLabel * del_tips;
    QPushButton* del_button;
    QTableWidget * del_information;
    //修改员工信息窗口部件
    QLineEdit * update_number;
    QLineEdit * update_sex;
    QLabel *tipup_sex_right;
    QLineEdit * update_name;
    QLabel *tipup_name_right;
    QLineEdit * update_con;
    QLabel* tipup_con_right;
    QLineEdit * update_address;
    QLabel *tipup_add_right;
    QLineEdit * update_depart;
    QLabel *tipup_dep_right;
    QLineEdit * update_job;
    QLabel *tipup_job_right;
    QPushButton* update_button;
    bool edit_able;
    //查询员工窗口部件
    QLineEdit * select_number;
    QLabel* tip_snumber_right;
    QPushButton* select_button;
    QTableWidget* select_information;
};




#endif // MAINSYS_H
