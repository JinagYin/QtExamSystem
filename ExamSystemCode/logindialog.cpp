#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->bgLabel->setScaledContents(true);
    resize(ui->bgLabel->width(), ui->bgLabel->height());      //match the size of window and bg
    setWindowTitle("考试科目一考试登录");                        //window title
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);   //window style, normal dialog and only need close button
    setFixedSize(width(), height());                          //fixed window size
}

LoginDialog::~LoginDialog()
{
    delete ui;
}


void LoginDialog::on_loginBtn_clicked()
{
    QString account = ui->accountEdit->text();
    QString password = ui->passwordEdit->text();
    if (account.isEmpty() || password.isEmpty()) {
        QMessageBox::information(this, "提示", "用户名或密码为空");
        return;
    }

    //验证邮箱地址格式  username@domain
    QRegExp rx("^[a-zA-Z0-9]+([_\.][a-zA-Z0-9]*)*@([A-Za-z0-9\-]+\.)+[A-Za-z0-9]{2,6}$");
    if (!rx.exactMatch(account)) {
        QMessageBox::information(this, "提示", "邮箱地址不合法，请重新输入。");
        ui->accountEdit->clear();
        ui->accountEdit->setFocus();
        return;
    }

    //打开文件
    QString filename = "../account.txt";
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(this, "提示", "文件打开失败");
        return;
    }

    //解析文件
    bool matched = false;
    QTextStream stream(&file);
    QString lineText;
    QStringList list;
    while (!stream.atEnd()) {
        lineText = stream.readLine();
        list = lineText.split(",");
        if (account ==list[0] && password == list[1]) {
            matched = true;
            break;
        }
    }
    file.close();

    if (!matched) {
        QMessageBox::information(this, "提示", "用户名或密码错误");
        ui->accountEdit->clear();
        ui->passwordEdit->clear();
        ui->accountEdit->setFocus();
        return;
    }

    QMessageBox::information(this, "提示", "欢迎进入科目一考试系统");
}
