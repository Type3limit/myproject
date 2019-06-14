#include "mainsys.h"
#include "ui_mainsys.h"
#include "info.h"

MainSys::MainSys(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainSys)
{
    ui->setupUi(this);
    connect_to_SQL();
    connect_to_TCP();
    readInfor();
    ui->textEdit->append("等待客户端接入……");
}

MainSys::~MainSys()
{
    socket->close();
    server->close();
    delete ui;
}

void MainSys::mousePressEvent(QMouseEvent *event)
{
    windowPos=this->pos();
    mousePos=event->globalPos();
    dPos=mousePos-windowPos;
}

void MainSys::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()-dPos);
}

void MainSys::connect_to_TCP()
{
    cur_time=QTime::currentTime();
    ui->textEdit->append(cur_time.toString()+"\n创建侦听套接字……");
    server=new QTcpServer(this);
    if(!server->listen(QHostAddress::Any,port))
    {
        QMessageBox::warning(this,"失败","套接字侦听异常");
        exit(-1);
    }
    ui->textEdit->append("已创建侦听套接字\n");
    connect(server,&QTcpServer::newConnection,this,&MainSys::new_connection);
}

void MainSys::connect_to_SQL()
{
    cur_time=QTime::currentTime();
    ui->textEdit->append(cur_time.toString()+"\n连接到数据库……");
    database=QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName("localhost");
    database.setPort(3306);
    database.setDatabaseName("competition");
    database.setUserName("root");
    database.setPassword("3939520");
    if(!database.open())
    {
        if(QMessageBox::Cancel==QMessageBox::critical(nullptr,"失败","数据库错误",QMessageBox::Cancel))
        {
            exit(-1);
        }
    }
    ui->textEdit->append("已连接到数据库\n");
}

void MainSys::readInfor()
{
    cur_time=QTime::currentTime();
    ui->textEdit->append(cur_time.toString()+"\n从数据库读入信息……");
    QSqlQuery query;
    if(query.exec("select COUNT(*) from student")){
        while(query.next())
        {
            QTableWidgetItem* st_number=new QTableWidgetItem(query.value(0).toString());
            ui->tableWidget->setItem(0,0,st_number);
        }
    }
    if(query.exec("select COUNT(*) from teacher")){
        while(query.next()){
            QTableWidgetItem* tc_number=new QTableWidgetItem(query.value(0).toString());
            ui->tableWidget->setItem(1,0,tc_number);}
    }
    if(query.exec("select COUNT(*) from project"))
    {
        while(query.next()){
            QTableWidgetItem* pr_number=new QTableWidgetItem(query.value(0).toString());
            ui->tableWidget->setItem(2,0,pr_number);}
    }
    
    if(query.exec("select pr_name,pr_trainaddress,pr_days from project"))
    {
        for(int i=3;query.next();i++)
            for(int j=0;j<3;j++)
            {
                QTableWidgetItem* pr_info=new QTableWidgetItem(query.value(j).toString());
                ui->tableWidget->setItem(i,j+1,pr_info);
            }
    }
    ui->tableWidget->horizontalHeader()->setResizeContentsPrecision(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setResizeContentsPrecision(QHeaderView::Stretch);
    ui->tableWidget->show();
    
    ui->textEdit->append("已从数据库读入主要信息\n");
}

void MainSys::on_pushButton_clicked()
{
    if(QMessageBox::Yes==QMessageBox::warning(this,"确认？","关闭服务将使所有的客户端丧失链接",QMessageBox::Yes,QMessageBox::Cancel))
    {
        QApplication* app;
        this->close();
        app->exit();
    }
}

void MainSys::new_connection()
{
    cur_time=QTime::currentTime();
    ui->textEdit->append(cur_time.toString()+"\n新的客户端接入……");
    
    socket=server->nextPendingConnection();
    connect(socket,&QTcpSocket::readyRead,this,&MainSys::recv_command);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(MSGError(QAbstractSocket::SocketError)));
    QString hostAddress=socket->QAbstractSocket::peerAddress().toString();
    ui->textEdit->append("客户端地址："+hostAddress);
}

void MainSys::MSGError(QAbstractSocket::SocketError)
{
    int error = socket->error();
    switch(error)
    {
    case QAbstractSocket::RemoteHostClosedError://客户端断开
    {
        cur_time=QTime::currentTime();
        ui->textEdit->append(cur_time.toString());
        QString hostAddress=socket->QAbstractSocket::peerAddress().toString();
        ui->textEdit->append(tr("客户端[%1]断开连接\r\n").arg(hostAddress));
        break;
    }
    default:
    {
        error = -1;
        QMessageBox::information(this, "show", socket->errorString());
        break;
    }
    }
}

