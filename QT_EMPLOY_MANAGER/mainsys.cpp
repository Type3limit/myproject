#include "mainsys.h"
#include "ui_mainsys.h"

Employ::Employ(QString c_sex,QString c_num,QString c_name,QString c_contact
        ,QString c_address,QString c_department,QString c_job,QString c_password,QString c_status)
    :sex(c_sex),number(c_num),name(c_name),contact(c_contact),address(c_address),department(c_department),job(c_job),password(c_password),status(c_status)
{
}

info::info(CMD c_command,bool cur_sucess,QString c_sex,QString c_num,QString c_name,QString c_contact
        ,QString c_address,QString c_department,QString c_job,QString c_password,QString c_status)
    :cmd_success(cur_sucess),command(c_command)
{
    data=new Employ(c_sex,c_num,c_name,c_contact,c_address,c_department,c_job,c_password,c_status);
}


MainSys::MainSys(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainSys)
{

    //通用数据初始化
    server=new QTcpSocket(this);
    server->connectToHost(QHostAddress(host_address),port);
    connect(server,&QTcpSocket::readyRead,this,&MainSys::recv);

    connect(this,&MainSys::root_loggin,this,&MainSys::on_root_loggin);

    IS_LOGGIN=FALSE ;
    IS_ROOT=FALSE;

    server_info=new info(CMD_LOGIN);

    //登录界面初始化
    QPushButton *loggin_style =new QPushButton;
    loggin_style->setText("更换界面皮肤");
    connect(loggin_style,&QAbstractButton::clicked,this,&MainSys::on_pushButton_clicked);
    loggin_window=new QMainWindow;
    loggin_window->setWindowTitle("登录");
    QWidget* loggin_widget=new QWidget;
    loggin_window->setCentralWidget(loggin_widget);
    loggin_window->resize(640,480);
    QHBoxLayout* loggin_layout =new QHBoxLayout;
    loggin_widget->setLayout(loggin_layout);
    QPushButton* comfirm_loggin=new QPushButton;
    connect(comfirm_loggin,&QAbstractButton::clicked,this,&MainSys::on_loggin_button_clicked);
    name_edit= new QLineEdit;
    passwd_edit= new QLineEdit;
    passwd_edit->setEchoMode(QLineEdit::Password);
    QLabel * name_Label = new QLabel;
    QLabel * passwd_Label= new QLabel;
    QLabel * loggin_Label =new QLabel;
    loggin_layout->addWidget(loggin_style);
    loggin_layout->addWidget(loggin_Label);
    loggin_layout->addWidget(name_Label);
    loggin_layout->addWidget(name_edit);
    loggin_layout->addWidget(passwd_Label);
    loggin_layout->addWidget(passwd_edit);
    loggin_layout->addWidget(comfirm_loggin);
    name_Label->setText("用户名");
    passwd_Label->setText("密码");
    comfirm_loggin->setText("登录");
    loggin_Label->setText("欢迎使用员工管理系统，请登录：");

    //考勤界面初始化
    QPushButton * att_style =new QPushButton;
    att_style->setText("更改界面皮肤");
    connect(att_style,&QAbstractButton::clicked,this,&MainSys::on_pushButton_clicked);
    attendance_window=new QMainWindow;
    attendance_window->resize(640,480);
    attendance_window->setWindowTitle("考勤");
    QWidget* attendance_widget=new QWidget;
    attendance_window->setCentralWidget(attendance_widget);
    QGridLayout* attendance_layout =new QGridLayout;
    welcom_label =new QLabel;
    welcom_label->resize(100,100);
    passwd_change=new QPushButton("更改当前账户密码");
    QPushButton * makesure_button=new QPushButton;
    connect(passwd_change,&QPushButton::clicked,this,&MainSys::change_passwd);
    connect(makesure_button,&QAbstractButton::clicked,this,&MainSys::on_makesure_button_clicked);
    box=new QComboBox;
    box->insertItem(0,"考勤");
    box->insertItem(1,"缺勤");
    box->insertItem(2,"请假");
    box->insertItem(3,"加班");
    box->insertItem(4,"补休");
    box->insertItem(5,"调班");
    box->insertItem(6,"出差");
    box->insertItem(7,"调工");
    welcom_label->setText("");
    makesure_button->setText("提交");
    attendance_layout->setSpacing(10);
    attendance_layout->addWidget(att_style,1,1);
    attendance_layout->addWidget(welcom_label,2,2);
    attendance_layout->addWidget(box,2,1);
    attendance_layout->addWidget(makesure_button,3,1);
    attendance_layout->addWidget(passwd_change,3,2);
    attendance_widget->setLayout(attendance_layout);

    //root用户界面初始化，直接使用界面文件,部分部件在槽函数中完成
    ui->setupUi(this);
    QAction* add_employ=new QAction(QIcon(":/source/ADD.png"),"添加新员工(&a)");
    QAction* del_employ=new QAction(QIcon(":/source/DELETE.png"),"删除员工(&d)");
    QAction* update_employ=new QAction(QIcon(":/source/UPDATE.png"),"修改员工信息(&u)");
    QAction* select_employ=new QAction(QIcon(":/source/SELECT.png"),"查询员工信息(&s)");
    ui->menu->addAction(add_employ);
    ui->menu_2->addAction(del_employ);
    ui->menu_3->addAction(update_employ);
    ui->menu_4->addAction(select_employ);
    ui->mainToolBar->addAction(add_employ);
    ui->mainToolBar->addAction(del_employ);
    ui->mainToolBar->addAction(update_employ);
    ui->mainToolBar->addAction(select_employ);
    connect(add_employ,&QAction::triggered,this,&MainSys::add_employ);
    connect(del_employ,&QAction::triggered,this,&MainSys::del_employ);
    connect(update_employ,&QAction::triggered,this,&MainSys::update_employ);
    connect(select_employ,&QAction::triggered,this,&MainSys::select_employ);
    table=new QTableWidget(1,8);
    ui->centralWidget->layout()->addWidget(table);
    QTableWidgetItem* numbers=new QTableWidgetItem("编号");
    QTableWidgetItem* names=new QTableWidgetItem("姓名");
    QTableWidgetItem* sexs=new QTableWidgetItem("性别");
    QTableWidgetItem* contancts=new QTableWidgetItem("联系方式");
    QTableWidgetItem* address=new QTableWidgetItem("地址");
    QTableWidgetItem* departments=new QTableWidgetItem("部门");
    QTableWidgetItem* jobs=new QTableWidgetItem("职称");
    QTableWidgetItem* attendances=new QTableWidgetItem("考勤状态");
    table->setHorizontalHeaderItem(0,numbers);
    table->setHorizontalHeaderItem(1,names);
    table->setHorizontalHeaderItem(2,sexs);
    table->setHorizontalHeaderItem(3,contancts);
    table->setHorizontalHeaderItem(4,address);
    table->setHorizontalHeaderItem(5,departments);
    table->setHorizontalHeaderItem(6,jobs);
    table->setHorizontalHeaderItem(7,attendances);

    //密码更改界面
    changed_able=FALSE;
    passwd_dialog=new QDialog;
    passwd_dialog->resize(400,400);
    QGridLayout *passwd_layout=new QGridLayout;
    QLabel * passwd_input_tip=new QLabel("您原本的密码：");
    passwd_c_tip=new QLabel;
    passwd_new_tip=new QLabel;
    passwd_re_tip=new QLabel;
    input_passwd=new QLineEdit;
    input_passwd->setEchoMode(QLineEdit::Password);
    QLabel* passwd_new_tips=new QLabel("输入您的新密码：");
    new_passwd=new QLineEdit;
    new_passwd->setEchoMode(QLineEdit::Password);
    QLabel* passwd_re_tips=new QLabel("重复您的新密码：");
    re_passwd=new QLineEdit;
    re_passwd->setEchoMode(QLineEdit::Password);
    passwd_sure=new QPushButton("确认");
    passwd_layout->addWidget(passwd_input_tip,1,1);
    passwd_layout->addWidget(input_passwd,1,2);
    passwd_layout->addWidget(passwd_c_tip,1,3);
    passwd_layout->addWidget(passwd_new_tips,2,1);
    passwd_layout->addWidget(new_passwd,2,2);
    passwd_layout->addWidget(passwd_new_tip,2,3);
    passwd_layout->addWidget(passwd_re_tips,3,1);
    passwd_layout->addWidget(re_passwd,3,2);
    passwd_layout->addWidget(passwd_re_tip,3,3);
    passwd_layout->addWidget(passwd_sure,4,3);
    passwd_dialog->setLayout(passwd_layout);
    connect(passwd_sure,&QPushButton::clicked,this,&MainSys::on_passwd_change_clicked);

    //增加员工界面
    regis_able=FALSE;
    add_dialog=new QDialog;
    add_dialog->resize(1000,600);
    add_dialog->setWindowTitle("增加员工中");
    QGridLayout * add_layout =new QGridLayout;
    add_dialog->setLayout(add_layout);
    add_layout->setMargin(10);

    QLabel* add_tips=new QLabel("在此页填入员工基本信息");
    add_layout->addWidget(add_tips,1,2);
    QLabel *add_tip_sex=new QLabel("性别:");
    add_layout->addWidget(add_tip_sex,2,1);
    add_sex=new QLineEdit("在此输入员工性别");
    connect(add_sex,&QLineEdit::cursorPositionChanged,this,&MainSys::on_addLineEdit_cursorPositionChanged);
    add_layout->addWidget(add_sex,2,2);
    tip_sex_right=new QLabel("");
    add_layout->addWidget(tip_sex_right,2,3);

    QLabel *add_tip_number=new QLabel("员工编号:");
    add_layout->addWidget(add_tip_number,3,1);
    add_number=new QLineEdit("在此输入员工编号");
    connect(add_number,&QLineEdit::cursorPositionChanged,this,&MainSys::on_addLineEdit_cursorPositionChanged);
    add_layout->addWidget(add_number,3,2);
    tip_number_right=new QLabel("");
    add_layout->addWidget(tip_number_right,3,3);

    QLabel* add_tip_name=new QLabel("姓名：");
    add_layout->addWidget(add_tip_name,4,1);
    add_name=new QLineEdit("在此输入员工姓名");
    connect(add_number,&QLineEdit::cursorPositionChanged,this,&MainSys::on_addLineEdit_cursorPositionChanged);
    add_layout->addWidget(add_name,4,2);
    tip_name_right=new QLabel("");
    add_layout->addWidget(tip_name_right,4,3);

    QLabel* add_tip_con=new QLabel("联系方式：");
    add_layout->addWidget(add_tip_con,5,1);
    add_con=new QLineEdit("在此输入员工联系方式");
    connect(add_con,&QLineEdit::cursorPositionChanged,this,&MainSys::on_addLineEdit_cursorPositionChanged);
    add_layout->addWidget(add_con,5,2);
    tip_con_right=new QLabel("");
    add_layout->addWidget(tip_con_right,5,3);

    QLabel *add_tip_address=new QLabel("地址：");
    add_layout->addWidget(add_tip_address,6,1);
    add_address=new QLineEdit("在此输入员工居住地址");
    connect(add_address,&QLineEdit::cursorPositionChanged,this,&MainSys::on_addLineEdit_cursorPositionChanged);
    add_layout->addWidget(add_address,6,2);
    tip_add_right=new QLabel("");
    add_layout->addWidget(tip_add_right,2,3);

    QLabel *add_tip_depart=new QLabel("所属部门:");
    add_layout->addWidget(add_tip_depart,2,4);
    add_depart=new QLineEdit("在此输入员工部门");
    connect(add_depart,&QLineEdit::cursorPositionChanged,this,&MainSys::on_addLineEdit_cursorPositionChanged);
    add_layout->addWidget(add_depart,2,5);
    tip_dep_right=new QLabel("");
    add_layout->addWidget(tip_dep_right,2,6);

    QLabel *add_tip_job=new QLabel("相应职位：");
    add_layout->addWidget(add_tip_job,3,4);
    add_job=new QLineEdit("在此输入员工职位");
    connect(add_job,&QLineEdit::cursorPositionChanged,this,&MainSys::on_addLineEdit_cursorPositionChanged);
    add_layout->addWidget(add_job,3,5);
    tip_job_right=new QLabel("");
    add_layout->addWidget(tip_job_right,3,6);

    QLabel *add_tip_passwd=new QLabel("登录密码:");
    add_layout->addWidget(add_tip_passwd,4,4);
    add_passwd=new QLineEdit("");
    add_passwd->setEchoMode(QLineEdit::Password);
    connect(add_passwd,&QLineEdit::cursorPositionChanged,this,&MainSys::on_addLineEdit_cursorPositionChanged);
    add_layout->addWidget(add_passwd,4,5);
    tip_pass_right=new QLabel("");
    add_layout->addWidget(tip_pass_right,4,6);

    QLabel *add_tip_repeatpass=new QLabel("再次输入密码:");
    add_layout->addWidget(add_tip_repeatpass,5,4);
    repeat_pass=new QLineEdit("");
    repeat_pass->setEchoMode(QLineEdit::Password);
    connect(repeat_pass,&QLineEdit::cursorPositionChanged,this,&MainSys::on_addLineEdit_cursorPositionChanged);
    add_layout->addWidget(repeat_pass,5,5);
    tip_repass_right=new QLabel("");
    add_layout->addWidget(tip_repass_right,5,6);

    add_register=new QPushButton("确认提交");
    connect(add_register,&QAbstractButton::clicked,this,&MainSys::on_add_button_clicked);
    add_layout->addWidget(add_register,6,4);
    add_register->setEnabled(regis_able);

    //删除界面初始化
    del_dialog=new QDialog;
    del_dialog->resize(1000,600);
    del_dialog->setWindowTitle("删除员工中");
    QGridLayout* del_layout =new QGridLayout;
    del_dialog->setLayout(del_layout);

    del_tips=new QLabel("输入员工编号后，下方会有相应的信息显示，请确认无误后再作操作。");
    del_number=new QLineEdit("在此输入员工编号");
    del_information=new QTableWidget(1,8);
    QTableWidgetItem* del_numbers=new QTableWidgetItem("编号");
    QTableWidgetItem* del_names=new QTableWidgetItem("姓名");
    QTableWidgetItem* del_sexs=new QTableWidgetItem("性别");
    QTableWidgetItem* del_contancts=new QTableWidgetItem("联系方式");
    QTableWidgetItem* del_address=new QTableWidgetItem("地址");
    QTableWidgetItem* del_departments=new QTableWidgetItem("部门");
    QTableWidgetItem* del_jobs=new QTableWidgetItem("职称");
    QTableWidgetItem* del_attendances=new QTableWidgetItem("考勤状态");
    del_information->setHorizontalHeaderItem(0,del_numbers);
    del_information->setHorizontalHeaderItem(1,del_names);
    del_information->setHorizontalHeaderItem(2,del_sexs);
    del_information->setHorizontalHeaderItem(3,del_contancts);
    del_information->setHorizontalHeaderItem(4,del_address);
    del_information->setHorizontalHeaderItem(5,del_departments);
    del_information->setHorizontalHeaderItem(6,del_jobs);
    del_information->setHorizontalHeaderItem(7,del_attendances);
    del_button=new QPushButton("确认删除");

    del_layout->addWidget(del_tips,1,1);
    del_layout->addWidget(del_number,2,1);
    del_layout->addWidget(del_information,3,1);
    del_layout->addWidget(del_button,3,2);

    connect(del_button,&QPushButton::clicked,this,&MainSys::on_del_button_clicked);

    //修改员工信息界面
    edit_able=FALSE;
    update_dialog=new QDialog;
    update_dialog->resize(1000,600);
    update_dialog->setWindowTitle("修改员工信息中");
    QGridLayout * update_layout =new QGridLayout;
    update_dialog->setLayout(update_layout );
    update_layout ->setMargin(10);

    update_number=new QLineEdit("在此输入员工编号");
    update_layout ->addWidget(update_number,1,2);
    connect(update_number,&QLineEdit::editingFinished,this,&MainSys::on_update_LineEdit_EditingFinished);

    QLabel *update_Tip_sex=new QLabel("性别：");
    update_sex=new QLineEdit;
    update_layout ->addWidget(update_sex,2,2);
    update_layout ->addWidget(update_Tip_sex,2,1);

    QLabel* update_Tip_con=new QLabel("联系方式：");
    update_con=new QLineEdit;
    update_layout ->addWidget(update_con,3,2);
    update_layout ->addWidget(update_Tip_con,3,1);

    QLabel* update_Tip_job=new QLabel("职位：");
    update_job=new QLineEdit;
    update_layout ->addWidget(update_job,4,2);
    update_layout ->addWidget(update_Tip_job,4,1);

    QLabel*update_Tip_name=new QLabel("姓名：");
    update_name=new QLineEdit;
    update_layout ->addWidget(update_name,5,2);
    update_layout ->addWidget(update_Tip_name,5,1);

    QLabel* update_Tip_dep=new QLabel("部门:");
    update_depart=new QLineEdit;
    update_layout ->addWidget(update_depart,6,2);
    update_layout ->addWidget(update_Tip_dep,6,1);

    QLabel* update_Tip_address=new QLabel("地址:");
    update_address=new QLineEdit;
    update_layout ->addWidget(update_address,7,2);
    update_layout ->addWidget(update_Tip_address,7,1);

   update_button=new QPushButton("提交修改");
   connect(update_button,&QPushButton::clicked,this,&MainSys::on_update_button_clicked);
   update_layout ->addWidget(update_button,8,2);

   //查询界面初始化
   select_dialog=new QDialog;
   select_dialog->resize(1000,600);
   select_dialog->setWindowTitle("查询员工信息中");
   QGridLayout* select_layout=new QGridLayout;
   select_dialog->setLayout(select_layout);

   QLabel* select_tips=new QLabel("在此页查询员工信息");

   QLabel* select_tip_number=new QLabel("在此栏输入员工编号：");
   select_number=new QLineEdit;
   tip_snumber_right=new QLabel;
   select_layout->addWidget(select_tips,1,1);
   select_layout->addWidget(select_tip_number,2,1);
   select_layout->addWidget(select_number,2,2);
   select_layout->addWidget(tip_snumber_right,2,3);

   select_information=new QTableWidget(1,8);
   select_layout->addWidget( select_information,3,2);
   QTableWidgetItem* select_numbers=new QTableWidgetItem("编号");
   QTableWidgetItem* select_names=new QTableWidgetItem("姓名");
   QTableWidgetItem* select_sexs=new QTableWidgetItem("性别");
   QTableWidgetItem* select_contancts=new QTableWidgetItem("联系方式");
   QTableWidgetItem* select_address=new QTableWidgetItem("地址");
   QTableWidgetItem* select_departments=new QTableWidgetItem("部门");
   QTableWidgetItem* select_jobs=new QTableWidgetItem("职称");
   QTableWidgetItem* select_attendances=new QTableWidgetItem("考勤状态");
    select_information->setHorizontalHeaderItem(0,select_numbers);
    select_information->setHorizontalHeaderItem(1,select_names);
    select_information->setHorizontalHeaderItem(2,select_sexs);
    select_information->setHorizontalHeaderItem(3,select_contancts);
    select_information->setHorizontalHeaderItem(4,select_address);
    select_information->setHorizontalHeaderItem(5,select_departments);
    select_information->setHorizontalHeaderItem(6,select_jobs);
    select_information->setHorizontalHeaderItem(7,select_attendances);
    select_button=new QPushButton("确认查询");
   select_layout->addWidget(select_button,4,3);

   connect(select_button,&QPushButton::clicked,this,&MainSys::on_select_button_clicked);
}

