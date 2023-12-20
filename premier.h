#ifndef PREMIER_H
#define PREMIER_H

#include <QDialog>
#include <QTimer>
#include <QDateTime>

namespace Ui {
class Premier;
}

class Premier : public QDialog
{
    Q_OBJECT

public:
    explicit Premier(QWidget *parent = nullptr);
    ~Premier();
    void updateTaskList();

private slots:
    void Timer_TimeOut_Event_Slot();
    void on_calendarWidget_clicked(const QDate &date);

private:
    Ui::Premier *ui;
    QTimer *Timer;
};

#endif // PREMIER_H
