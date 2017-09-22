#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include <QtNetwork/QtNetwork>
#include <QMouseEvent>
#include <QLineEdit>
namespace Ui {
class LoginDlg;
}

class LoginDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDlg(QWidget *parent = 0);
    ~LoginDlg();


private:
    Ui::LoginDlg *ui;
    QNetworkAccessManager * m_am = NULL;
    QPoint mousePoint;      //鼠标相对于窗体的位置
    bool isMousePressed;    //鼠标是否按下
    virtual void mousePressEvent(QMouseEvent *event);  //鼠标按下事件
    virtual void mouseMoveEvent(QMouseEvent *event);   //窗体拖动事件
    virtual void mouseReleaseEvent(QMouseEvent *event);//鼠标释放事件


private slots:
    void finishedSlot(QNetworkReply* reply);
    void LoginAction();
    void CancelAction();
    //void on_txtAccount_returnPressed();
};

#endif // LOGINDLG_H