MainSys::~MainSys()
{
    delete ui;
    delete loggin_window;
    delete attendance_window;
}

void MainSys::Loggin()
{
    loggin_window->show();
}

void MainSys::Show()
{
    if(!IS_ROOT)
        {
          attendance_window->show();
          welcom_label->setText(QString("欢迎!"+server_info->data->name+"!"));
        }
    if(IS_ROOT)
    {
        this->show();
    }
}

void MainSys::on_root_loggin()
{
    if(disconnect(server,&QTcpSocket::readyRead,this,&MainSys::recv))
    {
        qDebug()<<"disconnected to recv slot,connected to recvall slot";
        connect(server,&QTcpSocket::readyRead,this,&MainSys::recvall);
        server_info->command=CMD_SELECT_ALL;
        send();
        if(server->waitForReadyRead(1000))
        {
            if(disconnect(server,&QTcpSocket::readyRead,this,&MainSys::recvall))
           {
            qDebug()<<"disconnected to recvall slot,connected to recv slot";
            connect(server,&QTcpSocket::readyRead,this,&MainSys::recv);
            }
        }
    }
}

void MainSys::send()
{
    QString buf;
    buf=buf.number(server_info->command);
    qDebug()<<"in send command"<<server_info->command;
    buf+=':';
    buf+=server_info->data->number;
    qDebug()<<"in send number"<<server_info->data->number;
    buf+=':';
    buf+=server_info->data->password;
   qDebug()<<"in send passwd"<<server_info->data->password;
    buf+=':';
    buf+=server_info->data->sex;
    qDebug()<<"in send sex"<<server_info->data->sex;
    buf+=':';
    buf+=server_info->data->name;
    qDebug()<<"in send name"<<server_info->data->name;
    buf+=':';
    buf+=server_info->data->job;
    qDebug()<<"in send job"<<server_info->data->job;
    buf+=':';
    buf+=server_info->data->address;
    qDebug()<<"in send address"<<server_info->data->address;
    buf+=':';
    buf+=server_info->data->contact;
    qDebug()<<"in send contact"<<server_info->data->contact;
    buf+=':';
    buf+=server_info->data->department;
    qDebug()<<"in send depart"<<server_info->data->department;
    buf+=':';
    buf+=server_info->data->status;
    qDebug()<<"in send status"<<server_info->data->status;
    buf+="\0";
    qDebug()<<"in send:  "<<buf;

    server->write(buf.toUtf8());
}

