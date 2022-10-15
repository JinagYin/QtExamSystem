#include "examdialog.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>

ExamDialog::ExamDialog(QWidget * parent):QDialog(parent)
{
    setWindowTitle("考试已用时[0:0]");
    QFont font;
    font.setPointSize(12);
    setFont(font);

    //设置窗体颜色
    setPalette(QPalette(QColor(209, 215, 255)));

    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    resize(800, 900);
    setFixedSize(width(), height());

    initTimer();
    initLayout();
    if (!initTextEdit()) {
        QMessageBox::information(this, "提示", "初始化题目失败");
        QTimer::singleShot(0, qApp, SLOT(quit()));
    }
    initButtons();
}

void ExamDialog::initTimer()
{
    m_timePassed = 0;
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    m_timer->start();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(on_fresh_time()));
}

void ExamDialog::initLayout()
{
    m_layout = new QGridLayout(this);
    m_layout->setSpacing(10);           //设置控件之间间距
    m_layout->setMargin(10);            //设置控件与窗体间距

}

bool ExamDialog::initTextEdit()
{
    m_content = new QTextEdit(this);
    m_content->setReadOnly(true);
    m_layout->addWidget(m_content, 0, 0, 1, 10);
    QString filename = "exam.txt";
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QString linetext = "";
    QString content;
    QStringList answerList;
    QTextStream stream(&file);
    stream.setCodec("UTF-8");

    int cnt = 6;
    while (!stream.atEnd()) {
        cnt++;
        linetext = stream.readLine();
        if (cnt % 6 != 0) {
            content = content + linetext + "\r\n";
        }
        else {
            QStringList list = linetext.split(" ");
            m_answerList.append(list[1]);
            content += "\r\n";
        }
    }

    m_content->setText(content);
    return true;
}

void ExamDialog::initButtons()
{
    QStringList list = {"A", "B", "C", "D"};

    for (int i = 0; i<10; i++) {
        m_orderLabels[i] = new QLabel(this);
        m_orderLabels[i]->setText("第" + QString::number(i+1) + "题");
        m_layout->addWidget(m_orderLabels[i], 1, i, 1, 1);
        m_buttonGroup[i] = new QButtonGroup(this);

        for (int j = 0; j<list.length(); j++)
        {
            QRadioButton * radioButton = new QRadioButton(this);
            radioButton->setText(list[j]);
            m_layout->addWidget(radioButton, j + 2, i, 1, 1);
            m_buttonGroup[i]->addButton(radioButton);
            m_radioButtons[i*4 + j] = radioButton;
        }
    }

    m_commitButton = new QPushButton(this);
    m_commitButton->setText("提交");
    connect(m_commitButton, SIGNAL(clicked(bool)), this, SLOT(on_commit_clicked()));
    m_layout->addWidget(m_commitButton, 6, 9, 1, 1);
}

bool ExamDialog::isAnswerComplete() {
    bool ret = true;
    for (int i = 0; i<10; i++) {
        if (!m_buttonGroup[i]->checkedButton()) {
            ret = false;
            break;
        }
    }

    return ret;
}

int ExamDialog::getMatchCnt() {
    int ret = 0;

    for (int i =0; i<m_answerList.length(); i++) {
           QString answer = m_buttonGroup[i]->checkedButton()->text();

        if (m_answerList[i] == answer) {
            ret ++;
        }
    }

    return ret;
}

void ExamDialog::on_fresh_time()
{
    m_timePassed++;
    QString minute = QString::number(m_timePassed / 60);
    QString second = QString::number(m_timePassed % 60);

    setWindowTitle("考试已用时[" + minute + ":" + second + "]");
}

void ExamDialog::on_commit_clicked() {
    if(!isAnswerComplete()) {
        QMessageBox::information(this, "tips", "please complete your answers!");
        return;
    }

    int cnt = getMatchCnt();
    QString message = "你做对了" + QString::number(cnt) + "道题。";
    QMessageBox::information(this, "tips", message);
}
