#ifndef PREMIER_H
#define PREMIER_H

#include "QtWidgets/qlistwidget.h"
#include <QDialog>
#include <QTimer>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

namespace Ui {
class Premier;
}

class Premier : public QDialog
{
    Q_OBJECT

public:
    explicit Premier(QWidget *parent = nullptr);
    ~Premier();
    void updateTaskList(const QString &currentQuery, QString str);
    void updateTaskList_2(const QString &currentQuery, QString str);
    int key;
    QDate date;

private slots:
    void Timer_TimeOut_Event_Slot();
    void on_calendarWidget_clicked(const QDate &date);

    void on_addExercise_clicked();

    void on_addFood_clicked();

    void on_listWidget_itemChanged(QListWidgetItem *item);

    void on_seatchB_clicked();

private:
    Ui::Premier *ui;
    QTimer *Timer;
    QSqlDatabase db;
};

#endif // PREMIER_H