void MainSys::recv()
{
    QByteArray *data_buf = new QByteArray(server->readAll());
    QString buf =QString::fromUtf8(*data_buf);
    QString::iterator cur_data=buf.begin();
    qDebug()<<"in recv have data:"<<buf;

    QString cmd_buf;

    while(*cur_data!=':')
        cmd_buf+=(*cur_data++);
    cur_data++;

    qDebug()<<"in recv cmd_buf :"<<cmd_buf;

    if(cmd_buf=="1")
        server_info->cmd_success=TRUE;
    else
        server_info->cmd_success=FALSE;

    QString number_buf;
    while(*cur_data!=':')
        number_buf+= (*cur_data++);
    cur_data++;
    qDebug()<<"in recv number"<<number_buf;
    server_info->data->number=number_buf;

    QString passwd_buf;
    while(*cur_data!=':')
        passwd_buf+=(*cur_data++);
    cur_data++;
    qDebug()<<"in recv passwd"<<passwd_buf;
    server_info->data->password=passwd_buf;

    QString sex_buf;
    while(*cur_data!=':')
        sex_buf+=(*cur_data++);
    cur_data++;
    server_info->data->sex=sex_buf;

    QString name_buf;
    while(*cur_data!=':')
        name_buf+=(*cur_data++);
    cur_data++;
    server_info->data->name=name_buf;

    QString job_buf;
    while(*cur_data!=':')
        job_buf+=(*cur_data++);
    cur_data++;
    server_info->data->job=job_buf;

    QString address_buf;
    while(*cur_data!=':')
        address_buf+=(*cur_data++);
    cur_data++;
    server_info->data->address=address_buf;

    QString contact_buf;
    while(*cur_data!=':')
        contact_buf+=(*cur_data++);
    cur_data++;
    server_info->data->contact=contact_buf;

    QString depart_buf;
    while(*cur_data!=':')
        depart_buf+=(*cur_data++);
    cur_data++;
    server_info->data->department=depart_buf;

    QString status_buf;
    while(*cur_data!='\0')
        status_buf+=(*cur_data++);
    server_info->data->status=status_buf;

}

