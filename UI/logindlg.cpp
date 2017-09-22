#include "logindlg.h"
#include "ui_logindlg.h"
#include <QtNetwork/QtNetwork>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDlg)
{
    ui->setupUi(this);
    connect(ui->btnLogin,SIGNAL(clicked(bool)),this,SLOT(LoginAction()));//绑定click事件到按钮上
    m_am = new QNetworkAccessManager(this);
    connect(m_am,SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(CancelAction()));//绑定click事件到按钮上
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
}

LoginDlg::~LoginDlg()
{
    delete ui;
    if(m_am != NULL){
        delete m_am;
        m_am = NULL;
    }
}
void LoginDlg::LoginAction(){

    QString account = ui->txtAccount->text();
    QString passwd = ui->txtPasswd->text();
    if(account == NULL || account.trimmed() == ""){
        //QMessageBox::critical(this,QString("提示"),QString("ohh,请请输入账号（手机号码）！"));
        ui->txtAccount->setFocus();
        return;
    }
    if(passwd == NULL || passwd.trimmed() == ""){
        //QMessageBox::critical(this,QString("提示"),QString("ohh，请输入账号对应的密码！"));
        ui->txtPasswd->setFocus();
        return;
    }
    QString u = "http://";
    if(ui->cmbApp->currentText() =="研秘直播"){
        u += "service.zbzhixue.com/?method=user.login&telephone=";
    }else{
        u += "gks.zbzhixue.com/?method=user.login&telephone=";
    }
    u += account + "&password=" + passwd;
    QUrl url;
    url.setUrl(u);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json;charset=utf-8");
    request.setRawHeader("Accept","application/json");

    m_am->get(request);

    //ui->txtAccount->setText("string");
    //this->accept();
    //this->reject();
}
void LoginDlg::finishedSlot(QNetworkReply* reply){

    if(reply->error() == QNetworkReply::NoError){
        QByteArray repData = reply->readAll();
        QString string = QString::fromUtf8(repData);

        QJsonParseError json_error;
        QJsonDocument doc = QJsonDocument::fromJson(repData, &json_error);

        QString  ss = "";

        if(json_error.error == QJsonParseError::NoError){
            if(doc.isObject()){
                QJsonObject obj = doc.object();
                if(obj.contains("code")){
                    QJsonValue code = obj.take("code");
                    if(code.toString() == "0"){//登录成功
                        this->accept();
                        return;
                    }
                    ss += code.toString();
                }
                if(obj.contains("msg")){
                    QJsonValue msg = obj.take("msg");
                    ss += msg.toString();
                }
                if(obj.contains("data")){
                    QJsonValue data = obj.take("data");
                    QJsonObject dd = data.toObject();
                    ss += dd.take("nickName").toString();
                }

            }
        }
        ui->txtAccount->setText(ss);
    }

}

void LoginDlg::CancelAction(){
    this->reject();
}

//鼠标按下事件
void LoginDlg::mousePressEvent(QMouseEvent *event)
{
     mousePoint = event->pos();    //鼠标相对于窗体的位置
     isMousePressed = true;        //鼠标按下
     event->accept();
}

//窗体拖动事件
void LoginDlg::mouseMoveEvent(QMouseEvent *event)
{
     //若鼠标左键被按下
     if(isMousePressed == true)
    {
          //鼠标相对于屏幕的位置
          QPoint curMousePoint = event->globalPos() - mousePoint;
         //移动主窗体位置
          move(curMousePoint);
     }
     event->accept();
}

//鼠标释放事件
void LoginDlg::mouseReleaseEvent(QMouseEvent *event)
{
      //鼠标未按下
      isMousePressed = false;
      event->accept();
}