void MainSys::recv_command()
{
    data.recvbuf.clear();
    data.recvbuf=socket->readAll();
    cur_time=QTime::currentTime();
    ui->textEdit->append(cur_time.toString()+"\n接受到数据流:"+data.recvbuf);
    QString::iterator cmd_itr=data.recvbuf.begin();
    QString cur_cmd_buf;
    while(*cmd_itr!=":")
    {
        cur_cmd_buf+=*(cmd_itr++);
    }
    CMD cur_cmd=(cur_cmd_buf.toInt());
    if(cur_cmd>=CMD_STUDENT_REGIST&&cur_cmd<=CMD_SELECT_ALL_PROJECT)
    {
        data.cmd.cmd_new_status(cur_cmd);
        exec_command();
    }
    else
    {
        data.sendbuf="0";
        send();
    }
    readInfor();
}

void MainSys::exec_command()
{
    ui->textEdit->append("解析指令……");
    switch(this->data.cmd.get_cur_status())
    {
    case (CMD_STUDENT_REGIST):
    {
        ui->textEdit->append("学生注册\n");
        exec_student_regist();
        break;
    }
    case(CMD_STUDENT_LOGIN):
    {
        ui->textEdit->append("学生登录\n");
        exec_loggin();
        break;
    }
    case(CMD_TEACHER_LOGIN):
    {
        ui->textEdit->append("教师登录\n");
        exec_loggin();
        break;
    }
    case(CMD_ADMIN_LOGIN):
    {
        ui->textEdit->append("管理员登录\n");
        exec_loggin();
        break;
    }
    case(CMD_SCORE_SELECT):
    {
        ui->textEdit->append("学生成绩查询\n");
        exec_select_score();
        break;
    }
    case(CMD_TAKE_MATCH):
    {
        ui->textEdit->append("学生参与竞赛\n");
        exec_join_pr();
        break;
    }
    case(CMD_GIVE_SCORE):
    {
        ui->textEdit->append("教师评分\n");
        exec_take_score();
        break;
    }
    case(CMD_ATTENPT_PROJECT):
    {
        ui->textEdit->append("发布竞赛信息\n");
        exec_attempt_pr();
        break;
    }
    case(CMD_SELECT_ALL_STUDENT):
    {
        ui->textEdit->append("查询所有学生信息\n");
        exec_select_all();
        break;
    }
    case(CMD_STUDENT_DEL):
    {
        ui->textEdit->append("学生删除\n");
        exec_del_student();
        break;
    }
    case(CMD_STUDENT_UPDATE):
    {
        ui->textEdit->append("学生信息修改\n");
        exec_update_student();
        break;
    }
    case(CMD_SELECT_ALL_PROJECT):
    {
        ui->textEdit->append("查询竞赛项目");
        exec_select_project();
        break;
    }
    default:
        ui->textEdit->append("未知指令，忽略\n");
    }
}

void MainSys::send()
{
    cur_time=QTime::currentTime();
    ui->textEdit->append(cur_time.toString()+"执行完成，向接入客户端反馈信息："+data.sendbuf);
    socket->write(data.sendbuf.toUtf8());
}

void MainSys::exec_student_regist()
{
    data.sendbuf.clear();
    student* cur_buf=new student();
    QString::iterator itr=(data.recvbuf.begin()+2);
    while(*itr!=':')
    {
        cur_buf->no+=*(itr++);
    }itr++;
    while(*itr!=':')
    {
        cur_buf->name+=*(itr++);
    }itr++;
    while(*itr!=':')
    {
        cur_buf->sex+=*(itr++);
    }itr++;
    while(*itr!=':')
    {
        cur_buf->class_id+=*(itr++);
    }itr++;
    while(*itr!=':')
    {
        cur_buf->passwd+=*(itr++);
    }itr++;
    while(*itr!=';'&&*itr!=":"&&*itr!='\0')
    {
        cur_buf->dp_id+=*(itr++);
    }
    
    QString exec_sentence=("insert into student(st_no,st_password,st_name,st_sex,class_id,dp_id) "
                           "values('"+cur_buf->no+"','"+cur_buf->passwd+"','"+cur_buf->name+"','"
                           +cur_buf->sex+"','"+cur_buf->class_id+"','"+cur_buf->dp_id+"');");
    QSqlQuery query;
    query.exec(exec_sentence)?data.sendbuf="1":data.sendbuf="0";
    send();
}

