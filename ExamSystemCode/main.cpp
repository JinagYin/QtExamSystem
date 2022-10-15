#include "logindialog.h"
#include "examdialog.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDialog login;
    int ret = login.exec();
    if (ret != QDialog::Accepted) {
        return 0;
    }

    ExamDialog exam;
    exam.show();
    return a.exec();
}