void MainSys::recvall()
{
     qDebug()<<"enter the recvall";

     QByteArray data_buf=server->readAll();
     QString buf = QString :: fromUtf8(data_buf);
     qDebug()<<"in recvall have data:"<<buf;

     QString::iterator cur_buf=buf.begin();

     QString count_buf;
     while(*cur_buf!=':')
     {
         count_buf+=(*cur_buf++);
     }
     cur_buf++;
     int size=count_buf.toInt();
     qDebug()<<"in recvall size is: "<<size;

     table->setRowCount(size);

     Employ* all=new Employ[size];
     for(int j=0;j<size&&cur_buf!=buf.end();j++)
     {
            if(*(cur_buf)==';')
            {
                cur_buf++;
                continue;
            }

            QString number_buf;
            while(*cur_buf!=':')
             number_buf+= (*cur_buf++);
            qDebug()<<"in recv all number_buf is:"<<number_buf;
            all[j].number=number_buf;
            cur_buf++;

            QString passwd_buf;
            while(*cur_buf!=':')
             number_buf+=(*cur_buf++);
            all[j].password=passwd_buf;
            cur_buf++;

            QString sex_buf;
            while(*cur_buf!=':')
             sex_buf+=(*cur_buf++);
            all[j].sex=sex_buf;
            cur_buf++;

            QString name_buf;
            while(*cur_buf!=':')
             name_buf+=(*cur_buf++);
            all[j].name=name_buf;
            cur_buf++;

            QString job_buf;
            while(*cur_buf!=':')
             job_buf+=(*cur_buf++);
            all[j].job=job_buf;
            cur_buf++;

            QString address_buf;
            while(*cur_buf!=':')
             address_buf+=(*cur_buf++);
            all[j].address=address_buf;
            cur_buf++;

            QString contact_buf;
            while(*cur_buf!=':')
             contact_buf+=(*cur_buf++);
            all[j].contact=contact_buf;
            cur_buf++;

            QString depart_buf;
            while(*cur_buf!=':')
             depart_buf+=(*cur_buf++);
            all[j].department=depart_buf;
            cur_buf++;

            QString status_buf;
            while(*cur_buf!=';')
             status_buf+=(*cur_buf++);
            all[j].status=status_buf;
            cur_buf++;

     }

     for(int x=0;x<size;x++)
     {
           QTableWidgetItem* numberItem=new QTableWidgetItem(all[x].number);
           table->setItem(x,0,numberItem);
           QTableWidgetItem* nameItem=new QTableWidgetItem(all[x].name);
           table->setItem(x,1,nameItem);
           QTableWidgetItem* sexItem=new QTableWidgetItem(all[x].sex);
           table->setItem(x,2,sexItem);
           QTableWidgetItem* contactItem=new QTableWidgetItem(all[x].contact);
           table->setItem(x,3,contactItem);
           QTableWidgetItem* addressItem=new QTableWidgetItem(all[x].address);
           table->setItem(x,4,addressItem);
           QTableWidgetItem* departItem=new QTableWidgetItem(all[x].department);
           table->setItem(x,5,departItem);
           QTableWidgetItem* jobItem=new QTableWidgetItem(all[x].job);
           table->setItem(x,6,jobItem);
           QTableWidgetItem* statusItem=new QTableWidgetItem(all[x].status);
           table->setItem(x,7,statusItem);
     }
     table->show();

}