void MainSys::exec_loggin()
{
    QString no_buf;
    QString passwd_buf;
    QString cmd_buf;
    QString::iterator itr= (data.recvbuf.begin());
    while(*itr!=":")
    {
        cmd_buf+=(*itr++);
    }itr++;
    while(*itr!=":")
    {
        no_buf+=(*itr++);
    }itr++;
    while(*itr!=":"&&*itr!=";"&&*itr!="\0")
    {
        passwd_buf+=(*itr++);
    }
    qDebug()<<"no_buf is :"<<no_buf;
    QSqlQuery query;
    switch(cmd_buf.toInt())
    {
    case(CMD_STUDENT_LOGIN):
    {
        student_loggin=query.exec("select st_password from student where st_no='"+no_buf+"';");
        break;
    }
    case(CMD_TEACHER_LOGIN):
    {
        teacher_loggin=query.exec("select tc_password from teacher where tc_no='"+no_buf+"';");
        break;
    }
    case(CMD_ADMIN_LOGIN):
    {
        admin_loggin=query.exec("select ad_password from admin where ad_name='"+no_buf+"';");
        break;
    }
    }
    QString true_passwd;
    while(query.next())
        true_passwd=query.value(0).toString();
    qDebug()<<"true passwd is :"<<true_passwd<<"recv passwd is :"<<passwd_buf;
    (true_passwd==passwd_buf)?data.sendbuf="1":data.sendbuf="0";
    if(passwd_buf.isNull())
        data.sendbuf="0";
    send();
}

void MainSys::exec_select_score()
{
    data.sendbuf="";
    QString no_buf;
    QString::iterator itr=data.recvbuf.begin()+2;
    while(*itr!=":"&&*itr!=";")
    {
        no_buf+=(*itr++);
    }
    QSqlQuery query;
    if(query.exec(
                "select score from performance where st_name =(select st_name from student where st_no='"
                +no_buf+"');"))
        while(query.next())
            data.sendbuf=query.value(0).toString();
    else
    {
        data.sendbuf="-1";
    }
    send();
}

void MainSys::exec_join_pr()
{
    QString no_buf,pr_id;
    QString::iterator itr=data.recvbuf.begin()+2;
    while(*itr!=":")
    {
        no_buf+=*itr++;
    }itr++;
    while(*itr!=":"&&*itr!=";"&&*itr!='\0')
    {
        pr_id+=*itr++;
    }
    qDebug()<<pr_id;
    QSqlQuery query;
    if(query.exec("insert into st_project(st_id,pr_id)values"
                  "((select st_id from student where st_no='"+no_buf+"'),'"+pr_id+"');"))
        data.sendbuf="1";
    else
        data.sendbuf="0";
    send();
}

void MainSys::exec_take_score()
{
    QString no_buf,score_buf;
    QString::iterator itr=data.recvbuf.begin()+2;
    while(*itr!=":")
    {
        no_buf+=*itr++;
    }itr++;
    while(*itr!=":"&&*itr!=";"&&*itr!='\0')
    {
        score_buf+=*itr++;
    }
    qDebug()<<no_buf<<score_buf;
    QSqlQuery query;
    if(query.exec("insert into performance (st_name,st_sex,score)"
                  "values((select st_name from student where st_no='"+no_buf+"'),(select st_sex from student where st_no='"+no_buf+"'),'"+score_buf+"');"))
        data.sendbuf="1";
    else
        data.sendbuf="0";
    send();
}

