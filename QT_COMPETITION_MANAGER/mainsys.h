#ifndef MAINSYS_H
#define MAINSYS_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QTableView>
#include <QGridLayout>
#include <QMessageBox>
#include <QtWidgets>
#include <QDebug>
#include <QSqlQuery>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>
#include "info.h"

const qint16 port(6666);
const qint64 maxsize(1024 * 4);


namespace Ui {
class MainSys;
}

class MainSys : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainSys(QWidget *parent = nullptr);
    ~MainSys();
    void connect_to_SQL();//链接数据库
    void connect_to_TCP();//链接TCP
    void readInfor();//读入主要信息
    void mousePressEvent(QMouseEvent* event);//鼠标事件重写
    void mouseMoveEvent(QMouseEvent* event);
    void exec_command();//解析指令，交由具体函数执行
    void send();//发送消息

    void exec_student_regist();//学生注册
    void exec_loggin();//登录
    void exec_select_score();//查询分数
    void exec_join_pr();//参加竞赛
    void exec_take_score();//打分
    void exec_attempt_pr();//发布竞赛
    void exec_select_all();//查询所有学生信息
    void exec_del_student();//删除学生信息
    void exec_update_student();//修改学生信息
    void exec_select_project();//查询竞赛项目

private slots:
    void new_connection();
    void recv_command();
    void on_pushButton_clicked();
    void MSGError(QAbstractSocket::SocketError);

private:
    Ui::MainSys *ui;
    //主要信息
    bool student_loggin;
    bool teacher_loggin;
    bool admin_loggin;
    contain_info data;
    //tcp通信
    QTcpServer* server;
    QTcpSocket* socket;
    //数据库操作
    QSqlDatabase database;
    //窗口界面事件重写
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
    //日志使用
    QTime cur_time;
};

#endif // MAINSYS_H