void MainSys::on_loggin_button_clicked()
{
    server_info->command=CMD_LOGIN;
    server_info->data->number=name_edit->text();
    server_info->data->password=passwd_edit->text();
    qDebug()<<server_info->data->number<<":"<<server_info->data->password;
    send();
    if(server->waitForReadyRead(1000))
    {
        qDebug()<<"in loggin button cmd_success:"<<server_info->cmd_success;
        if(server_info->cmd_success)
        {
           IS_LOGGIN=TRUE;
           if(server_info->data->number=="root")
               IS_ROOT=TRUE;
        }
        else
        {
           QMessageBox::information(loggin_window,"","登录失败,请验证您的账号和密码");
        }
        qDebug()<<"in loggin button IS_LOGGIN:"<<IS_LOGGIN;
        qDebug()<<"in loggin button IS_ROOT:"<<IS_ROOT;

    }
    else
    {
        QMessageBox::warning(loggin_window,"","登录失败，请检查网络链接");
    }
    if(IS_LOGGIN)
    {
        Show();
        if(IS_ROOT)
                    emit root_loggin();
        loggin_window->close();
    }
}

void MainSys::on_makesure_button_clicked()
{

    server_info->command=CMD_ATTENDANCE;
    QString str=box->currentText();
    qDebug()<<"in attendance button:"<<str;
    server_info->data->status=str;
    send();
    if(server->waitForReadyRead(1000)){
    if(server_info->cmd_success)
       QMessageBox::information(attendance_window,"完成","已提交至服务器！");
    else
        QMessageBox::information(attendance_window,"失败","未知原因！");
    }
    else
    {
       QMessageBox::information(attendance_window,"失败","请检查网络链接！");
    }
}

