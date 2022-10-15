#ifndef EXAMDIALOG_H
#define EXAMDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QGridLayout>

class ExamDialog : public QDialog
{
    Q_OBJECT
public:
    ExamDialog(QWidget * parent = 0);

private:
    QTimer * m_timer;
    int m_timePassed;
    QGridLayout *m_layout;

    QTextEdit * m_content;
    QLabel * m_orderLabels[10];
    QRadioButton * m_radioButtons[40];
    QButtonGroup * m_buttonGroup[10];
    QPushButton * m_commitButton;

    QStringList m_answerList;

    void initTimer();
    void initLayout();
    bool initTextEdit();
    void initButtons();
    bool isAnswerComplete();
    int getMatchCnt();

private slots:
    void on_fresh_time();
    void on_commit_clicked();

};

#endif // EXAMDIALOG_H