void MainSys::exec_attempt_pr()
{
    QString::iterator itr=data.recvbuf.begin()+2;
    project cur_buf;
    while(*itr!=":")
    {
        cur_buf.pr_name+=*itr++;
    }itr++;
    while(*itr!=":")
    {
        cur_buf.dp_id+=*itr++;
    }itr++;
    while(*itr!=":")
    {
        cur_buf.dp_address+=*itr++;
    }itr++;
    while(*itr!=":")
    {
        cur_buf.pr_time+=*itr++;
    }itr++;
    while(*itr!=":")
    {
        cur_buf.pr_trainaddress+=*itr++;
    }itr++;
    while(*itr!=":")
    {
        cur_buf.pr_start+=*itr++;
    }itr++;
    while(*itr!=":")
    {
        cur_buf.pr_end+=*itr++;
    }itr++;
    while(*itr!=":")
    {
        cur_buf.pr_days+=*itr++;
    }itr++;
    while(*itr!=":")
    {
        cur_buf.pr_info+=*itr++;
    }itr++;
    while(*itr!=":"&&*itr!=";"&&*itr!='\0')
    {
        cur_buf.pr_active+=*itr++;
        
    }qDebug()<<cur_buf.pr_name<<cur_buf.dp_id<<cur_buf.pr_info<<cur_buf.pr_active;
    QSqlQuery query;
    if(query.exec("insert into project (pr_name,dp_id,dp_address,pr_time,pr_trainaddress,pr_start,pr_end,pr_days,pr_info,pr_active)values('"
                  +cur_buf.pr_name+"','"+cur_buf.dp_id+"','"+cur_buf.dp_address+"','"+cur_buf.pr_time+"','"+cur_buf.pr_trainaddress
                  +"','"+cur_buf.pr_start+"','"+cur_buf.pr_end+"','"+cur_buf.pr_days+"','"+cur_buf.pr_info+"','"+cur_buf.pr_active+"'); "))
        data.sendbuf="1";
    else
        data.sendbuf="0";
    send();
}

void MainSys::exec_select_all()
{
    data.sendbuf.clear();
    int count=0;
    QSqlQuery query;
    query.exec("select COUNT(*) from student;");
    while(query.next())
        count=(query.value(0).toString()).toInt();
    query.exec("select * from student;");
    for(int i=0;i<count;i++)
    {
        while(query.next())
        {
            data.sendbuf+=(query.value(0).toString()+":"+query.value(1).toString()+":"+query.value(2).toString()
                           +":"+query.value(3).toString()+":"+query.value(4).toString()+":"+query.value(5).toString()+":"+query.value(6).toString()+";");
        }
    }
    send();
}

void MainSys::exec_del_student()
{
    QString no_buf;
    QString::iterator itr = data.recvbuf.begin()+2;
    while(*itr!=":"&&*itr!=";"&&*itr!='\0')
    {
        no_buf+=*itr++;
    }
    qDebug()<<no_buf;
    QSqlQuery query;
    if(query.exec("delete from student where st_no='"+no_buf+"';"))
        data.sendbuf="1";
    else
        data.sendbuf="0";
    send();
}

void MainSys::exec_update_student()
{
    data.sendbuf.clear();
    QString::iterator itr=data.recvbuf.begin()+3;
    student  cur_buf;
    while(*itr!=":")
    {
        cur_buf.no+=*itr++;
    }itr++;
    while(*itr!=":")
    {
        cur_buf.name+=*itr++;
    }itr++;
    while(*itr!=":")
    {
        cur_buf.sex+=*itr++;
    }itr++;
    while(*itr!=":")
    {
        cur_buf.class_id+=*itr++;
    }itr++;
    while(*itr!=":")
    {
        cur_buf.passwd+=*itr++;
    }itr++;
    while(*itr!=":"&&*itr!=";"&&*itr!='\0')
    {
        cur_buf.dp_id+=*itr++;
    }
    qDebug()<<"in update :  cur_buf.no="<< cur_buf.no;
    QSqlQuery query;
    if(query.exec("update student set st_name='"+cur_buf.name+"',st_sex='"+cur_buf.sex
                  +"',class_id='"+cur_buf.class_id+"',st_password='"+cur_buf.passwd+"',dp_id="+cur_buf.dp_id+" where st_no='"+cur_buf.no+"';"))
        data.sendbuf="1";
    else
        data.recvbuf="0";
    send();
}

void MainSys::exec_select_project()
{
    data.sendbuf.clear();
    int count=0;
    QSqlQuery query;
    query.exec("select count(*) from project;");
    while(query.next())
        count=(query.value(0).toString()).toInt();
    query.exec("select * from project;");
    for(int i=0;i<count;i++)
    {
        while(query.next())

        {
            data.sendbuf+=(query.value(0).toString()+"^"+query.value(1).toString()+"^"+query.value(2).toString()
                           +"^"+query.value(3).toString()+"^"+query.value(4).toString()+"^"
                           +query.value(5).toString()+"^"+query.value(6).toString()+"^"+query.value(7).toString()+"^"
                           +query.value(8).toString()+"^"+query.value(9).toString()+"^"+query.value(10).toString()+";");
        }
    }
    send();
    data.sendbuf.clear();
}