void MainSys::on_pushButton_clicked()
{
    static int num=1;
    num=num%8;
    switch(num)
    {
    case 0:{
        QFile file(":/source/style_1.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = tr(file.readAll());
        qApp->setStyleSheet(styleSheet);
        break;}
    case 1:{
        QFile file(":/source/style_2.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet1 = tr(file.readAll());
        qApp->setStyleSheet(styleSheet1);
        break;}
    case 2:{
        QFile file(":/source/style_3.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet2 = tr(file.readAll());
        qApp->setStyleSheet(styleSheet2);
        break;}
    case 3:{
        QFile file(":/source/style_4.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet2 = tr(file.readAll());
        qApp->setStyleSheet(styleSheet2);
        break;}
    case 4:{
        QFile file(":/source/style_5.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet2 = tr(file.readAll());
        qApp->setStyleSheet(styleSheet2);
        break;}
    case 5:{
        QFile file(":/source/style_6.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet2 = tr(file.readAll());
        qApp->setStyleSheet(styleSheet2);
        break;}
    case 6:{
        QFile file(":/source/style_7.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet2 = tr(file.readAll());
        qApp->setStyleSheet(styleSheet2);
        break;}
    case 7:{
        QFile file(":/source/style_8.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet2 = tr(file.readAll());
        qApp->setStyleSheet(styleSheet2);
        break;}
    default:
        qApp->setStyle(QStyleFactory::create("fusion"));
    }
    num++;
}

void MainSys::change_passwd()
{
    passwd_dialog->show();
}

void MainSys::on_passwd_change_clicked()
{
    changed_able=TRUE;
    QString _old_passwd=input_passwd->text();
    QString _new_passwd=new_passwd->text();
    QString _re_passwd=re_passwd->text();
    QPixmap* correct=new QPixmap(":/source/correct.ico");
    QPixmap* error=new QPixmap(":/source/error.png");

    if(_old_passwd!=server_info->data->password)
    {
        qDebug()<<" _old_passwd is "<<_old_passwd;
        qDebug()<<"now paswd is "<<server_info->data->password;
        passwd_c_tip->setPixmap(*error);
        changed_able=FALSE;
    }
    else {
        passwd_c_tip->setPixmap(*correct);
    }
    if((_new_passwd!=_re_passwd)||(_new_passwd.isEmpty()))
    {
        passwd_new_tip->setPixmap(*error);
        passwd_re_tip->setPixmap(*error);
        changed_able=FALSE;
    }
    else
    {
        passwd_new_tip->setPixmap(*correct);
        passwd_re_tip->setPixmap(*correct);
    }

    if(changed_able)
    {
        server_info->data->password=_new_passwd;
        server_info->command=CMD_UPDATE;
        send();
        if(server->waitForReadyRead(1000))
        {
            if(server_info->cmd_success)
            QMessageBox::information(passwd_dialog,"成功","已修改您的密码");
            else {
                QMessageBox::warning(passwd_dialog,"失败","未知原因");
            }
        }
        else
            QMessageBox::warning(passwd_dialog,"失败","网络链接错误");
        passwd_dialog->close();
    }

}

void MainSys::on_addLineEdit_cursorPositionChanged()
{
    QPixmap * error=new QPixmap(":/source/error.png");
    QPixmap * correct=new QPixmap(":source/correct.ico");
    if(((add_sex->text()!="男"))&&((add_sex->text()!="女")))
    {
         tip_sex_right->setPixmap(*error);
         regis_able=FALSE;
    }
    else if((add_passwd->text()!=repeat_pass->text())||(add_passwd->text().isEmpty()))
    {
        tip_pass_right->setPixmap(*error);
        tip_repass_right->setPixmap(*error);
        regis_able=FALSE;
    }
    else
    {
        tip_add_right->setPixmap(*correct);
        tip_job_right->setPixmap(*correct);
        tip_number_right->setPixmap(*correct);
        tip_dep_right->setPixmap(*correct);
        tip_name_right->setPixmap(*correct);
        tip_con_right->setPixmap(*correct);
        tip_sex_right->setPixmap(*correct);
        tip_pass_right->setPixmap(*correct);
        tip_repass_right->setPixmap(*correct);
        regis_able=TRUE;
    }
    add_register->setEnabled(regis_able);
}

void MainSys::add_employ()
{
    add_dialog->show();
}

void MainSys::on_add_button_clicked()
{
    server_info->command=CMD_ADD;
    server_info->data->sex=add_sex->text();
    server_info->data->name=add_name->text();
    server_info->data->number=add_number->text();
    server_info->data->password=add_passwd->text();
    server_info->data->job=add_job->text();
    server_info->data->address=add_address->text();
    server_info->data->contact=add_con->text();
    server_info->data->department=add_depart->text();
    server_info->data->status="缺勤";
    send();
    if(server->waitForReadyRead(1000))
    {
        if(server_info->cmd_success)
            QMessageBox::information(add_dialog,"成功","已增加新的员工信息");
        else
            QMessageBox::warning(add_dialog,"失败","请检查员工编号是否无误");
    }
    else
        QMessageBox::information(add_dialog,"失败","请检查网络连接");
    add_dialog->close();
}

void MainSys::del_employ()
{
    del_dialog->show();
}

void MainSys::on_del_button_clicked()
{
    bool del_able=false;
    server_info->command=CMD_SELECT;
    server_info->data->number=del_number->text();
    send();
    if(server->waitForReadyRead(1000))
    {
       if(server_info->cmd_success)
       {
         QTableWidgetItem* numberItem=new QTableWidgetItem(server_info->data->number);
         del_information->setItem(0,0,numberItem);
         QTableWidgetItem* nameItem=new QTableWidgetItem(server_info->data->name);
         del_information->setItem(0,1,nameItem);
         QTableWidgetItem* sexItem=new QTableWidgetItem(server_info->data->sex);
         del_information->setItem(0,2,sexItem);
         QTableWidgetItem* contactItem=new QTableWidgetItem(server_info->data->contact);
         del_information->setItem(0,3,contactItem);
         QTableWidgetItem* addressItem=new QTableWidgetItem(server_info->data->address);
         del_information->setItem(0,4,addressItem);
         QTableWidgetItem* departItem=new QTableWidgetItem(server_info->data->department);
         del_information->setItem(0,5,departItem);
         QTableWidgetItem* jobItem=new QTableWidgetItem(server_info->data->job);
         del_information->setItem(0,6,jobItem);
         QTableWidgetItem* statusItem=new QTableWidgetItem(server_info->data->status);
         del_information->setItem(0,7,statusItem);
         del_information->show();
        QMessageBox::StandardButton choice=QMessageBox::question(del_dialog,"","确认信息无误？");
        (choice==QMessageBox::Yes)?del_able=true:del_able=false;
       }
       else
       {
           QMessageBox::information(del_dialog,"失败","没有对应编号的职员");
           del_able=false;
       }
       if(del_able)
       {
           server_info->command=CMD_DEL;
           send();
          if(server->waitForReadyRead(1000))
         {
              if(server_info->cmd_success)
              {
                  QMessageBox::information(del_dialog,"成功","成功删除该职员");
                  del_dialog->close();
              }
          }
             else
             {
             QMessageBox::warning(del_dialog,"失败","网络链接出错");
             del_able=false;
             }
      }
    }
    else
        QMessageBox::warning(del_dialog,"失败","请检查网络链接");
    del_button->setEnabled(del_able);
}

void MainSys::update_employ()
{
   update_dialog->show();
}

void MainSys::on_update_LineEdit_EditingFinished()
{
    server_info->command=CMD_SELECT;
    server_info->data->number=update_number->text();
    send();
    if(server->waitForReadyRead(1000))
    {
       if(server_info->cmd_success)
       {
        QMessageBox::information(update_dialog,"成功","查找成功，相应内容已显示");
         edit_able=TRUE;
       }
       else {
        QMessageBox::information(update_dialog,"失败","查找失败，无此信息");
        edit_able=FALSE;
       }
    }
    else {
       QMessageBox::warning(update_dialog,"失败","请检查网络连接");
    }
    update_sex->setText(server_info->data->sex);
    update_con->setText(server_info->data->contact);
    update_job->setText(server_info->data->job);
    update_name->setText(server_info->data->name);
    update_depart->setText(server_info->data->department);
    update_address->setText(server_info->data->address);
    update_button->setEnabled(edit_able);
}

void MainSys::on_update_button_clicked()
{
    server_info->command=CMD_UPDATE;
    server_info->data->sex=update_sex->text();
    server_info->data->contact=update_con->text();
    server_info->data->job=update_job->text();
    server_info->data->name=update_name->text();
    server_info->data->department=update_depart->text();
    server_info->data->address=update_address->text();
    send();
    if(server->waitForReadyRead(1000))
    {
         if(server_info->cmd_success)
        {
        QMessageBox::information(update_dialog,"成功","已提交至服务器");
        update_dialog->close();
        }
       else {
        QMessageBox::information(update_dialog,"失败","请检查输入内容是否合法");
         }
    }
    else
        QMessageBox::warning(update_dialog,"失败","请检查网络连接");
}

void MainSys::select_employ()
{
    select_dialog->show();
}

void MainSys::on_select_button_clicked()
{
    server_info->command=CMD_SELECT;
    server_info->data->number=select_number->text();
    send();
    if(server->waitForReadyRead(1000))
    {
       if(server_info->cmd_success)
       {
        QTableWidgetItem* numberItem=new QTableWidgetItem(server_info->data->number);
         select_information->setItem(0,0,numberItem);
        QTableWidgetItem* nameItem=new QTableWidgetItem(server_info->data->name);
         select_information->setItem(0,1,nameItem);
        QTableWidgetItem* sexItem=new QTableWidgetItem(server_info->data->sex);
         select_information->setItem(0,2,sexItem);
        QTableWidgetItem* contactItem=new QTableWidgetItem(server_info->data->contact);
         select_information->setItem(0,3,contactItem);
        QTableWidgetItem* addressItem=new QTableWidgetItem(server_info->data->address);
        select_information->setItem(0,4,addressItem);
        QTableWidgetItem* departItem=new QTableWidgetItem(server_info->data->department);
         select_information->setItem(0,5,departItem);
        QTableWidgetItem* jobItem=new QTableWidgetItem(server_info->data->job);
         select_information->setItem(0,6,jobItem);
        QTableWidgetItem* statusItem=new QTableWidgetItem(server_info->data->status);
         select_information->setItem(0,7,statusItem);
         select_information->show();
       }
       else
       {
          QMessageBox::information(select_dialog,"失败","请检测编号正确性或网络是否链接");
           select_dialog->close();
        }
    }
    else
        QMessageBox::warning(select_dialog,"失败","请检查编号正确性或网络是否链接");
}

void MainSys::on_pushButton_2_clicked()
{
    server_info->command=CMD_SELECT_ALL;
    if(disconnect(server,&QTcpSocket::readyRead,this,&MainSys::recv))
    {
        qDebug()<<"disc with recv ,conn with recvall";
        connect(server,&QTcpSocket::readyRead,this,&MainSys::recvall);
        send();
    }

    if(server->waitForReadyRead())
    {
        if(server_info->cmd_success)
        {
              if(disconnect(server,&QTcpSocket::readyRead,this,&MainSys::recvall))
              {
                  connect(server,&QTcpSocket::readyRead,this,&MainSys::recv);
                  qDebug()<<"disc with recv ,conn with recvall";
              }
        }
        else
            QMessageBox::information(this,"失败","未知原因");
    }
    else
        QMessageBox::warning(this,"失败","网络链接中断");
}
